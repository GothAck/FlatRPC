#include "timeout_flatrpc.hpp"


#pragma once


class ExpensiveServer : public ExpensiveServerBase {
public:
  using ExpensiveServerBase::ExpensiveServerBase;
protected:

  std::future<ExpensiveReplyTPtr> handleExpensiveMethod(ExpensiveRequestTPtr &&req, const TRouting &routing) override;
};
