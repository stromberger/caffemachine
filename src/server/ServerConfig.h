//
// Created by alex on 20.07.17.
//

#ifndef CAFFEMACHINE_SERVERCONFIG_H
#define CAFFEMACHINE_SERVERCONFIG_H

#include "../caffe/ModelManager.h"

using std::string;
using nlohmann::json;

class ServerConfig {
 public:
  explicit ServerConfig(string config);
  explicit ServerConfig(json config);

  const ModelManager &GetModelManager() const;
  unsigned int GetPort() const;

 private:
  ModelManager model_manager_;
  unsigned int port_;
  void ParseJson(json config);
};

#endif //CAFFEMACHINE_SERVERCONFIG_H
