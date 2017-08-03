//
// Created by alex on 17.07.17.
//

#include "RpcResponse.h"

RpcResponse::RpcResponse() = default;

RpcResponse::RpcResponse(RpcError err) : error_(std::move(err)) {
  throwError_ = true;
}

RpcResponse::RpcResponse(const json &id) : id_(id) {}

const json &RpcResponse::GetId() const {
  return id_;
}
void RpcResponse::SetId(const json &id) {
  RpcResponse::id_ = id;
}

const json &RpcResponse::GetResult() const {
  return result_;
}
void RpcResponse::SetResult(const json &result) {
  RpcResponse::result_ = result;
}

const RpcError &RpcResponse::GetError() const {
  return error_;
}
void RpcResponse::SetError(const RpcError &error) {
  RpcResponse::error_ = error;
}

bool RpcResponse::ShouldThrowError() const {
  return throwError_;
}
void RpcResponse::SetThrowError(bool throwError) {
  RpcResponse::throwError_ = throwError;
}

json RpcResponse::ToJson() {
  json j = {
      {"jsonrpc", "2.0"},
      {"id", id_}
  };

  //if the id string is empty return 'null'
  if (id_.empty())
    j["id"] = nullptr;

  //if the throwError flag is set, send error instead of result
  if (throwError_) {
    j["error"] = error_.ToJson();
  } else {
    j["result"] = result_;
  }

  return j;
}