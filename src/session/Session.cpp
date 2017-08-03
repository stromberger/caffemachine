//
// Created by alex on 13.07.17.
//

#include "Session.h"

const string &Session::GetSessionId() const {
  return sessionId_;
}

void Session::SetSessionId(const string &sessionId) {
  Session::sessionId_ = sessionId;
}

const string &Session::GetName() const {
  return name_;
}

void Session::SetName(const string &name) {
  Session::name_ = name;
}

const Model &Session::GetLoadedModel() const {
  return loadedModel_;
}

void Session::SetLoadedModel(const Model &loadedModel) {
  Session::loadedModel_ = loadedModel;
}
const std::chrono::system_clock::time_point &Session::GetLastSeen() const {
  return last_seen_;
}
void Session::SetLastSeen(const std::chrono::system_clock::time_point &last_seen) {
  last_seen_ = last_seen;
}
