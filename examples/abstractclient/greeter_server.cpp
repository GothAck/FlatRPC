#include "greeter_server.hpp"

using namespace std;
using namespace examples::simple;

GreeterServer::HelloReplyTFuture GreeterServer::handleSayHello(
  HelloRequestTPtr &&req,
  const TRouting &routing
) {
  HelloReplyTPromise promise;
  auto fut = promise.get_future();
  thread([](HelloRequestTPtr &&req, HelloReplyTPromise &&promise) {
    auto rep = make_shared<HelloReplyT>();
    PLOG_INFO << "Serving request for " << req->name;
    rep->message = "Hello " + req->name + "!";
    promise.set_value(rep);
  }, move(req), move(promise)).detach();

  return fut;
}
