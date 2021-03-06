// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TIMEOUT_EXAMPLES_TIMEOUTS_H_
#define FLATBUFFERS_GENERATED_TIMEOUT_EXAMPLES_TIMEOUTS_H_

#include "flatbuffers/flatbuffers.h"

namespace examples {
namespace timeouts {

struct ExpensiveReply;
struct ExpensiveReplyBuilder;
struct ExpensiveReplyT;

struct ExpensiveRequest;
struct ExpensiveRequestBuilder;
struct ExpensiveRequestT;

struct Empty;
struct EmptyBuilder;
struct EmptyT;

bool operator==(const ExpensiveReplyT &lhs, const ExpensiveReplyT &rhs);
bool operator!=(const ExpensiveReplyT &lhs, const ExpensiveReplyT &rhs);
bool operator==(const ExpensiveRequestT &lhs, const ExpensiveRequestT &rhs);
bool operator!=(const ExpensiveRequestT &lhs, const ExpensiveRequestT &rhs);
bool operator==(const EmptyT &lhs, const EmptyT &rhs);
bool operator!=(const EmptyT &lhs, const EmptyT &rhs);

inline const flatbuffers::TypeTable *ExpensiveReplyTypeTable();

inline const flatbuffers::TypeTable *ExpensiveRequestTypeTable();

inline const flatbuffers::TypeTable *EmptyTypeTable();

struct ExpensiveReplyT : public flatbuffers::NativeTable {
  typedef ExpensiveReply TableType;
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "examples.timeouts.ExpensiveReplyT";
  }
  uint32_t number;
  ExpensiveReplyT()
      : number(0) {
  }
};

inline bool operator==(const ExpensiveReplyT &lhs, const ExpensiveReplyT &rhs) {
  return
      (lhs.number == rhs.number);
}

inline bool operator!=(const ExpensiveReplyT &lhs, const ExpensiveReplyT &rhs) {
    return !(lhs == rhs);
}


struct ExpensiveReply FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ExpensiveReplyT NativeTableType;
  typedef ExpensiveReplyBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return ExpensiveReplyTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "examples.timeouts.ExpensiveReply";
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NUMBER = 4
  };
  uint32_t number() const {
    return GetField<uint32_t>(VT_NUMBER, 0);
  }
  bool mutate_number(uint32_t _number) {
    return SetField<uint32_t>(VT_NUMBER, _number, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_NUMBER) &&
           verifier.EndTable();
  }
  ExpensiveReplyT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ExpensiveReplyT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ExpensiveReply> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveReplyT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ExpensiveReplyBuilder {
  typedef ExpensiveReply Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_number(uint32_t number) {
    fbb_.AddElement<uint32_t>(ExpensiveReply::VT_NUMBER, number, 0);
  }
  explicit ExpensiveReplyBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ExpensiveReplyBuilder &operator=(const ExpensiveReplyBuilder &);
  flatbuffers::Offset<ExpensiveReply> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ExpensiveReply>(end);
    return o;
  }
};

inline flatbuffers::Offset<ExpensiveReply> CreateExpensiveReply(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t number = 0) {
  ExpensiveReplyBuilder builder_(_fbb);
  builder_.add_number(number);
  return builder_.Finish();
}

struct ExpensiveReply::Traits {
  using type = ExpensiveReply;
  static auto constexpr Create = CreateExpensiveReply;
};

flatbuffers::Offset<ExpensiveReply> CreateExpensiveReply(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveReplyT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct ExpensiveRequestT : public flatbuffers::NativeTable {
  typedef ExpensiveRequest TableType;
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "examples.timeouts.ExpensiveRequestT";
  }
  uint32_t milliseconds;
  uint32_t number;
  ExpensiveRequestT()
      : milliseconds(0),
        number(0) {
  }
};

inline bool operator==(const ExpensiveRequestT &lhs, const ExpensiveRequestT &rhs) {
  return
      (lhs.milliseconds == rhs.milliseconds) &&
      (lhs.number == rhs.number);
}

inline bool operator!=(const ExpensiveRequestT &lhs, const ExpensiveRequestT &rhs) {
    return !(lhs == rhs);
}


struct ExpensiveRequest FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ExpensiveRequestT NativeTableType;
  typedef ExpensiveRequestBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return ExpensiveRequestTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "examples.timeouts.ExpensiveRequest";
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MILLISECONDS = 4,
    VT_NUMBER = 6
  };
  uint32_t milliseconds() const {
    return GetField<uint32_t>(VT_MILLISECONDS, 0);
  }
  bool mutate_milliseconds(uint32_t _milliseconds) {
    return SetField<uint32_t>(VT_MILLISECONDS, _milliseconds, 0);
  }
  uint32_t number() const {
    return GetField<uint32_t>(VT_NUMBER, 0);
  }
  bool mutate_number(uint32_t _number) {
    return SetField<uint32_t>(VT_NUMBER, _number, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_MILLISECONDS) &&
           VerifyField<uint32_t>(verifier, VT_NUMBER) &&
           verifier.EndTable();
  }
  ExpensiveRequestT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ExpensiveRequestT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ExpensiveRequest> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveRequestT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ExpensiveRequestBuilder {
  typedef ExpensiveRequest Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_milliseconds(uint32_t milliseconds) {
    fbb_.AddElement<uint32_t>(ExpensiveRequest::VT_MILLISECONDS, milliseconds, 0);
  }
  void add_number(uint32_t number) {
    fbb_.AddElement<uint32_t>(ExpensiveRequest::VT_NUMBER, number, 0);
  }
  explicit ExpensiveRequestBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ExpensiveRequestBuilder &operator=(const ExpensiveRequestBuilder &);
  flatbuffers::Offset<ExpensiveRequest> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ExpensiveRequest>(end);
    return o;
  }
};

