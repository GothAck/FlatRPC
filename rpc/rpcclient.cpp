#include <pthread.h>

#include "rpcclientbase.hpp"

using namespace std;
using namespace zmqpp;

inline bool coinflip(float occ) {
  return rand() < (RAND_MAX * occ);
}

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
