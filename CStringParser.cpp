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
//  CStringParser implementation
//  NOTES: Home grown CString parser thats evolved over time

#include "StdAfx.h"

#include "Float.h"
#include "CStringParser.h"
#include "Msgexception.h"

void MessageThrow ( LPCTSTR lpszMessage, ... );

//
//
//  Description: CLParser object
//               NOTES: Use this object for command line 
//                      parsing.
//

///////////////////////////////////////////////////////////////////////
//   Constructors and destructor

//
//  Description: Constructors
//
//
//  Parameters:  LPCTSTR lpszRawCommand
//               Raw parser command
//
CStringParser::CStringParser ( LPCTSTR lpszRawCommand )
{
    // Firstly
    RenderParserSafe();

    size_t iSize = wcslen(lpszRawCommand);
    m_szRawCommand = new TCHAR [iSize+2];
    wmemcpy ( m_szRawCommand, lpszRawCommand, iSize );
    m_szRawCommand[iSize++] = 0;
    m_szRawCommand[iSize++] = 0;

    m_pClpEnv = new ClpEnv ( m_szRawCommand );
}

CStringParser::CStringParser ( )
{
    // Firstly
    RenderParserSafe ();
}

CStringParser::~CStringParser () noexcept
{
    // Garbage collection
    delete [] m_szRawCommand;
    delete m_pClpEnv;
}

void
CStringParser::RenderParserSafe()
{
    // Attributes
    //m_szRawCommand = 0;
    //m_pClpEnv      = 0;
}

///////////////////////////////////////////////////////////////////////
//  Overloaded operators

//
//  Description: operator []
//
//
//  Parameters:  int nIndex
//               Element to be retrieved
//
//  Returns:     char
//
TCHAR
CStringParser::operator [] ( int nIndex )
{
    // Simply
    return m_pClpEnv->m_pCompleted[nIndex];
}

///////////////////////////////////////////////////////////////////////
//  Parsing

