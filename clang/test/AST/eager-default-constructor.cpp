// Test without serialization:
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -std=c++17 -ast-dump %s \
// RUN: | FileCheck -strict-whitespace %s
//
// Test with serialization:
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -std=c++17 -emit-pch -o %t %s
// RUN: %clang_cc1 -x c++ -triple x86_64-unknown-unknown -std=c++17 -include-pch %t -ast-dump-all /dev/null \
// RUN: | sed -e "s/ <undeserialized declarations>//" -e "s/ imported//" \
// RUN: | FileCheck -strict-whitespace %s

struct DefaultedDefCtor1 {};
// CHECK DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
struct DefaultedDefCtor2 { DefaultedDefCtor2() = default; };

struct DefaultedDefCtorUninitialized1 { int x; };
struct DefaultedDefCtorUninitialized2 { int x; DefaultedDefCtorUninitialized2() = default; };
struct DeletedDefCtor { DeletedDefCtor() = delete; DeletedDefCtor(int); };
class PrivateDefCtor { PrivateDefCtor() = default; public: PrivateDefCtor(int); };
struct DeletedDtor { ~DeletedDtor() = delete; };
class PrivateDtor { ~PrivateDtor() = default; };
class Friend {
  Friend() = default; ~Friend() = default;
  friend struct NotDeleted6c;
  friend struct NotDeleted7i;
  friend struct NotDeleted7j;
  friend struct NotDeleted7k;
};
struct UserProvidedDefCtor { UserProvidedDefCtor() {} };
int n;


// A defaulted default constructor for a class X is defined as deleted if:

// - X is a union-like class that has a variant member with a non-trivial
// default constructor,
union Deleted1a { UserProvidedDefCtor u; };
// CHECK: CXXRecordDecl {{.*}} union Deleted1a definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted1a 'void (){{.*}}' {{.*}} default_delete {{.*}}


union NotDeleted1a { DefaultedDefCtor1 nu; };
// CHECK: CXXRecordDecl {{.*}} union NotDeleted1a definition
// CHECK: DefaultConstructor exists trivial needs_implicit
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit NotDeleted1a 'void (){{.*}}' {{.*}}
union NotDeleted1b { DefaultedDefCtor2 nu; };
// CHECK: CXXRecordDecl {{.*}} union NotDeleted1b definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted1b 'void ()' {{.*}}

// - any non-static data member with no brace-or-equal-initializer is of
// reference type,
class Deleted2a {
  int &a;
};
// CHECK: CXXRecordDecl {{.*}} class Deleted2a definition
// CHECK: DefaultConstructor exists needs_implicit defaulted_is_deleted
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit Deleted2a 'void (){{.*}}' {{.*}}

struct Deleted2b {
  int &&b;
};
// CHECK: CXXRecordDecl {{.*}} struct Deleted2b definition
// CHECK: DefaultConstructor exists needs_implicit defaulted_is_deleted
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit Deleted2b 'void (){{.*}}' {{.*}}

class NotDeleted2a { int &a = n; };
// CHECK: CXXRecordDecl {{.*}} class NotDeleted2a definition
// CHECK: DefaultConstructor exists non_trivial constexpr needs_implicit defaulted_is_constexpr
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit NotDeleted2a 'void (){{.*}}' {{.*}}

class NotDeleted2c { int &&a = static_cast<int&&>(n); };

// Note: this one does not have a deleted default constructor even though the
// implicit default constructor is ill-formed!
class NotDeleted2d { int &&a = 0; };


// - any non-variant non-static data member of const qualified type (or array
// thereof) with no brace-or-equal-initializer is not const-default-constructible
class Deleted3a { const int a; };
// CHECK: CXXRecordDecl {{.*}} class Deleted3a definition
// CHECK: DefaultConstructor exists needs_implicit defaulted_is_deleted
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit Deleted3a 'void (){{.*}}' {{.*}}

class Deleted3b { const DefaultedDefCtorUninitialized1 a[42]; };
// CHECK: CXXRecordDecl {{.*}} class Deleted3b definition
// CHECK: DefaultConstructor exists needs_implicit defaulted_is_deleted
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit Deleted3b 'void (){{.*}}' {{.*}}

class Deleted3c { const DefaultedDefCtorUninitialized2 a; };
// CHECK: CXXRecordDecl {{.*}} class Deleted3c definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted3c 'void (){{.*}}' {{.*}}

class NotDeleted3a { const int a = 0; };
// CHECK: CXXRecordDecl {{.*}} class NotDeleted3a definition
// CHECK: DefaultConstructor exists non_trivial constexpr needs_implicit defaulted_is_constexpr
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit NotDeleted3a 'void (){{.*}}' {{.*}}

class NotDeleted3b { const DefaultedDefCtorUninitialized1 a[42] = {}; };
// CHECK: CXXRecordDecl {{.*}} class NotDeleted3b definition
// CHECK: DefaultConstructor exists non_trivial constexpr needs_implicit defaulted_is_constexpr
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit NotDeleted3b 'void (){{.*}}' {{.*}}

class NotDeleted3c { const DefaultedDefCtorUninitialized2 a = DefaultedDefCtorUninitialized2(); };
// CHECK: CXXRecordDecl {{.*}} class NotDeleted3c definition
// CHECK: DefaultConstructor exists non_trivial constexpr defaulted_is_constexpr
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr NotDeleted3c 'void (){{.*}}' {{.*}}

