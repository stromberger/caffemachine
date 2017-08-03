//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_RPCREQUEST_H
#define CAFFEMACHINE_RPCREQUEST_H

#include <string>
#include "../../vendors/json.h"
#include "RpcResponse.h"

using std::string;
using nlohmann::json;

class RpcRequest {
 public:

  // getter/setter
  const string &GetMethod() const;
  void SetMethod(const string &method);

  const json &GetId() const;
  void SetId(const json &id);

  const json &GetParameter() const;
  void SetParameter(const json &parameter);

  RpcResponse CreateResponse();

  // static members
  static RpcRequest FromJson(string json);

 private:
  string method_;
  json id_;
  json parameter_;

};

#endif //CAFFEMACHINE_RPCREQUEST_H
