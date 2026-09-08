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
//  CListCtrl extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"
#include "P2Pmsg.h"

//
//  CListCtrl operational helpers
//  NOTES: Static functions are just simpler
MsgcoreMFC_EXT int
CListCtrl_FormatItem ( CListCtrl& oCListCtrl, int nRow, int nColumn
                     , LPCTSTR lpszFormat, ... );

MsgcoreMFC_EXT void
CListCtrl_DeleteAllColumns ( CListCtrl& oCListCtrl );

MsgcoreMFC_EXT void
CListCtrl_ClearTextSubItem ( CListCtrl& oCListCtrl, int nItem, int nSubItem );

MsgcoreMFC_EXT void
CListCtrl_ClearTextAllSubItems ( CListCtrl& oCListCtrl, int nItem );

MsgcoreMFC_EXT void
CListCtrl_DeselectAllItems ( CListCtrl& oCListCtrl );

MsgcoreMFC_EXT BOOL
CListCtrl_SwapItems ( CListCtrl& oCListCtrl, int nItem1, int nItem2);

//
//  CListCtrl style management
//  NOTES: Static functions are just simpler
MsgcoreMFC_EXT DWORD
CListCtrl_GridLines ( CListCtrl& oCListCtrl, BOOL bEoD );

//
//  CHeaderCtrl operational helpers
MsgcoreMFC_EXT BOOL
CHeaderCtrl_GetItemText ( CHeaderCtrl *pHeader, CString& strItemText );
MsgcoreMFC_EXT int
CHeaderCtrl_FindItemLPARAM( CHeaderCtrl *pHeader, LPARAM lParam );
MsgcoreMFC_EXT int
CHeaderCtrl_SetItemLPARAM( CHeaderCtrl *pHeader, int nColm, LPARAM lParam );

//
//  P2Pmsg - CListCtrl operational helpers
//  NOTES: Static function set for injecting P2Pmsg data sets into
//         CListCtrl objects
MsgcoreMFC_EXT BOOL
CListCtrl_MakeColumns ( CListCtrl& oCListCtrl, const P3PmsgItem& oItemColumns );
MsgcoreMFC_EXT BOOL
CListCtrl_InsertRow ( CListCtrl& oCListCtrl, const P3PmsgItem& oItemColumns );

//
//  P2Pmsg - CListCtrl serialisation helpers
//  NOTES: Static function for serialising and deserialising header sizeng etc
MsgcoreMFC_EXT BOOL
CListCtrl_SerialiseHeader   ( CListCtrl& oCListCtrl, P3PmsgItem& oItemHeader );
MsgcoreMFC_EXT BOOL
CListCtrl_DeserialiseHeader ( CListCtrl& oCListCtrl, P3PmsgItem& oItemHeader );