//
//  Description: Decodes encapsulated command according to
//               passed format
//
//  Parameters:  const char *pFormat
//               Command decode specification
//
//               ...
//               Variable argument associated with above format
//
int
CStringParser::Scanf ( LPCTSTR pFormat, ... )
{
    // Introduce the locals
    va_list      ap;
    int         *pInteger = nullptr;   // Integer decode pointer
    UINT        *pUINT = nullptr;      // HEX     decode pointer
    double      *pDouble = nullptr;    // Double  decode pointer
    int         *pAscii = nullptr;     // Ascii   decode pointer
    TCHAR       *pString = nullptr;    // String  decode pointer
    TCHAR       *pChar = nullptr;      // Char    decode pointer
    TCHAR       *pCompleted = m_pClpEnv -> m_pCompleted;
    int          nMatched   = 0;

    // Initialise
    va_start ( ap, pFormat );

    // Loop until passed format exhausted
    while ( *pFormat )
    {
LPCTSTR lpszCommand=m_pClpEnv->m_pCommand;//DELETE-ME
LPCTSTR lpszCompleted=m_pClpEnv->m_pCompleted;//DELETE-ME
      // Manage format control character
      if ( *pFormat == '$' )
      {
        pFormat++;                     // Consume '$' character

        // Enumerated data field
        if ( *pFormat == _T('E') )
        {
          pFormat++;                   // Consume 'E' character
          if ( *pFormat != '{' )
            MessageThrow ( _T("Enumerated format must start with '$E{'") );

          m_pClpEnv = m_pClpEnv -> Push ( m_pClpEnv -> m_pCommand
                                        , 0
                                        , m_pClpEnv -> m_pCommand );

          pFormat++;                   // Consume '{' character
          m_pClpEnv -> m_pFormat = (TCHAR *)pFormat;
          m_pClpEnv -> m_eFCF    = _T('E');
          m_pClpEnv -> m_ePtr    = va_arg ( ap, int * );
        *(m_pClpEnv -> m_ePtr)   = -1;
          m_pClpEnv -> m_eSeqNum =  0;

          m_pClpEnv = m_pClpEnv -> Push ( 0, 0, 0 );
          continue;
        }

        // Jump sequence
        if ( *pFormat == _T('J') )
        {
          pFormat++;                   // Consume 'F' character
          if ( *pFormat != _T('{') )
            EVERR->MODULE
                 ->Message( _T("Jump sequence must start with '$J{'") )
                 ->Throw ( );

          pFormat++;                   // Consume '{' character
          m_pClpEnv -> m_pFormat = (TCHAR *)pFormat;
          m_pClpEnv = m_pClpEnv -> Push ( m_pClpEnv -> m_pCommand
                                        , 0
                                        , m_pClpEnv -> m_pCommand );

          m_pClpEnv -> m_eFCF    = _T('J');
          m_pClpEnv -> m_ePtr    =  va_arg ( ap, int * );
        *(m_pClpEnv -> m_ePtr)   = -1;
          m_pClpEnv -> m_eSeqNum =  0;

          m_pClpEnv = m_pClpEnv -> Push ( 0, 0, 0 );
          continue;
        }

        // Consumed command position update
        // NOTES: Ignore when skip mode switched on
        if ( *pFormat == L'C' )
        {
          pFormat++;                   // Consume 'C' character
          if ( !m_pClpEnv -> m_bSkip )
            pCompleted = m_pClpEnv -> m_pCommand;
//          ppChar = va_arg ( ap, char ** );
//          if ( !m_pClpEnv -> m_bSkip )
//            *ppChar = m_pClpEnv -> m_pCommand;
lpszCommand=m_pClpEnv->m_pCommand;//DELETE-ME
lpszCompleted=m_pClpEnv->m_pCompleted;//DELETE-ME
          continue;
        }

        // Abandon for NULL remaining command
        if ( *pFormat == _T('N') )
        {
          pFormat++;                   // Consume 'N' character
          if ( *(m_pClpEnv -> m_pCommand) )
            continue;                  // Data still remains

          while ( m_pClpEnv -> m_nLevel > 0 )
            m_pClpEnv = m_pClpEnv -> Pop ( 0, 0, 0 );
          break;
        }

        // Skip to current position
        if ( *pFormat == _T('S') )
        {
          pFormat++;                   // Consume 'S' character
          if ( !m_pClpEnv -> m_bSkip )
            pCompleted = m_pClpEnv -> m_pCommand;
          continue;
        }

        // Unknown logic control field
        else
          EVERR->MODULE
               ->Message( _T("Unknown format logic control field ($%c)") )
               ->Throw ( );
      }

      // Manage data control fields
      // NOTES: These fields are NOT decoded if the clp_env->skip
      //        flag is set.  However, the corresponding pointer
      //        in the variable argument list is always addressed
      //        to keep synchronisation.
      //      : The decode function is relied upon to move the
      //        format pointer forwards.
      if ( *pFormat == '%' )
      {
        int xSize = 0;
        pFormat++;                     // Consume '%' character

        if ( isdigit(*pFormat) )
        {
          while ( isdigit(*pFormat) )
            xSize = xSize*10 + (int)(*pFormat++ - '0');
        }
        else if ( *pFormat == '*' )
        {
          xSize = va_arg ( ap, int );
          pFormat++;                   // Consume '*' character
        }

        // Manage INTEGER control field
        if ( *pFormat == _T('i') )
        {
          pInteger = va_arg ( ap, int * );
          GetInteger ( &pFormat, pInteger, xSize );
        }

        // Manage HEXADECIMAL control field
        else if ( *pFormat == _T('x') )
        {
          pUINT = va_arg ( ap, UINT * );
          GetHexadecimal ( &pFormat, pUINT );
        }

        // Manage FLOAT control field
        else if ( *pFormat == _T('f') )
        {
          pDouble = va_arg ( ap, double * );
          GetFloat ( &pFormat, pDouble );
        }

        // Manage FLOAT currency field
        else if ( *pFormat == _T('F') )
        {
          pDouble = va_arg ( ap, double * );
          GetFloatCurrency ( &pFormat, pDouble );
        }

        // Manage ASCII control field
        else if ( *pFormat == _T('a') )
        {
          pAscii = va_arg ( ap, int * );
          GetAscii ( &pFormat, pAscii );
        }

        // Manage STRING control field
        else if ( *pFormat == _T('s') )
        {
          pString = va_arg ( ap, TCHAR * );
          GetString ( &pFormat, pString, xSize );
        }

        // Manage tabbed STRING control field
        else if ( *pFormat == L'T' )
        {
          pString = va_arg ( ap, TCHAR * );
          GetStringTabbed ( &pFormat, pString, xSize );
        }

        // Manage CHARACTER control field
        else if ( *pFormat == _T('c') )
        {
          pChar = va_arg ( ap, TCHAR * );
          GetChar ( &pFormat, pChar );
        }

        // Unknown control field type
        else
          MessageThrow ( _T("Valid data control fields %%(i,x,f,s,a or c) ")
                         _T("not [%c]\n")
                         _T("ADVICE\t: Bug(SNHappen)")
                        , *pFormat);

        if ( !m_pClpEnv -> m_bSkip )
          nMatched++;
        continue;
      }

      // Manage optional logic
      if ( *pFormat == _T('[') )
      {
        pFormat++;                       // Consume '[' character
        m_pClpEnv = m_pClpEnv -> Push ( 0
                                      , 0
                                      , m_pClpEnv -> m_pCommand );
        m_pClpEnv -> m_bOptional = true; // Flag optional logic
        continue;
      }

      if ( *pFormat == _T(']') )
      {
        pFormat++;                     // Consume ']' character
        if ( !m_pClpEnv -> m_bOptional )
          MessageThrow( _T("Decode format ] mismatch") );
        m_pClpEnv = m_pClpEnv -> Pop ( 0
                                     , 0
                                     , m_pClpEnv -> m_pCommand );
        continue;
      }

      // End of enumerated data field flags
      if ( *pFormat == '|' ||
           *pFormat == '}'    )
      {
        if ( m_pClpEnv -> m_eFCF != _T('E') &&
             m_pClpEnv -> m_eFCF != _T('J')    )
          MessageThrow ( _T("| and } only valid in $E and $F field types") );

        // Matching field flagged by skip mode not set
        if ( !m_pClpEnv -> m_bSkip )
        {
lpszCommand=m_pClpEnv->m_pCommand;//DELETE-ME
lpszCompleted=m_pClpEnv->m_pCompleted;//DELETE-ME
          m_pClpEnv = m_pClpEnv -> Pop (  0
                                       , m_pClpEnv -> m_pCommand
                                       , m_pClpEnv -> m_pCheckpoint );
lpszCommand=m_pClpEnv->m_pCommand;//DELETE-ME
lpszCompleted=m_pClpEnv->m_pCompleted;//DELETE-ME

          if ( m_pClpEnv -> m_eFCF == _T('E') )
          {
            if ( *(m_pClpEnv -> m_ePtr) >= 0 )
              MessageThrow ( _T("Command sequence (%s) is not unique")
                            , m_pClpEnv -> m_pCommand );
            *(m_pClpEnv -> m_ePtr) = m_pClpEnv -> m_eSeqNum;
          }
          if ( m_pClpEnv -> m_eFCF == _T('J') )
          {
            if ( *(m_pClpEnv -> m_ePtr) >= 0 )
              MessageThrow ( _T("Jump sequence (%s) is not unique")
                            , m_pClpEnv -> m_pCommand );
            *(m_pClpEnv -> m_ePtr) = m_pClpEnv -> m_eSeqNum;
          }
          nMatched++;
        }

        // Non-matching field flagged by skip mode being set
        else
          m_pClpEnv = m_pClpEnv -> Pop ( 0, 0, 0 );

        // Manage start of new enumeration field
        // NOTES: Move command pointer back to checkpoint position
        if ( *pFormat == L'|' )
        {
          m_pClpEnv -> m_eSeqNum++;    // This sequence number
          m_pClpEnv = m_pClpEnv -> Push ( 0
                                        , 0
                                        , m_pClpEnv -> m_pCheckpoint );
          pFormat++;                   // Consume '|' character
          continue;
        }

        // Multi-field termination character
        if ( *pFormat == _T('}') )
        {
          pFormat++;                   // Consume '}' character
          // Handle enumerated fields
          if ( m_pClpEnv -> m_eFCF == _T('E') )
          {
            if ( *(m_pClpEnv -> m_ePtr)     < 0 &&
                  !m_pClpEnv -> m_bOptional        )
              MessageThrow ( _T("Cannot enumerate sequence (%.64s)")
                           , m_pClpEnv -> m_pCommand );
            m_pClpEnv = m_pClpEnv -> Pop ( 0
                                         , 0
                                         , m_pClpEnv -> m_pCompleted );
            continue;
          }
          // Handle jump fields
          if ( m_pClpEnv -> m_eFCF == _T('J') )
          {
            if ( *(m_pClpEnv -> m_ePtr)     < 0 &&
                  !m_pClpEnv -> m_bOptional     &&
                 !*m_pClpEnv -> m_pCommand         )
              MessageThrow ( _T("Cannot jump to sequence (%.64s)")
                           , m_pClpEnv -> m_pCommand );
            // Located
            if ( *(m_pClpEnv->m_ePtr) >= 0 )
            {
              m_pClpEnv = m_pClpEnv -> Pop ( 0
                                           , 0
                                           , m_pClpEnv -> m_pCompleted );
              continue;                // Normal success
            }
            // Failure - Move character pointer forward and start again
            // NOTES: Done by resetting pointers etc
            pFormat   = m_pClpEnv -> m_pFormat;
            m_pClpEnv -> m_eSeqNum = 0;
            m_pClpEnv -> m_pCommand++;
            m_pClpEnv -> m_pCompleted  = m_pClpEnv -> m_pCommand;
            m_pClpEnv -> m_pCheckpoint = m_pClpEnv -> m_pCommand;
            m_pClpEnv -> m_bSkip   = false;
            m_pClpEnv = m_pClpEnv->Push ( m_pClpEnv->m_pCommand/*initially 0 21/6/11*/
                                        , m_pClpEnv->m_pCommand/*initially 0 20/6/11*/
                                        , m_pClpEnv->m_pCommand/*Was 20/6/11 + 1*/ );
            //m_pClpEnv = m_pClpEnv -> Pop ( 0
            //                             , m_pClpEnv -> m_pCommand + 1
            //                             , m_pClpEnv -> m_pCompleted );
            continue;
          }

lpszCommand=m_pClpEnv->m_pCommand;//DELETE-ME
lpszCompleted=m_pClpEnv->m_pCompleted;//DELETE-ME
          ASSERT(0);
          continue;
        }
      }

      // Abandon scanning
      // NOTES: Only applies when no enumerated alteratives
      //        matched.
      //      : Must be within $E{...|...|^} sequence
      if ( *pFormat == _T('^') )
      {
        if ( m_pClpEnv -> m_eFCF == _T('E') )
        {
          if ( *(m_pClpEnv -> m_ePtr) < 0 )
          {
            while ( m_pClpEnv -> m_nLevel > 0 )
              m_pClpEnv = m_pClpEnv -> Pop ( m_pClpEnv -> m_pCompleted
                                           , m_pClpEnv -> m_pCompleted
                                           , m_pClpEnv -> m_pCompleted );
            break;
          }
          pFormat++;
          m_pClpEnv -> m_bSkip = true;   // Flag field for skipping
          continue;
        }
        ASSERT(0);
      }

      // Pattern matching is the default
      if ( *pFormat == '\\' )
        pFormat++;

      if ( !m_pClpEnv -> m_bSkip )     // Not if skip mode is on
      {
        if (   *pFormat                   == ' '      &&
             ( *(m_pClpEnv -> m_pCommand) == ' ' ||
               *(m_pClpEnv -> m_pCommand) == '\t'   )    )
        {
          while ( *(++pFormat)               == ' ' );
          while ( *(m_pClpEnv -> m_pCommand) == ' '  ||
                  *(m_pClpEnv -> m_pCommand) == '\t'    )
            ++m_pClpEnv->m_pCommand;
          continue;
        }

        if ( toupper( (int)*pFormat )
                         == toupper ( (int)*(m_pClpEnv->m_pCommand) ) )
        {
          pFormat++;                   // Consume character
          m_pClpEnv -> m_pCommand++;
          continue;
        }

        if ( m_pClpEnv -> m_bOptional            ||
             m_pClpEnv -> m_eFCF      == _T('E')    )
        {
          m_pClpEnv -> m_bSkip = 1;    // Activate skip flag
          pFormat++;                   // Consume character
          continue;
        }

        if ( m_pClpEnv -> m_bOptional            ||
             m_pClpEnv -> m_eFCF      == _T('J')    )
        {
          m_pClpEnv -> m_bSkip = 1;    // Activate skip flag
          pFormat++;                   // Consume character
          continue;
        }

        EVERR->MODULE
             ->Message( _T("Invalid command sequence (%-.12s)"),
                        m_pClpEnv -> m_pCompleted )
             ->Throw();
      }
      pFormat++;                       // Increment format position
    }

    // Tidy up
    if ( m_pClpEnv -> m_nLevel != 0 )
      MessageThrow ( _T("Command level(%i) did not return to zero"),
                     m_pClpEnv -> m_nLevel );
    m_pClpEnv -> m_pCommand    = pCompleted;
    m_pClpEnv -> m_pCompleted  = pCompleted;
    m_pClpEnv -> m_pCheckpoint = pCompleted;
    va_end ( ap );
    return ( nMatched );
}

