/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <future>

#include <flatbuffers/flatbuffers.h>
#include <plog/Log.h>

#include "rpcbase.hpp"

#pragma once


class RpcClientBase : public RpcBase {
public:
  RpcClientBase(zmqpp::context &ctx);

  using RpcBase::RpcBase;

  bool connect(std::string connStr) override;

protected:
  virtual void timeoutRequest(size_t id) = 0;
  void workerThread() override;
  virtual void handleResponse(TData &&response) = 0;
  std::unordered_map<size_t, std::chrono::_V2::steady_clock::time_point> _requestTimes;
  std::unordered_map<size_t, std::string> _requestCallNames;
};

