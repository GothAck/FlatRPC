#include <thread>
#include <deque>
#include <utility>
#include <array>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "timeout_flatrpc.hpp"

using namespace std;
using namespace examples::timeouts;

zmqpp::context ctx;
ExpensiveClient client(ctx);

int main(int argc, char *argv[]) {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  client.connect("unix:///tmp/timeoutsserver");

  thread clientThread([] {
    client.run(100); // Run the client event loop
  });

  deque<pair<int, int>> calls;
  mutex call_mutex;
  int expected_timeouts = 0;
  int expected_calls = 0;
  int num_timeouts = 0;
  int num_success = 0;
  int num_calls = 0;

  for (int i = 0; i < 5000; i++) {
    uint32_t milliseconds = rand() * (2000.0 / RAND_MAX);
    if (milliseconds >= 1000)
      expected_timeouts++;
    expected_calls++;
    calls.emplace_back(i, milliseconds);
  }

  array<thread, 100> threads;

  for (int i = 0; i < threads.size(); i++) {
    threads[i] = thread([&] {
      pair<int, int> call;
      while (true) {
        {
          lock_guard<mutex> lock(call_mutex);
          if (!calls.size())
            return;
          call = calls.at(0);
          calls.pop_front();
          num_calls++;
        }
        auto req = make_shared<ExpensiveRequestT>();
        req->number = call.first;
        req->milliseconds = call.second;
        PLOG_INFO << "Making request " << call.first << " ms " << call.second;
        try {
          client.ExpensiveMethod(move(req)).get();
          {
            lock_guard<mutex> lock(call_mutex);
            num_success++;
          }
        } catch (exception &e) {
          lock_guard<mutex> lock(call_mutex);
          num_timeouts++;
        }
      }
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  PLOG_ERROR << "Finished";
  PLOG_ERROR << "calls " << num_calls << " / " << expected_calls;
  PLOG_ERROR << "timeouts " << num_timeouts << " / " << expected_timeouts;
  PLOG_ERROR << "success " << num_success << " vs timeouts " << num_timeouts << " == " << (num_success + num_timeouts);


  client.stop();
  clientThread.join();
  return 0;
}
