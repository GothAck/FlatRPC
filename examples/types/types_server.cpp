#include "types_server.hpp"

using namespace std;
using namespace examples::simple;

TypesServer::BoolTableTFuture TypesServer::handleBool(BoolTableTPtr &&req, const TRouting &routing) {
  BoolTableTPromise prom;
  auto rep = make_shared<BoolTableT>();

  rep->bool_field = req->bool_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::ByteTableTFuture TypesServer::handleByte(ByteTableTPtr &&req, const TRouting &routing) {
  ByteTableTPromise prom;
  auto rep = make_shared<ByteTableT>();

  rep->byte_field = req->byte_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::UbyteTableTFuture TypesServer::handleUbyte(UbyteTableTPtr &&req, const TRouting &routing) {
  UbyteTableTPromise prom;
  auto rep = make_shared<UbyteTableT>();

  rep->ubyte_field = req->ubyte_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::ShortTableTFuture TypesServer::handleShort(ShortTableTPtr &&req, const TRouting &routing) {
  ShortTableTPromise prom;
  auto rep = make_shared<ShortTableT>();

  rep->short_field = req->short_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::UshortTableTFuture TypesServer::handleUshort(UshortTableTPtr &&req, const TRouting &routing) {
  UshortTableTPromise prom;
  auto rep = make_shared<UshortTableT>();

  rep->ushort_field = req->ushort_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::IntTableTFuture TypesServer::handleInt(IntTableTPtr &&req, const TRouting &routing) {
  IntTableTPromise prom;
  auto rep = make_shared<IntTableT>();

  rep->int_field = req->int_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::UintTableTFuture TypesServer::handleUint(UintTableTPtr &&req, const TRouting &routing) {
  UintTableTPromise prom;
  auto rep = make_shared<UintTableT>();

  rep->uint_field = req->uint_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::LongTableTFuture TypesServer::handleLong(LongTableTPtr &&req, const TRouting &routing) {
  LongTableTPromise prom;
  auto rep = make_shared<LongTableT>();

  rep->long_field = req->long_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::UlongTableTFuture TypesServer::handleUlong(UlongTableTPtr &&req, const TRouting &routing) {
  UlongTableTPromise prom;
  auto rep = make_shared<UlongTableT>();

  rep->ulong_field = req->ulong_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::FloatTableTFuture TypesServer::handleFloat(FloatTableTPtr &&req, const TRouting &routing) {
  FloatTableTPromise prom;
  auto rep = make_shared<FloatTableT>();

  rep->float_field = req->float_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::DoubleTableTFuture TypesServer::handleDouble(DoubleTableTPtr &&req, const TRouting &routing) {
  DoubleTableTPromise prom;
  auto rep = make_shared<DoubleTableT>();

  rep->double_field = req->double_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::StringTableTFuture TypesServer::handleString(StringTableTPtr &&req, const TRouting &routing) {
  StringTableTPromise prom;
  auto rep = make_shared<StringTableT>();

  rep->string_field = req->string_field;

  prom.set_value(rep);

  return prom.get_future();
}

TypesServer::EmptyTFuture TypesServer::handleQuit(EmptyTPtr &&req, const TRouting &routing) {
  EmptyTPromise prom;
  auto rep = make_shared<EmptyT>();
  prom.set_value(rep);

  stop();

  return prom.get_future();
}