union NotDeleted3d { const int a; int b; };
// CHECK: CXXRecordDecl {{.*}} union NotDeleted3d definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted3d 'void (){{.*}}' {{.*}}

union NotDeleted3e { const DefaultedDefCtor1 a[42]; int b; };
// CHECK: CXXRecordDecl {{.*}} union NotDeleted3e definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted3e 'void (){{.*}}' {{.*}}

union NotDeleted3f { const DefaultedDefCtor2 a; int b; };
// CHECK: CXXRecordDecl {{.*}} union NotDeleted3f definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted3f 'void (){{.*}}' {{.*}}

struct NotDeleted3g { union { const int a; int b; }; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted3g definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted3g 'void (){{.*}}' {{.*}}

struct NotDeleted3h { const DefaultedDefCtor1 a[42]; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted3h definition
// CHECK: DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
// CHECK-NOT: CXXConstructorDecl {{.*}} implicit NotDeleted3h 'void (){{.*}}' {{.*}}


struct NotDeleted3i { const DefaultedDefCtor2 a; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted3i definition
// CHECK: DefaultConstructor exists trivial constexpr defaulted_is_constexpr
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr NotDeleted3i 'void (){{.*}}' {{.*}}

// - X is a union and all of its variant members are of const-qualified type (or
// array thereof),
union Deleted4a {
  const int a;
  const int b;
  const UserProvidedDefCtor c;
};
// CHECK: CXXRecordDecl {{.*}} union Deleted4a definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted4a 'void (){{.*}}' {{.*}} default_delete {{.*}}

union NotDeleted4a { const int a; int b; };
// CHECK: CXXRecordDecl {{.*}} union NotDeleted4a definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted4a 'void (){{.*}}' {{.*}}

// - X is a non-union class and all members of any anonymous union member are of
// const-qualified type (or array thereof),
struct Deleted5a {
  union { const int a; };
  union { int b; };
};
// CHECK: CXXRecordDecl {{.*}} struct Deleted5a definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted5a 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct NotDeleted5a { union { const int a; int b; }; union { const int c; int d; }; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted5a definition
// CHECK: DefaultConstructor exists trivial
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted5a 'void (){{.*}}' {{.*}}


class Deleted2aDerivable {
  Deleted2aDerivable() = default; 
  int &a;
};

// - any direct or virtual base class, or non-static data member with no
// brace-or-equal-initializer, has class type M (or array thereof) and either
// M has no default constructor or overload resolution as applied to M's default
// constructor results in an ambiguity or in a function that is deleted or
// inaccessible from the defaulted default constructor, or
struct Deleted6a : Deleted2aDerivable {};
// CHECK: CXXRecordDecl {{.*}} struct Deleted6a definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted6a 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted6b : virtual Deleted2aDerivable {};
// CHECK: CXXRecordDecl {{.*}} struct Deleted6b definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted6b 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted6c { Deleted2aDerivable a; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted6c definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted6c 'void (){{.*}}' {{.*}} default_delete {{.*}}


struct Deleted6d { DeletedDefCtor a; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted6d definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted6d 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct NotDeleted6a { DeletedDefCtor a = 0; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted6a definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit {{.*}} NotDeleted6a 'void (){{.*}}' {{.*}}

struct Deleted6e { PrivateDefCtor a; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted6e definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted6e 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct NotDeleted6b { PrivateDefCtor a = 0; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted6b definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr NotDeleted6b 'void (){{.*}}' {{.*}}

struct NotDeleted6c { Friend a; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted6c definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr NotDeleted6c 'void (){{.*}}' {{.*}}

// - any direct or virtual base class or non-static data member has a type with
// a destructor that is deleted or inaccessible from the defaulted default
// constructor.
struct Deleted7a : DeletedDtor {};
// CHECK: CXXRecordDecl {{.*}} struct Deleted7a definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted7a 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7b : virtual DeletedDtor {};
// CHECK: CXXRecordDecl {{.*}} struct Deleted7b definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted7b 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7c { DeletedDtor a; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted7c definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted7c 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7d { DeletedDtor a = {}; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted7d definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted7d 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7e : PrivateDtor {};
// CHECK: CXXRecordDecl {{.*}} struct Deleted7e definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted7e 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7f : virtual PrivateDtor {};
// CHECK: CXXRecordDecl {{.*}} struct Deleted7f definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit Deleted7f 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7g { PrivateDtor a; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted7g definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted7g 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct Deleted7h { PrivateDtor a = {}; };
// CHECK: CXXRecordDecl {{.*}} struct Deleted7h definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr Deleted7h 'void (){{.*}}' {{.*}} default_delete {{.*}}

struct NotDeleted7i : Friend {};
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted7i definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr NotDeleted7i 'void (){{.*}}' {{.*}}

struct NotDeleted7j : virtual Friend {};
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted7j definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit NotDeleted7j 'void (){{.*}}' {{.*}}

struct NotDeleted7k { Friend a; };
// CHECK: CXXRecordDecl {{.*}} struct NotDeleted7k definition
// CHECK: DefaultConstructor exists
// CHECK: CXXConstructorDecl {{.*}} implicit constexpr NotDeleted7k 'void (){{.*}}' {{.*}}
