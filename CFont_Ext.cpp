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

#include "StdAfx.h"
#include "shellscalingapi.h"
#include "CFont_Ext.h"

//
//  Populates LOGFONT object using passed StockObjectID
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients
//
//  Parameters:  int iStockObjectID
//               Default stockObject to be used (DEFAULT_GUI_FONT)
//
//               LOGFONT& oLOGFONT
//               Item to be populated
//
//               LPCTSTR lpszFacename = 0
//               Optional LOGFONT facename
BOOL
StockObject2LOGFONT ( int iStockObjectID, LOGFONTW& oLOGFONT, LPCTSTR lpszFacename )
{
	  CFont *pCFont = CFont::FromHandle ( (HFONT)GetStockObject(iStockObjectID) );
	  pCFont -> GetLogFont ( &oLOGFONT );
    if ( lpszFacename             &&
         _tcslen(lpszFacename) > 0    )
      lstrcpy ( oLOGFONT.lfFaceName, lpszFacename );
    return TRUE;
}

//
//  Fetches Point size from LOGFONT structure
//
//  Parameters: const LOGFONT& oLOGFONT
//
//  Returns:    UINT
//              Calculated Point Size
MsgcoreMFC_EXT UINT
LOGFONT2PointSize ( CDC *pDC, const LOGFONTW& oLOGFONT )
{
    if ( oLOGFONT.lfHeight >= 0 )
    {
      //LONG lfHeight = (72 * oLOGFONT.lfHeight) / GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY);
      //ASSERT(lfHeight==oLOGFONT.lfHeight);
      return oLOGFONT.lfHeight;
    }
    return MulDiv ( -oLOGFONT.lfHeight, 72, ::GetDeviceCaps(pDC->GetSafeHdc(),LOGPIXELSY) );
}

//
//  Fetches Font size from LOGFONT structure in pixels
//  NOTES: LOGFONT.lfHeight is stored in points which has to be
//         translated according to screen resolution
//       : Points translation to pixels involves division by 72 to 
//         convert to inches and them multiplies by DPI for the monitor
//         to convert to pixels.
//
//  Parameters: const LOGFONT& oLOGFONT
// 
//              LONG lfHeight
//              Raw unattached LOGFONT.lfHeight member
//
//  Returns:    UINT
//              Calculated Pixel Size/Height
//
MsgcoreMFC_EXT UINT
LOGFONT2PixelSize ( CDC *pDC, const LOGFONTW& oLOGFONT )
{
    if ( oLOGFONT.lfHeight >= 0 )
    {
      int nDpiY = ::GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY);
      //return MulDiv(oLOGFONT.lfHeight, ::GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 72);
      int nSize =  MulDiv(oLOGFONT.lfHeight, nDpiY, 72);
      return nSize;
    }
    // LOGFONT.lfHeight in raw pixels
    return -oLOGFONT.lfHeight;
}
MsgcoreMFC_EXT UINT
LOGFONT2PixelSize ( CDC *pDC, LONG lfHeight )
{
    if ( lfHeight >= 0 )
    {
      int nDpiY = ::GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY);
      int nSize =  MulDiv(lfHeight, nDpiY, 72);
      return nSize;
    }
    // LOGFONT.lfHeight in raw pixels
    return lfHeight;
}

//  Converts font Point size into LOGFONT.lfHeight format
//  NOTES: In MM_TEXT mapping mode, following formula is used to
//         specify a height for a font with a specified point size
MsgcoreMFC_EXT long
Point2LOGFONTSize ( CDC *pDC, long nPointSize )
{
    ASSERT(nPointSize>0);
    return -MulDiv ( nPointSize, GetDeviceCaps(pDC->GetSafeHdc(),LOGPIXELSY), 72 );
}

MsgcoreMFC_EXT void
LOGFONTcrxSerialise( LOGFONTcrx& oLOGFONTcrx, CDC *pDC )
{
    CDC *pDC1 = pDC;
    if ( pDC1 == nullptr )
      pDC1 = AfxGetMainWnd() -> GetDC ( );
    oLOGFONTcrx.lfHeightPoints = LOGFONT2PointSize ( pDC, oLOGFONTcrx );
    if ( pDC1 )
      AfxGetMainWnd() -> ReleaseDC ( pDC1 );
}
MsgcoreMFC_EXT void
LOGFONTcrxRestore  ( LOGFONTcrx& oLOGFONTcrx, CDC *pDC )
{
    CDC *pDC1 = pDC;
    if ( pDC1 == nullptr )
      pDC1 = AfxGetMainWnd()->GetDC ( );
    if ( oLOGFONTcrx.lfHeightPoints > 0 )
      oLOGFONTcrx.lfHeight = Point2LOGFONTSize ( pDC1, oLOGFONTcrx.lfHeightPoints );
    if ( pDC1 )
      AfxGetMainWnd() -> ReleaseDC ( pDC1 );
}

