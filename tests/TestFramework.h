// Copyright © 2026 Khrustal & Mann
//              MELBOURNE, VICTORIA, AUSTRALIA, 3000
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
// TestFramework.h
//
// A tiny, dependency-free assertion harness for the MsgcoreMFC suites.
//
// PROVENANCE. This is Msgcore's tests/TestFramework.h, carried here verbatim in
// its TF_CASE / TF_CHECK / TF_CHECK_EQ surface and deliberately so: the two
// repositories' suites should mean the same thing by the same assertion, and a
// divergence in the macros is the one way a ported case can quietly change
// meaning. Only the suite entry points below differ.
//
// The CWinApp anchor and the assert trap matter MORE here than they do upstream.
// MsgcoreMFC is an MFC extension DLL: its objects are MFC objects, and a Debug
// build of MFC asserts freely. Without the trap the first ASSERT opens a modal
// dialog and a headless run hangs with no output.
//
// Reporting is via narrow printf using %ls for wide strings, which is valid on
// a default (non _O_U16TEXT) Windows console. The process exit code is 0 only
// when every check passed.
#pragma once

struct TestStats
{
    int checks        = 0;   // individual TF_CHECK evaluations
    int failures      = 0;   // checks that failed
    int cases         = 0;   // test cases entered
    int caseFailures  = 0;   // cases with at least one failed check
};

extern TestStats g_tf;

// Case lifecycle. Prefer the TF_CASE scope guard below over calling directly.
void tf_begin_case(const char* name);
void tf_end_case();

// Records a failed check (also used by the assert hook to fold an MFC/CRT
// ASSERT into the current case as a failure instead of aborting the run).
void tf_fail(const char* file, int line, const char* expr);

// RAII scope guard: `TF_CASE("name") { ... checks ... }`
struct TfCaseGuard
{
    TfCaseGuard(const char* name) { tf_begin_case(name); }
    ~TfCaseGuard()                { tf_end_case(); }
    operator bool() const         { return true; }
};

#define TF_CONCAT_(a, b) a##b
#define TF_CONCAT(a, b)  TF_CONCAT_(a, b)
#define TF_CASE(name)    if (TfCaseGuard TF_CONCAT(_tfcase_, __LINE__) = TfCaseGuard(name))

#define TF_CHECK(cond)                                             \
    do {                                                           \
        ++g_tf.checks;                                             \
        if (!(cond)) tf_fail(__FILE__, __LINE__, #cond);           \
    } while (0)

#define TF_CHECK_EQ(a, b)                                          \
    do {                                                           \
        ++g_tf.checks;                                             \
        if (!((a) == (b)))                                         \
            tf_fail(__FILE__, __LINE__, #a " == " #b);             \
    } while (0)

// Runner lifecycle, implemented in TestFramework.cpp. Returns false only if the
// harness itself cannot start; tf_runner_finish returns the process exit code
// (0 pass, 1 failure, 2 = everything that ran passed but suites were compiled
// out).
bool tf_runner_startup(const char* title);
int  tf_runner_finish(int nSkipped);

// Suite entry points, defined in their respective .cpp files.
//
// Only the suites THIS repository owns are declared. Msgcore's RunMsgcoreSuite
// and RunMsgcoreCApiSuite test Msgcore and stay in Msgcore; declaring them here
// would read as though this runner covered them.
void RunMsgcoreMFCSuite();
void RunP2PmsgBridgeSuite();
