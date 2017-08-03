//
// Created by alex on 17.07.17.
//

#include "RpcRequest.h"

using std::string;

const string &RpcRequest::GetMethod() const {
  return method_;
}
void RpcRequest::SetMethod(const string &method) {
  RpcRequest::method_ = method;
}

const json &RpcRequest::GetParameter() const {
  return parameter_;
}
void RpcRequest::SetParameter(const json &parameter) {
  RpcRequest::parameter_ = parameter;
}

RpcRequest RpcRequest::FromJson(string rawJson) {
  try {
    using nlohmann::json;

    auto parsed = json::parse(rawJson);
    RpcRequest rpcRequest;

    rpcRequest.SetMethod(parsed.at("method").get<string>());

    //check if id_ is string or integer - both are allowed according to the specification
    rpcRequest.SetId(parsed.at("id"));

    //check if params is omitted
    try {
      rpcRequest.SetParameter(parsed.at("params"));
    } catch (...) {
      json emptyParams;
      rpcRequest.SetParameter(emptyParams);
    }

    return rpcRequest;
  } catch (...) {
    throw std::runtime_error("malformed json");
  }
}

RpcResponse RpcRequest::CreateResponse() {
  RpcResponse response(id_);
  return response;
}

const json &RpcRequest::GetId() const {
  return id_;
}

void RpcRequest::SetId(const json &id) {
  RpcRequest::id_ = id;
}
