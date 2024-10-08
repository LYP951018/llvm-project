// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --version 5
// RUN: %clang_cc1 -fnative-half-arguments-and-returns -triple amdgcn-amd-amdhsa-gnu -target-cpu gfx900 -emit-llvm -o - %s | FileCheck -check-prefixes=CHECK,SAFE %s
// RUN: %clang_cc1 -fnative-half-arguments-and-returns -triple amdgcn-amd-amdhsa-gnu -target-cpu gfx900 -emit-llvm -munsafe-fp-atomics -o - %s | FileCheck -check-prefixes=CHECK,UNSAFE %s

// SAFE-LABEL: define dso_local float @test_float_post_inc(
// SAFE-SAME: ) #[[ATTR0:[0-9]+]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_float_post_inc.n to ptr), float 1.000000e+00 seq_cst, align 4
// SAFE-NEXT:    ret float [[TMP0]]
//
// UNSAFE-LABEL: define dso_local float @test_float_post_inc(
// UNSAFE-SAME: ) #[[ATTR0:[0-9]+]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_float_post_inc.n to ptr), float 1.000000e+00 seq_cst, align 4, !amdgpu.no.fine.grained.memory [[META3:![0-9]+]], !amdgpu.ignore.denormal.mode [[META3]]
// UNSAFE-NEXT:    ret float [[TMP0]]
//
float test_float_post_inc()
{
    static _Atomic float n;
    return n++;
}

// SAFE-LABEL: define dso_local float @test_float_post_dc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_float_post_dc.n to ptr), float 1.000000e+00 seq_cst, align 4
// SAFE-NEXT:    ret float [[TMP0]]
//
// UNSAFE-LABEL: define dso_local float @test_float_post_dc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_float_post_dc.n to ptr), float 1.000000e+00 seq_cst, align 4, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    ret float [[TMP0]]
//
float test_float_post_dc()
{
    static _Atomic float n;
    return n--;
}

// SAFE-LABEL: define dso_local float @test_float_pre_dc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_float_pre_dc.n to ptr), float 1.000000e+00 seq_cst, align 4
// SAFE-NEXT:    [[TMP1:%.*]] = fsub float [[TMP0]], 1.000000e+00
// SAFE-NEXT:    ret float [[TMP1]]
//
// UNSAFE-LABEL: define dso_local float @test_float_pre_dc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_float_pre_dc.n to ptr), float 1.000000e+00 seq_cst, align 4, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    [[TMP1:%.*]] = fsub float [[TMP0]], 1.000000e+00
// UNSAFE-NEXT:    ret float [[TMP1]]
//
float test_float_pre_dc()
{
    static _Atomic float n;
    return --n;
}

// SAFE-LABEL: define dso_local float @test_float_pre_inc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_float_pre_inc.n to ptr), float 1.000000e+00 seq_cst, align 4
// SAFE-NEXT:    [[TMP1:%.*]] = fadd float [[TMP0]], 1.000000e+00
// SAFE-NEXT:    ret float [[TMP1]]
//
// UNSAFE-LABEL: define dso_local float @test_float_pre_inc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca float, align 4, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_float_pre_inc.n to ptr), float 1.000000e+00 seq_cst, align 4, !amdgpu.no.fine.grained.memory [[META3]], !amdgpu.ignore.denormal.mode [[META3]]
// UNSAFE-NEXT:    [[TMP1:%.*]] = fadd float [[TMP0]], 1.000000e+00
// UNSAFE-NEXT:    ret float [[TMP1]]
//
float test_float_pre_inc()
{
    static _Atomic float n;
    return ++n;
}

// SAFE-LABEL: define dso_local double @test_double_post_inc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_double_post_inc.n to ptr), double 1.000000e+00 seq_cst, align 8
// SAFE-NEXT:    ret double [[TMP0]]
//
// UNSAFE-LABEL: define dso_local double @test_double_post_inc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_double_post_inc.n to ptr), double 1.000000e+00 seq_cst, align 8, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    ret double [[TMP0]]
//
double test_double_post_inc()
{
    static _Atomic double n;
    return n++;
}

// SAFE-LABEL: define dso_local double @test_double_post_dc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_double_post_dc.n to ptr), double 1.000000e+00 seq_cst, align 8
// SAFE-NEXT:    ret double [[TMP0]]
//
// UNSAFE-LABEL: define dso_local double @test_double_post_dc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_double_post_dc.n to ptr), double 1.000000e+00 seq_cst, align 8, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    ret double [[TMP0]]
//
double test_double_post_dc()
{
    static _Atomic double n;
    return n--;
}

