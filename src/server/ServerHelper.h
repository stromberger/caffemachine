//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_SERVERHELPER_H
#define CAFFEMACHINE_SERVERHELPER_H

#include <string>
#include <vector>
#include "../../vendors/json.h"
#include "../rpc/RpcRequest.h"
#include "../caffe/Model.h"

namespace serverhelper {

using nlohmann::json;

namespace method {
  enum RpcMethod {
    getModels, loadModel, classify, startSession, endSession, info, generateHeatmap, notImplemented
  };
}

method::RpcMethod stringToRpcMethod(const std::string &input) {
  if (input == "getModels") {
    return method::getModels;
  } else if (input == "loadModel") {
    return method::loadModel;
  } else if (input == "classify") {
    return method::classify;
  } else if (input == "startSession") {
    return method::startSession;
  } else if (input == "endSession") {
    return method::endSession;
  } else if (input == "info") {
    return method::info;
  } else if (input == "generateHeatmap") {
    return method::generateHeatmap;
  }
  return method::notImplemented;
}

std::string getSessionIdFromRequest(const RpcRequest &request) {
  return request.GetParameter().at("sessionId").get<string>();
}

json modelsToJsonArray(std::vector<Model> models) {
  std::vector<json> js_list;

  for (auto model : models)
    js_list.push_back(model.ToJson());

  return js_list;
}

}

#endif //CAFFEMACHINE_SERVERHELPER_H
