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
//  CPenDialog implementations.
//  NOTES: Facilitates population of LOGPEN structure
//

#include "stdafx.h"
#include "MsgcoreMFC.h"
#include "PenDialog.h"
#include "afxdialogex.h"

#include "Resource.h"
#include "Msgexception.h"

// CPenDialog dialog

IMPLEMENT_DYNAMIC(CPenDialog, CDialogEx)

CPenDialog::CPenDialog ( LOGPEN& oLOGPEN, CWnd *pParent )
	        : CDialogEx(IDD_LOGPEN, pParent)
{
    m_oLOGPEN = oLOGPEN;
    RenderDialogSafe ( );
}
CPenDialog::CPenDialog ( LPLOGPEN pLOGPENinitial
                       , DWORD dwFlags
                       , CDC* pdcPrinter
                       , CWnd *pCWndParent )
	        : CDialogEx ( IDD_LOGPEN, pCWndParent )
{
    UNREFERENCED_PARAMETER(dwFlags);
    UNREFERENCED_PARAMETER(pdcPrinter);
    if ( pLOGPENinitial )
      memcpy ( &m_oLOGPEN, pLOGPENinitial, sizeof(m_oLOGPEN) );
    RenderDialogSafe ( );
}

CPenDialog::~CPenDialog()
{
    delete m_pCBrush;
}

void
CPenDialog::RenderDialogSafe ( )
{
    m_nSolid  = -1;
    m_pCBrush =  0;
    if ( m_oLOGPEN.lopnStyle == PS_SOLID )
      m_nSolid = 0;
    else if ( m_oLOGPEN.lopnStyle == PS_DASH )
      m_nSolid = 1;
    else if ( m_oLOGPEN.lopnStyle == PS_DOT )
      m_nSolid = 2;
    else if ( m_oLOGPEN.lopnStyle == PS_DASHDOT )
      m_nSolid = 3;
    else if ( m_oLOGPEN.lopnStyle == PS_DASHDOTDOT )
      m_nSolid = 4;
    m_uPenSize = max ( 1, min(m_oLOGPEN.lopnWidth.x,20) ); 
}
///////////////////////////////////////////////////////////////////////
//  MFC Virtual Overrides

//
//  Resource management override
//
INT_PTR
CPenDialog::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

void
CPenDialog::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialogEx::DoDataExchange(pDX);
    DDX_Control( pDX, IDC_SPN_PENSIZE, m_spnPenSize );
    DDX_Control( pDX, IDC_EDT_PENSIZE, m_edtPenSize );
    DDX_Radio ( pDX, IDC_RAD_SOLID, m_nSolid);
    //DDX_Radio ( pDX, IDC_RAD_DASH, m_bDash);
    //DDX_Radio ( pDX, IDC_RAD_DOT, m_bDot);
    //DDX_Radio ( pDX, IDC_RAD_DASHDOT, m_bDashDot);
    //DDX_Radio ( pDX, IDC_RAD_DASHDOTDOT, m_bDashDotDot);
}

//
//  OnInitDialog MFC Virtual override
//
BOOL
CPenDialog::OnInitDialog ( )
{
    // Compliments Class Wizard
    MANAGE_RESOURCE_STATE;
  __super::OnInitDialog();

    // Expect issues
    try
    {
      m_spnPenSize.SetRange ( 1, 20 );
      //m_spnOpacity.SetBuddy ( &m_edtOpacity );
      m_uPenSize = m_oLOGPEN.lopnWidth.x;
      if ( m_uPenSize < 1 )
        m_uPenSize = 1;
      else if ( m_uPenSize > 20 )
        m_uPenSize = 20;
      m_spnPenSize.SetPos ( m_uPenSize );
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
CPenDialog::OnCtlColor ( CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    MANAGE_RESOURCE_STATE;
    // Call the base class implementation first! Otherwise, it may 
    // undo what we're trying to accomplish here.
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // Isolate control of interest 
    if ( IDC_STA_COLOR == pWnd->GetDlgCtrlID() )
   {
      COLORREF oColorref = m_oLOGPEN.lopnColor;
      if ( m_pCBrush == nullptr )
        m_pCBrush = new CBrush(oColorref);
      // Set the selected color
      pDC->SetTextColor(oColorref);

      // Set the background mode for text to transparent  
      // so background will show thru.
      pDC->SetBkMode(TRANSPARENT);

      // Return handle to our CBrush object
      hbr = *m_pCBrush;
    }

    return hbr;
}

///////////////////////////////////////////////////////////////////////
//  MFC Message map

BEGIN_MESSAGE_MAP(CPenDialog, CDialogEx)
  ON_WM_CTLCOLOR() 
  ON_COMMAND(IDOK, &CPenDialog::OnIDOK)
  ON_BN_CLICKED(IDC_BTN_COLOR, &CPenDialog::OnBnClickedBtnColor)
END_MESSAGE_MAP()

//
//  ID_OK command handler
void
CPenDialog::OnIDOK ( )
{
    // TODO: Add your command handler code here
    UpdateData ( );
    if ( m_nSolid == 0 )
      m_oLOGPEN.lopnStyle = PS_SOLID;
    else if ( m_nSolid == 1 )
      m_oLOGPEN.lopnStyle = PS_DASH;
    else if ( m_nSolid == 2 )
      m_oLOGPEN.lopnStyle = PS_DOT;
    else if ( m_nSolid == 3 )
      m_oLOGPEN.lopnStyle = PS_DASHDOT;
    else if ( m_nSolid == 4 )
      m_oLOGPEN.lopnStyle = PS_DASHDOTDOT;
    else
      m_oLOGPEN.lopnStyle = PS_SOLID;
    m_uPenSize = m_spnPenSize.GetPos ( );
    m_oLOGPEN.lopnWidth.x = m_uPenSize;
    m_oLOGPEN.lopnWidth.y = 0;    
  __super::OnOK ( );  
}

void
CPenDialog::OnBnClickedBtnColor()
{
    MANAGE_RESOURCE_STATE;
    // TODO: Add your control notification handler code here
    CColorDialog oColorDialog ( m_oLOGPEN.lopnColor, 0u, this );
    if ( oColorDialog.DoModal() == IDOK )
      m_oLOGPEN.lopnColor = oColorDialog.GetColor();
    delete m_pCBrush;
           m_pCBrush = nullptr;
    GetDlgItem(IDC_STA_COLOR)->Invalidate();

    // Tidy up, and
    UpdateData(FALSE);
}
