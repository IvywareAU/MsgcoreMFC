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
//  CBrush extension prototypes and definitions
//

#pragma   once
#include "P2Pmsg.h"
#include "MsgcoreMFC.h"

///  LOGPEN extension
//  NOTES: Extension for CMFCPropertyGridPenCtrl convenience
#define   PEN_DEFAULT_OPACITY 40
#define   PEN_LOW_OPACITY 25
#define   PEN_REGRESSION_OPACITY 30
struct LOGPENcrx : LOGPEN
{
    DWORD uAlpha;                      // Opacity
    DWORD uEoD;                        // Enables or Disables pen usage
    DWORD uSpare1;
}; 
typedef P2Pc_vBlob<LOGPENcrx>  P2P_LOGPENcrx;

//  EXTLOGPEN extension
//  NOTES: Extension for CMFCPropertyGridCtrl convenience
typedef P2Pc_vBlob<LOGPEN>     P2P_LOGPEN;
typedef P2Pc_vBlob<EXTLOGPEN>  P2P_EXTLOGPEN;

//  Translation of existing LOGPEN item to LOGPENcrx
BOOL MsgcoreMFC_EXT
LOGPEN2LOGPENcrx ( const P3PmsgItem& oItemParent, LPCTSTR lpszName );
