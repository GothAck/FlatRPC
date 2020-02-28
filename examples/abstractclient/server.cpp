#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "greeter_server.hpp"

zmqpp::context ctx;
GreeterServer server(ctx);

int main(int argc, char *argv[]) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  server.connect("unix+listen:///tmp/greeterserver");
  server.run();
  return 0;
}
