// Copyright © 2001-2004, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CBitmap exhancement class prototypes and definitions
//  NOTES: Simply extends the functionality of the CBitmap class
//
#pragma once
#include "MsgcoreMFC.h"
//#include "afxwin.h"

//
//  CBitmap extension class
//  NOTES: Allocated name may be eventually consumed by Microsoft
//
class MsgcoreMFC_EXT CBitmapExt : public CBitmap
{
    // Constructors and destructor
    public:
        CBitmapExt ( );
      virtual
       ~CBitmapExt (void);

    // Operations
    public:
      HBITMAP
        LoadFromFile ( LPCTSTR lpszFilename );
      HBITMAP
        SaveFromDC ( CDC *pDC, RECT *pRECT );
      HBITMAP
        SaveClientFromDC ( CDC *pDC );
      HBITMAP
        Restore2DC ( CDC *pDC, CPoint *pCPoint );
      BOOL
        Display ( CWnd *pCWnd );
      int
        StretchDIBits( HDC hDC, RECT *pRect );
      void
        ScaleImage ( RECT *pRect );
      void
        Empty ( );

    // Utilities
    public:
      BITMAPINFO*
        GetBITMAPINFO ( HDC hDC );
      BOOL
        IsEmpty ( );

    // Attributes
    private:
      BITMAPINFO   m_oBitMapInfo;
      BITMAPINFO  *m_pBitMapInfo;
      CRect        m_oCRect;
};

//
//  CBitmap extensions
//  NOTES: Copy and pasting CDC regions
MsgcoreMFC_EXT CDC*
CBitmap_Copy2memDC ( CWnd *pCWnd, RECT *pRECT );
MsgcoreMFC_EXT CDC*
CBitmap_Paste2DC ( CWnd *pCWnd, CDC *pmemDC, CPoint *pCPoint );

///////////////////////////////////////////////////////////////////////
//  CPictureBox
//  NOTES: Displays a bitmap on a dialog
class MsgcoreMFC_EXT CPictureBox : public CStatic
{
    DECLARE_DYNAMIC(CPictureBox)

    // Constructors and distructor
    public:
        CPictureBox();
      virtual
       ~CPictureBox();
      void
        LoadBitmap ( LPCTSTR lpszBitmap );

    // Operations
    protected:
      void
       ShowBitmap ( CPaintDC *pDC );
      void
       DrawItem ( LPDRAWITEMSTRUCT ds );

    // Attributes
    protected:
      CBitmapExt m_bmpBitmap;
      BITMAP     m_bm;
   
    // MFC message map
    protected:
    DECLARE_MESSAGE_MAP()
      afx_msg void
        OnPaint ( );
};
