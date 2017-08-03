//
// Created by alex on 17.07.17.
//

#include <websocketpp/endpoint.hpp>
#include <future>
#include <highgui.h>
#include "CaffeServer.h"
#include "ServerHelper.h"

#define BIND_HANDLER(handler, requireAuthentication) std::pair<std::function<void(connection_hdl&,RpcRequest)>,bool>(std::bind(&handler,this,std::placeholders::_1,std::placeholders::_2),requireAuthentication);

CaffeServer::CaffeServer(uint16_t port) : port_(port) {

  try {
    {
      using websocketpp::log::alevel;
      using websocketpp::lib::placeholders::_1;
      using websocketpp::lib::placeholders::_2;
      using websocketpp::lib::bind;

      server_.set_reuse_addr(true);

      server_.set_access_channels(alevel::all);
      server_.clear_access_channels(alevel::frame_payload);

      server_.set_message_handler(bind(&CaffeServer::OnMessageHandler, this, _1, _2));
      server_.set_open_handler(bind(&CaffeServer::OnOpenHandler, this, _1));
      server_.set_close_handler(bind(&CaffeServer::OnCloseHandler, this, _1));
    }

    server_.init_asio();

    InitRoutes();

  } catch (...) {
    throw std::runtime_error("couldn't initialize CaffeServer");
  }
}

/*
 * Start & stop routines
 */

void CaffeServer::Start() {
  server_.listen((uint16_t) port_);
  server_.start_accept();

  auto thread = std::thread([this]() {
    server_.run();
  });
  thread.detach();

  isStarted = true;
}

void CaffeServer::Stop() {
  for (const auto &connection : current_connections_) {
    server_.pause_reading(connection);
    server_.close(connection, websocketpp::close::status::normal, "");
  }

  server_.stop_perpetual();

  isStarted = false;
}

void CaffeServer::InitRoutes() {
  using namespace serverhelper::method;

  //No auth
  routes_[RpcMethod::startSession] = BIND_HANDLER(CaffeServer::StartSessionHandler, false);
  routes_[RpcMethod::info] = BIND_HANDLER(CaffeServer::InfoHandler, false);

  //Auth
  routes_[RpcMethod::getModels] = BIND_HANDLER(CaffeServer::GetModelsHandler, true);
  routes_[RpcMethod::loadModel] = BIND_HANDLER(CaffeServer::LoadModelHandler, true);
  routes_[RpcMethod::classify] = BIND_HANDLER(CaffeServer::ClassifyHandler, true);
  routes_[RpcMethod::endSession] = BIND_HANDLER(CaffeServer::EndSessionHandler, true);
  routes_[RpcMethod::generateHeatmap] = BIND_HANDLER(CaffeServer::GenerateHeatmapHandler, true);
}

/*
 * Server handler methods (e.g. OnOpen, OnEnd)
 */

void CaffeServer::OnOpenHandler(connection_hdl hdl) {
  current_connections_.insert(hdl);
}

void CaffeServer::OnCloseHandler(connection_hdl hdl) {
  current_connections_.erase(hdl);
}

void CaffeServer::OnMessageHandler(connection_hdl hdl, WebsocketServer::message_ptr message) {
  try {
    using namespace serverhelper;

    auto request = RpcRequest::FromJson(message->get_payload());
    auto currentMethod = stringToRpcMethod(request.GetMethod());

    //If method is not available
    if (!(routes_.find(currentMethod) != routes_.end())) {
      NotImplementedHandler(hdl, request);
      return;
    }

    //If method is available
    auto currentRoute = routes_[currentMethod];

    //Check sessionId if needed
    if (currentRoute.second && ValidateSessionId(hdl, request)) {
      auto sessionId = serverhelper::getSessionIdFromRequest(request);
      session_manager_.UpdateLastSeen(sessionId);
    } else if (currentRoute.second) {
      return;
    }

    //finally call the method
    currentRoute.first(hdl, request);
  } catch (const std::exception &ex) {
    RpcResponse response(RpcError(-32700,
                                  "Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text."));
    std::cout << ex.what() << std::endl;
    SendJson(hdl, response.ToJson());
  }

}

/*
 *
 * Handler methods
 *
 */

/*
 * InfoHandler
 */

void CaffeServer::InfoHandler(connection_hdl &hdl, RpcRequest request) {
  auto response = request.CreateResponse();
  response.SetResult({
                         {"serverName", "caffemachine"},
                         {"version", "0.1 - Espresso"}});
  SendJson(hdl, response.ToJson());
}

/*
 * GenerateHeatmap
 */

void CaffeServer::GenerateHeatmapHandler(connection_hdl &hdl, RpcRequest request) {
  auto model = session_manager_.GetSessionById(
      serverhelper::getSessionIdFromRequest(request)).GetLoadedModel();
  auto response = request.CreateResponse();

  auto encodedImage = request.GetParameter().at("image").get<string>();
  auto resolution = request.GetParameter().at("resolution").get<unsigned int>();
  thread_pool_.AddJob([=]() {
    GenerateHeatmapThread(model_manager_.GetClassifierById(model.GetId()), encodedImage, hdl, response, resolution);
  });
}

