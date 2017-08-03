//
// Created by alex on 13.07.17.
//

#ifndef CAFFEMACHINE_SESSIONMANAGER_H
#define CAFFEMACHINE_SESSIONMANAGER_H

#include <ctime>
#include <iostream>
#include <map>
#include <boost/date_time/posix_time/ptime.hpp>
#include "Session.h"

#include <locale>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <mutex>

using std::string;
using std::map;
using boost::posix_time::ptime;

class SessionManager {
 public:
  SessionManager();

  Session &GetSessionById(string sessionId);
  Session CreateSession();

  void UpdateSession(string sessionId, Session &session);
  void UpdateLastSeen(string sessionId);

  void EndSession(string sessionId);
  bool IsValidSessionId(string &sessionId);

 private:
  map<string, Session> sessions_;
  void AssertValidSessionId(string sessionId);
  std::mutex collector_mutex_;

};

#endif //CAFFEMACHINE_SESSIONMANAGER_H
