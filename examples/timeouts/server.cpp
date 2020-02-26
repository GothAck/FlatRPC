#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "expensive_server.hpp"

zmqpp::context ctx;
ExpensiveServer server(ctx);

int main(int argc, char *argv[]) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  server.connect("unix+listen:///tmp/timeoutsserver");
  server.run(200);
  return 0;
}
