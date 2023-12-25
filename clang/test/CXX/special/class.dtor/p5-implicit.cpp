// RUN: %clang_cc1 -std=c++11 %s -ast-dump | FileCheck %s

struct A { ~A() = delete; };
// CHECK-LABEL: CXXRecordDecl {{.*}} struct A
// CHECK: Destructor user_declared

struct B : A {};
// CHECK-LABEL: CXXRecordDecl {{.*}} struct B
// CHECK: Destructor needs_overload_resolution

struct C : B {};
// CHECK-LABEL: CXXRecordDecl {{.*}} struct C
// CHECK: Destructor needs_overload_resolution

struct D { ~D(); };
struct E : D {};
union U {
  E e;
};
// CHECK-LABEL: CXXRecordDecl {{.*}} union U
// CHECK: Destructor needs_implicit defaulted_is_deleted