// SAFE-LABEL: define dso_local double @test_double_pre_dc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_double_pre_dc.n to ptr), double 1.000000e+00 seq_cst, align 8
// SAFE-NEXT:    [[TMP1:%.*]] = fsub double [[TMP0]], 1.000000e+00
// SAFE-NEXT:    ret double [[TMP1]]
//
// UNSAFE-LABEL: define dso_local double @test_double_pre_dc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test_double_pre_dc.n to ptr), double 1.000000e+00 seq_cst, align 8, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    [[TMP1:%.*]] = fsub double [[TMP0]], 1.000000e+00
// UNSAFE-NEXT:    ret double [[TMP1]]
//
double test_double_pre_dc()
{
    static _Atomic double n;
    return --n;
}

// SAFE-LABEL: define dso_local double @test_double_pre_inc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_double_pre_inc.n to ptr), double 1.000000e+00 seq_cst, align 8
// SAFE-NEXT:    [[TMP1:%.*]] = fadd double [[TMP0]], 1.000000e+00
// SAFE-NEXT:    ret double [[TMP1]]
//
// UNSAFE-LABEL: define dso_local double @test_double_pre_inc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca double, align 8, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test_double_pre_inc.n to ptr), double 1.000000e+00 seq_cst, align 8, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    [[TMP1:%.*]] = fadd double [[TMP0]], 1.000000e+00
// UNSAFE-NEXT:    ret double [[TMP1]]
//
double test_double_pre_inc()
{
    static _Atomic double n;
    return ++n;
}

// SAFE-LABEL: define dso_local half @test__Float16_post_inc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test__Float16_post_inc.n to ptr), half 0xH3C00 seq_cst, align 2
// SAFE-NEXT:    ret half [[TMP0]]
//
// UNSAFE-LABEL: define dso_local half @test__Float16_post_inc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test__Float16_post_inc.n to ptr), half 0xH3C00 seq_cst, align 2, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    ret half [[TMP0]]
//
_Float16 test__Float16_post_inc()
{
    static _Atomic _Float16 n;
    return n++;
}

// SAFE-LABEL: define dso_local half @test__Float16_post_dc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test__Float16_post_dc.n to ptr), half 0xH3C00 seq_cst, align 2
// SAFE-NEXT:    ret half [[TMP0]]
//
// UNSAFE-LABEL: define dso_local half @test__Float16_post_dc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test__Float16_post_dc.n to ptr), half 0xH3C00 seq_cst, align 2, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    ret half [[TMP0]]
//
_Float16 test__Float16_post_dc()
{
    static _Atomic _Float16 n;
    return n--;
}

// SAFE-LABEL: define dso_local half @test__Float16_pre_dc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test__Float16_pre_dc.n to ptr), half 0xH3C00 seq_cst, align 2
// SAFE-NEXT:    [[TMP1:%.*]] = fsub half [[TMP0]], 0xH3C00
// SAFE-NEXT:    ret half [[TMP1]]
//
// UNSAFE-LABEL: define dso_local half @test__Float16_pre_dc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fsub ptr addrspacecast (ptr addrspace(1) @test__Float16_pre_dc.n to ptr), half 0xH3C00 seq_cst, align 2, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    [[TMP1:%.*]] = fsub half [[TMP0]], 0xH3C00
// UNSAFE-NEXT:    ret half [[TMP1]]
//
_Float16 test__Float16_pre_dc()
{
    static _Atomic _Float16 n;
    return --n;
}

// SAFE-LABEL: define dso_local half @test__Float16_pre_inc(
// SAFE-SAME: ) #[[ATTR0]] {
// SAFE-NEXT:  [[ENTRY:.*:]]
// SAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// SAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// SAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test__Float16_pre_inc.n to ptr), half 0xH3C00 seq_cst, align 2
// SAFE-NEXT:    [[TMP1:%.*]] = fadd half [[TMP0]], 0xH3C00
// SAFE-NEXT:    ret half [[TMP1]]
//
// UNSAFE-LABEL: define dso_local half @test__Float16_pre_inc(
// UNSAFE-SAME: ) #[[ATTR0]] {
// UNSAFE-NEXT:  [[ENTRY:.*:]]
// UNSAFE-NEXT:    [[RETVAL:%.*]] = alloca half, align 2, addrspace(5)
// UNSAFE-NEXT:    [[RETVAL_ASCAST:%.*]] = addrspacecast ptr addrspace(5) [[RETVAL]] to ptr
// UNSAFE-NEXT:    [[TMP0:%.*]] = atomicrmw fadd ptr addrspacecast (ptr addrspace(1) @test__Float16_pre_inc.n to ptr), half 0xH3C00 seq_cst, align 2, !amdgpu.no.fine.grained.memory [[META3]]
// UNSAFE-NEXT:    [[TMP1:%.*]] = fadd half [[TMP0]], 0xH3C00
// UNSAFE-NEXT:    ret half [[TMP1]]
//
_Float16 test__Float16_pre_inc()
{
    static _Atomic _Float16 n;
    return ++n;
}
//.
// UNSAFE: [[META3]] = !{}
//.
//// NOTE: These prefixes are unused and the list is autogenerated. Do not add tests below this line:
// CHECK: {{.*}}