//
//  Description: Reformats encapsulated command to remove
//               leading, trailing and multiple space instances.
//
int
CStringParser::Reformat ( )
{
    // Introduce locals
    int          i;
    int          iLength = 0;
    TCHAR       *pCommand= 0;

    // Remove leading spaces
    for ( i = 0; ( i < iLength        &&
                   pCommand[i] == ' '    ); i++ );
    if ( i > 0 )
      wmemcpy ( pCommand, &pCommand[1], iLength -= i );
    pCommand[iLength] = 0;

    // Finally
    return iLength;
}

//
//  Description: Empties the contents of this object
//
//
void
CStringParser::Empty ( )
{
    // Simply
    while ( *m_pClpEnv->m_pCommand )
      m_pClpEnv -> m_pCommand++;
    m_pClpEnv -> m_pCompleted = m_pClpEnv -> m_pCommand;
}

//
//  Description: Retrieves switch from command string
//
//
//  Parameters:  int nArg
//               Argument from which switch is to be retrieved
//
//               LPCTSTR lpszArgDelimiters
//               Argument delimiters
//
//               int nSwitch
//               Switch in above argument
//
//               LPCTSTR lpszSwitchdelimiters
//               Switch delimiters
//
//  Returns:     CString
//               Retrived switch
//
CString
CStringParser::Switch ( int nArg, LPCTSTR lpszArgDelimiters
                 , int nSwitch, LPCTSTR lpszSwitchDelimiters )
{
    // Introduce locals
    CString csArgument, csSwitch;
    int                 nSwitchTag = 0;
    bool                bQuotes = false;

    // Fetch argument
    csArgument = Argument ( nArg, lpszArgDelimiters );

    // Fetch switch
    for ( int i = 0; ( i < csArgument.GetLength() &&
                       nSwitchTag <= nSwitch         ); i++ )
    {
      TCHAR      cNext = csArgument[i];
      LPCTSTR lpszDels = lpszSwitchDelimiters;

      // Manage opening quotations
      if ( !bQuotes       &&
            cNext == '\"'    )
        bQuotes = true;

      while ( *lpszDels &&
              !bQuotes     )
      {
        if ( cNext != *lpszDels++ )
          continue;
        nSwitchTag++;
        break;
      }

      if ( nSwitch == nSwitchTag &&
           cNext   != '\"'          )
        csSwitch += cNext;

      // Manage closing quotations
      if ( bQuotes       &&
           cNext == '\"'    )
        bQuotes = true;
    }

    // Tidy up and
    return csSwitch;
}

