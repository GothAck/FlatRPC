/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <pthread.h>

#include "rpcclientbase.hpp"

using namespace std;
using namespace zmqpp;

inline bool coinflip(float occ) {
  return rand() < (RAND_MAX * occ);
}

struct LocalSocket : zmqpp::socket {
  LocalSocket(zmqpp::context &ctx, zmqpp::socket_type type) : zmqpp::socket(ctx, type) {
    connect("inproc://frontend");
  }
  ~LocalSocket() {
    disconnect("inproc://frontend");
  }
};

RpcClientBase::RpcClientBase(zmqpp::context &ctx) :
  RpcBase(ctx, zmqpp::socket_type::dealer)
  {}

bool RpcClientBase::connect(std::string connStr) {
  int fd = createSocketConnect(connStr);
  if (fd < 0)
    return false;

  _socket.set(zmqpp::socket_option::use_fd, fd);
  _socket.connect("ipc://" + connStr);

  return true;
}

void RpcClientBase::workerThread() {
  pthread_setname_np(pthread_self(), __func__);
  zmqpp::socket backend(_context, socket_type::router);
  zmqpp::reactor react;
  backend.connect("inproc://backend");

  react.add(backend, [this, &backend] {
    message msg;
    string buf;
    backend.receive(msg);
    auto routing = getRouting(msg);
    msg >> buf;
    handleResponse(move(buf));
  });

  while (running.load()) {
    if (coinflip(0.2) || !react.poll(50)) {
      scoped_lock<recursive_mutex> lock(_mutex);
      vector<size_t> timeouts;
      auto now = std::chrono::steady_clock::now();
      int count = 0;
      for (auto it = _requestTimes.begin(); count < 50 && it != _requestTimes.end(); ++it) {
        if (chrono::duration_cast<chrono::milliseconds>(now - it->second).count() > 1000) {
          timeouts.push_back(it->first);
          count++;
        }
      }
      for (auto id: timeouts) {
        timeoutRequest(id);
      }
    }
  }
}

void RpcClientBase::makeRequest(uint64_t requestId, flatrpc::rpc::RPCType type, string callName, std::vector<signed char> req) {
  thread_local LocalSocket socket(_context, zmqpp::socket_type::dealer);

  auto buf = packInt(requestId, type, callName, req);

  zmqpp::message msg;
  msg << "";
  msg << buf;
  socket.send(msg);
}
