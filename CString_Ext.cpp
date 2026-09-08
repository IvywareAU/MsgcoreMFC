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
//  Implementation for CString extensions
//
#include "StdAfx.h"
#include "Sys/timeb.h"

#include "CString_Ext.h"

//
//  Truncates string at first occurence of passed characters
//
//  Parameters:  const CString& oCString
//               String to be truncated
//
//  Returns:     CString
//               Truncated string
CString
CString_Truncate ( const CString& oCString, LPCTSTR lpszCharset )
{
    // Introduce locals
    int idx = oCString.FindOneOf ( lpszCharset );
    if ( idx > 0 )
      return oCString.Left ( idx );
    return oCString;
}

//
//  Extracts string after first occurence of passed characters
//
//  Parameters:  const CString& oCString
//               String from which sub-string is to be extracted
//
//               LPCTSTR lpszCharset
//               List of possible characters after which extraction occurs
//
//  Returns:     CString
//               Extracted string
CString
CString_Extract ( const CString& oCString, LPCTSTR lpszCharset )
{
    // Introduce locals
    int idx = oCString.FindOneOf ( lpszCharset );
    if ( idx < 0 )
      return oCString;
    int nLength = oCString.GetLength();
    if ( idx < nLength-1 )
      return oCString.Right ( nLength-idx-1 );
    return CString();
}


BOOL
CString_RightCompare ( const CString& oCString, LPCTSTR lpszCompare )
{
    LPCTSTR lpszString  = oCString;
    size_t     nString  = oCString.GetLength();
    size_t     nCompare = _tcslen(lpszCompare);
    for ( size_t i = 1; i <= nCompare; i++ )
    {
      if ( lpszString[nString-i] != lpszCompare[nCompare-i] )
        return FALSE;
    }
    return TRUE;
}

BOOL
CString_RightReplace ( CString& oCString, LPCTSTR lpszRight, LPCTSTR lpszReplace )
{
    LPCTSTR lpszString  = oCString;
    int     nString  = oCString.GetLength();
    int     nRight   = (int)_tcslen(lpszRight);
    for ( int i = 1; i <= nRight; i++ )
    {
      if ( lpszString[nString-i] != lpszRight[nRight-i] )
        return FALSE;
    }
    oCString  = oCString.Left(nString-nRight);
    oCString += lpszReplace;
    return TRUE;
}


//
//  Performs generic DOS style wildcard pattern matching
//
//
//  Parameters:  const CString& oCString
//               String to be compared against wildcard
//
//               P2PmsgID strMsgWildcard
//               Comparision wildcard
//
//               BOOL bNocase = FALSE
//               Ignore case
//
//  Returns:     bool
//                 true... Message matches wildcard
//                 false.. No match
//
bool
CString_Wildcard ( const CString& oCString, LPCTSTR lpszWildcard, BOOL bNocase )
{
    // Locals
    LPCTSTR lpszString = oCString;
    BOOL         bStar = FALSE;

    // Implmentation
TOP:const TCHAR *p, *s;
    for ( s = lpszString, p = lpszWildcard; *s; ++s, ++p )
    {
      switch (*p)
      {
        case L'?':
          if ( *s == L'.' )
          {
            goto starCheck;
          }
          break;
        case L'*':
          bStar = TRUE;
          lpszString = s, lpszWildcard = p;
          if ( !*++lpszWildcard )
            return TRUE;
          goto TOP;
        case '#':
          if ( !isdigit(*s) )
            goto starCheck;
          while ( isdigit(*(s+1)) )
            s++;
          break;
        default:
          //if (mapCaseTable[*s] != mapCaseTable[*p])
          if ( *s == *p )
            break;
          if ( bNocase && toupper(*s) == toupper(*p) )
            break;
          goto starCheck;
      }
    }
    if ( *p == L'*' ) ++p;
    return (!*p);

starCheck:
    if ( !bStar )
      return FALSE;
    lpszString++;
    goto TOP;
}

CString
CString_FormatwithCommas ( const CString& strRawValue )
{
    CString strTranslated;
    CString strReversed = strRawValue; strReversed.MakeReverse();
    // Process out decimal proportion
    if ( strReversed.Find(L'.') >= 0 )
    {
      while ( strReversed[0] != L'.' )
      {
        strTranslated += strReversed[0];
        strReversed.Delete ( 0, 1 );
      }
      strTranslated += strReversed[0];
      strReversed.Delete ( 0, 1 );
    }
    // Trailing whatever
    while ( !isdigit(strReversed[0]) )
    {
      strTranslated += strReversed[0];
      strReversed.Delete ( 0, 1 );
    }
    // Process pre-decimal proportion
    int nCount = 0;
    while ( isdigit(strReversed[0]) )
    {
      if ( nCount > 0 && (nCount%3) == 0 )
        strTranslated += L',';
      strTranslated += strReversed[0];
      strReversed.Delete ( 0, 1 );
      nCount++;
    }
    // Leading whatever
    while ( !strReversed.IsEmpty() )
    {
      strTranslated += strReversed[0];
      strReversed.Delete ( 0, 1 );
    }
    // Done
    return strTranslated.MakeReverse ();
}

//
//  Seach for sub-string in string ignoring case
//
//  Parameters:  const CString& oCString
//               String to be seached
//
//               const CString& oCStringFind
//               String to be located
//
//  Returns:     int
//                 -ve... Search failed
//                   ?... Location index
MsgcoreMFC_EXT int
CString_FindNocase ( const CString& oCString, const CString& oCStringFind )
{
    CString oCStringUpper     = oCString; oCStringUpper.MakeUpper();
    CString oCStringFindUpper = oCStringFind; oCStringFindUpper.MakeUpper();
    int iPos =oCStringUpper.Find(oCStringFindUpper);
    return iPos;
}

//
//  Replace sub-string in string ignoring case
//
//  Parameters:  const CString& oCString
//               String to be seached
//
//               LPCTSTR lpszSubstring
//               String to be located
//
//               LPCTSTR lpszReplace
//               Replacement string
//
//  Returns:     int
//
MsgcoreMFC_EXT CString
CString_ReplaceNC ( const CString& oCString, LPCTSTR lpszSubstring, LPCTSTR lpszReplace )
{
    CString strResult    = oCString;
    CString strSubstring = lpszSubstring;
    CString strReplace   = lpszReplace;
    if ( strReplace.CompareNoCase(strSubstring) == 0 )
      return strResult;
    int iPos;
    while ( (iPos = CString_FindNocase(strResult,lpszSubstring)) >= 0 )
    {
      CString strSubstringIP;
      for ( int i = 0; i < strSubstring.GetLength(); i++ )
        strSubstringIP += oCString[iPos+i];
      strResult.Replace(strSubstringIP,lpszReplace);
    }
    return strResult;
}

//
//  Replace tabs with spaces within string
//
//  Parameters:  const CString& oCString
//               String in which tabs are to be replaced
//
//               UINT nTabSize
//               Tab size
//
//  Returns:     CString
//               Resultant string
//
MsgcoreMFC_EXT CString
CString_ReplaceTabs ( const CString& oCString, UINT uiTabSize )
{
    CString  strString;
    LPCTSTR lpszString = oCString;
    int      k = 0;
    while ( *lpszString )
    {
      if ( *lpszString != L'\t') {
        strString += *lpszString++;
        k++;
        continue;
      }
      lpszString++;
      do {
        strString += L' ';
        k++;
      } while ( k % uiTabSize);
    }
    return strString;
}
