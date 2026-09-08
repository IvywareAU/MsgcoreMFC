// Copyright © 2003, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Description: GraphWare implementation
//
#include "StdAfx.h"

#include "CDC_Ext.h"

///////////////////////////////////////////////////////////////////////
//  CDC Extensions
//  NOTES: Static functions are simpler.  Bypasses need for
//         derivation and inheritance.  Simply use as required.

//
//  Determines the minimum width necessary to display contents of string.
//  NOTES: The returned width takes into account the current font properties 
//
//  Parameters:  CDC *pDC
//               Document context used to draw the string
//
//               LPCTSTR lpszString
//               String whose effective length is to be determined
//
//               
//  Returns:     int
//               Effective display length of string
//
int
CDC_GetStringWidth ( CDC *pDC, LPCTSTR lpszString, UINT nFormat )
{
   // Introduce locals
   int    nCount = (int)_tcslen(lpszString);
   CRect  oCRect;

   // To be sure, to be sure
   if ( nCount <= 0 )
     return 0;

   // Evaluate
   DrawText ( pDC->m_hDC, lpszString, nCount
            ,&oCRect
            , nFormat | DT_CALCRECT );

   // Tidy up and
   return oCRect.Width();
}

CRect
CDC_GetStringRect ( CDC *pDC, LPCTSTR lpszString, UINT nFormat )
{
   // Introduce locals
   int    nCount = (int)_tcslen(lpszString);
   CRect  oCRect;

   // To be sure, to be sure
   if ( nCount <= 0 )
     return oCRect;

   // Evaluate
   pDC -> DrawText ( /*pDC->m_hDC,*/ lpszString, nCount
                   ,&oCRect
                   , nFormat | DT_CALCRECT );

   // Tidy up and
   return oCRect;
}

BOOL
CDC_TextOut ( CDC *pDC, int nPosX, int nPosY, LPCTSTR lpszString
            , UINT uiFormat )
{
   // Introduce locals
   int   nCount = (int)_tcslen(lpszString);
   /*CRect oCRect;
         pDC -> DrawText(lpszString,nCount,&oCRect,DT_CALCRECT);
   int   nWidth = oCRect.Width();
   int   nHeight= oCRect.Height();
   int   nAdjX  = 0;
   int   nAdjY  = 0;

   // To be sure, to be sure
   if ( nCount <= 0 )
     return 0;

   // Horizontal ordinate adjustments
   if ( uiFormat&TA_Centre )
     nAdjX = -nWidth / 2;
   else if ( uiFormat&TA_Right )
     nAdjX =  0;
   else if ( uiFormat&TA_Left )
     nAdjX = -nWidth;
   else if ( uiFormat&TA_CentreOnDecimal )
   {
     int k;
     for ( k = 0; ( lpszString[k]                &&
                    lpszString[k] != T_DescDelim    ); k++ );
     nAdjX = CDC_GetStringWidth ( pDC, &lpszString[k] )
           - nWidth;
   }

   // Vertical ordinate adjustments
   if ( uiFormat&TA_Middle )
     nAdjY = -nHeight / 2;
   else if ( uiFormat&TA_Bottom )
     nAdjY =  0;
   else if ( uiFormat&TA_Top )
     nAdjY = -nHeight;*/

   // Perform through delegation
   // NOTES: Mandatory text alignment to push out legacy settings
   pDC -> SetTextAlign ( uiFormat );
   return pDC -> TextOut ( nPosX, nPosY, lpszString, nCount );
}

BOOL
CDC_TexttyOut( CDC *pDC, int nColX, int nRowY, UINT uiStyle
             , LPCTSTR lpszFormat )
{
    // Introduce locals
    TEXTMETRIC oTextMetrics;

    // 
    pDC -> GetTextMetrics ( &oTextMetrics );

    // Implement
    return CDC_TextOut ( pDC, nColX*oTextMetrics.tmHeight/2
                            , nRowY*oTextMetrics.tmHeight
                       , lpszFormat, uiStyle );
}

///////////////////////////////////////////////////////////////////////
//  Shapes

