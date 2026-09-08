// Copyright © 2002, 2024, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CStringParser definitions etc
//  NOTES: Home grown CString parser thats evolved over time

#pragma once

#ifndef NO_DEBUG_NEW
#define new DEBUG_NEW
#endif

#include "MsgcoreMFC.h"
#include "MsgCollectors.h"
//#include "VAParser.h"
//#include "ClpEnv.h"

//
//  Manages CStringParser environment stack and state
//  NOTES: Internal use only
class ClpEnv
{
      void
        RenderEnvSafe ( );

    // Constructors and destructor
    public:
        ClpEnv ( );

        ClpEnv ( TCHAR *pCommand );

      virtual
       ~ClpEnv ( );

    // Stack management
    public:
      ClpEnv*
        Push ( TCHAR *pCheckpoint, TCHAR *pConsumed, TCHAR *pCommand );
      ClpEnv*
        Pop  ( TCHAR *pCheckpoint, TCHAR *pConsumed, TCHAR *pCommand );

    // Attributes
    public:
      int      m_nLevel;               // This stack level
      bool     m_bSkip;                // Skip sequence flag
      bool     m_bOptional;            // Optional sequence flag
      int      m_eFCF;                 // Format control flag
      int     *m_ePtr{nullptr};
      int      m_eSeqNum;              //
      TCHAR   *m_pCommand{nullptr};    // Current    decode position
      TCHAR   *m_pCompleted{nullptr};  // Completed  decode position
//      char      *m_pConsumed;        // Consumed   decode position
      TCHAR   *m_pCheckpoint{nullptr}; // Checkpoint decode position
      TCHAR   *m_pFormat{nullptr};     // Format checkpoint position

      ClpEnv  *m_pClpEnvPrev{nullptr}; // Previous environment
};
typedef P2PSafePtr<ClpEnv> ClpEnvSP;

//
//
//  CLParser object
//  NOTES: General purpose command line parser.
//
class MsgcoreMFC_EXT CStringParser
{
      void
        RenderParserSafe();

    // Constructors and destructor
    public:
        CStringParser ( );

        CStringParser ( LPCTSTR lpszRawCommand );

      virtual
       ~CStringParser () noexcept;

    // Overloaded operators
    public:
      TCHAR
        operator [] ( int nIndex );

	  // Operations
    public:
	    int
	      Scanf ( LPCTSTR lpszFormat, ... );
      int
        Reformat ( );
      void
        Empty ( );
      CString
        Argument ( int nArg, LPCTSTR lpszArgDelimiters );
      CString
        Switch ( int nArg, LPCTSTR lpszArgDelimiters
               , int nSwitch, LPCTSTR lpszSwitchDelimiters );
      CString
        Substitute ( LPCTSTR lpszRawCommand );
      TCHAR
        Consume ( );
      BOOL
        JumpOver ( LPCTSTR lpszSequence );

    // Utilities
    private:
      int
        GetInteger ( const TCHAR **pFormat, int *pInteger, int xSize = 0 );
      UINT
        GetHexadecimal ( const TCHAR **pFormat, UINT *pUINT );
      double
        GetFloat   ( const TCHAR **pFormat, double *pFloat );
      double
        GetFloatCurrency ( const TCHAR **pFormat, double *pFloat );
      int
        GetAscii   ( const TCHAR **pFormat, int    *pAscii );
      int
        GetString  ( const TCHAR **pFormat, TCHAR  *pString, int xSize );
      int
        GetStringTabbed  ( const TCHAR **pFormat, TCHAR  *pString, int xSize );
      TCHAR
        GetChar    ( const TCHAR **pFormat, TCHAR  *pChar );

    // Property management
    public:
      bool
        IsEmpty ( );
      TCHAR*
        Command ( );

    // Attributes
    private:
      TCHAR     *m_szRawCommand{nullptr};
      ClpEnv    *m_pClpEnv{nullptr};
};
typedef P2PSafePtr<CStringParser> CStringParserSP;

//
//  Standard extractors using CLParser above
MsgcoreMFC_EXT CString
CLParser_ExtractDomain ( LPCTSTR lpszURL );


const   int WILDCARD = INT_MAX;

//
//
//  Helpers for variable argument parsering
//  NOTES: Use these static functions to manage variable argument list
//         parsing.
//       : Exceptions are thrown upon errors
//
char
VapCharacter ( char cDelimiter, const char *pFormat
             , const char **ppFormat, va_list *pap );

CString
VapString ( char cDelimiter, const char *pFormat
          , const char **ppFormat, va_list *pap );

int
VapInteger ( char cDelimiter, const char *pFormat
           , const char **ppFormat, va_list *pap );

int
VapAscii ( char cDelimiter, const char *pFormat
         , const char **ppFormat, va_list *pap );

short
VapShort   ( char cDelimiter, const char *pFormat
           , const char **ppFormat, va_list *pap );

double
VapDouble ( char cDelimiter, const char *pFormat
          , const char **ppFormat, va_list *pap );
DWORD
VapDWORD ( char cDelimiter, const char *pFormat
         , const char **ppFormat, va_list *pap );
