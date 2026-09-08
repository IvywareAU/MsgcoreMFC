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
#include "StdAfx.h"
#include "Windowsx.h"
#include "CBitMap_Ext.h"
#include "Msgexception.h"

//
//  Constructors and destructor
//
CBitmapExt::CBitmapExt (  )
{
    // Firstly
    m_pBitMapInfo = 0;
}

CBitmapExt::~CBitmapExt ( )
{
    // Resource recovery
    DeleteObject ( );                  // Attached bitmap
    if ( m_pBitMapInfo )
      GlobalFreePtr( m_pBitMapInfo );  // Allocated global memory
}

//
//  Loads bitmap image from file
//  NOTES: Attach()'s image to this object.  Existing image is deleted.
//
//
//  Parameters:  LPCTSTR lpszFilename
//               File containing bitmap to be loaded
//
//  Returns:     HBITMAP
//               Handle to loaded bitmap
//                 0.. Load failed
HBITMAP
CBitmapExt::LoadFromFile ( LPCTSTR lpszFilename )
{
    // Introduce locals
    HBITMAP hBitmap;

    // Resource recovery 
    DeleteObject ( );                  // Prior bitmap

    // Simply
    hBitmap = (HBITMAP)::LoadImage ( AfxGetInstanceHandle()
                                   , lpszFilename
                                   , IMAGE_BITMAP
                                   , 0
                                   , 0
                                   , LR_LOADFROMFILE | LR_CREATEDIBSECTION );
    if ( hBitmap )
      Attach ( hBitmap );

    // Tidy up and
    return (HBITMAP)m_hObject;
}

//
//  CBitmap extensions
//  NOTES: Copy and pasting CDC regions
HBITMAP
CBitmapExt::SaveFromDC ( CDC *pDC, RECT *pRECT )
{
    // Introduce locals
    BOOL bResult;
    //bool    bFull = false;
    //HBITMAP hBitmap;

    //CDC dc; 
    // if(FullWnd) 
    //     { /* full window */ 
    //      HDC hdc = ::GetWindowDC(wnd->m_hWnd); 
    //      dc.Attach(hdc); 
    //     } /* full window */ 
    //  else 
    //     { /* client area only */ 
    //      HDC hdc = ::GetDC(wnd->m_hWnd); 
    //      dc.Attach(hdc); 
    //     } /* client area only */ 

     // Memeory DC compatible with source bitmap
     CDC memDC; 
     bResult = memDC.CreateCompatibleDC(pDC); 
     CBitmap *pbm = this;
     //CBitmap bm; 
     // CRect r; 
     //if(FullWnd) 
     //    wnd->GetWindowRect(&r); 
     // else 
     //    { /* client only */ 
     //     wnd->GetClientRect(&r); 
     //    } /* client only */ 
     //CString s; 
     // wnd->GetWindowText(s); 

     CRect oCRect = *pRECT;
     if ( oCRect.left < 0 )
       oCRect.left = 0;
     if ( oCRect.right < 0 )
       oCRect.right = 0;
     CSize sz(oCRect.Width(), oCRect.Height()); 

     Empty();                          // Drop prior bitmap
     bResult = pbm->CreateCompatibleBitmap(pDC, sz.cx, sz.cy); 
     ASSERT(bResult);
     CBitmap *oldbm = memDC.SelectObject(pbm); 

     bResult = memDC.BitBlt(0, 0, sz.cx, sz.cy, pDC, pRECT->left, pRECT->top, SRCCOPY); 
     ASSERT(bResult);

     //wnd->GetParent()->OpenClipboard(); 
     // ::EmptyClipboard(); 
     // ::SetClipboardData(CF_BITMAP, bm.m_hObject); 
     // CloseClipboard(); 

     memDC.SelectObject(oldbm); 
     //pbm->Detach();  // make sure bitmap not deleted with CBitmap object 

//http://computer-programming-forum.com/82-mfc/c1c3f5a6a6981ef5.htm
    // Tidy up and
    m_oCRect = *pRECT;
    return (HBITMAP)m_hObject;
}
//
//  Saves client window image from CDC
//  NOTES: Maybe restored via Restore2DC()
//
HBITMAP
CBitmapExt::SaveClientFromDC ( CDC *pDC )
{
    // Introduce locals
    BOOL bResult;
    //bool    bFull = false;
    //HBITMAP hBitmap;

    //CDC dc; 
    // if(FullWnd) 
    //     { /* full window */ 
    //      HDC hdc = ::GetWindowDC(wnd->m_hWnd); 
    //      dc.Attach(hdc); 
    //     } /* full window */ 
    //  else 
    //     { /* client area only */ 
    //      HDC hdc = ::GetDC(wnd->m_hWnd); 
    //      dc.Attach(hdc); 
    //     } /* client area only */ 

     // Memeory DC compatible with source bitmap
     CDC memDC; 
     bResult = memDC.CreateCompatibleDC(pDC); 
     CBitmap *pbm = this;
     //CBitmap bm; 
     // CRect r; 
     CWnd * pCWnd = pDC->GetWindow();
     CRect oCRect;
     pCWnd -> GetClientRect( &oCRect );
     //if(FullWnd) 
     //    wnd->GetWindowRect(&r); 
     // else 
     //    { /* client only */ 
     //     wnd->GetClientRect(&r); 
     //    } /* client only */ 
     //CString s; 
     // wnd->GetWindowText(s); 

     //CRect oCRect = *pRECT;
     if ( oCRect.left < 0 )
       oCRect.left = 0;
     if ( oCRect.right < 0 )
       oCRect.right = 0;
     CSize sz(oCRect.Width(), oCRect.Height()); 

     Empty();                          // Drop prior bitmap
     bResult = pbm->CreateCompatibleBitmap(pDC, sz.cx, sz.cy); 
     ASSERT(bResult);
     CBitmap *oldbm = memDC.SelectObject(pbm); 

     bResult = memDC.BitBlt(0, 0, sz.cx, sz.cy, pDC, oCRect.left, oCRect.top, SRCCOPY); 
     ASSERT(bResult);

     //wnd->GetParent()->OpenClipboard(); 
     // ::EmptyClipboard(); 
     // ::SetClipboardData(CF_BITMAP, bm.m_hObject); 
     // CloseClipboard(); 

     memDC.SelectObject(oldbm); 
     //pbm->Detach();  // make sure bitmap not deleted with CBitmap object 

//http://computer-programming-forum.com/82-mfc/c1c3f5a6a6981ef5.htm
    // Tidy up and
    m_oCRect = oCRect;
    return (HBITMAP)m_hObject;
}

