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
//  Clipboard extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"
#include "P2Pmsg.h"

//
//  Clipboard operations
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients
//
MsgcoreMFC_EXT UINT
P2Pmsg_RegisterClipboardFormat ( LPCTSTR lpsz );
MsgcoreMFC_EXT void
P2Pmsg_Copy2Clipboard ( HWND hWndClipboard, UINT uiRegCBFormat, P3PmsgItem& oItemCopy );
MsgcoreMFC_EXT bool
P2Pmsg_Clipboard2Paste ( HWND hWndClipboard, unsigned int uiRegCBFormat, P3PmsgItem& oItemPasted );
MsgcoreMFC_EXT void
CWnd_CopyImage2Clipboard ( CWnd *pCWnd, bool bFull );

//
//  Safe Clipboard container
//  NOTES: Exception proofs OpenClipboard() operations
class MsgcoreMFC_EXT SafeClipboard
{
    public:
       SafeClipboard ( HWND hWndClipboard ) { OpenClipboard(hWndClipboard); }
      ~SafeClipboard ( ) { CloseClipboard(); }
};

//
//  Safe HGLOBAL
//  NOTES: Exception proofs HGLOBAL handles 
class MsgcoreMFC_EXT SafeHGLOBAL
{
    public:
       SafeHGLOBAL ( ) { }
      ~SafeHGLOBAL ( ) { GlobalUnlock(); GlobalFree(); }
    public:
         operator HGLOBAL ( ) { return m_hGlobal; };
       SafeHGLOBAL&
         operator = ( HGLOBAL hGlobal ) { GlobalUnlock(); GlobalFree(); m_hGlobal = hGlobal; return *this; }
    public:
       HGLOBAL
         GlobalAlloc ( UINT uFlags, SIZE_T dwBytes ) { ASSERT(m_hGlobal==0);return m_hGlobal=::GlobalAlloc(uFlags,dwBytes); }
       void*
         GlobalLock ( ) { if(m_pvGlobal==0) m_pvGlobal=::GlobalLock(m_hGlobal);return m_pvGlobal; }
       void
         GlobalUnlock ( ) { if(m_pvGlobal) ::GlobalUnlock(m_hGlobal); m_pvGlobal = 0; }
       void
         GlobalFree ( ) { if(m_hGlobal) m_hGlobal=::GlobalFree(m_hGlobal); }
       HGLOBAL
         Deref ( ) { HGLOBAL hReturn=m_hGlobal; ASSERT(m_pvGlobal==0); m_hGlobal=0; return hReturn; }
    private:
      HGLOBAL m_hGlobal{0};
      void   *m_pvGlobal{nullptr};
};