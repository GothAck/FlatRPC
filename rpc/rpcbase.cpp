#include <plog/Log.h>

#include "rpcbase.hpp"

using namespace std;
using namespace zmqpp;

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
      LOG_MSG(msg, "OOB");
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
        PLOG_WARNING << "Routing oob packet";
        LOG_MSG(msg, "pre");
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

        LOG_MSG(copy, "post");
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

RpcBase::TIntNativePtr RpcBase::unpackInt(std::string &&data) {
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

std::string RpcBase::packInt(RpcBase::TIntNativePtr nativePtr) {
  flatbuffers::FlatBufferBuilder fbb;
  flatbuffers::Offset<TIntRpc> rep;

  rep = TIntRpc::Pack(fbb, nativePtr.get());
  fbb.Finish(rep);

  std::string str(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
  return str;
}

RpcBase::TIntNativePtr RpcBase::makeReply(TIntNativePtr req) {
  auto rep = make_shared<TIntNative>();
  rep->requestId = req->requestId;
  cerr << static_cast<int>(req->type) << endl;
  switch(req->type) {
    case quteos::rpc::RPCType::CLIENT_REQ:
    case quteos::rpc::RPCType::SERVER_REQ:
      rep->type = static_cast<quteos::rpc::RPCType>(
        static_cast<uint8_t>(req->type) << 1);
      break;
    default:
      throw exception("Cannot make reply from a reply"); //
  }
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
