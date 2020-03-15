/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <string>

#include <zmqpp/zmqpp.hpp>

#include "rpcsockets.hpp"
#include "rpcrunnable.hpp"

#include "rpc_generated.h"

#pragma once

#define LOG_MSG(MSG, PREFIX) { \
  int c = msg.read_cursor(); \
  msg.reset_read_cursor(); \
  for (int i = 0; i < MSG.parts(); i++) { \
    string part; \
    MSG >> part; \
    PLOG_DEBUG << PREFIX << " part " << part.length() << " " << part; \
  } \
  MSG.reset_read_cursor(); \
  for (int i = 0; i < c; i++) \
    msg.next(); \
}

class RpcBase : public RpcSockets, public virtual RpcRunnable {
protected:
  RpcBase(zmqpp::context &ctx, zmqpp::socket_type type);

  using TIntRpc = flatrpc::rpc::RPC;
  using TIntBuilder = typename TIntRpc::Builder;
  using TIntNative = typename TIntRpc::NativeTableType;
  using TIntExceptionPtr = typeof(TIntNative::exception);
  using TIntException = typename TIntExceptionPtr::element_type;
  using TIntNativePtr = std::shared_ptr<TIntNative>;
  using TIntNativeFuture = std::future<TIntNativePtr>;
  using TIntNativePromise = std::promise<TIntNativePtr>;
public:
  using TData = std::string;

  class exception : public std::exception {
  public:
    exception(const std::string &what);

    const char *what();

  private:
    const std::string _what;
  };

  virtual bool connect(std::string connStr) = 0;

  virtual void run(size_t workers = 10);
  virtual void stop();
protected:
  void joinReactor();
  virtual void workerThread() = 0;

  TIntNativePtr unpackInt(const std::vector<unsigned char> &&data);
  const TIntRpc *unpackInt(const std::vector<unsigned char> &data);

  std::vector<unsigned char> packInt(uint64_t requestId, flatrpc::rpc::RPCType type, const std::string& name, std::vector<signed char> data);
  std::vector<unsigned char> packInt(uint64_t requestId, flatrpc::rpc::RPCType type, const std::string& name, std::exception &exception);

  flatrpc::rpc::RPCType getReplyType(flatrpc::rpc::RPCType type);
  TIntNativePtr makeReply(TIntNativePtr req);

  zmqpp::context &_context;
  zmqpp::socket _socket;
  int _fd = -1;
private:
  std::vector<std::thread> _requestThreads;
  std::thread _reactorThread;
  std::recursive_mutex _mutex;
  friend class RpcServerBase;
  template<class TRpc>
  friend class RpcServer;
  friend class RpcClientBase;
  template<class TRpc>
  friend class RpcClient;
};
