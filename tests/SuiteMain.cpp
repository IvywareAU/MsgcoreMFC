// Copyright © 2026 Ivyware Pty Ltd, Khrustal & Mann
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
// SuiteMain.cpp
//
// Entry point for this repository's unit-test runner.
//
// Until this landed, MsgcoreMFC had no tests at all, in a repository whose
// sibling ships two suites and a fuzz harness. The two suites carried here are
// the ones this library can honestly run headless:
//
//   * MsgcoreMFCSuite     - the exported helpers that need no window.
//   * P2PmsgBridgeSuite   - the Msgcore serialisation bridge, end to end
//                           against a real heap, including Save/Load.
//
// WHAT IS DELIBERATELY NOT HERE. The window-bound surface - everything taking a
// live CWnd, CDC, CListCtrl or CComboBox - is not covered, because exercising it
// needs a desktop and a message pump. It is left visibly absent rather than
// stubbed; a stub that always passes reads like coverage and is worse than a
// gap that is written down.
//
// A suite that is compiled out must SAY SO. Every guard below has an #else that
// reports the skip, and tf_runner_finish refuses to call a run with nSkipped > 0
// a pass: a dropped suite otherwise produces output indistinguishable from a
// clean full run.

#include <afx.h>
#include <afxwin.h>

#include <cstdio>

#include "TestFramework.h"

// ---------------------------------------------------------------------------
int main(int /*argc*/, char* /*argv*/[])
{
    if (!tf_runner_startup("MsgcoreMFC unit tests"))
        return 1;

    int nSkipped = 0;

    printf("\n[MsgcoreMFC]\n");
    RunMsgcoreMFCSuite();

    // The bridge suite links Msgcore as well as MsgcoreMFC. MSGCOREMFC_NO_BRIDGE
    // drops it; nothing in this tree defines that today, and the guard is kept
    // so the skip stays visible if anything ever does.
#if !defined(MSGCOREMFC_NO_BRIDGE)
    printf("\n[P2Pmsg bridge]\n");
    RunP2PmsgBridgeSuite();
#else
    printf("\n[P2Pmsg bridge]\n  SKIPPED (MSGCOREMFC_NO_BRIDGE)\n");
    ++nSkipped;
#endif

    return tf_runner_finish(nSkipped);
}