//
//  Description: Retrieves argument from command string
//               NOTES: The retrieved argument is not consumed
//
//
//  Parameters:  int nArg
//               Argument to be retrieved
//
//               LPCTSTR lpszArgDelimiters
//               Argument delimiters
//
//  Returns:     CString
//               Retrived argument
//
CString
CStringParser::Argument ( int nArg, LPCTSTR lpszArgDelimiters )
{
    // Introduce locals
    CString csArgument;
    size_t   iSizeCmds = lstrlen(m_szRawCommand);
    int      nArgTag   = 0;
    bool     bArgument = false;
    bool     bQuotes   = false;

    // Fetch argument
    for ( size_t i = 0; ( i < iSizeCmds   &&
                          nArgTag <= nArg    ); i++ )
    {
      TCHAR      cNext = m_szRawCommand[i];
      LPCTSTR lpszDels = lpszArgDelimiters;

      // Manage opening quotations
      if ( !bQuotes       &&
            cNext == '\"'    )
        bQuotes = true;

      while ( *lpszDels )
      {
        if ( cNext != *lpszDels ||
             bQuotes               )
        {
          lpszDels++;
          continue;
        }
        if ( bArgument )
          nArgTag++;
        bArgument = false;
        break;
      }
      
      if ( *lpszDels ==  0 )
      {
        if ( nArg == nArgTag )
          csArgument += cNext;
        bArgument   = true;
      }

      // Manage closing quotations
      if ( bQuotes       &&
           cNext == '\"'    )
        bQuotes = true;
    }

    // Tidy up and
    return csArgument;
}

//
//  Description: Subtitute a new parser command
//
//
//  Parameters:  LPCTSTR lpszRawCommand
//               Raw parser command
//
CString
CStringParser::Substitute ( LPCTSTR lpszRawCommand )
{
    // Introduce locals
    CString csCurrent = m_szRawCommand;

    // Firstly
    delete [] m_szRawCommand;
    delete m_pClpEnv;
    RenderParserSafe();

    // Reload
    size_t iSize = wcslen(lpszRawCommand);
    m_szRawCommand = new TCHAR [iSize+2];
    wmemcpy ( m_szRawCommand, lpszRawCommand, iSize );
    m_szRawCommand[iSize++] = 0;
    m_szRawCommand[iSize++] = 0;

    m_pClpEnv = new ClpEnv ( m_szRawCommand );

    // Tidy up and
    return csCurrent;
}

//
//  Consumes single command character
//
//  Returns:     TCHAR
//               Consumed character
TCHAR
CStringParser::Consume ( )
{
    // Introduce locals
    TCHAR nChar = m_pClpEnv->m_pCommand[0];

    // Adjust
    if ( nChar )
    {
      m_pClpEnv -> m_pCommand++;
      m_pClpEnv -> m_pCompleted++;
    }

    // Tidy up and
    return nChar;
}

//
//  Jump over multiple command characters
BOOL
CStringParser::JumpOver ( LPCTSTR lpszSequence )
{
    // Command characters loop
    LPCTSTR lpszCommand = m_pClpEnv -> m_pCommand;
    while ( *lpszCommand )
    {
        // Jump characters loop
        LPCTSTR lpszJumpover = lpszSequence;
        while ( *lpszJumpover )
        {
            if ( *lpszCommand != *lpszJumpover )
                break;
            lpszJumpover++;
            lpszCommand++;
            if ( *lpszJumpover )
                continue;
            m_pClpEnv -> m_pCommand   = (TCHAR *)lpszCommand;
            m_pClpEnv -> m_pCompleted = (TCHAR *)lpszCommand;
            return true;
        }
        lpszCommand++;
    }

    // Tidy up and
    return false;
}

///////////////////////////////////////////////////////////////////////
//  Utilities

//
//
//  Description: Decodes an integer field from the passed
//               command line
//               NOTES: Command line integer fields are assumed to
//                      be a string of digits commencing at the
//                      current decode position.
//                    : Integer fields are designated by the %i
//                      format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               char *pInteger
//               Decoded value
//
int
CStringParser::GetInteger ( const TCHAR **ppFormat, int *pInteger, int xSize )
{
    // Introduce the locals
    TCHAR *pCommand;
    long   iValue = INT_MAX;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      pCommand = m_pClpEnv -> m_pCommand;
      if ( xSize <= 0 )
        iValue   = wcstol ( pCommand, &m_pClpEnv->m_pCommand, 10 );
      else
      {
        iValue = 0;
        while ( xSize-- > 0 && isdigit(*m_pClpEnv->m_pCommand) )
        {
          iValue = iValue*10 + ( *m_pClpEnv->m_pCommand - _T('0') );
          m_pClpEnv->m_pCommand++;
        }
      }
      if ( m_pClpEnv->m_pCommand == pCommand )
      {
        if ( m_pClpEnv -> m_bOptional        ||
             m_pClpEnv -> m_eFCF      == 'E'    )
          m_pClpEnv -> m_bSkip = true;
        else
          MessageThrow ( _T("Missing integer data field") );
      }
      else
        *pInteger = iValue;
    }

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return iValue;
}

//
//
//  Description: Decodes hexadecimal field from the passed
//               command line
//               NOTES: Command line hexadecimal fields are assumed
//                      to be a string of digits commencing at the
//                      current decode position.
//                    : Hexadecimal fields are designated by the %x
//                      format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               char *pUINT
//               Decoded value
//
UINT
CStringParser::GetHexadecimal ( const TCHAR **ppFormat, UINT *pUINT )
{
    // Introduce the locals
    TCHAR  *pCommand;
    ULONG  uiValue = INT_MAX;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      pCommand = m_pClpEnv -> m_pCommand;
      uiValue  = wcstoul ( pCommand, &m_pClpEnv->m_pCommand, 0 );
      if ( m_pClpEnv->m_pCommand == pCommand )
      {
        if ( m_pClpEnv -> m_bOptional        ||
             m_pClpEnv -> m_eFCF      == 'E'    )
          m_pClpEnv -> m_bSkip = true;
        else
          MessageThrow ( _T("Missing integer data field") );
      }
      else
        *pUINT = uiValue;
    }

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return uiValue;
}

//
//
//  Decodes a float field from the passed command line
//  NOTES: Command line float fields are assumed to be a string of digits
//         commencing at the current decode position.
//       : Integer fields are designated by the %f format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               double *pDouble
//               Decoded value
//
double
CStringParser::GetFloat   ( const TCHAR **ppFormat, double *pDouble )
{
    // Introduce the locals
    TCHAR  *pCommand;
    double  dValue  = DBL_MAX;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      pCommand = m_pClpEnv -> m_pCommand;
      // Quotes interceptions
      if ( *pCommand == L'"' )
      {
       *m_pClpEnv->m_pCommand++;
        dValue = GetFloat ( ppFormat, pDouble );
        if ( *m_pClpEnv->m_pCommand != L'"')
          MessageThrow ( L"Miss-matched quotes for float data field" );
       *m_pClpEnv->m_pCommand++;
       *pDouble = dValue;
        return dValue;
      }
      dValue   = wcstod ( pCommand, &m_pClpEnv->m_pCommand );
      if ( m_pClpEnv->m_pCommand == pCommand )
      {
        if ( m_pClpEnv -> m_bOptional        ||
             m_pClpEnv -> m_eFCF      == 'E'    )
          m_pClpEnv -> m_bSkip = true;
        else
          MessageThrow ( L"Missing float data field" );
      }
      else
        *pDouble = dValue;
    }

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return dValue;
}

