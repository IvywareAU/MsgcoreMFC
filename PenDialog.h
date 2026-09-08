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
//  CPenDialog prototypes and definitions
//  NOTES: Facilitates population of LOGPEN structure
//
#pragma once

#include "MsgcoreMFC.h"
//#include "resource.h"

// CPenDialog dialog

class MsgcoreMFC_EXT CPenDialog : public CDialogEx
{
	  DECLARE_DYNAMIC(CPenDialog)
      void
        RenderDialogSafe ( );
    // Constructors and destructor
    public:
        CPenDialog ( LPLOGPEN pLOGPENinitial = NULL
                   , DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS
                   , CDC* pdcPrinter = NULL
                   , CWnd *pCWndParent = NULL);
	      CPenDialog ( LOGPEN& oLOGPEN
                   , CWnd* pParent = NULL );
	    virtual
       ~CPenDialog();

    // Attributes
    public:
      LOGPEN             m_oLOGPEN;
      int                m_nSolid;
      int                m_bDash;
      int                m_bDot;
      int                m_bDashDot;
      int                m_bDashDotDot;
      DWORD              m_uPenSize{1};
      CBrush            *m_pCBrush;
      CMFCSpinButtonCtrl m_spnPenSize;
      CEdit              m_edtPenSize;

      const LOGPEN*
        GetCurrentPen ( ) const { return &m_oLOGPEN; }
      const COLORREF
        GetColor ( ) const { return m_oLOGPEN.lopnColor; }
	  //enum { IDD = IDD_LOGPEN };
    // MFC Virtual Overrides
    public:
      virtual INT_PTR
        DoModal ( );
    protected:
	    virtual void
        DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
      virtual BOOL
        OnInitDialog ( );
      afx_msg HBRUSH
        OnCtlColor ( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

    // MFC Message map
	  DECLARE_MESSAGE_MAP()
public:
      afx_msg
        void OnIDOK ( );
      afx_msg
        void OnBnClickedBtnColor ( );
};
