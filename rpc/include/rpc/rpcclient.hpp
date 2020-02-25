#include <future>

#include <flatbuffers/flatbuffers.h>
#include <plog/Log.h>

#include "rpcclientbase.hpp"
#include "rpctypedbase.hpp"

#pragma once

template<class TRpc>
class RpcClient : public RpcClientBase, public RpcTypedBase<TRpc> {
public:
  using RpcClientBase::RpcClientBase;
  using TNativePtr = typename RpcTypedBase<TRpc>::TNativePtr;
  using TIntNative = typename RpcTypedBase<TRpc>::TIntNative;
  using TNativeData = typename RpcTypedBase<TRpc>::TNativeData;
  using TNativeDataPtr = typename RpcTypedBase<TRpc>::TNativeDataPtr;
  using TNativeFuture = typename RpcTypedBase<TRpc>::TNativeFuture;
  using TNativePromise = typename RpcTypedBase<TRpc>::TNativePromise;
  using TNativeDataPromise = typename RpcTypedBase<TRpc>::TNativeDataPromise;
  using TNativeDataFuture = typename RpcTypedBase<TRpc>::TNativeDataFuture;

  std::future<TNativeDataPtr> makeRequest(TNativePtr request) {
    using namespace std;

    auto promise = make_shared<TNativeDataPromise>();
    auto req = make_shared<TNativeData>();
    req->intData = make_shared<TIntNative>();
    req->data = request;

    size_t requestId;
    {
      lock_guard<recursive_mutex> lock(_mutex);
      requestId = _requestId++;
      if (_requestId == -1) _requestId++;

      if (_requestTimes.contains(requestId) && _requestFutures.contains(requestId)) {
        timeoutRequest(requestId);
      }

      _requestTimes[requestId] = chrono::steady_clock::now();
      _requestFutures[requestId] = promise;
    }

    req->intData->requestId = requestId;
    req->intData->type = quteos::rpc::RPCType::CLIENT_REQ;

    auto buf = pack(req);


    zmqpp::message msg;
    msg << "";
    msg << buf;
    _socket.send(msg);

    return promise->get_future();
  }


protected:
  using RpcTypedBase<TRpc>::pack;
  using RpcTypedBase<TRpc>::unpack;
  virtual void handleMessage(TNativePtr) = 0;

  void timeoutRequest(size_t id) override {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    if (_requestTimes.contains(id) && _requestFutures.contains(id)) {
      _requestFutures[id]->set_exception(std::make_exception_ptr(exception("Request timed out")));
      _requestFutures.erase(id);
    }
    _requestTimes.erase(id);
  }

  void handleResponse(TData &&response) override {
    std::shared_ptr<TNativeDataPromise> promise;
    auto reply = unpack(move(response));

    if (!reply) return;

    size_t requestId = reply->intData->requestId;
    PLOG_INFO << "Handle response" << requestId;

    // TODO: Handle non-response messages
    if (requestId == -1) {
      PLOG_ERROR << "non-response message not handled yet";
      return;
    }

    {
      std::lock_guard<std::recursive_mutex> lock(_mutex);
      if (!(_requestTimes.contains(requestId) && _requestFutures.contains(requestId))) {
        PLOG_ERROR << "Must have timed out request " << requestId;
        return;
      }
      promise = _requestFutures[requestId];
      _requestTimes.erase(requestId);
      _requestFutures.erase(requestId);
    }

    promise->set_value(reply);
  }

  std::unordered_map<size_t, std::shared_ptr<TNativeDataPromise>> _requestFutures;
  size_t _requestId = 0;
};
