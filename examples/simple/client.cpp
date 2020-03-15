#include <thread>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "simple_flatrpc.hpp"

using namespace std;
using namespace examples::simple;

zmqpp::context ctx;
GreeterClient client(ctx);

int main(int argc, char *argv[]) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  if (argc == 1) {
    PLOG_ERROR << "Please call this binary with your name as the first argument";
    return 1;
  }

  client.connect("unix:///tmp/greeterserver");

  thread clientThread([] {
    client.run(); // Run the client event loop
  });

  auto req = make_shared<HelloRequestT>();
  req->name = argv[1];
  try {
    auto rep = client.SayHello(move(req)).get();
    PLOG_INFO << "Reply: " << rep->message;
  } catch (exception &e) {
    PLOG_ERROR << "Exception: " << e.what();
    return 1;
  }

  try {
    client.Quit().get();
  } catch (std::exception &e) {
    PLOG_INFO << e.what() << " whilst calling quit";
  }

  client.stop();
  clientThread.join();
  return 0;
}
