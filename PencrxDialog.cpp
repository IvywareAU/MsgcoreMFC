// Copyright © 2018, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CPencrxDialog implementations.
//  NOTES: Facilitates population of LOGPENcrx structure
//

#include "stdafx.h"
#include "MsgcoreMFC.h"
#include "PencrxDialog.h"
#include "afxdialogex.h"

#include "Resource.h"
#include "Msgexception.h"

// CPencrxDialog dialog

IMPLEMENT_DYNAMIC(CPencrxDialog, CDialogEx)

CPencrxDialog::CPencrxDialog ( LOGPENcrx& oLOGPENcrx, CWnd *pParent )
	           : CDialogEx(IDD_LOGPENcrx, pParent)
{
    m_oLOGPENcrx = oLOGPENcrx;
    RenderDialogSafe ( );
}
CPencrxDialog::CPencrxDialog ( LOGPENcrx *pLOGPENinitial
                             , DWORD dwFlags
                             , CDC* pdcPrinter
                             , CWnd *pCWndParent )
	           : CDialogEx ( IDD_LOGPENcrx, pCWndParent )
{
    UNREFERENCED_PARAMETER(dwFlags);
    UNREFERENCED_PARAMETER(pdcPrinter);
    if ( pLOGPENinitial )
      memcpy ( &m_oLOGPENcrx, pLOGPENinitial, sizeof(m_oLOGPENcrx) );
    RenderDialogSafe ( );
}

CPencrxDialog::~CPencrxDialog()
{
    delete m_pCBrush;
}

void
CPencrxDialog::RenderDialogSafe ( )
{
    m_nSolid  = -1;
    m_pCBrush =  nullptr;
    if ( m_oLOGPENcrx.lopnStyle == PS_SOLID )
      m_nSolid = 0;
    else if ( m_oLOGPENcrx.lopnStyle == PS_DASH )
      m_nSolid = 1;
    else if ( m_oLOGPENcrx.lopnStyle == PS_DOT )
      m_nSolid = 2;
    else if ( m_oLOGPENcrx.lopnStyle == PS_DASHDOT )
      m_nSolid = 3;
    else if ( m_oLOGPENcrx.lopnStyle == PS_DASHDOTDOT )
      m_nSolid = 4;
    else { ASSERT(0); m_nSolid = 0; }
    m_uPenSize = max ( 1, min(m_oLOGPENcrx.lopnWidth.x,20) );
    m_bEoD     = TRUE; 
}
///////////////////////////////////////////////////////////////////////
//  MFC Virtual Overrides

//
//  Resource management override
//
INT_PTR
CPencrxDialog::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

void
CPencrxDialog::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialogEx::DoDataExchange(pDX);
    DDX_Control( pDX, IDC_SPN_PENSIZE, m_spnPenSize );
    DDX_Control( pDX, IDC_EDT_PENSIZE, m_edtPenSize );
    DDX_Radio ( pDX, IDC_RAD_SOLID, m_nSolid);
    DDX_Control( pDX, IDC_SPN_OPACITY, m_spnOpacity );
    DDX_Control( pDX, IDC_EDT_OPACITY, m_edtOpacity );
    //DDX_Radio ( pDX, IDC_RAD_DASH, m_bDash);
    //DDX_Radio ( pDX, IDC_RAD_DOT, m_bDot);
    //DDX_Radio ( pDX, IDC_RAD_DASHDOT, m_bDashDot);
    //DDX_Radio ( pDX, IDC_RAD_DASHDOTDOT, m_bDashDotDot);
}

