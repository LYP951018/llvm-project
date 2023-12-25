// RUN: %clang_cc1 -fsyntax-only -std=c++20 -verify %s -Wno-defaulted-function-deleted -fclang-abi-compat=17 -DCLANG_ABI_COMPAT=17
// RUN: %clang_cc1 -fsyntax-only -std=c++11 -verify %s -Wno-defaulted-function-deleted

constexpr bool NegIfAbiCompact(bool x) {
#if defined(CLANG_ABI_COMPAT) && CLANG_ABI_COMPAT < 18
  return !x;
#else
  return x;
#endif
}

// class with all copy constructor, copy assignment operator, move constructor,
// move assignment operator, implicit deleted or not exist

// copy ctor: deleted due to declares a move constructor
// copy assignment: deleted due to declares a move constructor
// move ctor: deleted due to declares a move constructor as deleted
// move assignment: not exist due to declares a move constructor as deleted
class AllDeleted1 {
  AllDeleted1(AllDeleted1 &&) = delete;
};

static_assert(NegIfAbiCompact(!__is_trivially_copyable(AllDeleted1)));
static_assert(NegIfAbiCompact(!__is_trivial(AllDeleted1)));

// copy ctor: deleted due declares a move assignment operator
// copy assignment: deleted due declares a move assignment operator
// move ctor: not implicit generated due to declares a move constructor as deleted
// move assignment: explicit deleted
class AllDeleted2 {
  AllDeleted2 &operator=(AllDeleted2 &&) = delete;
};

static_assert(NegIfAbiCompact(!__is_trivially_copyable(AllDeleted2)));
static_assert(NegIfAbiCompact(!__is_trivial(AllDeleted2)));


// copy ctor: deleted due to subobject overload resolution
// copy assignment: deleted due to subobject overload resolution
// move ctor: deleted due to subobject overload resolution
// move assignment: deleted due to subobject overload resolution
class AllDeleted3 : public AllDeleted1 {
};

static_assert(NegIfAbiCompact(!__is_trivially_copyable(AllDeleted3)));
static_assert(NegIfAbiCompact(!__is_trivial(AllDeleted3)));


struct AllDeleted4 {
  AllDeleted4(AllDeleted4&&) = delete;
  int&& r;
};

static_assert(NegIfAbiCompact(!__is_trivially_copyable(AllDeleted4)));
static_assert(NegIfAbiCompact(!__is_trivial(AllDeleted4)));
