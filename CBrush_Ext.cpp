// Copyright © 2024, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
#include "StdAfx.h"
#include "P2Pmsg_Ext.h"
#include "CBrush_Ext.h"

//
//  LOGPEN translation to LOGPENcrx
//  NOTES: Static functions are just simpler
//
BOOL MsgcoreMFC_EXT
LOGBRUSH2LOGBRUSHcrx ( const P3PmsgItem& oItemParent, LPCTSTR lpszName )
{
    if ( !oItemParent.DESC.Exists(lpszName) )
      return FALSE;
    P2P_LOGBRUSH oLogBrush = oItemParent.DESC.SelectItem ( lpszName ).r_Object();
    //  Refuse an item that is ALREADY widened. The comparison is against the
    //  TARGET type: this guard read sizeof(LOGBRUSH), the source type, so a
    //  correctly stored LOGBRUSH matched >= exactly and the function returned
    //  FALSE for every well-formed input -- it could never convert anything.
    //  CPen_Ext.cpp's twin always compared against LOGPENcrx and was correct.
    if ( oLogBrush.r_data().Sizeof() >= sizeof(LOGBRUSHcrx) )
      return FALSE;
    // Translate
    LOGBRUSHcrx oLogBrushcrx;
    ZeroMemory ( &oLogBrushcrx, sizeof(oLogBrushcrx) );
    oLogBrushcrx.uEoD    = TRUE;
    oLogBrushcrx.uAlpha  = 255;

    oLogBrushcrx.lbStyle = oLogBrush->lbStyle;
    oLogBrushcrx.lbColor = oLogBrush->lbColor;
    oLogBrushcrx.lbHatch = oLogBrush->lbHatch;
    // Update
    oItemParent.DESC.SelectItem ( lpszName ).c_memcpy ( &oLogBrushcrx, sizeof(oLogBrushcrx) );

    // Tidy up, and
    return TRUE;
}
BOOL MsgcoreMFC_EXT
LOGBRUSH2LOGBRUSHcrx ( P3PmsgAttr& oAttrParent, LPCTSTR lpszName )
{
    if ( !oAttrParent.Exists(lpszName) )
      return FALSE;
    P2P_LOGBRUSH oLogBrush = oAttrParent.SelectItem ( lpszName ).r_Object();
    //  Refuse an item that is ALREADY widened. The comparison is against the
    //  TARGET type: this guard read sizeof(LOGBRUSH), the source type, so a
    //  correctly stored LOGBRUSH matched >= exactly and the function returned
    //  FALSE for every well-formed input -- it could never convert anything.
    //  CPen_Ext.cpp's twin always compared against LOGPENcrx and was correct.
    if ( oLogBrush.r_data().Sizeof() >= sizeof(LOGBRUSHcrx) )
      return FALSE;
    // Translate
    LOGBRUSHcrx oLogBrushcrx;
    ZeroMemory ( &oLogBrushcrx, sizeof(oLogBrushcrx) );
    oLogBrushcrx.uEoD    = TRUE;
    oLogBrushcrx.uAlpha  = 255;

    oLogBrushcrx.lbStyle = oLogBrush->lbStyle;
    oLogBrushcrx.lbColor = oLogBrush->lbColor;
    oLogBrushcrx.lbHatch = oLogBrush->lbHatch;
    // Update
    oAttrParent.SelectItem ( lpszName ).c_memcpy ( &oLogBrushcrx, sizeof(oLogBrushcrx) );

    // Tidy up, and
    return TRUE;
}
