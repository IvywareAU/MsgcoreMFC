// Copyright © 2007, 2013, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CListCtrl extensions
//

#include "StdAfx.h"

#include "Msgexception.h"
#include "CListCtrl_ext.h"

//
//  Formats item in CListCtrl
//
//
//  Parameters:  CListCtrl& oCListCtrl
//               List control for which cell is to be formatted
//
//               bool bFull
//               Full window copy flag
//
//  Returns:     HTREEITEM
//               Moved item as a copy of hItem
int
CListCtrl_FormatItem ( CListCtrl& oCListCtrl, int nRow, int nColumn
                     , LPCTSTR lpszFormat, ... )
{
    // Introduce the locals
    va_list    ap;
    va_start ( ap, lpszFormat );

    TCHAR szText[2048];
    int nSize = vswprintf_s ( szText, ARRAYSIZE(szText), lpszFormat, ap );
    szText[nSize] = 0;

    return oCListCtrl.SetItemText ( nRow, nColumn, szText );
}

void
CListCtrl_ClearTextAllSubItems ( CListCtrl& oCListCtrl, int nItem )
{
    const int nCols = oCListCtrl.GetHeaderCtrl()->GetItemCount();
    for ( int i = 0; i < nCols; i++ )
      oCListCtrl.SetItemText(nItem, i, L"" );
}

void
CListCtrl_ClearTextSubItem ( CListCtrl& oCListCtrl, int nItem, int nSubItem )
{
    oCListCtrl.SetItemText(nItem, nSubItem, L"" );
}

void
CListCtrl_DeleteAllColumns ( CListCtrl& oCListCtrl )
{
    oCListCtrl.DeleteAllItems();
    const int nCols = oCListCtrl.GetHeaderCtrl()->GetItemCount();
    for ( int i = 0; i < nCols; i++ )
      oCListCtrl.DeleteColumn ( 0 );
}

MsgcoreMFC_EXT void
CListCtrl_DeselectAllItems ( CListCtrl& oCListCtrl )
{
    POSITION pos = oCListCtrl.GetFirstSelectedItemPosition();
    while ( pos )
    {
      const int index = oCListCtrl.GetNextSelectedItem( pos );
      oCListCtrl.SetItemState ( index, 0, /*LVIS_FOCUS |*/ LVIS_SELECTED);
    }
}

//
//  CListCtrl style management
//  NOTES:

DWORD
CListCtrl_GridLines ( CListCtrl& oCListCtrl, BOOL bEoD )
{
    DWORD dwExtendedStyle = oCListCtrl.GetExtendedStyle ( ) ;
    if ( bEoD )
      dwExtendedStyle = oCListCtrl.SetExtendedStyle ( dwExtendedStyle |  LVS_EX_GRIDLINES );
    else
      dwExtendedStyle = oCListCtrl.SetExtendedStyle ( dwExtendedStyle & ~LVS_EX_GRIDLINES );
    return dwExtendedStyle;
}

//
//  Fetches column header label
//
//  Parameters:  CHeaderCtrl *pHeader
//               Header control for which column text is to be retreived
//
//               int nColumn
//               Column, zero based
//
//  Returns:     LPCTSTR
//               Column label
MsgcoreMFC_EXT BOOL
CHeaderCtrl_GetItemText ( CHeaderCtrl *pHeader, int nColumn, CString& strItemText )
{
    TCHAR  lpBuffer[256] = { 0 };

    HDITEM hdi;
    hdi.mask       = HDI_TEXT;
    hdi.pszText    = lpBuffer;
    hdi.cchTextMax = ARRAYSIZE(lpBuffer);

    if ( !pHeader -> GetItem(nColumn, &hdi) )
      return FALSE;
    strItemText =  hdi.pszText;
    return TRUE;
}

