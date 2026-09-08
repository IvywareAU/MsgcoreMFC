# Contributing to MsgcoreMFC

## Before you spend time on a change

MsgcoreMFC is the MFC layer of the MSCS family, and most of what it exports is a thin
extension over an MFC class. Two constraints shape almost every change here:

- **It is an MFC extension DLL.** Everything crossing the boundary is `MsgcoreMFC_EXT`,
  built with `_AFXEXT` against the shared MFC runtime. Changing an exported signature
  breaks every consumer at link time, not compile time.
- **It links [Msgcore](../Msgcore).** The serialisation bridge takes `P3PmsgItem`,
  `P3PmsgAttr` and `P3PmsgField` by reference in its public signatures. A change to how
  those are used here is a change to a contract another repository owns.

If you are proposing something that alters either, say so in the issue before writing
the code.

## Sign your work — the Developer Certificate of Origin

Every commit must carry a `Signed-off-by` line:

```
Signed-off-by: Jane Developer <jane@example.com>
```

`git commit -s` adds it for you. Use your real name and an address you read.

That line means you certify the [Developer Certificate of Origin
1.1](https://developercertificate.org/): that you wrote the contribution or otherwise
have the right to submit it under the Apache License, Version 2.0, and that you
understand the contribution and its record are public and permanent.

**Why this is enforced from the first commit rather than added later.** Only a rights
holder can license code. Once a contribution arrives with no record of who held the
rights and under what terms, the project can no longer answer that question for its own
tree — and the option of ever relicensing, dual-licensing, or granting an exception
closes permanently, because there is nobody identifiable to ask. A sign-off is the
cheapest possible way to keep that answerable. A pull request without one cannot be
merged, no matter how good it is.

## Making a change

1. **One concern per commit.** A refactor and a behaviour change in the same commit
   cannot be reviewed, reverted or bisected independently.

2. **Write the message for someone reading it in five years.** Say what changed and
   *why* — the subject line as an imperative sentence, the body for the reasoning.

3. **Build all four configurations.** `Debug` / `Release` × `x86` / `x64`. They share one
   diagnostic regime — `/W4`, `/std:c++20`, Unicode, shared MFC — so a change that
   compiles under one should compile under all, but the linkage differs and 32-bit still
   catches pointer-width mistakes 64-bit does not. **Zero errors and zero warnings from
   this project is the standing bar**, and it is currently met.

   Note the platform naming: the solution calls the 32-bit platform `x86`, the project
   underneath calls it `Win32`. Build the solution with `-p:Platform=x86`.

4. **Run the suite.** `.\tests\build_run_suite.bat` — 30 cases / 82 checks, all passing.
   It builds against the `Debug|x64` DLL, so **build the library first**; the script
   links the import library it finds and will otherwise happily test the previous binary.

   If your change touches the serialisation bridge — the `P3Pmsg*` signatures, the crx
   widenings, the clipboard transport — **add a case to `P2PmsgBridgeSuite.cpp`**, and
   assert on the bytes that come back out of a real store rather than on the return
   value. A conversion that returns `TRUE` and writes the wrong bytes passes a
   return-value test; that is exactly how `LOGBRUSH2LOGBRUSHcrx` shipped for years
   comparing against the wrong `sizeof` and never converting anything.

   If your change touches a headless helper, add a case to `MsgcoreMFCSuite.cpp` instead.

5. **Do not add a warning.** See point 3.

6. **The window-bound surface has no tests, and that is written down rather than
   papered over.** Anything taking a live `CWnd`, `CDC`, `CListCtrl` or `CComboBox`
   needs a desktop and a message pump to exercise. Do not add a stub that always passes
   to close the gap — a stub reads like coverage and is worse than an absence that is
   documented.

## House style

Match the file you are editing — it is older than any convention document and it is
internally consistent. In particular:

- Spaces inside parentheses in calls and declarations: `Foo ( a, b )`.
- The return type on its own line above the function name, as every `*_Ext.cpp` does.
- `NOTES:` comment blocks above a declaration explain *why*, not what. When you fix
  something subtle, leave the reasoning behind in one of these.
- Do not delete a comment that records a hazard just because you fixed the code around
  it. Rewrite it to say what is true now.

## Three things that will get a change rejected on sight

- **Reading a stored blob through `operator->` or the `StructType&` conversion.**
  `P2Pmsg.h` says plainly that those overlay a struct on stored bytes that carry no
  alignment guarantee, and that every access through them is undefined behaviour for a
  type wanting alignment. Use `Load()` and `Store()`, which are the same bytes through a
  `memcpy`. The existing consumers predate the safe pair; new code does not get to.

- **Comparing a size against the source type in a widening guard.** The crx conversions
  refuse to run when the stored item is already at least as large as the *target*. Get
  that backwards and the function returns `FALSE` for every well-formed input while
  looking entirely reasonable.

- **A version number written anywhere but `MsgcoreMFC_version.h`.** The resource script
  reads its `FILEVERSION`, `PRODUCTVERSION` and both version strings from that header.
  It exists because those four numbers had drifted apart and shipped that way.

## Licence

Contributions are accepted under the Apache License, Version 2.0 — see [`LICENSE`](LICENSE).
Every new source file carries the standard header; copy one from a neighbouring file and
put the current year on it.
