//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_RPCERROR_H
#define CAFFEMACHINE_RPCERROR_H

#include <string>
#include "../../vendors/json.h"

using std::string;
using nlohmann::json;

class RpcError {
 public:
  RpcError() {
    code_ = 0;
  }
  RpcError(int code, const string &message);

  int GetCode() const;
  void SetCode(int code);

  const string &GetMessage() const;
  void SetMessage(const string &message);

  json ToJson();
  static RpcError FromJson(string json);

 private:
  int code_;
  string message_;

};

#endif //CAFFEMACHINE_RPCERROR_H
