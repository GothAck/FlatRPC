#include "simple_flatrpc.hpp"


#pragma once


class GreeterServer : public GreeterServerBase {
public:
  using GreeterServerBase::GreeterServerBase;
protected:

  std::future<HelloReplyTPtr> handleSayHello(HelloRequestTPtr &&req, const TRouting &routing) override;
};
