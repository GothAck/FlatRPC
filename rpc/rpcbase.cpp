/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <plog/Log.h>

#include "rpcbase.hpp"

using namespace std;
using namespace zmqpp;
using namespace flatbuffers;
using namespace flatrpc::rpc;

RpcBase::RpcBase(zmqpp::context &ctx, zmqpp
::socket_type type) :
  RpcRunnable(),
  _context(ctx),
  _socket(ctx, type)
  {
    PLOG_INFO << "My id: " << _myId;
    _socket.set(zmqpp::socket_option::identity, _myId);
  }

RpcBase::exception::exception(const string &what) :
  _what(what)
  {}

const char *RpcBase::exception::what() {
  return _what.c_str();
}

void RpcBase::run(size_t workers) {

  running.store(true);

  _reactorThread = thread([this]() {
    zmqpp::socket frontend(_context, socket_type::dealer);
    zmqpp::socket backend(_context, socket_type::dealer);
    zmqpp::socket oob(_context, socket_type::router);
    frontend.bind("inproc://frontend");
    backend.bind("inproc://backend");
    oob.bind("inproc://oob");
    zmqpp::reactor reactor;

    reactor.add(frontend, [this, &frontend] {
      message msg;
      frontend.receive(msg);
      _socket.send(msg);

    });

    reactor.add(backend, [this, &backend] {
      message msg;
      backend.receive(msg);
      _socket.send(msg);
    });

    reactor.add(oob, [this, &oob] {
      message msg;
      oob.receive(msg);
      msg.pop_front();
      _socket.send(msg);
    });

    reactor.add(_socket, [this, &backend, &oob] {
      message msg;
      _socket.receive(msg);
      auto routing = getRouting(msg);
      string buf;
      msg >> buf;
      if (find(routing.begin(), routing.end(), "OOB") == routing.end()) {
        backend.send(msg);
      } else {
        zmqpp::message copy;

        switch(routing.size()) {
          case 2:
            if (routing[0] == "OOB")
              routing = {routing[1]};
            break;
          case 3:
            if (routing[1] == "OOB")
              routing = {routing[2], routing[0]};
            break;
          default:
            PLOG_ERROR << "Unknown routing length " << routing.size();
        }

        for (auto &r: routing) {
          copy << r;
        }

        copy << "";
        copy << buf;

        oob.send(copy);
      }
    });
    while (running) {
      reactor.poll(50);
    }
  });

  for (size_t i = 0; i < workers; i++) {
    _requestThreads.push_back(move(thread(mem_fn(&RpcBase::workerThread), this)));
  }
}

RpcBase::TIntNativePtr RpcBase::unpackInt(const vector<unsigned char> &&data) {
  TIntNativePtr ptr;
  const RpcBase::TIntRpc *rpc = unpackInt(data);

  if (!rpc) {
    PLOG_ERROR << "Error verifying buffer";
    return ptr;
  }

  ptr = make_shared<TIntNative>();
  rpc->UnPackTo(ptr.get());

  return ptr;
}

const RpcBase::TIntRpc *RpcBase::unpackInt(const vector<unsigned char> &data) {
  flatbuffers::Verifier verifier((uint8_t *)data.data(), data.size());

  if (!verifier.VerifyBuffer<TIntRpc>(nullptr)) {
    PLOG_ERROR << "Error verifying buffer";
    return nullptr;
  }

  return flatbuffers::GetRoot<TIntRpc>(data.data());
}

flatrpc::rpc::RPCType RpcBase::getReplyType(flatrpc::rpc::RPCType type) {
  switch(type) {
    case flatrpc::rpc::RPCType::CLIENT_REQ:
    case flatrpc::rpc::RPCType::SERVER_REQ:
      return static_cast<flatrpc::rpc::RPCType>(
        static_cast<uint8_t>(type) << 1);
      break;
    default:
      throw exception("Cannot make reply from a reply");
  }
}

RpcBase::TIntNativePtr RpcBase::makeReply(TIntNativePtr req) {
  auto rep = make_shared<TIntNative>();
  rep->requestId = req->requestId;
  rep->type = getReplyType(req->type);
  rep->name = req->name;
  return rep;
}

void RpcBase::stop() {
  running.store(false);

  _reactorThread.join();
  for (auto &t : _requestThreads) {
    t.join();
  }

}

void RpcBase::joinReactor() {
  _reactorThread.join();
}

vector<unsigned char> RpcBase::packInt(uint64_t requestId, flatrpc::rpc::RPCType type, const string& name, vector<signed char> data) {
  FlatBufferBuilder fbb;
  auto fbbName = fbb.CreateString(name);
  auto fbbData = fbb.CreateVector<int8_t>(data);

  auto rpc = CreateRPC(fbb, requestId, type, fbbName, fbbData);
  fbb.Finish(rpc);

  return {fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize()};
}

vector<unsigned char> RpcBase::packInt(uint64_t requestId, flatrpc::rpc::RPCType type, const string& name, std::exception &exception) {
  FlatBufferBuilder fbb;
  auto fbbName = fbb.CreateString(name);
  auto fbbWhat = fbb.CreateString(exception.what());
  auto fbbException = CreateException(fbb, fbbWhat);


  auto rpc = CreateRPC(fbb, requestId, type, fbbName, 0, fbbException);
  fbb.Finish(rpc);

  return {fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize()};
}
