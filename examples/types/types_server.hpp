#include "types_flatrpc.hpp"


#pragma once


class TypesServer : public TypesServerBase {
public:
  using TypesServerBase::TypesServerBase;
protected:

  std::future<BoolTableTPtr> handleBool(BoolTableTPtr &&req, const TRouting &routing) override;
  std::future<ByteTableTPtr> handleByte(ByteTableTPtr &&req, const TRouting &routing) override;
  std::future<UbyteTableTPtr> handleUbyte(UbyteTableTPtr &&req, const TRouting &routing) override;
  std::future<ShortTableTPtr> handleShort(ShortTableTPtr &&req, const TRouting &routing) override;
  std::future<UshortTableTPtr> handleUshort(UshortTableTPtr &&req, const TRouting &routing) override;
  std::future<IntTableTPtr> handleInt(IntTableTPtr &&req, const TRouting &routing) override;
  std::future<UintTableTPtr> handleUint(UintTableTPtr &&req, const TRouting &routing) override;
  std::future<LongTableTPtr> handleLong(LongTableTPtr &&req, const TRouting &routing) override;
  std::future<UlongTableTPtr> handleUlong(UlongTableTPtr &&req, const TRouting &routing) override;
  std::future<FloatTableTPtr> handleFloat(FloatTableTPtr &&req, const TRouting &routing) override;
  std::future<DoubleTableTPtr> handleDouble(DoubleTableTPtr &&req, const TRouting &routing) override;
  std::future<StringTableTPtr> handleString(StringTableTPtr &&req, const TRouting &routing) override;
  std::future<EmptyTPtr> handleQuit(EmptyTPtr &&req, const TRouting &routing) override;
};
