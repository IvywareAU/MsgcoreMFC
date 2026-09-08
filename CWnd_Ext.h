// Copyright © 2002-2014, 2024, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CWnd extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"

//
//  CWnd message broadcasting
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients
//
MsgcoreMFC_EXT BOOL
CWnd_BCastRegister( CWnd *pCWnd, UINT nID, BOOL bRegister );
MsgcoreMFC_EXT BOOL
CWnd_BCastMessage ( UINT nID, WPARAM wParam, LPARAM lParam );
MsgcoreMFC_EXT BOOL
CWnd_BCastCancel  ( const CWnd *pCWnd );
MsgcoreMFC_EXT BOOL
IsRunAsAdministrator ( );

//
//  Targeted CWnd message broadcasting
//  NOTES: Static functions allow for scattered hierarchy of windows
//       : Facilitates message broadcasting to registered clients within domain
//MsgcoreMFC_EXT UINT
//CWnd_CreateTarget ( );
//MsgcoreMFC_EXT BOOL
//CWnd_RegisterTarget( UINT nCWndTargetID, CWnd *pCWnd, UINT nWMid, BOOL bRegister );
//MsgcoreMFC_EXT BOOL
//CWnd_BCastTargets ( UINT nCWndTargetID, UINT nWMid, WPARAM wParam, LPARAM lParam );
//MsgcoreMFC_EXT UINT
//CWnd_CloseTarget ( UINT nCWndTargetID );

