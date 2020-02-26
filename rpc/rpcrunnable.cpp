//
// Created by gothack on 14/02/2020.
//

#include "rpcrunnable.hpp"

#include <uuid/uuid.h>

using namespace std;

RpcRunnable::RpcRunnable() :
  _myId(genUUID())
  {}

string RpcRunnable::genUUID() {
  string out;
  uuid_t uuid;
  out.resize(37);
  uuid_generate_random(uuid);
  uuid_unparse(uuid, out.data());
  out.resize(36);
  return out;
}

vector<string> RpcRunnable::getRouting(zmqpp::message &msg, bool pop) {
  string buf;
  vector<string> routing;
  bool pop_empty = false;
  for (int i = 0; i < msg.parts(); i++) {
    msg >> buf;
    if (buf.size()) {
      routing.push_back(buf);
    } else {
      pop_empty = true;
      break;
    }
  }
  if (pop) {
    for (int i = 0; i < routing.size(); i++) {
      msg.pop_front();
    }
    if (pop_empty) {
      msg.pop_front();
    }
    msg.reset_read_cursor();
  }
  return routing;
}