//
//  Resets monitor details wrt passed HWND
//  NOTES: HWnd usually relates to CMainFrame
//
//  Parameters:  HWND hWnd
//               Handle to primary window
//
//UINT g_uiDpiX = 96;
//UINT g_uiDpiY = 96;
//MsgcoreMFC_EXT HRESULT
//LOGFONT2MonitorReset ( HWND hWnd )
//{
//    HMONITOR hMonitor = MonitorFromWindow ( hWnd, MONITOR_DEFAULTTONEAREST );

//    UINT uiDpiX = 0, uiDpiY = 0;
//    GetDpiForMonitor ( hMonitor, MDT_EFFECTIVE_DPI, &uiDpiX, &uiDpiY );
//    g_uiDpiX = uiDpiX ? g_uiDpiX : uiDpiX;
//    g_uiDpiY = uiDpiY ? g_uiDpiY : uiDpiY;
//    return S_OK;
//}
//
//  Migrates the LOGFONTcrx structure to new version
//  NOTES: LOGFONTcrx structures must be capable of migrating
//         between desktops with differing monitor DPI's or capable
//         of handling swapping of monitors on a given desktop
//         Not unlike Micrsoft Word.
//       : Process requires that the size of the LOGFONTcrx structure
//         be incremented and contents migrated
//       : This migration MUST be performed on the originating
//         desktop in order to preserve FONT size in points.  Usually
//         done in ::CreateGridProps() immediately prior to mapping
//         named P3PmsgItem. Mandatory to resize prior to mapping
//       : May be discarded after whole workspace processed, only
//         has to be formed once per LOGFONTcrxV3005 data structure
//
//  Parameters:  P3PmsgItem& oItemLOGFONTcrx
//               P2Pmsg placeholder for LOGFONTcrx structure to be
//               migrated
//
//               LPCWSTR lpszLOGFONTname
//               Name of item containing LOGFONTcrx structure
//
//  Returns:     BOOL
//                 TRUE... Resized and migrated
//                 FALSE.. No resizing or migration required
//
struct LOGFONTcrxV3005 : LOGFONTW
{   // Deprecated structure
    COLORREF oCOLORREF;
}; 
MsgcoreMFC_EXT BOOL
LOGFONTcrxLocalise ( LPCWSTR lpszLOGFONTname, P3PmsgField& oItemLOGFONTcrx )
{
    if ( !oItemLOGFONTcrx.Exists(lpszLOGFONTname) )
      return FALSE;          // Does NOT exist
    P3PmsgItem oExisting = oItemLOGFONTcrx.SelectObject(lpszLOGFONTname);
    if ( oExisting.r_data().c_size() >= sizeof(LOGFONTcrx) )
      return FALSE;          // Already migrated
    LOGFONTcrxV3005 *pLOGFONTcrxOld = (LOGFONTcrxV3005 *)oExisting.r_data().c_vBlob();
    LOGFONTcrx       oLOGFONTcrx;
    //LOGFONT oLOGFONT = *pLOGFONTcrxOld;
    //((LOGFONT)oLOGFONTcrx)     = oLOGFONT;
    oLOGFONTcrx.lfHeight        = pLOGFONTcrxOld -> lfHeight;   // Local LOGFONT pixels
    oLOGFONTcrx.lfWidth         = pLOGFONTcrxOld -> lfWidth; 
    oLOGFONTcrx.lfEscapement    = pLOGFONTcrxOld -> lfEscapement; 
    oLOGFONTcrx.lfOrientation   = pLOGFONTcrxOld -> lfOrientation; 
    oLOGFONTcrx.lfWeight        = pLOGFONTcrxOld -> lfWeight; 
    oLOGFONTcrx.lfItalic        = pLOGFONTcrxOld -> lfItalic; 
    oLOGFONTcrx.lfUnderline     = pLOGFONTcrxOld -> lfUnderline; 
    oLOGFONTcrx.lfStrikeOut     = pLOGFONTcrxOld -> lfStrikeOut; 
    oLOGFONTcrx.lfCharSet       = pLOGFONTcrxOld -> lfCharSet; 
    oLOGFONTcrx.lfOutPrecision  = pLOGFONTcrxOld -> lfOutPrecision; 
    oLOGFONTcrx.lfClipPrecision = pLOGFONTcrxOld -> lfClipPrecision; 
    oLOGFONTcrx.lfQuality       = pLOGFONTcrxOld -> lfQuality; 
    oLOGFONTcrx.lfPitchAndFamily= pLOGFONTcrxOld -> lfPitchAndFamily;
    wcscpy_s ( oLOGFONTcrx.lfFaceName, ARRAYSIZE(oLOGFONTcrx.lfFaceName)
             , pLOGFONTcrxOld->lfFaceName );
    // Resizes target data allocation, re-directed linkage etc
    // NOTES: Deframented on any copy operation.  Globalise to match
    //        the new standard
    oLOGFONTcrx.uiCBVersion     = 3006;
    oLOGFONTcrx.oCOLORREF       = pLOGFONTcrxOld->oCOLORREF;
    oLOGFONTcrx.lfHeightPoints  = LOGFONT2PointSize(nullptr,oLOGFONTcrx);
    LOGFONTcrxGlobalise ( oLOGFONTcrx );
    oExisting.r_data() = P3PmsgData((void*)&oLOGFONTcrx,sizeof(oLOGFONTcrx));
    return TRUE;
}
MsgcoreMFC_EXT BOOL
LOGFONTcvxLocalise ( LPCWSTR lpszLOGFONTname, P3PmsgField& oItemLOGFONTcrx )
{
    if ( !oItemLOGFONTcrx.Exists(lpszLOGFONTname) )
      return FALSE;          // Does NOT exist
    P3PmsgItem oExisting = oItemLOGFONTcrx.SelectObject(lpszLOGFONTname);
    LOGFONTcrx *pLOGFONTcrx = (LOGFONTcrx *)oExisting.r_data().c_vBlob();
    if ( oExisting.r_data().c_size() < sizeof(LOGFONTcrx) ||
         pLOGFONTcrx->uiCBVersion    < 3006                  ) {
      LOGFONTcrxLocalise ( lpszLOGFONTname, oItemLOGFONTcrx );
      pLOGFONTcrx = (LOGFONTcrx *)oExisting.r_data().c_vBlob();
      return TRUE;
    }
    // Now localise
    LOGFONTcrxLocalise  ( *pLOGFONTcrx );
    return TRUE;
}

