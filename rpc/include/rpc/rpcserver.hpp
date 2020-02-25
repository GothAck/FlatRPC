#include <future>

#include <flatbuffers/flatbuffers.h>
#include <plog/Log.h>

#include "rpcserverbase.hpp"
#include "rpctypedbase.hpp"

#pragma once

template<class TRpc>
class RpcServer : public RpcServerBase, public RpcTypedBase<TRpc> {
public:
  using RpcServerBase::RpcServerBase;
  using TNativePtr = typename RpcTypedBase<TRpc>::TNativePtr;
  using TIntNative = typename RpcTypedBase<TRpc>::TIntNative;
  using TIntException = typename RpcTypedBase<TRpc>::TIntException;
  using TNativeData = typename RpcTypedBase<TRpc>::TNativeData;
  using TNativeDataPtr = typename RpcTypedBase<TRpc>::TNativeDataPtr;
  using TNativeFuture = typename RpcTypedBase<TRpc>::TNativeFuture;
  using TNativePromise = typename RpcTypedBase<TRpc>::TNativePromise;
  using TNativeDataPromise = typename RpcTypedBase<TRpc>::TNativeDataPromise;
  using TNativeDataFuture = typename RpcTypedBase<TRpc>::TNativeDataFuture;
  using TNativeFutureData = typename RpcTypedBase<TRpc>::TNativeFutureData;
  using TNativeFutureDataPtr = typename RpcTypedBase<TRpc>::TNativeFutureDataPtr;

  TData handleRequest(TData &&data, std::vector<std::string> routing) override {
    using namespace std;

    PLOG_DEBUG << "handleRequest routing:";
    for (auto &addr : routing) PLOG_DEBUG << addr.size() << addr;
    auto rpcNative = unpack(move(data));
    if (!rpcNative)
      return {};

    rpcNative->routing = routing;

    try {
      auto replyNativeFuture = requestRouter(rpcNative);
      auto replyNative = make_shared<TNativeData>();
      replyNative->intData = replyNativeFuture->intData;
      replyNative->data = replyNativeFuture->data.get();
      replyNative->routing = replyNativeFuture->routing;
      return pack(replyNative);
    } catch (exception &e) {
      PLOG_ERROR << "Exception in server handler " << e.what();
      auto replyNative = make_shared<TNativeData>();
      replyNative->intData = make_shared<TIntNative>();
      replyNative->intData->exception = make_shared<TIntException>();
      replyNative->intData->exception->what = e.what();
      return pack(replyNative);
    }
  }


protected:
  using RpcTypedBase<TRpc>::pack;
  using RpcTypedBase<TRpc>::unpack;
  virtual TNativeFutureDataPtr requestRouter(TNativeDataPtr method) = 0;
};