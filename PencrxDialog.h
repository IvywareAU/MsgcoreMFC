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
//  CPencrxDialog prototypes and definitions
//  NOTES: Facilitates population of LOGPENcrx structure
//
#pragma once

#include "MsgcoreMFC.h"
#include "CPen_Ext.h"
//#include "resource.h"

// CPencrxDialog dialog

class MsgcoreMFC_EXT CPencrxDialog : public CDialogEx
{
	  DECLARE_DYNAMIC(CPencrxDialog)
      void
        RenderDialogSafe ( );
    // Constructors and destructor
    public:
        CPencrxDialog ( LOGPENcrx *pLOGPENinitial = NULL
                      , DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS
                      , CDC* pdcPrinter = NULL
                      , CWnd *pCWndParent = NULL);
	      CPencrxDialog ( LOGPENcrx& oLOGPENcrx
                      , CWnd* pParent = NULL );
	    virtual
       ~CPencrxDialog();

    // Attributes
    public:
      LOGPENcrx          m_oLOGPENcrx;
      int                m_nSolid;
      int                m_bDash;
      int                m_bDot;
      int                m_bDashDot;
      int                m_bDashDotDot;
      DWORD              m_uPenSize{1};
      CBrush            *m_pCBrush{nullptr};
      CMFCSpinButtonCtrl m_spnPenSize;
      CEdit              m_edtPenSize;
      int                m_bEoD{TRUE};
      DWORD              m_uAlpha{75};
      CMFCSpinButtonCtrl m_spnOpacity;
      CEdit              m_edtOpacity;

      const LOGPENcrx*
        GetCurrentPen ( ) const { return &m_oLOGPENcrx; }
      const COLORREF
        GetColor ( ) const { return m_oLOGPENcrx.lopnColor; }
	  //enum { IDD = IDD_LOGPEN };
    // MFC Virtual Overrides
    public:
      virtual INT_PTR
        DoModal ( ) override;
    protected:
	    virtual void
        DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
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
