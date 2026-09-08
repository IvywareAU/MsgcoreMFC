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
//  CFont extension prototypes and definitions
//

#pragma   once
#include "P2Pmsg.h"
#include "MsgcoreMFC.h"

//  LOGFONT COLORREF extension
//  NOTES: Extension for CMFCPropertyGridCtrl convenience
struct LOGFONTcrx : LOGFONTW
{
    UINT     uiCBVersion{3006};        // Chartboard version
    COLORREF oCOLORREF{0};
    LONG     lfHeightPoints{0};        // LOGFONT.lfHeight in Points
}; 
typedef P2Pc_vBlob<LOGFONT>    P2P_LOGFONT;
typedef P2Pc_vBlob<LOGFONTcrx> P2P_LOGFONTcrx;

MsgcoreMFC_EXT BOOL
LOGFONTcrxLocalise ( LPCWSTR lpszLOGFONTname, P3PmsgField& oItemLOGFONTcrx );
MsgcoreMFC_EXT void
LOGFONTcrxLocalise  ( LOGFONTcrx& oLOGFONTcrx, CDC *pDC = nullptr );
MsgcoreMFC_EXT void
LOGFONTcrxGlobalise  ( LOGFONTcrx& oLOGFONTcrx, CDC *pDC = nullptr );

//
//  CWnd message broadcasting
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients
//
MsgcoreMFC_EXT BOOL
StockObject2LOGFONT ( int iStockObjectID, LOGFONT& oLOGFONTW, LPCTSTR lpszFacename = 0 );
MsgcoreMFC_EXT BOOL
StockObject2LOGFONTcrx ( int iStockObjectID, LOGFONTcrx& oLOGFONTcrx, LPCTSTR lpszFacename = 0 );

//
//  LOGFONT sizing operations
MsgcoreMFC_EXT UINT
LOGFONT2PointSize  ( CDC *pDC, const LOGFONTW& oLOGFONT );
MsgcoreMFC_EXT UINT
LOGFONT2PixelSize  ( CDC *pDC, const LOGFONTW& oLOGFONT );
MsgcoreMFC_EXT UINT
LOGFONT2PixelSize  ( CDC *pDC, long lfHeight );
MsgcoreMFC_EXT long
Point2LOGFONTSize  ( CDC *pDC, long nPoints );
MsgcoreMFC_EXT void
LOGFONTcrxSerialise( LOGFONTcrx& oLOGFONTcrx, CDC *pDC = nullptr );
MsgcoreMFC_EXT void
LOGFONTcrxRestore  ( LOGFONTcrx& oLOGFONTcrx, CDC *pDC = nullptr );

//
//  Monitor resolution etc
//extern MsgcoreMFC_EXT UINT g_uiDpiX;
//extern MsgcoreMFC_EXT UINT g_uiDpiY;
//MsgcoreMFC_EXT HRESULT
//LOGFONT2MonitorReset ( HWND hWnd );
