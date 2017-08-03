//
// Created by alex on 17.07.17.
//

#include "RpcError.h"

int RpcError::GetCode() const {
  return code_;
}
void RpcError::SetCode(int code) {
  RpcError::code_ = code;
}

const string &RpcError::GetMessage() const {
  return message_;
}
void RpcError::SetMessage(const string &message) {
  RpcError::message_ = message;
}

RpcError::RpcError(int code, const string &message) : code_(code), message_(message) {}

json RpcError::ToJson() {
  using nlohmann::json;
  json j = {
      {"code", code_},
      {"message", message_}
  };
  return j;
}

RpcError RpcError::FromJson(string rawJson) {
  using nlohmann::json;
  auto j = json::parse(rawJson);

  auto code = j.at("code").get<int>();
  auto message = j.at("message").get<string>();
  RpcError error(code, message);

  return error;
}
