# MsgcoreMFC

**The MFC layer of the MSCS message-store family: an extension DLL that carries MFC user-interface
state in and out of a Msgcore store.**

[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/C%2B%2B-20-00599C.svg)](#requirements)
[![Windows](https://img.shields.io/badge/Windows-MFC%20extension%20DLL-0078D6.svg)](#requirements)
[![Version](https://img.shields.io/badge/version-3.0.0.0-6f42c1.svg)](#versioning)
[![Since](https://img.shields.io/badge/since-2001-6f42c1.svg)](#msgcoremfc)

MFC has no memory. A `CListCtrl` knows its column widths until the window closes, a `CFont` knows
its face and weight until the dialog is destroyed, and nothing in the framework will carry either
across a session. [Msgcore](../Msgcore) has memory but knows nothing about windows: it is a
hierarchical, offset-addressed store whose whole image can be written to a file verbatim.

MsgcoreMFC is the join between the two. It is an MFC extension DLL that exports two things — a
serialisation bridge between MFC's user-interface objects and Msgcore's `P2Pmsg` document model,
and the `P2Pevent` diagnostics vocabulary that the rest of the MSCS family raises and catches.

```cpp
#include "CListCtrl_Ext.h"

// Save the user's column layout into the store, and put it back next session.
CListCtrl_SerialiseHeader   ( m_oListCtrl, mgr.r_Desc()[L"columns"] );
CListCtrl_DeserialiseHeader ( m_oListCtrl, mgr.r_Desc()[L"columns"] );
```

---

## Status

All four configurations build clean — zero errors and **zero warnings** from this project at `/W4`,
under one C++ standard. The unit suites run green: 30 cases, 82 checks. Version identity is
3.0.0.0 and comes from a single header.

The window-bound surface — anything taking a live `CWnd`, `CDC`, `CListCtrl` or `CComboBox` — has
no automated coverage, because exercising it needs a desktop and a message pump. That gap is real
and is stated here rather than filled with stubs that would only look like coverage.

## What this library provides

### 1. The serialisation bridge to the P2Pmsg model

This is the reason the library exists. Msgcore's `P3PmsgItem`, `P3PmsgAttr` and `P3PmsgField`
appear directly in MsgcoreMFC's exported signatures, so a caller hands a node of a live store to an
MFC control and gets the control's state back into the same store:

```cpp
MsgcoreMFC_EXT BOOL CListCtrl_MakeColumns     ( CListCtrl&,  const P3PmsgItem& );
MsgcoreMFC_EXT BOOL CListCtrl_SerialiseHeader ( CListCtrl&,  P3PmsgItem& );
MsgcoreMFC_EXT BOOL CComboBox_Serialise       ( CComboBox&,  P3PmsgItem& );
MsgcoreMFC_EXT BOOL LOGBRUSH2LOGBRUSHcrx      ( const P3PmsgItem&, LPCTSTR );
MsgcoreMFC_EXT BOOL LOGPEN2LOGPENcrx          ( const P3PmsgItem&, LPCTSTR );
MsgcoreMFC_EXT BOOL LOGFONTcrxLocalise        ( LPCWSTR, P3PmsgField& );
MsgcoreMFC_EXT BOOL P2Pmsg_Copy2Clipboard     ( HWND, UINT, P3PmsgItem& );
```

Because a Msgcore store is a single contiguous image addressed by offset rather than by pointer,
state that reaches it needs no serialisation pass of its own. Column layouts, combo contents,
`LOGPEN`, `LOGBRUSH` and `LOGFONT` descriptions and clipboard payloads are written as ordinary
named fields, saved with the store, and read back verbatim.

The bridge covers list controls, combo boxes, pens, brushes, fonts and the clipboard. The clipboard
path is the widest of them: it registers a private format, packs a `P3PmsgItem` into a `P2Piomage`
and hands the global block to the clipboard, so a subtree copied out of one process pastes into
another as structure rather than as text.

The `LOGPEN`/`LOGBRUSH` conversions are in-place *widenings*: the plain structure already in the
store is replaced by the `crx` form carrying the extra opacity and enable fields. They deliberately
refuse to run twice, so a second pass cannot re-default an opacity a caller has set.

### 2. The P2Pevent diagnostics vocabulary

MsgcoreMFC raises and catches errors in the same vocabulary as the rest of the MSCS family, defined
in Msgcore's `Msgexception.h`. An error is a `P2Pevent` built by a severity macro, decorated by
chaining, and then thrown:

```cpp
if ( shGlobalCopy.GlobalAlloc(GMEM_MOVEABLE,nSizeof) == NULL )
  EVERR->MODULE
       ->Message_T("GlobalAlloc failed")
       ->Throw ( );
```

`EVERR`, `EVWRN`, `EVINF`, `EVDBG`, `EVTRC`, `EVLOG` and `EVRPT` select the severity; `MODULE`
stamps the raising function; `Message`, `Advice` and `HResult` add detail. Whether an event reaches
a log file or a message box is decided at run time by `P2Pmsg.cfg`, not by the raising code.

The catch side is a matching set of macros, so a handler states its policy rather than repeating it:

```cpp
catch_pP2Pevent_Cancel     // an event we raised: cancel it here
catch_pCException_Cancel   // an MFC CException: adopt and cancel
catch_ALL_Cancel           // anything else: report as unknown, cancel
```

`_Cancel` disposes of the event at this boundary; `_SetLast` records it as the last error and lets
the caller decide. Because these are the family's macros and not this library's, an event raised
inside MsgcoreMFC is caught, logged and reported the same way as one raised in Msgcore itself.

---

## Requirements

* Visual Studio 2026, platform toolset **v145**, Windows SDK 10.
* MFC, used as a **shared** (dynamic) library — this project is an MFC extension DLL, built with
  `_AFXEXT`, `/std:c++20` and Unicode throughout, in all four configurations.
* **Msgcore**, checked out as a sibling directory (`..\Msgcore`). It is built for you: see
  *Building* below.

No environment variable is required. `$(WDMSCS_LIB)` defaults to `MSCS\lib` through the repo-root
`Directory.Build.props`, and `Directory.Build.targets` fails the build rather than let a bad value
send the import library somewhere nobody will look for it. An explicit `WDMSCS_LIB` still wins.

**Outside the MSCS tree**, this repository's own `Directory.Build.props` takes over: it chains to
an outer one when there is one, so nothing changes for an in-tree build, and otherwise defaults
`$(WDMSCS_LIB)` to the `lib\` *beside* this repository — the shared staging directory for a row of
sibling clones, since this project links `Msgcore.lib` and must find it where Msgcore left it.
That default only covers the simplest layout. **If you clone the components side by side, set
`WDMSCS_LIB` once to a single absolute directory and build them all with it**; the sibling
repositories do not all agree on a default, and `Targetcore` has no repo-local
`Directory.Build.props` at all, so it needs the variable rather than merely preferring it.
`WDMSCS_VSUTILS` is optional: when defined, a post-build step publishes the DLL into the shared
MSCS tree; when not, the build succeeds and the DLL stays in its output directory.

## Building

```
msbuild "MsgcoreMFC(2026).sln" -p:Configuration=Debug   -p:Platform=x64
msbuild "MsgcoreMFC(2026).sln" -p:Configuration=Release -p:Platform=x64
msbuild "MsgcoreMFC(2026).sln" -p:Configuration=Debug   -p:Platform=x86
msbuild "MsgcoreMFC(2026).sln" -p:Configuration=Release -p:Platform=x86
```

Note that the solution names the 32-bit platform **`x86`**, while the project underneath it names
the same platform `Win32`. Building the solution takes `x86`; building the `.vcxproj` directly
takes `Win32`.

**Msgcore is built first, automatically.** The project carries a `<ProjectReference>` to it whose
only job is ordering — `LinkLibraryDependencies` is false, so the explicit `Msgcore.lib` stays the
one that is linked. Building this project can therefore no longer link a stale Msgcore silently.
Msgcore's own warnings appear in the log as a result; they belong to that project, and this one
still contributes none.

Everything lands under one root, `out\<Platform>\<Configuration>\`, with intermediates in `obj\`
inside it. At run time a consumer of `MsgcoreMFC.dll` also needs `Msgcore.dll` beside it.

## Tests

```
.\tests\build_run_suite.bat
```

Builds and runs the unit suites against the `Debug|x64` DLL — **build the library first**, or the
script will test the previous binary. 30 cases, 82 checks, exit code 0 only when every check
passed.

| | |
|---|---|
| `MsgcoreMFCSuite.cpp` | The headless helpers: `CString_Ext`, `CRect_Ext`, `COleTime_Ext`, `CStringParser`. Two long-standing quirks are pinned deliberately so a tidy-up has to be intentional. |
| `P2PmsgBridgeSuite.cpp` | The serialisation bridge end to end against a real `P2PmsgMgr` heap, including a `Save`/`Load` round trip. It asserts on the bytes that come back out, not on the return value — which is how it caught `LOGBRUSH2LOGBRUSHcrx` comparing its guard against the source type and therefore never converting anything. |
| `TestFramework.{h,cpp}` | Carried from Msgcore with the assertion macros deliberately identical. |

## Versioning

`MsgcoreMFC_version.h` is the single source of version identity, at **3.0.0.0**. The resource
script reads `FILEVERSION`, `PRODUCTVERSION` and both version strings from it, and `MsgcoreMFC.h`
exposes the macros to consumers:

```cpp
#if !MSGCOREMFC_VERSION_AT_LEAST(3,0,0)
#  error MsgcoreMFC 3.0.0 or later is required
#endif
```

The header exists because those numbers had drifted: the resource script declared
`FILEVERSION 2,1,0,1` beside `PRODUCTVERSION 3,0,0,0`, and shipped a DLL reporting a file version
no release ever carried.

## Repository layout

| | |
|---|---|
| `C*_Ext.{h,cpp}` | Extensions over the MFC classes they are named for — `CBitmap`, `CBrush`, `CComboBox`, `CDC`, `CDialog`, `CEdit`, `CFont`, `CListCtrl`, `CMenu`, `COleTime`, `CPen`, `CRect`, `CString`, `CTime`, `CTreeCtrl`, `CView`, `CWnd`. |
| `Clipboard_Ext.{h,cpp}` | Private-format clipboard transport for `P3PmsgItem` subtrees. |
| `CStringParser.{h,cpp}` | The family's own `CString` parser. |
| `MsgcoreMFC.h` | The `MsgcoreMFC_EXT` export/import macro and the `P2PresourceState` guard. |
| `MsgcoreMFC_version.h` | The single source of version identity. |
| `dllmain.cpp` | Extension-DLL initialisation and MFC resource-state management. |
| `PenDialog`, `PencrxDialog`, `DlgToolBar` | Dialogs and the dialog tool bar shipped with the library. |
| `tests/` | The unit suites and their runner. |
| `res/` | Icons, bitmaps and the `.rc2` resource script. |

## Security

See [`SECURITY.md`](SECURITY.md) for how to report a vulnerability, what is in scope — the
clipboard transport is the widest attack surface here — and the limitations already known.

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md). Every commit needs a `Signed-off-by` line, all four
configurations must build warning-free, and the suite must stay green.

---

# Licence

MsgcoreMFC is licensed under the **Apache License, Version 2.0**. See [`LICENSE`](LICENSE) for the
full text, or <http://www.apache.org/licenses/LICENSE-2.0>.

```
Copyright 2001-2026 Ivyware Pty Ltd, Khrustal & Mann

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

## Files not covered by the licence

Some files here are Microsoft project-template, wizard-generated or sample-derived files. They keep
Microsoft's own notices and are **not** licensed under Apache 2.0: `Targetver.h`, `resource.h`,
`MsgcoreMFC.rc`, `res/MsgcoreMFC.rc2`, `stdafx.h`, `stdafx.cpp`, and the Visual Studio solution and
project files. `MsgcoreMFC.rc` is a mixed file — its `VERSIONINFO` block is ours and is
Apache-licensed. See [`NOTICE`](NOTICE) for the full list.

## Dependencies licensed separately

This library is an MFC extension DLL and links against the Microsoft Foundation Classes, the Visual
C++ runtime, and the Windows SDK (`Secur32`, `ShCore`, `Propsys`) — all licensed by Microsoft, none
redistributed here. These are dependencies, not bundled source.

It also depends on **Msgcore**, a separate repository in the same family, under the same licence
and the same copyright holders. It too is a dependency rather than bundled code: no Msgcore source
is included in this repository.
