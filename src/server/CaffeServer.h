//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_CAFFESERVER_H
#define CAFFEMACHINE_CAFFESERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/endpoint.hpp>
#include "../session/SessionManager.h"
#include "../rpc/RpcRequest.h"
#include "../caffe/Model.h"
#include "../caffe/ModelManager.h"
#include "../caffe/Classifier.h"
#include "../../vendors/ThreadPool.h"

typedef websocketpp::server<websocketpp::config::asio> WebsocketServer;
typedef std::pair<string, float> Prediction;

using websocketpp::connection_hdl;
using nbsdx::concurrent::ThreadPool;

class CaffeServer {
 public:
  explicit CaffeServer(uint16_t port);
  virtual ~CaffeServer();

  void Start();
  void Stop();

  void OnMessageHandler(connection_hdl hdl, WebsocketServer::message_ptr message);

  int GetPort() const;

  const ModelManager &GetModelManager() const;
  void SetModelManager(const ModelManager &modelManager);

 private:
  //private vars
  unsigned int port_;
  bool isStarted = false;
  WebsocketServer server_;
  SessionManager session_manager_;
  ModelManager model_manager_;
  std::map<int, std::pair<std::function<void(connection_hdl &, RpcRequest)>, bool>> routes_;
  std::set<connection_hdl, std::owner_less<connection_hdl>> current_connections_;
  ThreadPool<4> thread_pool_;

  //helper methods
  void SendJson(connection_hdl &hdl, json js);
  bool ValidateSessionId(connection_hdl &hdl, RpcRequest request);
  void SendRpcError(connection_hdl &hdl, json id, int errorCode, string message);

  //MessageHandler
  void StartSessionHandler(connection_hdl &hdl, RpcRequest request);
  void EndSessionHandler(connection_hdl &hdl, RpcRequest request);
  void NotImplementedHandler(connection_hdl &hdl, RpcRequest request);
  void GetModelsHandler(connection_hdl &hdl, RpcRequest request);
  void LoadModelHandler(connection_hdl &hdl, RpcRequest request);
  void ClassifyHandler(connection_hdl &hdl, RpcRequest request);
  void InfoHandler(connection_hdl &hdl, RpcRequest request);

  void InitRoutes();

  void ClassifyThread(std::shared_ptr<Classifier> cl, string encodedImage, connection_hdl hdl, RpcResponse response);

  void OnOpenHandler(connection_hdl hdl);
  void OnCloseHandler(connection_hdl hdl);
  void GenerateHeatmapHandler(connection_hdl &hdl, RpcRequest request);
  void GenerateHeatmapThread(std::shared_ptr<Classifier> cl,
                             string encodedImage,
                             connection_hdl hdl,
                             RpcResponse response,
                             unsigned int resolution);
};

#endif //CAFFEMACHINE_CAFFESERVER_H
