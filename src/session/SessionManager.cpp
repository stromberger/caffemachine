//
// Created by alex on 13.07.17.
//

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <thread>
#include <mutex>
#include "SessionManager.h"

Session &SessionManager::GetSessionById(string sessionId) {
  AssertValidSessionId(sessionId);
  std::lock_guard<std::mutex> lock(collector_mutex_);
  return sessions_[sessionId];
}

bool SessionManager::IsValidSessionId(string &sessionId) {
  std::lock_guard<std::mutex> lock(collector_mutex_);
  return (sessions_.find(sessionId) != sessions_.end());
}

Session SessionManager::CreateSession() {
  using namespace boost;

  Session session;
  session.SetLastSeen(std::chrono::system_clock::now());

  auto randomUUID = (uuids::random_generator()) ();
  session.SetSessionId(uuids::to_string(randomUUID));

  std::lock_guard<std::mutex> lock(collector_mutex_);

  sessions_[session.GetSessionId()] = session;
  return session;
}

void SessionManager::EndSession(string sessionId) {
  AssertValidSessionId(sessionId);

  std::lock_guard<std::mutex> lock(collector_mutex_);

  sessions_.erase(sessionId);
}

void SessionManager::UpdateSession(string sessionId, Session &session) {
  AssertValidSessionId(sessionId);

  std::lock_guard<std::mutex> lock(collector_mutex_);

  sessions_[sessionId] = session;
}

void SessionManager::AssertValidSessionId(string sessionId) {
  if (!IsValidSessionId(sessionId))
    throw std::invalid_argument("invalid sessionId");
}

void SessionManager::UpdateLastSeen(string sessionId) {
  AssertValidSessionId(sessionId);

  std::lock_guard<std::mutex> lock(collector_mutex_);

  auto session = sessions_[sessionId];
  session.SetLastSeen(std::chrono::system_clock::now());
  sessions_[sessionId] = session;
}
SessionManager::SessionManager() {

  //SessionCollector - periodic task to ensure no old clients are active
  std::thread([&]() {
    for (;;) {
      auto currentTime = std::chrono::system_clock::now();

      for (auto const &entry : sessions_) {
        if ((currentTime - entry.second.GetLastSeen()) > std::chrono::minutes(25)) {
          std::lock_guard<std::mutex> lock(collector_mutex_);
          sessions_.erase(entry.first);
        }
      }

      std::this_thread::sleep_for(std::chrono::minutes(1));
    }
  }).detach();

}
