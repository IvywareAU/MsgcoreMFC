// Copyright © 2006, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CComboBox extensions
//

#include "StdAfx.h"
#include "CComboBox_Ext.h"

//
//  Fetches text for selected item in CComboBox
//
//  Parameters:  CComboBox oCComboBox
//               Control from which selected text is to be retrieved
//
//  Returns:     CString
//               Selected text
CString
CComboBox_GetCurSelText ( CComboBox& oCComboBox )
{
    CString strSelectedText;
    int idx = oCComboBox.GetCurSel ( );
    if ( idx >= 0 )
      oCComboBox.GetLBText ( idx, strSelectedText );
    return strSelectedText;
}

DWORD_PTR
CComboBox_GetCurSelData ( CComboBox& oCComboBox )
{
    CString strSelectedText;
    int idx = oCComboBox.GetCurSel ( );
    if ( idx >= 0 )
      return oCComboBox.GetItemData ( idx );
    return ~(DWORD_PTR)0;
}

//
//  Inserts item and associated data in CComboBox
//
//  Parameters:  CComboBox oCComboBox
//               Control into which item is to be inserted
//
//  Returns:     int
//               Inserted index as per CComboBox::InsertString()
int
CComboBox_InsertStringData ( CComboBox& oCComboBox
                           , LPCTSTR lpszString, DWORD_PTR dwItemData )
{
    int idx = oCComboBox.InsertString ( -1, lpszString );
    if ( idx >= 0 )
      idx = oCComboBox.SetItemData ( idx, dwItemData );
    return idx;
}

///////////////////////////////////////////////////////////////////////////////
//  CComboBox serialisation 

//
//  Serialises CComboBox contents
//
//  Parameters:  CComboBox& oCComboBox
//               Object to be serialised
//
//               P3PmsgItem& oItemCombo
//               Serialisation item
//
//  Returns:     int
//               Entries serialised
MsgcoreMFC_EXT int
CComboBox_Serialise ( CComboBox& oCComboBox, P3PmsgItem& oItemCombo )
{
    // Drop all existing entries
    oItemCombo.r_Desc().Truncate ( );
    CString strLBText;
    for ( int i = 0; i < oCComboBox.GetCount(); i++ )
    {
      oCComboBox.GetLBText ( i, strLBText );
      P3PmsgData oData ( oCComboBox.GetItemData(i) );
      LPCTSTR lpcnLBText = strLBText;
      oItemCombo.r_Desc() += P3PmsgItem ( lpcnLBText, oData );
    }
    // Current selection
    P3PmsgData oData(oCComboBox.GetCurSel());
    oItemCombo.r_Attr().DeclareItem(L"CurSel",oData);
    return oCComboBox.GetCount();
}
MsgcoreMFC_EXT int
CComboBox_Deserialise ( CComboBox& oCComboBox, P3PmsgItem& oItemCombo )
{
    if ( oItemCombo.r_Desc().GetCount() <= 0 )
      return 0;
    try
    {
      P3PmsgCurs oCurs(oItemCombo.r_Desc());
      for ( int i = 0; oCurs.Goto(i); i++ )
      {
        CString strName = oCurs.r_name().c_name();
        CComboBox_InsertStringData ( oCComboBox, strName, 0 );
      }
      P3PmsgField oCurSel = oItemCombo.r_Attr().DeclareItem(L"CurSel",(int)0).r_Object();
      if ( oCurSel.c_int() >= (int)oItemCombo.r_Desc().GetCount() )
        oCurSel.c_int(oItemCombo.r_Desc().GetCount() - 1);
      oCComboBox.SetCurSel ( oCurSel.c_int() );
    }
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel
    return 0;
}