HBITMAP
CBitmapExt::Restore2DC ( CDC *pDC, CPoint *pCPoint )
{
    BOOL bResult;
    if ( IsEmpty() )
      return 0;
    if ( pCPoint )
      m_oCRect.MoveToXY ( *pCPoint );
    CDC memDC;
    bResult = memDC.CreateCompatibleDC(pDC); 
    ASSERT(1);
    memDC.SelectObject(this);
    bResult = pDC->BitBlt (m_oCRect.left,m_oCRect.top,m_oCRect.Width(),m_oCRect.Height(),&memDC,0,0,SRCCOPY);
    ASSERT(bResult);
    //pDC->SelectObject ( *this );
    return (HBITMAP)m_hObject;
}

//
//  Displays loaded bitmap in nominated window
//
//  Parameters:  CWnd *pCWnd
//               Window in which bitmap is to be displayed
//
BOOL
CBitmapExt::Display ( CWnd *pCWnd )
{
    BOOL bResult;
    if ( IsEmpty() || pCWnd == nullptr )
      return 0;
    CDC  *pDC = pCWnd -> GetDC();
    CDC memDC;
    bResult = memDC.CreateCompatibleDC(pDC); 
    ASSERT(bResult);
    pCWnd -> GetClientRect ( m_oCRect );
    memDC.SelectObject(this);
    //StretchDIBits ( memDC, &m_oCRect );
    bResult = pDC->BitBlt (m_oCRect.left,m_oCRect.top,m_oCRect.Width(),m_oCRect.Height(),&memDC,0,0,SRCCOPY);
    ASSERT(bResult);
    return 0;
}

//
//  Stretches DIB to fit
//  NOTES: Encapsulation of the StretchDIBits() API function
//
//
//  Parameters:  HDC hDC
//               Handle to destination devide context
int
CBitmapExt::StretchDIBits ( HDC hDC, RECT *pRect )
{
    // Introduce locals
    HRESULT hr;
    CRect oCRect = pRect;
    BITMAPINFO *pBitmapInfo = GetBITMAPINFO(hDC);

    // 
#define PADWIDTH(x) (((x)*8 + 31)  & (~31))/8
    BITMAP oBitmap;
    if ( GetObject(sizeof(oBitmap),&oBitmap) == 0 )
      return 0;
    DWORD dwSize = PADWIDTH ( oBitmap.bmWidthBytes)
                            * oBitmap.bmHeight;
    void *pvBits = malloc ( dwSize );
    dwSize = GetBitmapBits ( dwSize, pvBits );

    // Implementation
    hr = ::StretchDIBits ( hDC
                         , oCRect.left, oCRect.top
                         , oCRect.Width(), oCRect.Height()
                         , 0, 0
                         , pBitmapInfo->bmiHeader.biWidth
                         , pBitmapInfo->bmiHeader.biHeight
                         , pvBits
                         , pBitmapInfo
                         , DIB_RGB_COLORS
                         , SRCCOPY );
    if ( hr == GDI_ERROR )
      EVTRC->MODULE
           ->Message_T("StretchDIBits() failed")
           ->HResult(GetLastError())->Cancel();

    free ( pvBits );
    return 0;
}

void
CBitmapExt::Empty ( )
{
    DeleteObject ( );                  // Attached bitmap
}

