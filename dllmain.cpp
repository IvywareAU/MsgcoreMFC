// Copyright © 2009-2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Defines the initialization routines for the DLL.
//  NOTES: Refer MsgcoreMFC.h for export and import of
//         DLL symbols
//       : Initialising MFC extension DLL's
//         http://msdn.microsoft.com/en-us/library/h5f7ck28.aspx
//       : Introducing AFX_MANAGE_STATE() produces following link error
//         error LNK2005: _DllMain@12 already defined in dllmain.obj
//         P2PresourceState class has been introduce to negate such

#include "stdafx.h"
#include "MsgcoreMFC.h"
#include <afxwin.h>                    // VS2012 Wizard extension
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//extern "C" { int _afxForceUSRDLL; } 
//HINSTANCE g_hMsgcoreMFC_DLL = 0;

static AFX_EXTENSION_MODULE NEAR MsgcoreMFC_DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);
  //g_hMsgcoreMFC_DLL = hInstance;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MsgcoreMFC.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if ( !AfxInitExtensionModule(MsgcoreMFC_DLL,hInstance) )
    {
      TRACE0("MsgcoreMFC.DLL AfxInitExtensionModule() failed");
			return 0;
    }

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(MsgcoreMFC_DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MsgcoreMFC.DLL Terminating!\n");
  
		// Terminate the library before destructors are called
		AfxTermExtensionModule(MsgcoreMFC_DLL);
	}
	return 1;   // ok
}


//
//  Manages MFC resource state for this MsgcoreMFC DLL
//  NOTES: Each extension DLL requires its own private implementation
//         based around the MsgcoreMFC_DLL equivalent external.
//       : Code ia duplicated for each MFC extension DLL.  Not unlike
//         the way Dllmain() is replicated
P2PresourceState::P2PresourceState()
{
    m_hRestore = AfxGetResourceHandle();
    if ( m_hRestore != MsgcoreMFC_DLL.hModule )
      AfxSetResourceHandle(MsgcoreMFC_DLL.hModule);
    else           // We are our own state
      m_hRestore = NULL;
}
P2PresourceState::~P2PresourceState()
{
    if ( m_hRestore )
      AfxSetResourceHandle(m_hRestore);
}
