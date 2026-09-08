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
#pragma once
#include "MsgcoreMFC.h"

//
//  Definitions
const double RADIANS = 57.29577951;

//  Colours
#define RED				RGB(255,0,0)
#define GREEN			RGB(0,255,0) 
#define BLUE			RGB(0,0,255)
#define YELLOW			RGB(255,255,0)
#define ORANGE			RGB(255,153,51)
#define HOT_PINK		RGB(255,51,153)
#define PURPLE			RGB(153,0,204)
#define CYAN			RGB(0,255,255)
#define BLACK			RGB(0,0,0)
#define WHITE			RGB(255,255,255)
#define LAVENDER		RGB(199,177,255)
#define PEACH			RGB(255,226,177)
#define SKY_BLUE		RGB(142,255,255)
#define FOREST_GREEN	RGB(0,192,0)
#define BROWN			RGB(80,50,0)
#define TURQUOISE		RGB(0,192,192)
#define ROYAL_BLUE		RGB(0,0,192)
#define GREY			RGB(192,192,192)
#define DARK_GREY		RGB(128,128,128)
#define TAN				RGB(255,198,107)
#define DARK_BLUE		RGB(0,0,128)
#define MAROON			RGB(128,0,0)
#define DUSK			RGB(255,143,107)
#define LIGHT_GREY		RGB(225,225,225)	//only for 3D graph lines

//
//  Text management
//
MsgcoreMFC_EXT int
CDC_GetStringWidth ( CDC *pDC, LPCTSTR lpszText, UINT uiFormat = 0 );
MsgcoreMFC_EXT int
CDC_GetStringHeight( CDC *pDC, LPCTSTR lpszText, UINT uiFormat = 0 );
MsgcoreMFC_EXT CRect
CDC_GetStringRect  ( CDC *pDC, LPCTSTR lpszText, UINT uiFormat = 0 );

#define  TA_Centre           (1<<0) // Horizontal alignment
#define  TA_Left             (1<<1) 
#define  TA_Right            (1<<2)
#define  TA_CentreOnDecimal  (1<<3)

#define  TA_Middle           (1<<5) // Vertical alignment
#define  TA_Bottom           (1<<6)
#define  TA_Top              (1<<7)

MsgcoreMFC_EXT BOOL
CDC_TextOut  ( CDC *pDC, int nPosX, int nPosY, LPCTSTR lpszText
             , UINT uiFormat );
MsgcoreMFC_EXT BOOL
CDC_TexttyOut( CDC *pDC, int nColX, int nRowY, UINT uiStyle
             , LPCTSTR lpszFormat );

//
//  Shapes
MsgcoreMFC_EXT BOOL
CDC_Circle ( CDC *pDC, int nPosX, int nPosY, int nRadius );
MsgcoreMFC_EXT BOOL
CDC_Circle ( CDC *pDC, int nPosX, int nPosY, int nRadius, BOOL bFill );
MsgcoreMFC_EXT BOOL
CDC_Cross  ( CDC *pDC, int nPosX, int nPosY, int nWidth, int nHeight );
MsgcoreMFC_EXT BOOL
CDC_Rect   ( CDC *pDC, int nPosX, int nPosY, int nWidth, int nHeight );
MsgcoreMFC_EXT BOOL
CDC_Rect   ( CDC *pDC, RECT *pRECT, BOOL bFill = TRUE );
MsgcoreMFC_EXT BOOL
CDC_Rect ( CDC *pDC, int nPosX1, int nPosY1, int nPosX2, int nPosY2, BOOL bFill );

//
//  Safe CDC container
//  NOTES: Exception proofs CDC:SaveDC() and RestoreDC() operations
static int s_nSaveDC = 0;
class MsgcoreMFC_EXT SafeCDC
{
    public:
       SafeCDC ( ) {}
       SafeCDC ( CDC *pDC ) { m_pDC = pDC; Save(); }
       SafeCDC ( CDC& oDC ) { m_pDC = &oDC; Save(); }
      ~SafeCDC ( ) { Restore(); }
    // Overloaded operators
    public:
      SafeCDC&
        operator = ( CDC *pDC )
        {
          if ( m_pDC == pDC )
            return *this;
          Restore();
          if ( pDC == NULL )
            return *this;
          m_pDC =  pDC;
          Save();
          return *this;
        }
        operator CDC* ( )
        {
          return m_pDC;
        }
      CDC*
        operator -> ( )
        {
          return m_pDC;
        }
    private:
      void
        Save ( )
        {
          if ( m_pDC == 0 )
            { ASSERT(0); return; }
          m_nSaveDC = m_pDC -> SaveDC();
          ASSERT(m_nSaveDC>0);
          //ASSERT(m_nSaveDC==(s_nSaveDC+1));
          s_nSaveDC = m_nSaveDC;
        }
      void
        Restore ( )
        {
          if ( m_pDC )
          {
            //ASSERT(m_nSaveDC == s_nSaveDC );
            if ( !m_pDC -> RestoreDC(m_nSaveDC) )
              ASSERT(0);
            s_nSaveDC--;
          }
          else { ASSERT(1); }
          m_pDC     = nullptr;
          m_nSaveDC = 0;
        }
      CDC *m_pDC{0};
      int  m_nSaveDC{0};
};

//
//  Geometry
MsgcoreMFC_EXT bool
RECT_Overlap ( RECT& oRECT1, RECT& oRECT2 );