//
//
//  Description: Decodes a float field from the passed
//               command line
//               NOTES: Command line float fields are assumed to
//                      be a string of digits commencing at the
//                      current decode position.
//                    : Integer fields are designated by the %f
//                      format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               double *pDouble
//               Decoded value
//
double
_tcstodcomma ( const TCHAR *nptr, TCHAR **endptr )
{
    double dValue = 0.0;
    int    nPoM   = 1;
    const TCHAR *nptr_work = nptr;
    *endptr = (TCHAR *)nptr;
    int    nQuoted = 0;
    while ( *nptr_work == _T(' ') ) nptr_work++;
    if ( *nptr_work == _T('\"') ) { nQuoted = 1; nptr_work++; }
    if ( *nptr_work == _T('-') ) { nPoM = -1; nptr_work++; }
    else if ( *nptr_work == _T('+') ) { nptr_work++; }

TOP:while ( isdigit(*nptr_work) )
      dValue = dValue * 10.0 + double ( (int)*nptr_work++ - (int)_T('0') );
    if ( *nptr_work == _T(',') )
    {
      nptr_work++;
      if (  isdigit(*(nptr_work+0)) &&
            isdigit(*(nptr_work+1)) &&
            isdigit(*(nptr_work+2)) &&
           !isdigit(*(nptr_work+3))    )
        goto TOP;
      return 0.0;
    }

    if ( *nptr_work == _T('.') )
    {
      double dFactor = 0.1;
      nptr_work++;
      while ( isdigit(*nptr_work) )
      {
        dValue   = dValue + double ( (int)*nptr_work++ - (int)_T('0') ) * dFactor;
        dFactor /= 10;
      }
      if (    nQuoted                   &&
           ( *nptr_work == _T('.') ||
             *nptr_work == _T(',')    )     )
        return 0.0;
    }

    if ( nQuoted )
    {
      if ( *nptr_work != _T('\"') )
        return 0.0;
      nptr_work++;
    }
    if ( nPoM < 0 )
      dValue = -dValue;
   *endptr = (TCHAR *)nptr_work;
    return dValue;
}
double
CStringParser::GetFloatCurrency ( const TCHAR **ppFormat, double *pDouble )
{
    // Introduce the locals
    TCHAR  *pCommand;
    double  dValue = DBL_MAX;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      pCommand = m_pClpEnv -> m_pCommand;
      // Quotes interceptions
      if ( *pCommand == L'"' )
      {
        m_pClpEnv->m_pCommand++;
        //dValue = GetFloat ( ppFormat, pDouble );
        dValue = _tcstodcomma ( pCommand, &m_pClpEnv->m_pCommand );
        //if ( *m_pClpEnv->m_pCommand != L'\"')
        //  MessageThrow ( L"Miss-matched quotes for float data field" );
        m_pClpEnv->m_pCommand++;
       *pDouble = dValue;
        return dValue;
      }
      dValue   = _tcstodcomma ( pCommand, &m_pClpEnv->m_pCommand );
      if ( m_pClpEnv->m_pCommand == pCommand )
      {
        if ( m_pClpEnv -> m_bOptional        ||
             m_pClpEnv -> m_eFCF      == 'E'    )
          m_pClpEnv -> m_bSkip = true;
        else
          MessageThrow ( _T("Missing currency data field") );
      }
      else
        *pDouble = dValue;
    }

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return dValue;
}

//
//  Decodes an ASCII field from the passed command line
//  NOTES: Command line ASCII fields are assumed to be a string of digits
//         commencing at the current decode position or a single alpha
//         character
//       : ASCII fields are designated by the %a format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               char *pFloat
//               Decoded value
//
int
CStringParser::GetAscii   ( const TCHAR **ppFormat, int *pAscii )
{
    // Introduce the locals
    TCHAR *pCommand;
    long   aValue = 0;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      pCommand = m_pClpEnv -> m_pCommand;
      if ( isdigit ( (int)*pCommand ) )
        aValue = wcstol ( pCommand, &m_pClpEnv->m_pCommand, 10 );
      else
      {
        aValue = (long)*m_pClpEnv -> m_pCommand++;
        if ( aValue == '*' )
          aValue = WILDCARD;
        else if ( (aValue < _T('A') || aValue > _T('Z') ) &&
                  (aValue < _T('a') || aValue > _T('z') )    )
          MessageThrow ( _T("Invalid integer-ascii field (%c)"), (char)aValue );
      }
        *pAscii = (int)aValue;
    }

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return aValue;
}

//
//
//  Decodes a string field from the passed command line
//  NOTES: Command line string fields are assumed to be a terminated
//         by a space character.  Alternatively strings may be
//         enclosed within quotes
//       : String fields are designated by the %s format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               char *pString
//               Decoded string
//
//               int xSize
//               Maximum size of the above string
//
//  Returns:     int
//               Number of decode characters
//
int
CStringParser::GetString  ( const TCHAR **ppFormat, TCHAR *pString, int xSize )
{
    // Introduce locals
    int   nChars  = 0;
    bool  bQuotes = false;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      while ( *m_pClpEnv -> m_pCommand )
      {
        // Quotes are special
        if ( *m_pClpEnv->m_pCommand == '"' )
        {
          m_pClpEnv -> m_pCommand++;
          if ( bQuotes )
            break;
          else
            bQuotes = true;
          continue;
        }

        if (   !bQuotes                                &&
             ( *m_pClpEnv->m_pCommand == _T(' ')  ||
               *m_pClpEnv->m_pCommand == _T(',')  ||
               *m_pClpEnv->m_pCommand == _T('\t')    )     )
          break;
/*      if ( *m_pClpEnv->m_pCommand ==        ' '     ||
             *m_pClpEnv->m_pCommand == *(*ppFormat+1) ||
             *m_pClpEnv->m_pCommand ==         0         )
          break;*/
        pString[nChars++] = *m_pClpEnv->m_pCommand++;
        if ( nChars >= xSize )
          MessageThrow ( _T("Attempted buffer overrun (<%i chars)"), xSize );
      }

      if (  nChars <= 0              &&
           !m_pClpEnv -> m_bOptional    )
        MessageThrow ( _T("Missing string in command line") );

      pString[nChars] = 0;
    }

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return nChars;
}

