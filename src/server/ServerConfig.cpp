//
// Created by alex on 20.07.17.
//

#include "ServerConfig.h"

ServerConfig::ServerConfig(string config) {
  ParseJson(json::parse(config));
}

ServerConfig::ServerConfig(json config) {
  ParseJson(config);
}

void ServerConfig::ParseJson(json config) {
  port_ = config.at("port").get<int>();
  model_manager_ = ModelManager::FromJson(config.at("models"));
}

const ModelManager &ServerConfig::GetModelManager() const {
  return model_manager_;
}
unsigned int ServerConfig::GetPort() const {
  return port_;
}
