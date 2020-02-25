#include <future>

#include <flatbuffers/flatbuffers.h>
#include <plog/Log.h>

#include "rpcbase.hpp"

#pragma once

// TODO: timeouts

class RpcServerBase : public RpcBase {
public:
  RpcServerBase(zmqpp::context &context);
  using TRouting = std::vector<std::string>;

  virtual TData handleRequest(TData &&data, const TRouting &addr) = 0;
  bool connect(std::string connStr) override;

  void run(size_t workers = 10) override;
protected:
  void workerThread() override;
};

