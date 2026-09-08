// Copyright © 2013, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CString extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"
typedef CList<CString> CMap_CString;

//
//  Simple CString extensions
//  NOTES: Static functions used to support CString
MsgcoreMFC_EXT CString
CString_Truncate ( const CString& oCString, LPCTSTR lpszCharset );
MsgcoreMFC_EXT CString
CString_Extract  ( const CString& oCString, LPCTSTR lpszCharset );

MsgcoreMFC_EXT BOOL
CString_RightCompare ( const CString& oCString, LPCTSTR lpszSearch );
MsgcoreMFC_EXT BOOL
CString_RightReplace ( CString& oCString, LPCTSTR lpszRight, LPCTSTR lpszReplace );
MsgcoreMFC_EXT CString
CString_ReplaceNC ( const CString& oCString, LPCTSTR lpszSubstring, LPCTSTR lpszReplace );

MsgcoreMFC_EXT bool
CString_Wildcard ( const CString& oCString, LPCTSTR lpszWildcard, BOOL bNocase = FALSE );

MsgcoreMFC_EXT CString
CString_FormatwithCommas ( const CString& strRawValue );

MsgcoreMFC_EXT int
CString_FindNocase ( const CString& oCString, const CString& oCStringFind );

MsgcoreMFC_EXT CString
CString_ReplaceTabs ( const CString& oCString, UINT uiTabSize );

