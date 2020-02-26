#include "timeout_flatrpc.hpp"

using namespace std;
using namespace quteos::rpc;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct LocalSocket : zmqpp::socket {
  LocalSocket(zmqpp::context &ctx, zmqpp::socket_type type) : zmqpp::socket(ctx, type) {
    connect("inproc://frontend");
  }
  ~LocalSocket() {
    disconnect("inproc://frontend");
  }
};


future<ExpensiveClient::ExpensiveReplyTPtr>
ExpensiveClient::ExpensiveMethod(
  ExpensiveRequestTPtr &&req
) {
  promise<ExpensiveReplyTPtr> prom;
  auto fut = prom.get_future();
  makeRequest("ExpensiveMethod", req, move(prom));
  return fut;

}

void ExpensiveClient::timeoutRequest(uint64_t requestId) {
  lock_guard<recursive_mutex> lock(_requestMutex);
  if (_requestPromises.contains(requestId)) {
    visit(overloaded {
      [&](ExpensiveReplyTPromise &prom) {
        prom.set_exception(std::make_exception_ptr(exception("timeout")));
        PLOG_WARNING << "Request id " << requestId << " timed out";
      },
    }, _requestPromises[requestId]);
    _requestPromises.erase(requestId);
  }
  _requestTimes.erase(requestId);
}

void ExpensiveClient::handleResponse(TData &&response) {
  auto nativeRes = unpackInt(move(response));
  auto requestId = nativeRes->requestId;
  TResponsePromiseVar resPromiseVar;
  string callName;

  {
    lock_guard<recursive_mutex> lock(_requestMutex);
    if (!_requestPromises.contains(requestId)) {
      PLOG_ERROR << "Unknown request id";
      return;
    }
    resPromiseVar.swap(_requestPromises[requestId]);
    callName = _requestCallNames[requestId];
    _requestPromises.erase(requestId);
    _requestCallNames.erase(requestId);
  }

  if (callName != nativeRes->name) {
    visit(overloaded {
      [&](ExpensiveReplyTPromise &prom) {
        prom.set_exception(std::make_exception_ptr(exception("call name mismatch")));
        PLOG_WARNING << "Request id " << requestId << " call name mismatch";
      },
    }, resPromiseVar);
    return;
  }

  if (nativeRes->exception) {
    visit(overloaded {
      [&](ExpensiveReplyTPromise &prom) {
        prom.set_exception(std::make_exception_ptr(exception(nativeRes->exception->what)));
      },
    }, resPromiseVar);
    return;
  }

  auto data = nativeRes->data;

  visit(overloaded {
    [&](ExpensiveReplyTPromise &prom) {
      ExpensiveReplyTPtr ptr;
      flatbuffers::Verifier verifier((uint8_t *)data.data(), data.size());

      if (!verifier.VerifyBuffer<examples::timeouts::ExpensiveReply>(nullptr)) {
        PLOG_ERROR << "Error verifying buffer";
        prom.set_exception(std::make_exception_ptr(exception("Invalid flatbuf data")));
        return;
      }

      auto rpc = flatbuffers::GetRoot<examples::timeouts::ExpensiveReply>(data.data());
      ptr = std::make_shared<examples::timeouts::ExpensiveReplyT>();
      rpc->UnPackTo(ptr.get());
      prom.set_value(ptr);
    },
  }, resPromiseVar);
}

uint64_t ExpensiveClient::nextRequestId(
  TResponsePromiseVar &&prom,
  const string &callName
) {
  lock_guard<recursive_mutex> lock(_requestMutex);
  uint64_t requestId = _requestId++;
  if (_requestPromises.contains(requestId)) {
    timeoutRequest(requestId);
  }
  _requestTimes[requestId] = chrono::steady_clock::now();
  _requestPromises[requestId] = move(prom);
  _requestCallNames[requestId] = callName;
  return requestId;
}

void ExpensiveClient::makeRequest(
  string callName,
  TRequestVar req,
  TResponsePromiseVar &&resProm
) {
  thread_local LocalSocket socket(_context, zmqpp::socket_type::dealer);
  auto nativeReq = make_shared<TIntNative>();
  nativeReq->requestId = nextRequestId(move(resProm), callName);
  nativeReq->type = RPCType::CLIENT_REQ;
  nativeReq->name = callName;
  nativeReq->data = visit(overloaded {
    [&](ExpensiveRequestTPtr &ptr) {
      flatbuffers::FlatBufferBuilder fbb;

      auto rep = examples::timeouts::ExpensiveRequest::Pack(fbb, ptr.get());
      fbb.Finish(rep);

      return vector<signed char>(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    },
  }, req);

  auto buf = packInt(nativeReq);
  zmqpp::message msg;
  msg << "";
  msg << buf;
  socket.send(msg);
}

