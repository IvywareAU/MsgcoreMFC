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

//  LOGBRUSH extension
//  NOTES: Extension for CMFCPropertyGridBrushCtrl convenience
//typedef P2Pc_vBlob<LOGBRUSH>  P2P_LOGBRUSH;
#define   DEFAULT_OPACITY 40
#define   DEFAULT_OPACITY_CANDLES 40
#define   DEFAULT_OPACITY_BGBrush 10
#define   LOW_OPACITY 25
struct LOGBRUSHcrx : LOGBRUSH
{
    DWORD uAlpha;                      // Opacity
    DWORD uEoD;                        // Enables or Disables brush usage
    DWORD uSpare1;
}; 
typedef P2Pc_vBlob<LOGBRUSHcrx>  P2P_LOGBRUSHcrx;
struct LOGBRUSH32crx : LOGBRUSH32
{
    DWORD uAlpha;                      // Opacity
    DWORD uEoD;                        // Enables or Disables brush usage
    DWORD uSpare1;
}; 
typedef P2Pc_vBlob<LOGBRUSH32crx>  P2P_LOGBRUSH32crx;
typedef P2Pc_vBlob<LOGBRUSH>  P2P_LOGBRUSH;

//
//  LOGBRUSH translation to LOGBRUSHcrx
//  NOTES: Static functions are just simpler
//
BOOL MsgcoreMFC_EXT
LOGBRUSH2LOGBRUSHcrx ( const P3PmsgItem& oItemParent, LPCTSTR lpszName );
BOOL MsgcoreMFC_EXT
LOGBRUSH2LOGBRUSHcrx ( P3PmsgAttr& oAttrParent, LPCTSTR lpszName );