//
//  Locates column with designated column key
//
//  Parameters:  CHeaderCtrl *pHeader
//               List control header whose items are to be searched
//
//               DWORD_PTR
//               Item key to be located
//
//   Result:     int
//               Index of located item
//                 0+.. Column index (zero based)
//                -1... Not located
MsgcoreMFC_EXT int
CHeaderCtrl_FindItemLPARAM ( CHeaderCtrl *pHeader, LPARAM lParam )
{
    ASSERT(lParam);
    int    nItems  = pHeader ? pHeader->GetItemCount() : 0;
    HDITEM oHDITEM;
    for ( int nc = 0; nc < nItems; nc++ )
    {
      ZeroMemory ( &oHDITEM, sizeof(oHDITEM) );
      oHDITEM.mask = HDI_LPARAM;
      pHeader -> GetItem ( nc, &oHDITEM );
      if ( oHDITEM.lParam == lParam )
        return nc;
    }
    // Not located
    return -1;
}

//
//  Sets column header LPARAM
//
//  Parameters:  CHeaderCtrl *pHeader
//               Header control header LPARAM is to be set
//
//               LPARAM
//               Value to be set
//
//   Result:     BOOL
//               Result
//                 0.. Failed
//                 ?.. Success
MsgcoreMFC_EXT BOOL
CHeaderCtrl_SetItemLPARAM( CHeaderCtrl *pHeader, int nColm, LPARAM lParam )
{
    HDITEM oHDITEM;
           oHDITEM.mask   = HDI_LPARAM;
           oHDITEM.lParam = lParam;
    const BOOL bResult = pHeader -> SetItem ( nColm, &oHDITEM );
    ASSERT(bResult);
    return bResult;
}

//
//  Generates list control column for each passed P3PmsgItem
//
//  Parameters:  ClistCtrl& oCListCtrl
//               List control in which columns are to be generated
//
//               const P3PmsgItem& oItemColumns
//               P3PmsgItem containing list of columns to be generated
BOOL
CListCtrl_MakeColumns ( CListCtrl& oCListCtrl, const P3PmsgItem& oItemColumns )
{
    P3PmsgCurs oCurs((P3PmsgDesc&)oItemColumns.r_Desc());
    int        nCol = oCListCtrl.GetHeaderCtrl()->GetItemCount();
    for ( int i = 0; oCurs.Goto(i); i++ )
    {
        if ( !oCurs.IsItem() )
          continue;
        CString strName = oCurs.r_item().c_name();
        oCListCtrl.InsertColumn ( nCol, strName, 0, LVSCW_AUTOSIZE_USEHEADER );
        int nWidth = 50;
        switch (oCurs.r_item().r_data().DataType())
        {
          case VBLockData_INT08:  nWidth =  50; break;
          case VBLockData_UINT08: nWidth =  50; break;
          case VBLockData_INT16:  nWidth =  50; break;
          case VBLockData_UINT16: nWidth =  50; break;
          case VBLockData_INT32:  nWidth =  75; break;
          case VBLockData_UINT32: nWidth =  75; break;
          case VBLockData_INT64:  nWidth =  75; break;
          case VBLockData_UINT64: nWidth =  75; break;
          case VBLockData_FLOAT:  nWidth =  75; break;
          case VBLockData_DOUBLE: nWidth =  75; break;
          case VBLockData_BOOL:   nWidth =  75; break;
          case VBLockData_TIME32: nWidth =  75; break;
          case VBLockData_TIME64: nWidth = 125; break;
        }
        //oCListCtrl.SetColumnWidth ( nCol, LVSCW_AUTOSIZE_USEHEADER );
        if ( oCListCtrl.GetColumnWidth(nCol) < nWidth )
          oCListCtrl.SetColumnWidth ( nCol, nWidth );
        nCol++;
    }
    return oCListCtrl.GetHeaderCtrl()->GetItemCount();
}

