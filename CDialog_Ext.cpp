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
//  Implementation for CDialog extensions
//

#include "StdAfx.h"

#include "CDialog_Ext.h"

//
//  Enables or disables all CDialog controls
//
//  Parameters:  CDialog oCDialog
//               Dialog for which all controls are to be disabled
//
//  Returns:     BOOL
//               Result
BOOL CALLBACK
EoDcontrol ( HWND hwnd, LPARAM lParam) noexcept
{
    EnableWindow ( hwnd, static_cast<BOOL>(lParam) );
    return TRUE;
}
BOOL
CDialog_EnableAllControls ( CDialog *pDialog ) noexcept
{
    return EnumChildWindows ( pDialog->m_hWnd, EoDcontrol, TRUE );
}
BOOL
CDialog_DisableAllControls ( CDialog *pDialog ) noexcept
{
    return EnumChildWindows ( pDialog->m_hWnd, EoDcontrol, FALSE );
}

//
//  Sets CDialog Icon (top left corner) to application icon
//
//  Parameters:  CDialog& oCDialog 
//
//  Returns:     HICON
//               Handle for set icon
HICON
CDialog_SetIconFromApp ( CDialog& oDialog ) noexcept
{
    // Establish icon
    // NOTES: Extreme left side of title bar
    HICON hIcon = AfxGetApp()->LoadIcon(128);
    oDialog.SetIcon ( hIcon, TRUE );
    oDialog.SetIcon ( hIcon, FALSE );
    return hIcon;
}
