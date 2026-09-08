// Copyright © 2014, 2024, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CPen extension prototypes and definitions
//

#include "StdAfx.h"
#include "P2Pmsg_Ext.h"
#include "CPen_Ext.h"

//
//  LOGPEN translation to LOGPENcrx
//  NOTES: Static functions are just simpler
//
BOOL MsgcoreMFC_EXT
LOGPEN2LOGPENcrx ( const P3PmsgItem& oItemParent, LPCTSTR lpszName )
{
    if ( !oItemParent.DESC.Exists(lpszName) )
      return FALSE;
    P2P_LOGPEN oLogPen = oItemParent.DESC.SelectItem ( lpszName ).r_Object();
    if ( oLogPen.r_data().Sizeof() >= sizeof(LOGPENcrx) )
      return FALSE;
    // Translate
    LOGPENcrx oLogPencrx;
    ZeroMemory ( &oLogPencrx, sizeof(oLogPencrx) );
    oLogPencrx.uEoD        = TRUE;
    oLogPencrx.uAlpha      = 255;

    oLogPencrx.lopnColor   = oLogPen->lopnColor;
    oLogPencrx.lopnStyle   = oLogPen->lopnStyle;
    oLogPencrx.lopnWidth.x = oLogPen->lopnWidth.x;
    oLogPencrx.lopnWidth.y = oLogPen->lopnWidth.y;
    // Update
    oItemParent.DESC.SelectItem ( lpszName ).c_memcpy ( &oLogPencrx, sizeof(oLogPencrx) );

    // Tidy up, and
    return TRUE;
}
