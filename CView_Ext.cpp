// Copyright © 2021, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CView & CScrollView extensions
//

#include "StdAfx.h"
#include "CView_Ext.h"

//
//  Summarises scroll bar visibility
//
//  Parameters:  CScrollView& oCScrollView
//               View to be checked
//
//               wBar
//               Scrollbar to be checked
//                 SB_HORZ
//                 SB_VERT
//  Returns:     BOOL
//                 TRUE... Visible
//                 FALSE.. Not visible
BOOL
CScrollView_IsVisible ( const CScrollView& oCScrollView, int wBar )
{
    BOOL bHorz;
    BOOL bVert;
    oCScrollView.CheckScrollBars ( bHorz, bVert );
    if ( wBar == SB_HORZ )
      return bHorz;
    if ( wBar == SB_VERT )
      return bVert;
    ASSERT(0);
    return FALSE;
}