//
//  Localises the LOGFONTcrx structure according to desktop
//  NOTES: LOGFONTcrx structures must be capable of migrating
//         between desktops with differing monitor DPI's or capable
//         of handling swapping of monitors on a given desktop
//         Not unlike Microsoft Word.
//       : Process requires that the LOGFONTcrx.lfHeightPoints be
//         translated into LOGFONTcrs.lfHeight according to desktop
//         monitor.
//       : Required operation every time Chart is actiavted to cover
//         for copied/imported Charts and Monitor swaps etc. Usually
//         NULL operation and optimised as such
//
//  Parameters:  LOGFONTcrx
//               Structure to be localised
// 
//               CDC *pDC = nullptr
//               Document context to be used.  Defaults to CMainWnd.
//
//  Returns:     void
//
MsgcoreMFC_EXT void
LOGFONTcrxLocalise  ( LOGFONTcrx& oLOGFONTcrx, CDC *pDC )
{
    CDC *pDC1 = pDC;
    if ( pDC1 == nullptr )
      pDC1 = AfxGetMainWnd()->GetDC ( );
    ASSERT(oLOGFONTcrx.uiCBVersion==3006u);
    ASSERT(oLOGFONTcrx.lfHeightPoints);
    if ( oLOGFONTcrx.lfHeightPoints > 0 )
      oLOGFONTcrx.lfHeight = Point2LOGFONTSize ( pDC1, oLOGFONTcrx.lfHeightPoints );
    if ( pDC1 )
      AfxGetMainWnd() -> ReleaseDC ( pDC1 );
}

//
//  Globalise the LOGFONTcrx structure according to desktop
//  NOTES: LOGFONTcrx structures must be capable of migrating
//         between desktops with differing monitor DPI's or capable
//         of handling swapping of monitors on a given desktop
//         Not unlike Micrsoft Word.
//       : Process requires that the LOGFONTcrx.lfHeight be
//         translated into LOGFONTcrs.lfHeightPoints according to desktop
//         monitor.
//
//  Parameters:  LOGFONTcrx
//               Structure to be Globalised
// 
//               CDC *pDC = nullptr
//               Document context to be used.  Defaults to CMainWnd.
//
//  Returns:     void
//
MsgcoreMFC_EXT void
LOGFONTcrxGlobalise  ( LOGFONTcrx& oLOGFONTcrx, CDC *pDC )
{
    CDC *pDC1 = pDC;
    if ( pDC1 == nullptr )
      pDC1 = AfxGetMainWnd()->GetDC ( );
    ASSERT(oLOGFONTcrx.uiCBVersion==3006u);
    ASSERT(oLOGFONTcrx.lfHeight);
    if ( oLOGFONTcrx.lfHeight )
      oLOGFONTcrx.lfHeightPoints = LOGFONT2PointSize ( pDC1, oLOGFONTcrx );
    if ( pDC1 )
      AfxGetMainWnd() -> ReleaseDC ( pDC1 );
}
