/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <sys/un.h>

#include <filesystem>
#include <optional>
#include <utility>
#include <regex>

#include <plog/Log.h>

#include "rpcsockets.hpp"

using namespace std;
namespace fs = std::filesystem;

regex protoDelim("://");
regex colonDelim(":");
regex listenDelim("\\+listen$");

const unordered_map<RpcSockets::SocketType, const char *> RpcSockets::SocketTypeToName = {
  {SocketType::UNIX, "unix"},
  {SocketType::TCP, "tcp"},
  {SocketType::UDP, "udp"},
  {SocketType::VSOCK, "vsock"},
};

optional<tuple<RpcSockets::SocketType, string, bool>> RpcSockets::determineSocketTypeNameListen(string addr) {
  string proto;
  bool listen = false;
  {
    std::smatch sm;
    if (!regex_search(addr, sm, protoDelim)) {
      return nullopt;
    }
    proto = sm.prefix();
    addr = sm.suffix();
    if (regex_search(proto, sm, listenDelim)) {
      listen = true;
      proto = sm.prefix();
    }
  }

  PLOG_WARNING << "proto " << proto;
  PLOG_WARNING << "addr " << addr;
  PLOG_WARNING << "listen " << listen;

  for (auto &t : SocketTypeToName) {
    if (proto == t.second) {
      return make_tuple(t.first, addr, listen);
    }
  }

  return nullopt;
}

int RpcSockets::createSocket(const std::string &addr) {
  auto t = determineSocketTypeNameListen(addr);
  if (!t)
    return -1;
  {
    auto [type, addr, listen] = *t;

    if (listen) {
      return createSocketListen(type, addr);
    } else {
      return createSocketConnect(type, addr);
    }
  }
}

int RpcSockets::createSocketListen(const std::string &addr) {
  auto t = determineSocketTypeNameListen(addr);
  if (!t)
    return -1;
  {
    auto[type, addr, listen] = *t;
    return createSocketListen(type, addr);
  }
}

int RpcSockets::createSocketListen(SocketType type, const string &addr) {
  int fd;

  switch (type) {
    case SocketType::UNIX: {
      // TODO: Handle @prefix for anonymous sockets
      struct sockaddr_un sa_listen = {
        .sun_family = AF_UNIX
      };

      strncpy(sa_listen.sun_path, addr.c_str(), sizeof(sa_listen.sun_path) - 1);

      if (fs::exists(addr)) {
        fs::remove(addr);
      }

      fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
      if (fd < 0) {
        PLOG_ERROR << "Could not open sock" << errno;
        return -1;
      }
      if (::bind(fd, (struct sockaddr *)&sa_listen, sizeof(sa_listen))) {
        PLOG_ERROR << "Could not bind socket" << addr << errno;
        return -1;
      }
      if (::listen(fd, 1)) {
        PLOG_ERROR << "Could not listen sock" << errno;
        return -1;
      }
      return fd;
    }
    case SocketType::VSOCK: {
      unsigned int port = stol(addr);
      struct sockaddr_vm sa_listen = {
        .svm_family = AF_VSOCK,
        .svm_port = port,
        .svm_cid = VMADDR_CID_ANY
      };

      fd = ::socket(AF_VSOCK, SOCK_STREAM, 0);
      if (fd < 0) {
        PLOG_ERROR << "Could not open socket" << errno;
        return -1;
      }
      if (::bind(fd, (struct sockaddr *)&sa_listen, sizeof(sa_listen))) {
        PLOG_ERROR << "Could not bind socket" << errno;
        return -1;
      }
      PLOG_INFO << "Bound to cid " << sa_listen.svm_cid << " port " << sa_listen.svm_port;
      if (::listen(fd, 1)) {
        PLOG_ERROR << "Could not listen socket" << errno;
        return -1;
      }
      return fd;
    }
    case SocketType::TCP:
    case SocketType::UDP:
    default:
      return -1;
  }
}

int RpcSockets::createSocketConnect(const std::string &addr) {
  auto t = determineSocketTypeNameListen(addr);
  if (!t)
    return -1;
  {
    auto[type, addr, listen] = *t;
    return createSocketConnect(type, addr);
  }
}

int RpcSockets::createSocketConnect(RpcSockets::SocketType type, const std::string &addr) {
  int fd;

  switch (type) {
    case SocketType::UNIX: {
      struct sockaddr_un sa_connect = {
        .sun_family = AF_UNIX
      };

      strncpy(sa_connect.sun_path, addr.c_str(), sizeof(sa_connect.sun_path) - 1);

      fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
      if (fd < 0) {
        PLOG_ERROR << "Could not open socket" << errno;
        return -1;
      }

      if (::connect(fd, (struct sockaddr *) &sa_connect, sizeof(sa_connect))) {
        PLOG_ERROR << "Could not connect socket" << errno;
        return -1;
      }

      if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK)) {
        PLOG_ERROR << "Could not set socket nonblock" << errno;
        return -1;
      }
      return fd;
    }
    case SocketType::VSOCK: {
      std::smatch sm;
      unsigned int cid;
      unsigned int port;
      {
        if (!regex_search(addr, sm, colonDelim)) {
          return -1;
        }
        cid = stol(sm.prefix());
        port = stol(sm.suffix());
      }

      struct sockaddr_vm sa_connect = {
        .svm_family = AF_VSOCK,
        .svm_port = port,
        .svm_cid = cid,
      };

      fd = ::socket(AF_VSOCK, SOCK_STREAM, 0);
      if (fd < 0) {
        PLOG_ERROR << "Could not open socket" << errno;
        return -1;
      }

      if (::connect(fd, (struct sockaddr *) &sa_connect, sizeof(sa_connect))) {
        PLOG_ERROR << "Could not connect socket" << errno;
        return -1;
      }

      if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK)) {
        PLOG_ERROR << "Could not set socket nonblock" << errno;
        return -1;
      }
      return fd;
    }
    case SocketType::TCP:
    case SocketType::UDP:
    default:
      return -1;
  }
}
