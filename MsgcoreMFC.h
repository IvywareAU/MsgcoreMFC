// Copyright © 2006-2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//
//  Implements MsgcoreMFC.DLL extension class definitions
//  NOTES: To be included with the definitions for any object exported
//         from the MsgcoreMFC.DLL
//       : Patterned on the MFC_EXT_CLASS concept
//       : Either the MsgcoreMFC project or the StdAfx.h file MUST
//         contain the MsgcoreMFC_EXPORTS definition.  For further Developer
//         Studio details refer Project Properties >> Config >> C/C++
//         Preprocessor
//

#pragma once

#include "MsgcoreMFC_version.h"

#if defined (MsgcoreMFC_EXPORTS)
  #define MsgcoreMFC_EXT __declspec(dllexport)
#else
  #define MsgcoreMFC_EXT __declspec(dllimport)
#endif

//
//  Manages MFC resource state for P2PmsgraphDlgs DLL
//  NOTES: Not to be exported, MUST remain private to DLL for which it's instanciated
//       : Each MFC extension DLL requires it's own specialised implementation
//         for its own resource instance
//       : MANAGE_RESOURCE_STATE preceeds any local resource reference. Stack
//         implementation restores previous state upon exit
//       : CDialog derived classes require DoModal_EoD() to be overridden with
//         int <name>Dlg::DoModal_EoD()
//         {
//           MANAGE_RESOURCE_STATE;
//           return CDialog::DoModal_EoD();
//         }
#if defined (MsgcoreMFC_EXPORTS)
class P2PresourceState                 // Do not export
{
    public:
      P2PresourceState();
     ~P2PresourceState();
    protected:
      HINSTANCE m_hRestore;
};
#define MANAGE_RESOURCE_STATE P2PresourceState oP2Pstate
#endif

