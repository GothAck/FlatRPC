#include <variant>
#include <future>
#include <memory>

#include <frozen/unordered_map.h>
#include <frozen/string.h>

#include <rpc/rpcserverbase.hpp>
#include <rpc/rpcclientbase.hpp>

#include "timeout_generated.h"

#pragma once


/* Base */

class ExpensiveBase {
public:
  using ExpensiveRequestTPtr = std::shared_ptr<examples::timeouts::ExpensiveRequestT>;
  using ExpensiveRequestTFuture = std::future<ExpensiveRequestTPtr>;
  using ExpensiveRequestTPromise = std::promise<ExpensiveRequestTPtr>;

  using ExpensiveReplyTPtr = std::shared_ptr<examples::timeouts::ExpensiveReplyT>;
  using ExpensiveReplyTFuture = std::future<ExpensiveReplyTPtr>;
  using ExpensiveReplyTPromise = std::promise<ExpensiveReplyTPtr>;

  using TRequestVar = std::variant<
    ExpensiveRequestTPtr
  >;
  using TResponseVar = std::variant<
    ExpensiveReplyTPtr
  >;

  static constexpr frozen::unordered_map<frozen::string, size_t, 1> nameToReqVarIdx = {
    {"ExpensiveMethod", 0 },
  };
  static constexpr frozen::unordered_map<frozen::string, size_t, 1> nameToResVarIdx = {
    {"ExpensiveMethod", 0 },
  };

protected:
  struct RPCCall {
    virtual std::shared_ptr<TRequestVar> getRequest() = 0;
    virtual std::shared_ptr<TResponseVar> getResponse() = 0;
  };

  struct ExpensiveMethodRPCCall : public RPCCall {
    std::shared_ptr<TRequestVar> getRequest() override {
      auto ret = std::make_shared<TRequestVar>();
      ret->emplace<0>();
      return ret;
    }
    std::shared_ptr<TResponseVar> getResponse() override {
      auto ret = std::make_shared<TResponseVar>();
      ret->emplace<0>();
      return ret;
    }
  };
};


/* Client */

class ExpensiveClient : public ExpensiveBase, public RpcClientBase {
  using TResponsePromiseVar = std::variant<
    ExpensiveReplyTPromise
  >;

public:
  using RpcClientBase::RpcClientBase;
  std::future<ExpensiveReplyTPtr> ExpensiveMethod(ExpensiveRequestTPtr &&req);

private:
  void timeoutRequest(uint64_t requestId) override;
  void handleResponse(TData &&response) override;
  uint64_t nextRequestId(TResponsePromiseVar &&prom, const std::string &callName);

  void makeRequest(std::string callName, TRequestVar, TResponsePromiseVar &&prom);

  std::recursive_mutex _requestMutex;
  uint64_t _requestId = 0;
  std::unordered_map<uint64_t, TResponsePromiseVar> _requestPromises;
};

/* ServerBase */

class ExpensiveServerBase : public ExpensiveBase, public RpcServerBase {
public:
  using RpcServerBase::RpcServerBase;
private:
  template<class TMethodT>
  std::shared_ptr<TMethodT> unpack(std::string data) {
    using TMethod = typename TMethodT::TableType;
    std::shared_ptr<TMethodT> ptr;
    flatbuffers::Verifier verifier((uint8_t *)data.data(), data.size());

    if (!verifier.VerifyBuffer<TMethod>(nullptr)) {
      PLOG_ERROR << "Error verifying buffer";
      return ptr;
    }

    auto method = flatbuffers::GetRoot<TMethod>(data.data());
    ptr = std::make_shared<TMethodT>();
    method->UnPackTo(ptr.get());

    return ptr;
  }

  template<class TMethodT>
  std::vector<signed char> pack(std::shared_ptr<TMethodT> ptr) {
    using TMethod = typename TMethodT::TableType;

    flatbuffers::FlatBufferBuilder fbb;
    flatbuffers::Offset<TMethod> rep;

    rep = TMethod::Pack(fbb, ptr.get());
    fbb.Finish(rep);

    return std::vector<signed char>(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
  }
protected:
  std::string handleRequest(std::string &&reqData, const TRouting &routing) {
    auto reqInt = unpackInt(std::move(reqData));
    auto repInt = makeReply(reqInt);

    try {
      if (reqInt->name == "ExpensiveMethod") {
        auto req = unpack<examples::timeouts::ExpensiveRequestT>({reqInt->data.data(), reqInt->data.data() + reqInt->data.size()});
        auto rep = handleExpensiveMethod(move(req), routing).get();
        repInt->data = pack(rep);
      } else
      {
        throw exception("Unknown request name");
      }
    } catch (std::exception &e) {
      repInt->exception = std::make_shared<quteos::rpc::ExceptionT>();
      repInt->exception->what = e.what();
    }

    return packInt(std::move(repInt));
  }
public:
  virtual ExpensiveReplyTFuture handleExpensiveMethod(ExpensiveRequestTPtr &&req, const TRouting &routing) = 0;
};