//
//
//  Decodes a string field from the passed command line
//  NOTES: Command line string fields are assumed to be a terminated
//         by a tab character.  Alternatively strings may be
//         enclosed within quotes
//       : Tabbed string fields are designated by the %T
//         format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               char *pString
//               Decoded string
//
//               int xSize
//               Maximum size of the above string
//
//  Returns:     int
//               Number of decode characters
//
int
CStringParser::GetStringTabbed  ( const TCHAR **ppFormat, TCHAR *pString, int xSize )
{
    // Introduce locals
    int   nChars  = 0;
    bool  bQuotes = false;

    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
    {
      while ( *m_pClpEnv -> m_pCommand )
      {
        // Quotes are special
        if ( *m_pClpEnv->m_pCommand == '"' )
        {
          m_pClpEnv -> m_pCommand++;
          if ( bQuotes )
            break;
          else
            bQuotes = true;
          continue;
        }

        if (   !bQuotes                                &&
             ( *m_pClpEnv->m_pCommand == _T(',')  ||
               *m_pClpEnv->m_pCommand == _T('\t')    )     )
          break;
/*      if ( *m_pClpEnv->m_pCommand ==        ' '     ||
             *m_pClpEnv->m_pCommand == *(*ppFormat+1) ||
             *m_pClpEnv->m_pCommand ==         0         )
          break;*/
        pString[nChars++] = *m_pClpEnv->m_pCommand++;
        if ( nChars >= xSize )
          MessageThrow ( L"Attempted buffer overrun (<%i chars)", xSize );
      }

      if (  nChars <= 0              &&
           !m_pClpEnv -> m_bOptional    )
        MessageThrow ( L"Missing string in command line" );

      pString[nChars] = 0;
    }

    // Strip trailing spaces
    while ( nChars > 0 && pString[--nChars] == L' ' )
      pString[nChars] = 0;

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return nChars;
}

//
//
//  Description: Decodes a character field from the passed
//               command line
//               NOTES: Command line character fields are assumed to
//                      be any non-null character.
//                    : Character fields are designated by the %c
//                      format sequence.
//
//
//  Parameters:  char **ppFormat
//               Command line parsing format definition
//
//               char *pChar
//               Decoded value
//
TCHAR
CStringParser::GetChar ( const TCHAR **ppFormat, TCHAR *pChar )
{
    // Observe current skip mode
    if ( !m_pClpEnv -> m_bSkip )
      *pChar = *m_pClpEnv -> m_pCommand++;

    // Tidy up and
    *ppFormat = *ppFormat + 1;
    return 0;
}

///////////////////////////////////////////////////////////////////////
//  Property management

//
//
//  Description: Summarizes data remaining in the parser
//
//
//  Returns:     bool
//                 true... Data remains
//                 false.. Empty
//
bool
CStringParser::IsEmpty ( )
{
    // Simply
    return *m_pClpEnv->m_pCommand ? false : true;
}

//
//  Description: Exposes loaded command string
//
//
//  Returns:     char*
//               Loaded command string
//
TCHAR*
CStringParser::Command ()
{
    // Simply
    return m_pClpEnv->m_pCommand;
}

void MessageThrow ( LPCTSTR lpszFormat, ... )
{
    // Encode request
    va_list    args;
    CString    strMsg;
    va_start ( args, lpszFormat);
    strMsg.FormatV ( lpszFormat, args);
    va_end( args);
    // Perform
    EVERR->Message(strMsg)->Throw();
}

///////////////////////////////////////////////////////////////////////
//  Standard extractors using CLParser above

//
//  Parses URL for domain component
//  FORMAT: [http][s][//][www.]domain[.whatever]
//
CString
CLParser_ExtractDomain ( LPCTSTR lpszURL )
{
    CStringParser oParser ( lpszURL );
    CString     strDomain;
    CString     strTest;

    // Scan to doamin name
    try
    {
      oParser.Scanf ( L"[http][s][:][//][www][.]$C");
      strDomain = oParser.Command();
      //while ( !oParser.IsEmpty() &&
      //        (oParser[0] != L':' && oParser[0] != '/' && oParser[0] != '.') )
      //{
      //  strDomain += oParser.Consume();
      //}
      // Strip trailing .com
      int nTruncate = strDomain.Find ( L".com", 0 );
      if ( nTruncate > 2 )
        strDomain.Truncate ( nTruncate );
      // Strip trailing .org
      nTruncate = strDomain.Find ( L".org", 0 );
      if ( nTruncate > 2 )
        strDomain.Truncate ( nTruncate );
      // Strip trailing .net
      nTruncate = strDomain.Find ( L".net", 0 );
      if ( nTruncate > 2 )
        strDomain.Truncate ( nTruncate );
      // Strip trailing .gov
      nTruncate = strDomain.Find ( L".gov", 0 );
      if ( nTruncate > 2 )
        strDomain.Truncate ( nTruncate );

      // Yahoo
      CString strDOMAIN = strDomain; strDOMAIN.MakeUpper();
      if ( strDOMAIN.Find(L"YAHOO") >= 0 )
        strDomain = L"Yahoo";
      else if ( strDOMAIN.Find(L"WIKIPEDIA") >= 0 )
        strDomain = L"Wikipedia";
      else if ( strDOMAIN.Find(L"SEARCH.ASX") >= 0 )
        strDomain = L"ASX";
      else if ( strDOMAIN.Find(L"HOTCOPPER") >= 0 )
        strDomain = L"Hotcopper";
      else if ( strDOMAIN.Find(L"FRED") >= 0 )
        strDomain = L"Fred";
      else if ( strDOMAIN.Find(L"MARKETWATCH") >= 0 )
        strDomain = L"M'Watch";
    }
    catch (...)
    {
      strDomain.Empty();
    }
    return strDomain;
}


///////////////////////////////////////////////////////////////////////


//
//
//  Parses an integer out of the passed command line or variable argument list.
//  Format: [delimiter]nnnnn
//          [delimiter]%i
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//
//               const char *pFormat
//               Formatted command from which integer is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     int
//               Parsed integer value
//
int
VapInteger ( char cDelimiter, const char *pFormat
           , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    int     iInteger;

    // Skip leading spaces
    while ( *pFormat    == ' ' &&
             cDelimiter != ' '    )
      pFormat++;

    // Confirm delimiter
    if ( cDelimiter )
    {
      MessageThrow ( _T("Missing %c delimiter"), cDelimiter );
      pFormat++;
    }

    // Integer assumed to exist in variable argument list
    if ( pFormat[0] == '%' )
    {
      if ( pFormat[1] != 'i' )
        MessageThrow ( _T("Expected 'i' field specifier") );
      if ( !pap )
        MessageThrow ( _T("Null Integer field") );

      iInteger  = va_arg ( *pap, int );
       pFormat += 2;
     *ppFormat  = pFormat;
    }

    // Integer is assumed to be inline
    else
    {
      iInteger = (int)strtol ( pFormat, (char **)ppFormat, 10 );
      if ( *ppFormat == pFormat )
        MessageThrow ( _T("Null Integer field parsed") );
    }

    // Tidy up and
    return iInteger;
}

