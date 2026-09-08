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
//  MsgcoreMFC_version.h - the single source of version identity.
//
//  NOTES: This header is the ONLY place a version number is written. It is
//         consumed by two parties that must never disagree:
//           - MsgcoreMFC.rc -> the DLL's VERSIONINFO resource
//           - MsgcoreMFC.h  -> the compile-time macros a consumer tests
//                              against
//         Bump it here and both of them move together.
//       : This file exists because they DID disagree. Before it, the resource
//         script hardcoded FILEVERSION 2,1,0,1 beside PRODUCTVERSION 3,0,0,0
//         and a "FileVersion" string of "2.1.0.1" beside a "ProductVersion"
//         string of "3.0.0.0" - four numbers, two of them stale, with nothing
//         in the build to notice. A shipped DLL reported a file version no
//         release ever carried.
//       : It must stay preprocessor-only. rc.exe compiles it as well as the
//         C++ compiler, and rc.exe understands #define and nothing else - no
//         types, no enums, no inline functions, no const. Anything that is
//         not a macro belongs in another header.
//       : Keep the release tag and this file in step: version 3.0.0 is tag
//         v3.0.0. A build whose DLL reports a version no tag matches cannot
//         be traced back to a source state, which defeats the point.
//
#pragma once

//  Component version. MAJOR.MINOR.PATCH is the released identity; BUILD is
//  reserved for a CI build counter and is 0 for a hand-built binary.
//
//  3.0.0.0, matching the PRODUCTVERSION this library has already been
//  declaring, and matching Msgcore's identity at this release. The two are
//  deliberately INDEPENDENT and under no obligation to stay equal: MsgcoreMFC
//  links Msgcore but does not ship as it, and a shared number would force a
//  lockstep release neither wants. They agree here because this is the first
//  public release of both.
#define MSGCOREMFC_VERSION_MAJOR  3
#define MSGCOREMFC_VERSION_MINOR  0
#define MSGCOREMFC_VERSION_PATCH  0
#define MSGCOREMFC_VERSION_BUILD  0

//  Comma form, for the FILEVERSION / PRODUCTVERSION resource statements,
//  which take four comma-separated words and cannot take a macro expression.
#define MSGCOREMFC_VERSION_COMMAS 3,0,0,0

//  String form. Kept spelled out rather than stringised from the parts above:
//  rc.exe's preprocessor has no reliable ## / # operator support, and a
//  VERSIONINFO string that silently expands to "MSGCOREMFC_VERSION_MAJOR.0.0"
//  would ship without anyone noticing.
#define MSGCOREMFC_VERSION_STRING "3.0.0.0"

//  Packed form, for a consumer that wants to compare rather than display.
//  0x03000000 is 3.0.0.0; the byte order is MAJOR, MINOR, PATCH, BUILD.
#define MSGCOREMFC_VERSION_HEX    0x03000000

//  Fixed identity strings shared by the resource.
#define MSGCOREMFC_COMPANY_NAME   "Ivyware Pty Ltd, Khrustal & Mann"
#define MSGCOREMFC_PRODUCT_NAME   "MsgcoreMFC"
#define MSGCOREMFC_COPYRIGHT      "Copyright \251 2001-2026 Ivyware Pty Ltd, Khrustal & Mann. " \
                                  "Licensed under the Apache License, Version 2.0."

#ifndef RC_INVOKED

//  Compile-time guard for a consumer that needs a minimum version. Not
//  available to rc.exe, which cannot evaluate a function-like macro.
//
//    #if !MSGCOREMFC_VERSION_AT_LEAST(3,0,0)
//    #  error MsgcoreMFC 3.0.0 or later is required
//    #endif
//
#define MSGCOREMFC_VERSION_AT_LEAST(maj,min,pat) \
    ( ( (maj) << 24 | (min) << 16 | (pat) << 8 ) <= MSGCOREMFC_VERSION_HEX )

#endif  // RC_INVOKED
