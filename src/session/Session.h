//
// Created by alex on 13.07.17.
//

#ifndef CAFFEMACHINE_SESSION_H
#define CAFFEMACHINE_SESSION_H

#include <string>
#include <boost/date_time/posix_time/ptime.hpp>
#include <chrono>
#include "../caffe/Model.h"

using std::time;
using std::string;

class Session {
 public:
  const string &GetSessionId() const;
  void SetSessionId(const string &sessionId);

  const string &GetName() const;
  void SetName(const string &name);

  const Model &GetLoadedModel() const;
  void SetLoadedModel(const Model &loadedModel);

  const std::chrono::system_clock::time_point &GetLastSeen() const;
  void SetLastSeen(const std::chrono::system_clock::time_point &last_seen);

 private:
  string sessionId_;
  std::chrono::system_clock::time_point last_seen_;
  string name_;
  Model loadedModel_;
};

#endif //CAFFEMACHINE_SESSION_H
