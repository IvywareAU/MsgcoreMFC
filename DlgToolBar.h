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
#pragma   once
#include "MsgcoreMFC.h"

class MsgcoreMFC_EXT CDlgToolBar : public CToolBar
{
	  //DECLARE_DYNAMIC(CDlgToolBar)
    //public:
	  //    CDlgToolBar();   // standard constructor
	  //  virtual
    //   ~CDlgToolBar();

    // Attributes
    public:

    // CDialog integration utilities
    public:
      BOOL
        CDialog_OnInitDialog ( CDialog *pCDialog );

    // MFC Virtuals
    protected:

    // MFC message map
    protected:
	  DECLARE_MESSAGE_MAP()
      afx_msg LRESULT
        OnIdleUpdateCmdUI ( WPARAM wParam, LPARAM lParam );
};
