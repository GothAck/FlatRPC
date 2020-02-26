#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"

#include <zmqpp/zmqpp.hpp>
#include "flatrpc/rpcrunnable.hpp"

using namespace std;
using namespace zmqpp;

struct TestRpcRunnable : RpcRunnable {
  static string genUUID() {
    return RpcRunnable::genUUID();
  }
  static vector<string> getRouting(message &msg, bool pop) {
    return RpcRunnable::getRouting(msg, pop);
  }
};

TEST_CASE("UUIDs are returned", "[getUUID]") {
  auto uuid = TestRpcRunnable::genUUID();
  REQUIRE(uuid.length() == 36);
  for (int i = 0 ; i < 10; i++) {
    REQUIRE(TestRpcRunnable::genUUID() != uuid);
  }
}

TEST_CASE("Prefixed routing is returned", "[getRouting]") {
  message msg;
  msg.push_back("Test");
  msg.push_back("123");
  msg.push_back("");
  msg.push_back("Data");

  auto routing = TestRpcRunnable::getRouting(msg, false);

  REQUIRE(routing == vector<string> {"Test", "123"});
  REQUIRE(msg.parts() == 4);
  REQUIRE(msg.read_cursor() == 3);
}

TEST_CASE("Prefixed routing is popped", "[getRouting]") {
  message msg;
  msg.push_back("Test");
  msg.push_back("123");
  msg.push_back("");
  msg.push_back("Data");

  auto routing = TestRpcRunnable::getRouting(msg, true);

  REQUIRE(routing == vector<string> {"Test", "123"});
  REQUIRE(msg.parts() == 1);
  REQUIRE(msg.read_cursor() == 0);
}

TEST_CASE("Works without data", "[getRouting]") {
  message msg;
  msg.push_back("Test");
  msg.push_back("123");
  msg.push_back("");

  auto routing = TestRpcRunnable::getRouting(msg, true);

  REQUIRE(routing == vector<string> {"Test", "123"});
  REQUIRE(msg.parts() == 0);
  REQUIRE(msg.read_cursor() == 0);
}

TEST_CASE("Works without empty frame", "[getRouting]") {
  message msg;
  msg.push_back("Test");
  msg.push_back("123");

  auto routing = TestRpcRunnable::getRouting(msg, true);

  REQUIRE(routing == vector<string> {"Test", "123"});
  REQUIRE(msg.parts() == 0);
  REQUIRE(msg.read_cursor() == 0);
}
