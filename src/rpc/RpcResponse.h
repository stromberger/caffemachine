//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_RPCRESPONSE_H
#define CAFFEMACHINE_RPCRESPONSE_H

#include <string>
#include "../../vendors/json.h"
#include "RpcError.h"

using std::string;
using nlohmann::json;

class RpcResponse {

 public:
  RpcResponse();
  explicit RpcResponse(RpcError error);
  explicit RpcResponse(const json &id);

  const json &GetId() const;
  void SetId(const json &id);

  const json &GetResult() const;
  void SetResult(const json &result);

  const RpcError &GetError() const;
  void SetError(const RpcError &error);

  bool ShouldThrowError() const;
  void SetThrowError(bool throwError);

  json ToJson();

 private:
  json id_;
  json result_;
  RpcError error_;
  bool throwError_ = false;
};

#endif //CAFFEMACHINE_RPCRESPONSE_H
