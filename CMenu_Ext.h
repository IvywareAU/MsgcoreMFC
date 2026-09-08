// Copyright © 2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CMenu extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"

//
//  WM_INITPOPUPMENU emulator
//  NOTES: Popups isolated in DLL's cannot use the CMainFrame::OnInitPopupMenu()
//         mechanism for CCmdUI updates since command ID's are both unique to
//         and isolated in the DLL
//       : Referencing CMenu_OnInitPopupMenu() immediately before calling
//         CMenu::TrackPopupMenu() populates the CCmdUI structures.
//
MsgcoreMFC_EXT BOOL
CMenu_OnInitPopupMenu ( CMenu *pCMenuPopup, CWnd *pCWnd );

//
//  Checks if menu item exists
MsgcoreMFC_EXT BOOL
CMenu_ItemExists ( const CMenu *pCMenu, UINT uiMenuItemID );
