#pragma once

#include "rpcbase.hpp"
#include "rpc_generated.h"

template<class TRpc>
class RpcTypedBase {
protected:
  using TIntRpc = quteos::rpc::RPC;
  using TIntBuilder = typename TIntRpc::Builder;
  using TIntNative = typename TIntRpc::NativeTableType;
  using TIntExceptionPtr = typeof(TIntNative::exception);
  using TIntException = typename TIntExceptionPtr::element_type;
  using TIntNativePtr = std::shared_ptr<TIntNative>;
  using TIntNativeFuture = std::future<TIntNativePtr>;
  using TIntNativePromise = std::promise<TIntNativePtr>;

public:
  using TBuilder = typename TRpc::Builder;
  using TNative = typename TRpc::NativeTableType;
  using TNativePtr = std::shared_ptr<TNative>;
  using TNativeFuture = std::future<TNativePtr>;
  using TNativePromise = std::promise<TNativePtr>;
  struct TNativeData {
    TIntNativePtr intData;
    TNativePtr data;
    std::vector<std::string> routing;
  };
  using TNativeDataPtr = std::shared_ptr<TNativeData>;
  using TNativeDataFuture = std::future<TNativeDataPtr>;
  using TNativeDataPromise = std::promise<TNativeDataPtr>;
  struct TNativeFutureData {
    TIntNativePtr intData;
    TNativeFuture data;
    std::vector<std::string> routing;
  };
  using TNativeFutureDataPtr = std::shared_ptr<TNativeFutureData>;
  using TNativeFutureDataFuture = std::future<TNativeFutureDataPtr>;
  using TNativeFutureDataPromise = std::promise<TNativeFutureDataPtr>;

protected:

  TIntNativePtr unpackInt(std::string &&data) {
    TIntNativePtr ptr;
    flatbuffers::Verifier verifier((uint8_t *)data.data(), data.size());

    if (!verifier.VerifyBuffer<TIntRpc>(nullptr)) {
      PLOG_ERROR << "Error verifying buffer";
      return ptr;
    }

    auto rpc = flatbuffers::GetRoot<TIntRpc>(data.data());
    ptr = std::make_shared<TIntNative>();
    rpc->UnPackTo(ptr.get());

    return ptr;
  }

  std::string packInt(TIntNativePtr nativePtr) {
    flatbuffers::FlatBufferBuilder fbb;
    flatbuffers::Offset<TIntRpc> rep;

    rep = TIntRpc::Pack(fbb, nativePtr.get());
    fbb.Finish(rep);

    std::string str(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    return str;
  }

  TNativeDataPtr unpack(std::string &&data) {
    TIntNativePtr intData;
    TNativeDataPtr ptr;
    if (!(intData = unpackInt(std::move(data)))) {
      return ptr;
    }

    flatbuffers::Verifier verifier((uint8_t *)intData->data.data(), intData->data.size());

    if (!verifier.VerifyBuffer<TRpc>(nullptr)) {
      PLOG_ERROR << "Error verifying request buffer";
      return ptr;
    }
    auto rpc = flatbuffers::GetRoot<TRpc>(intData->data.data());
    ptr = std::make_shared<TNativeData>();
    ptr->intData = intData;
    ptr->data = std::make_shared<TNative>();
    rpc->UnPackTo(ptr->data.get());

    return ptr;
  }

  std::string pack(TNativeDataPtr nativePtr) {
    PLOG_DEBUG << "Packing " << nativePtr->intData->requestId;

    if (!nativePtr->data) {
      return packInt(nativePtr->intData);
    }
    flatbuffers::FlatBufferBuilder fbb;
    flatbuffers::Offset<TRpc> rep;

    rep = TRpc::Pack(fbb, nativePtr->data.get());
    fbb.Finish(rep);

    nativePtr->intData->data = std::vector<uint8_t>(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());

    PLOG_DEBUG << "Packing " << nativePtr->intData->requestId;
    return packInt(nativePtr->intData);
  }
};