void CaffeServer::GenerateHeatmapThread(std::shared_ptr<Classifier> cl,
                                        string encodedImage,
                                        connection_hdl hdl,
                                        RpcResponse response,
                                        unsigned int resolution) {
  auto decodedString = websocketpp::base64_decode(encodedImage);
  vector<uchar> vdata(decodedString.begin(), decodedString.end());
  cv::Mat img = cv::imdecode(vdata, cv::IMREAD_UNCHANGED);

  auto prediction = cl->GenerateHeatmap(img, resolution);

  json js = {
      {"prediction", prediction.first},
      {"heatmap", prediction.second}
  };
  response.SetResult(js);

  SendJson(hdl, response.ToJson());
}

/*
 * ClassifyHandler
 */

void CaffeServer::ClassifyHandler(connection_hdl &hdl, RpcRequest request) {
  auto model = session_manager_.GetSessionById(
      serverhelper::getSessionIdFromRequest(request)).GetLoadedModel();
  auto response = request.CreateResponse();

  auto encodedImage = request.GetParameter().at("image").get<string>();
  thread_pool_.AddJob([=]() {
    ClassifyThread(model_manager_.GetClassifierById(model.GetId()), encodedImage, hdl, response);
  });
}

void CaffeServer::ClassifyThread(std::shared_ptr<Classifier> cl,
                                 string encodedImage,
                                 connection_hdl hdl,
                                 RpcResponse response) {
  using namespace std::chrono;

  auto decodedString = websocketpp::base64_decode(encodedImage);
  vector<uchar> vdata(decodedString.begin(), decodedString.end());
  cv::Mat img = cv::imdecode(vdata, cv::IMREAD_UNCHANGED);

  //measure elpased time
  auto begin = steady_clock::now();

  auto prediction = cl->Classify(img, 3);

  auto end = steady_clock::now();
  auto elapsedTime = duration_cast<milliseconds>(end - begin);

  response.SetResult({
                         {"predictions", prediction},
                         {"elapsedTime", elapsedTime.count()}
                     });
  SendJson(hdl, response.ToJson());
}

/*
 * GetModelsHandler
 */

void CaffeServer::GetModelsHandler(connection_hdl &hdl, RpcRequest request) {
  auto models = serverhelper::modelsToJsonArray(model_manager_.GetAllModels());
  auto response = request.CreateResponse();
  response.SetResult(models);
  SendJson(hdl, response.ToJson());
}

/*
 * LoadModelHandler
 */

void CaffeServer::LoadModelHandler(connection_hdl &hdl, RpcRequest request) {
  // nobody will ever read this comment - (╯°□°）╯︵ ┻━┻)
  auto response = request.CreateResponse();
  auto sessionId = serverhelper::getSessionIdFromRequest(request);

  auto session = session_manager_.GetSessionById(sessionId);
  auto modelId = request.GetParameter().at("id").get<int>();
  session.SetLoadedModel(model_manager_.GetModelById(modelId));
  session_manager_.UpdateSession(sessionId, session);

  response.SetResult(modelId);
  SendJson(hdl, response.ToJson());
}

/*
 * StartSessionHandler
 */

void CaffeServer::StartSessionHandler(connection_hdl &hdl, RpcRequest request) {
  auto session = session_manager_.CreateSession();
  auto response = request.CreateResponse();
  response.SetResult(session.GetSessionId());

  SendJson(hdl, response.ToJson());
}

/*
 * EndSessionHandler
 */

void CaffeServer::EndSessionHandler(connection_hdl &hdl, RpcRequest request) {
  auto sessionId = serverhelper::getSessionIdFromRequest(request);
  session_manager_.EndSession(sessionId);

  auto response = request.CreateResponse();
  response.SetResult(true);
  SendJson(hdl, response.ToJson());
}

void CaffeServer::NotImplementedHandler(connection_hdl &hdl, RpcRequest request) {
  SendRpcError(hdl, request.GetId(), -32601, "The method does not exist / is not available.");
}

/*
 * Utils
 */

bool CaffeServer::ValidateSessionId(connection_hdl &hdl, RpcRequest request) {
  auto sessionId = serverhelper::getSessionIdFromRequest(request);

  if (session_manager_.IsValidSessionId(sessionId)) {
    return true;
  }

  SendRpcError(hdl, request.GetId(), -32001, "invalid sessionId");
  return false;
}

void CaffeServer::SendRpcError(connection_hdl &hdl, json id, int errorCode, string message) {
  auto response = RpcResponse(RpcError(errorCode, message));
  response.SetId(id);

  SendJson(hdl, response.ToJson());
}

void CaffeServer::SendJson(connection_hdl &hdl, json js) {
  if (current_connections_.find(hdl) != current_connections_.end())
    server_.send(hdl, js.dump(), websocketpp::frame::opcode::text);
}

int CaffeServer::GetPort() const {
  return port_;
}

const ModelManager &CaffeServer::GetModelManager() const {
  return model_manager_;
}
void CaffeServer::SetModelManager(const ModelManager &modelManager) {
  CaffeServer::model_manager_ = modelManager;
}

/*
 * Destructor
 */

CaffeServer::~CaffeServer() {
  if (isStarted)
    Stop();
}