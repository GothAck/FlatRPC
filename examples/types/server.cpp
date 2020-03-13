#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "types_server.hpp"

zmqpp::context ctx;
TypesServer server(ctx);

int main(int argc, char *argv[]) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  server.connect("unix+listen:///tmp/typesserver");
  server.run();
  return 0;
}
