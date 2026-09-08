// Copyright © 2002-2004, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CMenu extensions
//

#include "StdAfx.h"
#include "CMenu_Ext.h"

//
//  WM_INITPOPUPMENU emulator
//  NOTES: Popups isolated in DLL's cannot use the CMainFrame::OnInitPopupMenu()
//         mechanism for CCmdUI updates since command ID's are both unique to
//         and isolated in the DLL
//       : Referencing CMenu_OnInitPopupMenu() immediately before calling
//         CMenu::TrackPopupMenu() populates the CCmdUI structures.
//
BOOL
CMenu_OnInitPopupMenu ( CMenu *pCMenuPopup, CWnd *pCWnd )
{
    CCmdUI oCCmdUI;
    oCCmdUI.m_pMenu       = pCMenuPopup;
    oCCmdUI.m_pParentMenu = pCMenuPopup;
    oCCmdUI.m_nIndexMax   = pCMenuPopup -> GetMenuItemCount();

    for ( oCCmdUI.m_nIndex = 0; oCCmdUI.m_nIndex < oCCmdUI.m_nIndexMax; oCCmdUI.m_nIndex++ ) 
    {
      oCCmdUI.m_nID = pCMenuPopup -> GetMenuItemID(oCCmdUI.m_nIndex);

      // Menu separator or invalid command
      if ( oCCmdUI.m_nID == 0 )
        continue;                      // Nothing to do

      // Subordinate popup Menu, recurse
      if ( oCCmdUI.m_nID == (UINT)-1 )
      {
        CMenu *pCMenuSub = pCMenuPopup -> GetSubMenu ( oCCmdUI.m_nIndex );
        if ( pCMenuSub )
          CMenu_OnInitPopupMenu ( pCMenuSub, pCWnd );
        continue;
      }
      
      // Menu item, Auto disable if command is 
      oCCmdUI.m_pSubMenu = NULL;
      oCCmdUI.DoUpdate ( pCWnd, FALSE );
    }
    return TRUE;
}


//
//  Checks if menu item exists
MsgcoreMFC_EXT BOOL
CMenu_ItemExists ( const CMenu *pCMenu, UINT uiMenuItemID )
{
    return pCMenu->GetMenuState(uiMenuItemID,MF_BYCOMMAND) == 0xFFFFFFFF ? FALSE : TRUE;
}
