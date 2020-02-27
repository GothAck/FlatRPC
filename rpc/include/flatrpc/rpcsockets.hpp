#include <string>
#include <optional>

#pragma once

class RpcSockets {
public:
  enum class SocketType {
    UNIX = 0,
    TCP,
    UDP,
    VSOCK,
  };

  static const std::unordered_map<SocketType, const char *> SocketTypeToName;

  static std::optional<std::tuple<SocketType, std::string, bool>> determineSocketTypeNameListen(std::string addr);

  static int createSocket(const std::string &addr);
  static int createSocketListen(const std::string &addr);
  static int createSocketListen(SocketType type, const std::string &addr);
  static int createSocketConnect(const std::string &addr);
  static int createSocketConnect(SocketType type, const std::string &addr);
};
