// Copyright © 2013, 2018, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CWnd extensions
//

#include "StdAfx.h"
#include "CWndApp_Ext.h"
#include "CString_Ext.h"
#define   SECURITY_WIN32
#include "security.h"
#include "Msgexception.h"

//
//  Run message pump to completion
//  NOTES: Periodically we need to run the message pump to completion to 
//         implement transitory actions locked in the windows message queue.
//
DWORD_PTR
g_dwRunMsgPump2Completion = 1;
MsgcoreMFC_EXT void
RunMsgPump2Completion ( ) noexcept
{
    // Run the message loop. It will run until GetMessage() returns 0
    MSG messages;
    while ( g_dwRunMsgPump2Completion                     &&
            PeekMessage(&messages,NULL,0,0,PM_REMOVE) > 0    )
    {
      // Translate virtual-key messages into character messages
      TranslateMessage ( &messages );
      // Send message to WindowProcedure
      DispatchMessage ( &messages );
    }
}

//
//  CWndApp application file helpers
//  NOTES: Static functions are just simpler
//
//  Parameters:  LPCTSTR lpszDataFileName
//               Folder/file path to be appended to FOLDERID_LocalAppData
//
//  Returns:     CString
//               Constructed FOLDERID_LocalAppData file path
MsgcoreMFC_EXT CString
CWinApp_LocalAppDataFilePath( LPCTSTR lpszDataFileName )
{
    USES_CONVERSION;

    WCHAR *pwszPathname;
    SHGetKnownFolderPath ( FOLDERID_LocalAppData, 0, NULL, &pwszPathname);
    CString strLocalAppDataFile((W2A(pwszPathname)));
    CoTaskMemFree(pwszPathname);
    strLocalAppDataFile += _T("\\");
    strLocalAppDataFile += lpszDataFileName;
    return strLocalAppDataFile;
}

//
//  CWndApp application file helpers
//  NOTES: Static functions are just simpler
//
//  Parameters:  LPCTSTR lpszDataFileName
//               Folder/file path to be appended to FOLDERID_ProgramData
//
//  Returns:     CString
//               Constructed FOLDERID_ProgramData file path
MsgcoreMFC_EXT CString
CWinApp_ProgramDataFilePath( LPCTSTR lpszDataFileName )
{
    USES_CONVERSION;

    WCHAR *pwszPathname;
    SHGetKnownFolderPath ( FOLDERID_ProgramData, 0, NULL, &pwszPathname);
    CString strProgramDataFile((W2A(pwszPathname)));
    CoTaskMemFree(pwszPathname);
    strProgramDataFile += _T("\\");
    strProgramDataFile += lpszDataFileName;
    return strProgramDataFile;
}

//
//  CWndApp application file helpers
//  NOTES: Static functions are just simpler
//
//  Returns:     CString
//               Path to folder from which module is executing
CString
CWinApp_ModuleFolderPath ( )
{
    USES_CONVERSION;
    // Build name of sample workspace
    // NOTES: Relative to installation directory
    TCHAR tcsModulePathname[_MAX_PATH+1];
    DWORD  dwSize = GetModuleFileName ( NULL, tcsModulePathname, ARRAYSIZE(tcsModulePathname) );
    if ( dwSize < 0 )
      EVERR->MODULE
           ->Message ( L"GetModuleFileName() failed" )
           ->HResult(0)
           ->Throw ( );
#ifdef _DEBUG                          // Because in debug we don't run from install directory
#ifdef _WIN64
    dwSize = swprintf_s ( &tcsModulePathname[0], ARRAYSIZE(tcsModulePathname)
                        , L"C:\\Program Files\\Chartboard\\bin\\Chartboard.exe" );
#else
    dwSize = swprintf_s ( &tcsModulePathname[0], ARRAYSIZE(tcsModulePathname)
                          , L"C:\\Program Files (x86)\\Chartboard\\bin\\Chartboard.exe");
#endif
#endif
    TCHAR tcsDrive    [_MAX_DRIVE];
    TCHAR tcsDirectory[_MAX_DIR];
    TCHAR tcsFilename [_MAX_FNAME];
    TCHAR tcsExtension[_MAX_EXT];
   _tsplitpath_s( tcsModulePathname
                , tcsDrive, tcsDirectory, tcsFilename, tcsExtension );
    CString strModuleFolderPath;
            strModuleFolderPath  = tcsDrive;
            strModuleFolderPath += tcsDirectory;

    // Tidy up, and
    return strModuleFolderPath;
}

//
//  Fetches active CView for CWndApp application
//  NOTES: Static functions are just simpler
//
MsgcoreMFC_EXT CView*
CWndApp_GetActiveView ( )
{
    CMDIFrameWnd *pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
    // Get the active MDI child window.
    CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
    // or CMDIChildWnd *pChild = pFrame->MDIGetActive();
    // Get the active view attached to the active MDI child window.
    return pChild->GetActiveView();
}

//
//  Post URL message to CWinApp
//  NOTES: WM_PostHtmlURL setting requirement
//
//  Parameters:    LPCTSTR lpszURL
//                 URL to be activated
//
//                 BOOL bExternalOverride
//                 External override flag.
MsgcoreMFC_EXT
UINT g_uiWM_PostHtmlURL = 0;           // PostHtmlURL notification message
MsgcoreMFC_EXT BOOL
CWndApp_PostHtmlURL ( LPCTSTR lpszURL, BOOL bExternalOverride )
{
    ASSERT(g_uiWM_PostHtmlURL);
    if ( g_uiWM_PostHtmlURL )
    {
      CString *pstrURL = new CString ( lpszURL );
      return AfxGetApp()->PostThreadMessage ( g_uiWM_PostHtmlURL, (WPARAM)pstrURL, (LPARAM)bExternalOverride );
    }
    return FALSE;
}

//
//  Retreives username under which application is running.
//  NOTES: Not suitable for use from a Windows Service
//
//  Returns:       CString
//                 Username under which application is running
//
CString
CWndApp_GetUsername ( )
{
    TCHAR szUsername[UNLEN+1];
    DWORD szUsernameSize = ARRAYSIZE(szUsername);
    GetUserNameEx ( NameSamCompatible, &szUsername[0], &szUsernameSize );
    CString strUsername = &szUsername[0];
    int nIdx = strUsername.ReverseFind ( L'\\' );
    if ( nIdx >= 0 )
      strUsername = strUsername.Right ( strUsername.GetLength() - nIdx - 1 );
    return strUsername;
}

