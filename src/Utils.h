//
// Created by alex on 24.07.17.
//

#ifndef CAFFEMACHINE_UTILS_H
#define CAFFEMACHINE_UTILS_H

#include <fstream>
#include <sstream>

namespace utils {
  using std::string;

  string readAllLines(string filepath) {
    std::ifstream file(filepath);

    std::stringstream out_stream;
    string buffer;
    while (file >> buffer)
      out_stream << buffer;

    return out_stream.str();
  }
}

#endif //CAFFEMACHINE_UTILS_H
