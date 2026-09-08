// Copyright © 2013, 2018, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CWndApp extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"

//
//  CWndApp application file helpers
//  NOTES: Static functions are just simpler
//
MsgcoreMFC_EXT CString
CWinApp_LocalAppDataFilePath( LPCTSTR lpszDataFileName );
MsgcoreMFC_EXT CString
CWinApp_ProgramDataFilePath( LPCTSTR lpszDataFileName );
MsgcoreMFC_EXT CString
CWinApp_ModuleFolderPath( );
MsgcoreMFC_EXT CView*
CWndApp_GetActiveView ( );
MsgcoreMFC_EXT CString
CWndApp_GetUsername ( );
MsgcoreMFC_EXT BOOL
CWndApp_PostHtmlURL ( LPCTSTR lpszURL, BOOL bExternalOverride );
extern MsgcoreMFC_EXT
UINT g_uiWM_PostHtmlURL;               // PostHtmlURL notification message

//
//  System sequence helpers
MsgcoreMFC_EXT void
RunMsgPump2Completion ( ) noexcept;
MsgcoreMFC_EXT DWORD_PTR
extern g_dwRunMsgPump2Completion;
