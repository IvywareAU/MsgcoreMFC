# Security policy

## Reporting a vulnerability

Please do **not** open a public issue for a suspected vulnerability. Report it through
**GitHub private vulnerability reporting** — the *Report a vulnerability* button on this
repository's *Security* tab. That channel is private to the maintainers until an advisory
is published, and it keeps the report attached to the code it concerns.

> **One setup step, and it must be done before this file is published:** private
> vulnerability reporting is off by default. Enable it under *Settings → Code security →
> Private vulnerability reporting*. Until it is on, the button this file tells you to press
> is not on the page, and a policy that names a channel which does not exist is worse than
> one that names none.

Include in the report:

- the version or commit you tested, and which of the four configurations you built,
- which surface you reached it through — the serialisation bridge, the clipboard
  transport, `CStringParser`, or one of the window-bound helpers,
- the input that triggers it: a stored `.p2p` image, a clipboard payload, or the exact
  string handed to the parser,
- what you observed: a crash and its faulting address, a heap corruption report, a read
  or write outside an allocation, or an assertion.

Expect an acknowledgement within a week. There is no bounty programme.

## What is in scope

This library's exposure is narrower than Msgcore's but not zero, and it is concentrated in
the two places where bytes arrive from somewhere the process does not control:

- **The clipboard transport.** `P2Pmsg_Clipboard2Paste` takes a global memory block that
  *any* process on the desktop can place on the clipboard under a registered format, and
  interprets it as a `P2Piomage`. Anything reachable there — a length that does not match
  the block, a structure that walks outside the allocation — is in scope and is the most
  valuable class of report against this repository.
- **Deserialising a store written elsewhere.** `CListCtrl_DeserialiseHeader`,
  `CComboBox_Deserialise` and the crx widenings read fields out of a `P2PmsgMgr` image
  that may have come from a file another party wrote.
- **`CStringParser`.** It is a hand-rolled parser over caller-supplied text with its own
  `Scanf` implementation and fixed-size internal buffers.

A report that shows any of these reading or writing outside an allocation, or trusting a
declared length, is in scope.

## What is NOT in scope, and will not be treated as a vulnerability

- **Passing a malformed structure directly to an exported function in-process.** These are
  C++ APIs taking references; a caller that hands one a garbage `P3PmsgItem` is already
  inside the trust boundary. The interesting question is always whether the *stored bytes*
  were trusted.
- **The unaligned-overlay operators on `P2Pc_vBlob`.** `operator->` and the `StructType&`
  conversion reinterpret stored bytes with no alignment guarantee. This is known,
  documented in `P2Pmsg.h`, owned by Msgcore, and reported there — not here. It is
  undefined behaviour, not a reachable memory-safety bug on x86/x64, and the safe `Load()`
  / `Store()` pair exists beside it.
- **`ASSERT` firing on malformed input in a Debug build.** That is the assertion doing its
  job. A Release build that then proceeds to corrupt memory *is* in scope; say so
  explicitly if that is what you saw.
- **Anything that requires the attacker to already run code in the process**, control the
  DLL search path, or replace `Msgcore.dll` beside the binary.
- **Denial of service through a deliberately huge but well-formed store.** Reading a large
  image takes proportionate memory and time.

## Known limitations we already know about

These are recorded so a report does not spend effort rediscovering them. None is a secret,
and a working exploit for any of them is still worth reporting — it changes the priority.

- **The window-bound surface is untested.** Everything taking a live `CWnd`, `CDC`,
  `CListCtrl` or `CComboBox` has no automated coverage, because exercising it needs a
  desktop and a message pump. The `tests/` suites cover the headless helpers and the
  serialisation bridge only. This is a coverage gap, stated plainly rather than hidden
  behind stubs.
- **`CStringParser` predates the rest of the tree** and does its own `%d`/`%lf`/`%s`
  extraction into caller-sized buffers. It has the shape that rewards fuzzing, and it has
  not been fuzzed.
- **`Normalised2DATE` and `DATE2Normalised` are identity functions.** Both branches of
  each return the argument unchanged. They are not a security problem, but a caller
  assuming they normalise anything is mistaken.
- **The clipboard path has no fuzz harness.** Msgcore has `tests/fuzz`; this repository has
  no equivalent for the `P2Piomage` a paste accepts.

## Supported versions

| Version | Supported |
|---|---|
| 3.0.x | Yes |
| earlier | No |

Version identity comes from `MsgcoreMFC_version.h`; the DLL reports it in its
`VERSIONINFO` resource, so `FileVersion` on the shipped binary is the number to quote in a
report.
