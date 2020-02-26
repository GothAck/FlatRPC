#include "expensive_server.hpp"

using namespace std;
using namespace examples::timeouts;

ExpensiveServer::ExpensiveReplyTFuture ExpensiveServer::handleExpensiveMethod(
  ExpensiveRequestTPtr &&req,
  const TRouting &routing
) {
  ExpensiveReplyTPromise promise;
  auto fut = promise.get_future();
  thread([](ExpensiveRequestTPtr &&req, ExpensiveReplyTPromise &&promise) {
    auto rep = make_shared<ExpensiveReplyT>();
    PLOG_INFO << "Sleeping for " << req->milliseconds << " ms";
    this_thread::sleep_for(chrono::milliseconds(req->milliseconds));
    rep->number = req->number;
    promise.set_value(rep);
  }, move(req), move(promise)).detach();

  return fut;
}