//
//
//  Parses an integer out of the passed command line or variable argument list.
//  Format: [delimiter]c
//          [delimiter]%a
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//
//               const char *pFormat
//               Formatted command from which integer is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     int
//               Parsed integer value
//
int
VapAscii ( char cDelimiter, const char *pFormat
         , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    int     iInteger = 0;

    // Skip leading spaces
    while ( *pFormat == ' ' )
      pFormat++;

    // Confirm delimiter
    if ( cDelimiter )
    {
      if ( *pFormat != cDelimiter    )
        MessageThrow ( L"Missing %c delimiter", cDelimiter );
      pFormat++;
    }

    // Integer assumed to exist in variable argument list
    if ( pFormat[0] == '%' )
    {
      if ( !pap )
        MessageThrow ( L"Null Integer field" );

      if ( pFormat[1] == 'i' )
      {
         iInteger = va_arg ( *pap, int );
         pFormat += 2;
       *ppFormat  = pFormat;
      }
      
      else if ( pFormat[1] == 'a' )
      {
         iInteger  = va_arg ( *pap, char );
         if ( iInteger == '*' )
           iInteger = WILDCARD;
         pFormat  += 2;
       *ppFormat   = pFormat;
      }

      else
        MessageThrow ( L"Expected 'i|a' field specifier" );
    }

    // Integer is assumed to be inline
    else
    {
      if ( isdigit(*pFormat) )
      {
        iInteger = (int)strtol ( pFormat, (char **)ppFormat, 10 );
        if ( *ppFormat == pFormat )
          MessageThrow ( L"Null Integer field parsed" );
      }
      else
      {
        iInteger = *pFormat++;
        if ( iInteger == '*' )
          iInteger = WILDCARD;
       *ppFormat = pFormat;
      }
    }

    // Tidy up and
    return iInteger;
}

//
//
//  Parses a short out of the passed command line or variable argument list.
//  Format: (delimiter)nnnnn
//          (delimiter)%i
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//
//               const char *pFormat
//               Formatted command from which short is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     int
//               Parsed short value
//
short
VapShort ( char cDelimiter, const char *pFormat
         , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    short        iInteger;

    // Skip leading spaces
    while ( *pFormat == ' ' )
      pFormat++;

    // Confirm delimiter
    if (  cDelimiter )
    {
      if ( *pFormat != cDelimiter )
        MessageThrow ( _T("Missing %c delimiter"), cDelimiter );
      pFormat++;
    }

    // Integer assumed to exist in variable argument list
    if ( pFormat[0] == '%' )
    {
      if ( pFormat[1] != 'i' )
        MessageThrow ( _T("Expected 'i' field specifier") );
      if ( !pap )
        MessageThrow ( _T("Null short field") );

      iInteger  = (short)va_arg ( *pap, int );
       pFormat += 2;
     *ppFormat  = pFormat;
    }

    // Integer is assumed to be inline
    else
    {
      iInteger = (short)strtol ( pFormat, (char **)ppFormat, 10 );
      if ( *ppFormat == pFormat )
        MessageThrow ( _T("Null short field parsed") );
    }

    // Validation
    if ( iInteger < SHRT_MIN ||
         iInteger > SHRT_MAX    )
      MessageThrow ( _T("%i out of absolute range (%i to %i)")
                    , iInteger
                    , SHRT_MIN
                    , SHRT_MAX );

    // Tidy up and
    return iInteger;
}

//
//
//  Parses a character out of the passed command line or variable argument list.
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//               NOTES: Variable argument is assumed when this
//                      delimiter is found not to exist.
//
//               const char *pFormat
//               Formatted command from which the character is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     int
//               Parsed integer value
//
char
VapCharacter ( char cDelimiter, const char *pFormat
             , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    char       cChar = 0;

    // Skip leading spaces
    while ( *pFormat == ' ' )
      pFormat++;

    // Delimiter exists, then integer is assumed to be inline
    if ( *pFormat == cDelimiter )
    {
      pFormat++;
      cChar = *(pFormat++);
      if ( !cChar )
        MessageThrow ( L"Null character field parsed" );
    }

    // Integer assumed to exist in variable argument list
    else if ( pap )
      cChar = va_arg ( *pap, char );

    // Failure
    else
      MessageThrow ( L"Null integer field" );

    // Tidy up and
    *ppFormat = pFormat;
    return cChar;
}

//
//
//  Parses string out of the passed command line or variable argument list.
//  Format: [delimiter]sssssss
//          [delimiter]%s
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//               NOTES: Variable argument is assumed when this
//                      delimiter is found not to exist.
//
//               const char *pFormat
//               Formatted command from which string is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     CString
//               Parsed string
//
CString
VapString ( char cDelimiter, const char *pFormat
          , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    CString oCString;
    int     xSize = 255;

    // Skip leading spaces
    while ( *pFormat == ' ' )
      pFormat++;

    // Confirm delimiter
    if ( cDelimiter )
    {
      if ( *pFormat != cDelimiter    )
        MessageThrow ( _T("Missing %c delimiter"), cDelimiter );
      pFormat++;
    }

    // String assumed to exist in variable argument list
    if ( pFormat[0] == '%' )
    {
      if ( pFormat[1] != 's' )
        MessageThrow ( _T("Expected 's' field specifier") );
      if ( !pap )
        MessageThrow ( _T("Null String field") );

      oCString = (char *)va_arg ( *pap, char * );
      pFormat += 2;
    }

    // String is assumed to be inline
    else
    {
    /*while ( *pFormat )
        oCString += *pFormat++;
      if ( oCString.GetLength() <= 0 )
        EVERR->MODULE
             ->Message("Null string field parsed" )
             ->Group("VAP")->Throw();DELETE-ME Previous version*/
      bool bQuotes = false;
      while ( *pFormat )
      {
        // Quotes are special
        if ( *pFormat == '"' )
        {
          pFormat++;
          if ( bQuotes )
            break;
          else
            bQuotes = true;
          continue;
        }

        if ( !bQuotes        &&
             *pFormat == ' '    )
          break;
        oCString += *pFormat++;
        if ( oCString.GetLength() >= xSize )
          MessageThrow ( L"Attempted buffer overrun (<%i chars)", xSize );
      }
    }

    // Tidy up and
    *ppFormat = pFormat;
    return oCString;
}