//
//  Populates and exposes pointer to the BITMAPINFO structure for
//  the encapsulated bitmap
//
//
//  Parameters:  HDC hDC
//
//
//  Returns:     BITMAPINFO*
//                 0.. Failed
BITMAPINFO*
CBitmapExt::GetBITMAPINFO ( HDC hDC )
{
    // Introduce locals
    BITMAP oBitmap;
    if ( GetObject(sizeof(oBitmap),&oBitmap) == 0 )
      return 0;

    // Evaluation color-depth
    // NOTES: This is necessary to BIMATINFO memory.
    WORD wColors;
    switch ( oBitmap.bmBitsPixel )
    {
      case 1:
        wColors = 2;
        break;
      case 4:
        wColors = 16;
        break;
      case 8 :
        wColors = 256;
        break;
      default :
        wColors = 0;
        break;
    }

    // Create and initialise a BITMAPINFO buffer
    // NOTES: Needs to be allocated from global memory
    m_pBitMapInfo = (BITMAPINFO *)GlobalAllocPtr ( GHND
                                  , sizeof(BITMAPINFOHEADER)
                                  + wColors*sizeof(RGBQUAD) );
    m_pBitMapInfo -> bmiHeader.biSize = sizeof( BITMAPINFOHEADER ) ;

    // Populate BITMAPINFO
    // NOTES: Use NULL lpvBits argument to achieve this
    if ( !GetDIBits ( hDC
                    ,(HBITMAP)m_hObject
                    , 0
                    ,-WORD(oBitmap.bmHeight)
                    , NULL /*lpvBits*/
                    , m_pBitMapInfo
                    , DIB_RGB_COLORS  ) )
    {
      GlobalFreePtr ( m_pBitMapInfo );
                      m_pBitMapInfo = 0;
    }

    // Tidy up and
    return m_pBitMapInfo;
}

BOOL
CBitmapExt::IsEmpty ( )
{
    return (HBITMAP)m_hObject ? FALSE : TRUE;
}


// PictureBox.cpp : implementation file
   //
   
//   #include "stdafx.h"
//   #include "PictureBox.h"
//   #include ".\picturebox.h"
   
   
// CPictureBox
   
IMPLEMENT_DYNAMIC(CPictureBox, CStatic)
CPictureBox::CPictureBox()
{
}
   
CPictureBox::~CPictureBox()
{
}
   
   
BEGIN_MESSAGE_MAP(CPictureBox, CStatic)
   ON_WM_PAINT()
END_MESSAGE_MAP()
   
   
   
// CPictureBox message handlers
   
void CPictureBox::ShowBitmap(CPaintDC *pdc) 
{
   	//Create a device context to load the bitmap into
   	CDC dcMem;
   	dcMem.CreateCompatibleDC(pdc);
   
   	//Get the Display area available
   	CRect lRect;
   	GetClientRect(lRect);
   	lRect.NormalizeRect();
   
   	//select the bitmap into compatible device context
   	//CBitmap* pOldBitmap = (CBitmap*)dcMem.SelectObject(&m_bmpBitmap);
   	//m_bmpBitmap.SetBitmapDimension(lRect.Width(),lRect.Height());
   
   	//copy & resize the window to the dialog window
   	pdc->StretchBlt(0,0,lRect.Width(),lRect.Height(),&dcMem,0,0,m_bm.bmWidth,m_bm.bmHeight,SRCCOPY);
}
void CPictureBox::OnPaint()
{
   	CPaintDC dc(this); // device context for painting
   
   	RECT rect;
   	GetClientRect(&rect);
   	dc.FillSolidRect(&rect, RGB(255,255,255));
   
   	if( m_bmpBitmap.IsEmpty() )
   		ShowBitmap(&dc);
}
void
CPictureBox::LoadBitmap ( LPCTSTR lpszBitmap )
{
   	HBITMAP hBitmap
      //= (HBITMAP) ::LoadImage ( AfxGetInstanceHandle()
   	  //                        , m_strBitmap, IMAGE_BITMAP, 0, 0
   	  //                        , LR_LOADFROMFILE | LR_CREATEDIBSECTION|LR_LOADTRANSPARENT);
      = (HBITMAP) ::LoadImage ( AfxGetInstanceHandle()
   	                          , lpszBitmap, IMAGE_BITMAP, 0, 0
   	                          , /*LR_LOADFROMFILE |*/ LR_CREATEDIBSECTION|LR_LOADTRANSPARENT);
   
   	// Do we have a valid handle for the loaded image?
   	if ( hBitmap )
   	{
   		// Delete the current bitmap
   		m_bmpBitmap.Empty();
   		// Attach the currently loaded bitmap to the bitmap object
   		m_bmpBitmap.Attach(hBitmap);
   	}
   	m_bmpBitmap.GetBitmap(&m_bm);  //Get Bitmap Structure
    OnPaint();//TODO:LJM 25/11/2018
   	Invalidate();
}
   
void
CPictureBox::DrawItem ( LPDRAWITEMSTRUCT ds )
{
    UNREFERENCED_PARAMETER(ds);
//Do nothing
}