//
//  OnInitDialog MFC Virtual override
//
BOOL
CPencrxDialog::OnInitDialog ( )
{
    // Compliments Class Wizard
    MANAGE_RESOURCE_STATE;
  __super::OnInitDialog();

    // Expect issues
    try
    {
      m_spnPenSize.SetRange ( 1, 80 );
      //m_spnOpacity.SetBuddy ( &m_edtOpacity );
      m_uPenSize = m_oLOGPENcrx.lopnWidth.x;
      if ( m_uPenSize < 1 )
        m_uPenSize = 1;
      else if ( m_uPenSize > 80 )
        m_uPenSize = 80;
      m_spnPenSize.SetPos ( m_uPenSize );
      m_spnOpacity.SetRange ( 10, 255 );
      //m_spnOpacity.SetBuddy ( &m_edtOpacity );
      m_uAlpha = m_oLOGPENcrx.uAlpha;
      if ( m_uAlpha < 10 )
        m_uAlpha = 10;
      else if ( m_uAlpha > 255 )
        m_uAlpha = 255;
      m_spnOpacity.SetPos ( m_uAlpha );
    }

    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

//  ON_WM_CTLCOLOR handler lifted from Microsoft example
//  NOTES: This OnCtlColor handler will change the color of a static control 
//         with the ID of IDC_STA_COLOR. The code assumes that the CBStoreShadesetDlg
//         class has an initialized and created CBrush member named m_pCBrush. 
//         The control will be painted with selected background color of m_brush.
HBRUSH
CPencrxDialog::OnCtlColor ( CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    MANAGE_RESOURCE_STATE;
    // Call the base class implementation first! Otherwise, it may 
    // undo what we're trying to accomplish here.
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // Isolate control of interest 
    if ( IDC_STA_COLOR == pWnd->GetDlgCtrlID() )
   {
      COLORREF oColorref = m_oLOGPENcrx.lopnColor;
      if ( m_pCBrush == nullptr )
        m_pCBrush = new CBrush(oColorref);
      // Set the selected color
      pDC -> SetTextColor(oColorref);

      // Set the background mode for text to transparent  
      // so background will show thru.
      pDC -> SetBkMode(TRANSPARENT);

      // Return handle to our CBrush object
      hbr = *m_pCBrush;
    }

    return hbr;
}

///////////////////////////////////////////////////////////////////////
//  MFC Message map

BEGIN_MESSAGE_MAP(CPencrxDialog, CDialogEx)
  ON_WM_CTLCOLOR() 
  ON_COMMAND(IDOK, &CPencrxDialog::OnIDOK)
  ON_BN_CLICKED(IDC_BTN_COLOR, &CPencrxDialog::OnBnClickedBtnColor)
END_MESSAGE_MAP()

//
//  ID_OK command handler
void
CPencrxDialog::OnIDOK ( )
{
    MANAGE_RESOURCE_STATE;
    // TODO: Add your command handler code here
    UpdateData ( );
    if ( m_nSolid == 0 )
      m_oLOGPENcrx.lopnStyle = PS_SOLID;
    else if ( m_nSolid == 1 )
      m_oLOGPENcrx.lopnStyle = PS_DASH;
    else if ( m_nSolid == 2 )
      m_oLOGPENcrx.lopnStyle = PS_DOT;
    else if ( m_nSolid == 3 )
      m_oLOGPENcrx.lopnStyle = PS_DASHDOT;
    else if ( m_nSolid == 4 )
      m_oLOGPENcrx.lopnStyle = PS_DASHDOTDOT;
    else
      m_oLOGPENcrx.lopnStyle = PS_SOLID;
    m_uPenSize = m_spnPenSize.GetPos ( );
    m_oLOGPENcrx.lopnWidth.x = m_uPenSize;
    m_oLOGPENcrx.lopnWidth.y = 0;    
    m_uAlpha                 = m_spnOpacity.GetPos ( );
    m_oLOGPENcrx.uEoD        = m_bEoD;
    m_oLOGPENcrx.uAlpha      = m_uAlpha;
  __super::OnOK ( );  
}

void
CPencrxDialog::OnBnClickedBtnColor()
{
    MANAGE_RESOURCE_STATE;
    // TODO: Add your control notification handler code here
    CColorDialog oColorDialog ( m_oLOGPENcrx.lopnColor, 0u, this );
    if ( oColorDialog.DoModal() == IDOK )
      m_oLOGPENcrx.lopnColor = oColorDialog.GetColor();
    delete m_pCBrush;
           m_pCBrush = nullptr;
    GetDlgItem(IDC_STA_COLOR)->Invalidate();

    // Tidy up, and
    UpdateData(FALSE);
}