//
//  Draws circle using CDC::Ellipse() and fills with nominated brush
//
//  Parameters:  CDC *pDC
//               Document context used to draw circle
//
//               int nDeviceX and nDeviceY
//               Circle origin in device units
//
//               int nRadius
//               Radius in device units
//
//               CPen *pCPen
//               Pen used to draw cicle
//                 0... Use current
//
//               CBrush *pCBrush
//               Brush used to fill circle
//                 0.. Use current
BOOL
CDC_Circle ( CDC *pDC, int nPosX, int nPosY, int nRadius )
{
    return pDC -> Ellipse ( nPosX - nRadius, nPosY - nRadius
                          , nPosX + nRadius, nPosY + nRadius );
}

BOOL
CDC_Circle ( CDC *pDC, int nXPos, int nYPos, int nRadius, BOOL bFill )
{
    // Filled circle
    if ( bFill )
      return pDC -> Ellipse ( nXPos - nRadius, nYPos - nRadius
                            , nXPos + nRadius, nYPos + nRadius );

    // Empty circle
    CRect  oCRect  (nXPos - nRadius, nYPos - nRadius, nXPos + nRadius, nYPos + nRadius );
    CPoint oCPoint1( nXPos - nRadius, nYPos - nRadius );
    CPoint oCPoint2( nXPos + nRadius, nYPos + nRadius );
    BOOL bResult  = pDC -> Arc ( oCRect, oCPoint1, oCPoint2 );
         bResult &= pDC -> Arc ( oCRect, oCPoint2, oCPoint1 );
    return bResult;
}

BOOL
CDC_Cross  ( CDC *pDC, int nPosX, int nPosY, int nWidth, int nHeight )
{
    BOOL bResult = TRUE;
    CPoint oCPoint = pDC -> MoveTo ( nPosX-nWidth/2, nPosY );
    bResult &= pDC -> LineTo ( nPosX+nWidth, nPosY );
    oCPoint  = pDC -> MoveTo ( nPosX, nPosY-nHeight/2 );
    bResult &= pDC -> LineTo ( nPosX, nPosY+nHeight );
    return bResult;
}

BOOL
CDC_Rect   ( CDC *pDC, int nPosX, int nPosY, int nWidth, int nHeight )
{
    RECT oRECT;
    oRECT.left   = nPosX - nWidth/2;
    oRECT.right  = oRECT.left + nWidth;
    oRECT.top    = nPosY - nHeight/2;
    oRECT.bottom = oRECT.top + nHeight;
    return pDC -> Rectangle ( &oRECT );
}
BOOL
CDC_Rect   ( CDC *pDC, RECT *pRECT, BOOL bFill )
{
    if ( bFill )
      return pDC -> Rectangle ( pRECT );
    BOOL bResult = TRUE;
    CPoint oCPoint = pDC -> MoveTo ( pRECT->left, pRECT->top );
    bResult &= pDC -> LineTo ( pRECT->right, pRECT->top );
    bResult &= pDC -> LineTo ( pRECT->right, pRECT->bottom );
    bResult &= pDC -> LineTo ( pRECT->left, pRECT->bottom );
    bResult &= pDC -> LineTo ( pRECT->left, pRECT->top );
    return bResult;
}
MsgcoreMFC_EXT BOOL
CDC_Rect ( CDC *pDC, int nPosX1, int nPosY1, int nPosX2, int nPosY2, BOOL bFill )
{
    if ( bFill )
    {
      //CDC_Circle(pDC,nPosX1,nPosY1,5);
      if ( nPosX1 < nPosX2 )
        nPosX2++;
      else
        nPosX1++;
      if ( nPosY1 < nPosY2 )
        nPosY2++;
      else
        nPosY1++;
      return pDC -> Rectangle ( nPosX1, nPosY1, nPosX2, nPosY2 );
    }
    BOOL bResult = TRUE;
    CPoint oCPoint = pDC -> MoveTo ( nPosX1, nPosY1 );
    bResult &= pDC -> LineTo ( nPosX1, nPosY2 );
    bResult &= pDC -> LineTo ( nPosX2, nPosY2 );
    bResult &= pDC -> LineTo ( nPosX2, nPosY1 );
    bResult &= pDC -> LineTo ( nPosX1, nPosY1 );
    return bResult;
}

///////////////////////////////////////////////////////////////////////
//  Geometry

//
//  Overlapping rectangles
bool
RECT_Overlap ( RECT& oRECT1, RECT& oRECT2 )
{
    if ( oRECT1.left   > oRECT2.right  ||
         oRECT1.right  < oRECT2.left   ||
         oRECT1.top    > oRECT2.bottom ||
         oRECT1.bottom < oRECT2.top       )
      return false;
    return true;
}

