// Copyright © 2010-2011, 2026 Ivyware Pty Ltd, Khrustal & Mann
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

//  CDlgToolBar object
//  NOTES: Suitable for CDialog integration.  Implementation
//         pattern is to substitute CDlgToolBar for CToolBar
//       : Lifted from Microsoft MSDN
//         DLGCBR32: Demonstrates Adding a Status Bar and Toolbar
//         to Dialog Boxes
//
#include "StdAfx.h"
#include <afxpriv.h>                   // For WM_IDLEUPDATECMDUI
#include "DlgToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////
//  CDialog integration and processing utilities
//               NOTES: Following facilitates the CToolBar integration
//                      within a CDialog derived object
//

//  CDialog::OnInitDialog() integration and positioning
//
//
//  Parameters:  CDialog *pCDialog
//               Toolbar integration dialog
//
//  Returns:     BOOL
//               Integration result summary
BOOL
CDlgToolBar::CDialog_OnInitDialog ( CDialog *pCDialog )
{
    // Introduce locals

    // Size control bars
    CRect oCRectClientStart;
    CRect oCRectClientNow;
    pCDialog -> GetClientRect  ( oCRectClientStart );
    pCDialog -> RepositionBars ( AFX_IDW_CONTROLBAR_FIRST
                               , AFX_IDW_CONTROLBAR_LAST
                               , 0, CWnd::reposQuery, oCRectClientNow );

    // Reposition controls
    CPoint oCPointOS( oCRectClientNow.left - oCRectClientStart.left 
                    , oCRectClientNow.top -oCRectClientStart.top );
    CRect oCRectChild;
    CWnd  *pCWndChild = pCDialog -> GetWindow ( GW_CHILD );
    while ( pCWndChild )
    {
      pCWndChild -> GetWindowRect ( oCRectChild );
      pCDialog   -> ScreenToClient ( oCRectChild );
      oCRectChild.OffsetRect ( oCPointOS );
      pCWndChild -> MoveWindow ( oCRectChild, FALSE );
      pCWndChild =  pCWndChild -> GetNextWindow();
    }

    // Adjust the dialog window dimensions to make room for control bars
    CRect oCRectWindow;
    pCDialog -> GetWindowRect ( oCRectWindow );
    oCRectWindow.right  += oCRectClientStart.Width()  - oCRectClientNow.Width();
    oCRectWindow.bottom += oCRectClientStart.Height() - oCRectClientNow.Height();
    pCDialog -> MoveWindow ( oCRectWindow, FALSE );

    // Position Control Bars
    pCDialog -> RepositionBars ( AFX_IDW_CONTROLBAR_FIRST
                               , AFX_IDW_CONTROLBAR_LAST, 0 );

    // Tidy up and
    return TRUE;
}

//  CP2PeventDlg message handlers
BEGIN_MESSAGE_MAP(CDlgToolBar, CToolBar)
    ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

LRESULT
CDlgToolBar::OnIdleUpdateCmdUI ( WPARAM wParam, LPARAM )
{
    if ( IsWindowVisible() )
    {
      CFrameWnd *pParent = (CFrameWnd *)GetParent();
      if ( pParent )
        OnUpdateCmdUI ( pParent, (BOOL)wParam );
    }
    return 0L;
}