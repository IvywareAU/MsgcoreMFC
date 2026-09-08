// Copyright © 2002-2004, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for Clipboard extensions
//

#include "StdAfx.h"
#include "Msgexception.h"
#include "P2PmsgBSTR.h"
#include "Clipboard_Ext.h"

///////////////////////////////////////////////////////////////////////////////
//  Clipboard operations

unsigned int
P2Pmsg_RegisterClipboardFormat ( LPCTSTR lpszClipboardFormat )
{
    return RegisterClipboardFormat ( lpszClipboardFormat ); 
}

//
//  Copies P2PmsgNode contents to the Clipboard as binary P2Piomage 
//
//
//  Parameters:  UINT uiRegCBFormat
//               Registered clipboard format
//
//               P3PmsgItem& oItemCopy
//               Object containing content to be copied to clipboard
//
void
P2Pmsg_Copy2Clipboard ( HWND hWndClipboard, UINT uiRegCBFormat, P3PmsgItem& oItemCopy )
{
    //P2Pmsgnn_t nDefault = P3PmsgBSTR::GetDefaultP2Pmsgnn ( );
    // Problematic
    try
    {
      SafeHGLOBAL shGlobalCopy;
ASSERT(IsWindow(hWndClipboard));
      // Preparation
      //P3PmsgBSTR::SetDefaultP2Pmsgnn ( VBLock_Addr32 );
      P3PmsgBSTR32 oMsgBSTR;
//oMsgBSTR.AssertValid();
//oMsgBSTR.Print(stdout,1);
                 oMsgBSTR.InitItem ( VBLockBSTR_MSG, oItemCopy.c_name() );
                 oMsgBSTR.r_datn() += oItemCopy;
//oMsgBSTR.AssertValid();
//oMsgBSTR.Print(stdout,1);
//VBLaddr aFirstBlock=oMsgBSTR.r_item(VBLockBSTR_ROOT).r_Object().m_aVBLock;
//VBLsize aFirstSize =oMsgBSTR.r_item(VBLockBSTR_ROOT).r_Object().Msg2Size(aFirstBlock);
      P2Piomage *pP2Piomage = (P2Piomage *)oMsgBSTR.PrepareP2Piomage(~0u);
//P3PmsgBSTR oBSTR1(*pP2Piomage);
//oBSTR1.AssertValid();
//oBSTR1.Print(stdout,1);
      SafeClipboard oSafeClipboard ( hWndClipboard );
      EmptyClipboard ( );
      SIZE_T nSizeof = P2Piomage_Sizeof(pP2Piomage);

      // Allocate a global memory object for the text. 
      if ( shGlobalCopy.GlobalAlloc(GMEM_MOVEABLE,nSizeof) == NULL )
        EVERR->MODULE
             ->Message_T("GlobalAlloc failed")
             ->Throw ( );

      // Lock the handle and copy P3PmsgItem into the buffer. 
      void  *pvGlobalCopy = shGlobalCopy.GlobalLock();
      if ( pvGlobalCopy == nullptr )
        EVERR->MODULE
             ->Message_T("GlobalLock failed")
             ->HResult(GetLastError())->Throw ( );
      memcpy ( pvGlobalCopy, pP2Piomage, nSizeof );
      //P2Piomage_Release ( pP2Piomage );
      shGlobalCopy.GlobalUnlock();
      if ( SetClipboardData(uiRegCBFormat,shGlobalCopy) == NULL )
        EVERR->MODULE
             ->Message_T("SetClipboardData() failed")
             ->Throw ( );
      shGlobalCopy.Deref();            // HGLOBAL now owned by clipboard
    }

    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel
}         

//
//  Pastes Clipboard contents to P2PmsgNode
//  NOTES: 
//
//
//  Parameters:  UINT uiRegCBFormat
//               Registered clipboard format
//
//               P3PmsgItem& oItemCopy
//               Object to which Clipboard contents are pasted
//
MsgcoreMFC_EXT bool
P2Pmsg_Clipboard2Paste ( HWND hWndClipboard, unsigned int uiRegCBFormat, P3PmsgItem& oItemPasted )
{
    // Problematic
    try
    {
      SafeHGLOBAL shGlobalCopy;
      if ( !IsClipboardFormatAvailable(uiRegCBFormat) )
        return false;                  // Nothing is valid

      // Preparation
      SafeClipboard oSafeClipboard ( hWndClipboard );
      //EmptyClipboard ( );
      //SIZE_T nSizeof = 0;

      // Allocate a global memory object for the text.
      shGlobalCopy = GetClipboardData(uiRegCBFormat); 
      if ( shGlobalCopy == NULL )
        EVERR->MODULE
             ->Message_T("GetClipboardData() failed")
             ->Throw ( );

      // Lock the handle and copy P3PmsgItem into the buffer. 
      P2Piomage *pGlobalP2Piomage = (P2Piomage *)shGlobalCopy.GlobalLock();
      if ( !pGlobalP2Piomage )
        return false;
      P3PmsgBSTR oBSTR ( *pGlobalP2Piomage );
      shGlobalCopy.GlobalUnlock();
      shGlobalCopy.Deref();             // Now have our own copy
//VBLaddr aFirstBlock=oBSTR.r_item(VBLockBSTR_ROOT).r_Object().m_aVBLock;
//oBSTR.Print(stdout,1);
      oBSTR.AssertValid();
        //ASSERT(0);//TODO:LJM activate below without P2PeerMsg
        //P3PmsgBSTR oBSTR ( *pGlobalP2Piomage );
      oItemPasted = oBSTR.r_datn().SelectItem(oBSTR.r_datn().c_wstr());
      oItemPasted.AssertValid();
      return true;
    }

    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel
    return false;
}

//
//  Copies window image to the Clipboard in  bitmap format
//
//
//  Parameters:  CWnd *pCWnd
//               Window contents to be copied
//
//               bool bFull
//               Full window copy flag
//
void
CWnd_CopyImage2Clipboard ( CWnd *pCWnd, bool bFull )
{
    // Indroduce locals
    CDC   dc;
    HDC  hDC;

    // Observe copy type
    if ( bFull )                       // Full window copy
      hDC = ::GetWindowDC ( pCWnd->m_hWnd );
    else                               // Client window copy
      hDC = ::GetDC ( pCWnd -> m_hWnd);
    dc.Attach ( hDC );
 
    CDC memDC;
    memDC.CreateCompatibleDC ( &dc );
 
    CBitmap bm;
    CRect   oCRect;
    if ( bFull )
      pCWnd -> GetWindowRect ( &oCRect );
    else
      pCWnd -> GetClientRect ( &oCRect );
 
     CSize sz ( oCRect.Width(), oCRect.Height() );
     bm.CreateCompatibleBitmap ( &dc, sz.cx, sz.cy );
     CBitmap *oldbm = memDC.SelectObject(&bm);
     memDC.BitBlt(0, 0, sz.cx, sz.cy, &dc, 0, 0, SRCCOPY);

     CWnd *pCWndParent = pCWnd->GetParent();
     if ( pCWndParent == nullptr )
       pCWndParent = pCWnd;
     
     pCWndParent->OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();
 
     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
                   // read my essay on this technique
     ::ReleaseDC(pCWnd->m_hWnd,dc.Detach()); // 
}