inline flatbuffers::Offset<ExpensiveRequest> CreateExpensiveRequest(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t milliseconds = 0,
    uint32_t number = 0) {
  ExpensiveRequestBuilder builder_(_fbb);
  builder_.add_number(number);
  builder_.add_milliseconds(milliseconds);
  return builder_.Finish();
}

struct ExpensiveRequest::Traits {
  using type = ExpensiveRequest;
  static auto constexpr Create = CreateExpensiveRequest;
};

flatbuffers::Offset<ExpensiveRequest> CreateExpensiveRequest(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveRequestT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct EmptyT : public flatbuffers::NativeTable {
  typedef Empty TableType;
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "examples.timeouts.EmptyT";
  }
  EmptyT() {
  }
};

inline bool operator==(const EmptyT &, const EmptyT &) {
  return true;
}

inline bool operator!=(const EmptyT &lhs, const EmptyT &rhs) {
    return !(lhs == rhs);
}


struct Empty FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef EmptyT NativeTableType;
  typedef EmptyBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return EmptyTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "examples.timeouts.Empty";
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
  EmptyT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(EmptyT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Empty> Pack(flatbuffers::FlatBufferBuilder &_fbb, const EmptyT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct EmptyBuilder {
  typedef Empty Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  explicit EmptyBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  EmptyBuilder &operator=(const EmptyBuilder &);
  flatbuffers::Offset<Empty> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Empty>(end);
    return o;
  }
};

inline flatbuffers::Offset<Empty> CreateEmpty(
    flatbuffers::FlatBufferBuilder &_fbb) {
  EmptyBuilder builder_(_fbb);
  return builder_.Finish();
}

struct Empty::Traits {
  using type = Empty;
  static auto constexpr Create = CreateEmpty;
};

flatbuffers::Offset<Empty> CreateEmpty(flatbuffers::FlatBufferBuilder &_fbb, const EmptyT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline ExpensiveReplyT *ExpensiveReply::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  std::unique_ptr<examples::timeouts::ExpensiveReplyT> _o = std::unique_ptr<examples::timeouts::ExpensiveReplyT>(new ExpensiveReplyT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ExpensiveReply::UnPackTo(ExpensiveReplyT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = number(); _o->number = _e; }
}

inline flatbuffers::Offset<ExpensiveReply> ExpensiveReply::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveReplyT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateExpensiveReply(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ExpensiveReply> CreateExpensiveReply(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveReplyT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ExpensiveReplyT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _number = _o->number;
  return examples::timeouts::CreateExpensiveReply(
      _fbb,
      _number);
}

inline ExpensiveRequestT *ExpensiveRequest::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  std::unique_ptr<examples::timeouts::ExpensiveRequestT> _o = std::unique_ptr<examples::timeouts::ExpensiveRequestT>(new ExpensiveRequestT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ExpensiveRequest::UnPackTo(ExpensiveRequestT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = milliseconds(); _o->milliseconds = _e; }
  { auto _e = number(); _o->number = _e; }
}

inline flatbuffers::Offset<ExpensiveRequest> ExpensiveRequest::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveRequestT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateExpensiveRequest(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ExpensiveRequest> CreateExpensiveRequest(flatbuffers::FlatBufferBuilder &_fbb, const ExpensiveRequestT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ExpensiveRequestT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _milliseconds = _o->milliseconds;
  auto _number = _o->number;
  return examples::timeouts::CreateExpensiveRequest(
      _fbb,
      _milliseconds,
      _number);
}

inline EmptyT *Empty::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new EmptyT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void Empty::UnPackTo(EmptyT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
}

inline flatbuffers::Offset<Empty> Empty::Pack(flatbuffers::FlatBufferBuilder &_fbb, const EmptyT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateEmpty(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Empty> CreateEmpty(flatbuffers::FlatBufferBuilder &_fbb, const EmptyT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const EmptyT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  return examples::timeouts::CreateEmpty(
      _fbb);
}

inline const flatbuffers::TypeTable *ExpensiveReplyTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 }
  };
  static const char * const names[] = {
    "number"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 1, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *ExpensiveRequestTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_UINT, 0, -1 }
  };
  static const char * const names[] = {
    "milliseconds",
    "number"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 2, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *EmptyTypeTable() {
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 0, nullptr, nullptr, nullptr, nullptr
  };
  return &tt;
}

}  // namespace timeouts
}  // namespace examples

#endif  // FLATBUFFERS_GENERATED_TIMEOUT_EXAMPLES_TIMEOUTS_H_