BOOL
CListCtrl_InsertRow ( CListCtrl& oCListCtrl, const P3PmsgItem& oItemColumns )
{
    P3PmsgItem&   oItemRow = (P3PmsgItem&)oItemColumns;
    CHeaderCtrl  *pHeaderCtrl = oCListCtrl.GetHeaderCtrl();
    CString     strColumnText;
    int           nColumns    = pHeaderCtrl->GetItemCount();
    int           nItems      = 0;
    int           nItem       = oCListCtrl.InsertItem ( oCListCtrl.GetItemCount(), L"" );
    for ( int nSubItem = 0; nSubItem < nColumns; nSubItem++ )
    {
      CHeaderCtrl_GetItemText ( pHeaderCtrl, nSubItem, strColumnText );
      if ( strColumnText.IsEmpty() )
        continue;
      LPCTSTR lpszColumnText = strColumnText;
      P3PmsgItem oItem = oItemRow.r_Desc().SelectObject(lpszColumnText);
      if ( !oItemRow )
        continue;
      oCListCtrl.SetItemText ( nItem, nSubItem, oItem.r_data().ToString() );
      nItems++;
    }
    return nItems;
}

//
//  Swaps existing CListCtrl items (aka rows)
//
//  Parameters:  ClistCtrl& oCListCtrl
//               List control in which items are to be swapped
//
//               int nItem1
//               Forst item in pair to be swapped
//
//               int nItem2
//               Second item in pair to be swapped
// 
//   Result:     BOOL
//                 TRUE... OK
//                 FALSE.. Failed
BOOL
CListCtrl_SwapItems ( CListCtrl& oCListCtrl, int nItem1, int nItem2)
{
    // Loop for all sub items
    CHeaderCtrl *pHeader = oCListCtrl.GetHeaderCtrl();
    int          nItems  = pHeader ? pHeader->GetItemCount() : 1;
    for ( int i = 0; i < nItems; i++ )
    {
      // Fetch swap item1 details
      LVITEM oItem1;
             oItem1.iItem    =  nItem1;
             oItem1.iSubItem =  i;
             oItem1.mask     = ~0u & ~TVIF_TEXT;
      if ( !oCListCtrl.GetItem(&oItem1) )
        return FALSE;
      // Fetch swap item2 details
      LVITEM oItem2;
             oItem2.iItem    = nItem2;
             oItem2.iSubItem = i;
             oItem2.mask     = ~0u & ~TVIF_TEXT;
      if ( !oCListCtrl.GetItem(&oItem2) )
        return FALSE;
      // Swap attributes
      oItem1.iItem = nItem2;
      if ( !oCListCtrl.SetItem(&oItem1) )
        return FALSE;
      oItem2.iItem = nItem1;
      if ( !oCListCtrl.SetItem(&oItem2) )
        return FALSE;
      // Swap text
      CString strText = oCListCtrl.GetItemText ( nItem1, i );
		  oCListCtrl.SetItemText ( nItem1, i, oCListCtrl.GetItemText(nItem2,i) );
		  oCListCtrl.SetItemText ( nItem2, i, strText );
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  P2Pmsg - CListCtrl serialisation helpers
//  NOTES: Static function for serialising and deserialising header sizing etc
BOOL
CListCtrl_SerialiseHeader   ( CListCtrl& oCListCtrl, P3PmsgItem& oItemHeader )
{
#define MAX_COLS 512
    int iaWidths[MAX_COLS+1];

    // Process all columns
    int nCols = oCListCtrl.GetHeaderCtrl()->GetItemCount();
    for ( int i = 0; i < nCols && i < MAX_COLS; i++ )
      iaWidths[i+1] = oCListCtrl.GetColumnWidth ( i );
    iaWidths[0] = nCols++;
    oItemHeader.r_data() = P3PmsgData ( (void*)iaWidths,nCols*sizeof(iaWidths[0]) );

    // Tidy up, and
    return TRUE;
}

BOOL
CListCtrl_DeserialiseHeader ( CListCtrl& oCListCtrl, P3PmsgItem& oItemHeader )
{
    try
    {
      if ( oItemHeader.DataType() < VBLockData_BLOB08    ||
           oItemHeader.DataType() > VBLockData_BLOB16var    )
        return FALSE;
      int *piWidths = (int *)oItemHeader.c_vBlob();
      int    nCols  = piWidths[0];
      for ( int i = 0; i < nCols; i++ )
        oCListCtrl.SetColumnWidth ( i, piWidths[i+1] );
      return TRUE;
    } catch ( ... ) {}

    // Tidy up, and
    return FALSE;
}
