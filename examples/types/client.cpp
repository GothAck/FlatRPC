#include <thread>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "types_flatrpc.hpp"

using namespace std;
using namespace examples::simple;

zmqpp::context ctx;
TypesClient client(ctx);

int main(int argc, char *argv[]) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  client.connect("unix:///tmp/typesserver");

  thread clientThread([] {
    client.run(); // Run the client event loop
  });

  try {
    PLOG_INFO << "Bool request";
    PLOG_INFO << "Reply: " << (int) client.Bool(true).get()->bool_field;
    PLOG_INFO << "Byte request";
    PLOG_INFO << "Reply: " << (int) client.Byte(-1).get()->byte_field;
    PLOG_INFO << "Ubyte request";
    PLOG_INFO << "Reply: " << (int) client.Ubyte(-1).get()->ubyte_field;
    PLOG_INFO << "Short request";
    PLOG_INFO << "Reply: " << client.Short(-1).get()->short_field;
    PLOG_INFO << "Ushort request";
    PLOG_INFO << "Reply: " << client.Ushort(-1).get()->ushort_field;
    PLOG_INFO << "Int request";
    PLOG_INFO << "Reply: " << client.Int(-1).get()->int_field;
    PLOG_INFO << "Uint request";
    PLOG_INFO << "Reply: " << client.Uint(-1).get()->uint_field;
    PLOG_INFO << "Long request";
    PLOG_INFO << "Reply: " << client.Long(-1).get()->long_field;
    PLOG_INFO << "Ulong request";
    PLOG_INFO << "Reply: " << client.Ulong(-1).get()->ulong_field;
    PLOG_INFO << "Float request";
    PLOG_INFO << "Reply: " << client.Float(-1).get()->float_field;
    PLOG_INFO << "Double request";
    PLOG_INFO << "Reply: " << client.Double(-1).get()->double_field;
    PLOG_INFO << "String request";
    PLOG_INFO << "Reply: " << client.String("Hello World").get()->string_field;
  } catch (std::exception &e) {
    PLOG_ERROR << "An exception " << e.what() << " occurred during one of our calls";
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
