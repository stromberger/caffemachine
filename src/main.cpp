#include <iostream>
#include "server/CaffeServer.h"
#include "server/ServerConfig.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "caffemachine <configFilePath>" << std::endl;
    return 0;
  }

  try {
    ServerConfig config(utils::readAllLines(argv[1]));

    CaffeServer server(static_cast<uint16_t>(config.GetPort()));
    server.SetModelManager(config.GetModelManager());
    server.Start();

    string waitHandle;
    std::cin >> waitHandle;
    server.Stop();
  } catch (...) {
    std::cerr << "Please check the config file and/or the configured server port!";
  }
}