/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <atomic>
#include <string>
#include <vector>

#include <zmqpp/zmqpp.hpp>

#pragma once


class RpcRunnable {
protected:
  RpcRunnable();
  std::atomic_bool running = false;
  const std::string _myId;

  static std::string genUUID();
  static std::vector<std::string> getRouting(zmqpp::message &msg, bool pop = false);
};

