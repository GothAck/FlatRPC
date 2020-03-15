/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <pthread.h>

#include "rpcserverbase.hpp"

using namespace std;
using namespace zmqpp;

RpcServerBase::RpcServerBase(zmqpp::context &context) :
  RpcBase(context, zmqpp::socket_type::router)
  {}

void RpcServerBase::run(size_t workers) {
  RpcBase::run(workers);
  joinThreads();
}

void RpcServerBase::workerThread() {
  pthread_setname_np(pthread_self(), __func__);
  zmqpp::socket backend(_context, socket_type::router);
  backend.connect("inproc://backend");

  reactor react;
  react.add(backend, [this, &backend] {
    string buf;
    message msg;

    backend.receive(msg);

    auto routing = getRouting(msg, true);
    assert(msg.parts() == 1);
    msg >> buf; // Read message from buffer

    vector<unsigned char> bufVec(buf.data(), buf.data() + buf.size());

    auto repVec = handleRequest(move(bufVec), routing);

    string rep(repVec.data(), repVec.data() + repVec.size());

    message repMsg;
    for (auto &addr : routing) {
      repMsg << addr;
    }
    repMsg << "";
    repMsg << rep;
    backend.send(repMsg);
  });

  while (running.load()) {
    react.poll(50);
  }
}

bool RpcServerBase::connect(std::string connStr) {
  int fd = createSocketListen(connStr);
  if (fd < 0)
    return false;

  _socket.set(zmqpp::socket_option::use_fd, fd);
  _socket.bind("ipc://" + connStr);

  return true;
}