//
//
//  Parses a double out of the passed command line or variable argument list.
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//               NOTES: Variable argument is assumed when this
//                      delimiter is found not to exist.
//
//               const char *pFormat
//               Formatted command from which double is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     double
//               Parsed double value
//
double
VapDouble ( char cDelimiter, const char *pFormat
          , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    double       dDouble = 0;

    // Skip leading spaces
    while ( *pFormat == ' ' )
      pFormat++;

    // Delimiter exists, then integer is assumed to be inline
    if ( *pFormat == cDelimiter )
    {
      dDouble = strtod ( ++pFormat, (char **)ppFormat );
      if ( *ppFormat == pFormat )
        MessageThrow ( L"Null double field parsed" );
    }

    // Integer assumed to exist in variable argument list
    else if ( pap )
      dDouble = va_arg ( *pap, double );

    // Failure
    else
      MessageThrow ( L"Null double field" );

    // Tidy up and
    return dDouble;
}

//
//
//  Parses DWORD out of the passed command line or variable argument list.
//  Format: [delimiter]nnnnn
//          [delimiter]%i
//
//
//  Parameters:  char cDelimiter
//               Leading argument delimiter.
//
//               const char *pFormat
//               Formatted command from which DWORD is to be
//               optionally parsed.
//
//               const char **ppFormat
//               Consumed position after parsing in pFormat above
//
//               va_list *pap
//               Pointer to current variable argument list position
//
//
//  Returns:     int
//               Parsed integer value
//
DWORD
VapDWORD ( char cDelimiter, const char *pFormat
         , const char **ppFormat, va_list *pap )
{
    // Introduce locals
    int     dwDWORD;

    // Skip leading spaces
    while ( *pFormat == ' ' )
      pFormat++;

    // Confirm delimiter
    if ( cDelimiter )
    {
      if ( *pFormat != cDelimiter    )
        MessageThrow ( _T("Missing %c delimiter"), cDelimiter );
      pFormat++;
    }

    // DWORD assumed to exist in variable argument list
    if ( pFormat[0] == '%' )
    {
      if ( pFormat[1] != 'i' )
        MessageThrow ( L"Expected 'i' field specifier" );
      if ( !pap )
        MessageThrow ( L"Null DWORD field" );

      dwDWORD   = va_arg ( *pap, DWORD );
       pFormat += 2;
     *ppFormat  = pFormat;
    }

    // DWORD is assumed to be inline
    else
    {
      dwDWORD = (int)strtoul ( pFormat, (char **)ppFormat, 10 );
      if ( *ppFormat == pFormat )
        MessageThrow ( _T("Null DWORD field parsed") );
    }

    // Tidy up and
    return dwDWORD;
}

///////////////////////////////////////////////////////////////////////

//
//  CStringParser environment manager
//  NOTES: Pushes and pops environment as parsing progresses
ClpEnv::ClpEnv ()
{
    // Firstly
    RenderEnvSafe();
}

ClpEnv::ClpEnv ( TCHAR *pCommand )
{
    // Firstly
    RenderEnvSafe();
    m_pCommand    = pCommand;
//    m_pCompleted  = pCommand;
    m_pCompleted  = pCommand;
    m_pCheckpoint = pCommand;
}

ClpEnv::~ClpEnv ()
{
    // Garbage collection
    delete m_pClpEnvPrev;
    m_pClpEnvPrev = 0;
}

void
ClpEnv::RenderEnvSafe ( )
{
    // Attributes
    m_nLevel      =  0;
    m_bSkip       = false;
    m_bOptional   = false;
    m_eFCF        =  0;
    m_ePtr        =  0;
    m_eSeqNum     = -1;
    m_pCommand    =  0;
//    m_pCompleted  =  0;
    m_pCompleted  =  0;
    m_pCheckpoint =  0;
    m_pFormat     =  0;

    m_pClpEnvPrev =  0;
}

//
//  Pushes and returns pointer to new parsing environment.
//  NOTES: Complimentary method is Pop().
//
//  Parameters:  char *pCheckpoint
//               Command decode checkpoint position.
//                 0.. Carry forward from previous level
//
//               char *pCompleted
//               Command decode completed position.
//                 0.. Carry forward from previous level
//
//               char *pCommand
//               Command decode current position
//                 0.. Carry forward from previous level
//
//  Returns:     int
//               New environment level
//
ClpEnv*
ClpEnv::Push ( TCHAR *pCheckpoint, TCHAR *pCompleted, TCHAR *pCommand )
{
    // Introduce locals
    ClpEnvSP spClpEnv = new ClpEnv ( );

    // Attribute transfer
    spClpEnv -> m_nLevel      = m_nLevel + 1;
    spClpEnv -> m_bSkip       = m_bSkip;
    spClpEnv -> m_bOptional   = m_bOptional;
    spClpEnv -> m_eFCF        = m_eFCF;
    spClpEnv -> m_ePtr        = m_ePtr;
    spClpEnv -> m_eSeqNum     = m_eSeqNum;
    spClpEnv -> m_pCommand    = m_pCommand;
    spClpEnv -> m_pCompleted  = m_pCompleted;
    spClpEnv -> m_pCheckpoint = m_pCheckpoint;
    spClpEnv -> m_pFormat     = m_pFormat;

    spClpEnv -> m_pClpEnvPrev = this;

    // Initialise new level
    // NOTES: Status of previous level is carried forward
    // Manage command decode positions
    if ( pCheckpoint )
      spClpEnv -> m_pCheckpoint = pCheckpoint;

    if ( pCompleted )
      spClpEnv -> m_pCompleted  = pCompleted;

    if ( pCommand )
      spClpEnv -> m_pCommand    = pCommand;

    // Tidy up and
    return spClpEnv.Dereference();
}

//
//  Pops and returns pointer to previous parsing environment.
//  NOTES: Complimentary method is Push().
//
//  Parameters:  char *pCheckpoint
//               Command decode checkpoint position.
//                 0.. Retain previous value
//
//               char *pCompleted
//               Command decode completed position.
//                 0.. Retain previous value
//
//               char *pCommand
//               Command decode current position
//                 0.. Retain previous value
//
//  Returns:     int
//               New environment level
//
ClpEnv*
ClpEnv::Pop ( TCHAR *pCheckpoint, TCHAR *pCompleted, TCHAR *pCommand )
{
    // Introduce locals
    ClpEnv   *pClpEnvPrev = m_pClpEnvPrev;
//    ClpEnvSP spClpEnv     = this;
    m_pClpEnvPrev         = 0;
    
    // Manage command decode position on previous level
    if ( pCheckpoint )
      pClpEnvPrev -> m_pCheckpoint = pCheckpoint;

    if ( pCompleted )
      pClpEnvPrev -> m_pCompleted  = pCompleted;

    if ( pCommand )
      pClpEnvPrev -> m_pCommand    = pCommand;

    // Tidy up and
    delete this;
    return pClpEnvPrev;
}


