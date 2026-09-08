// Copyright © 2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CFont extension prototypes and definitions
//

#include "StdAfx.h"
#include "CRect_Ext.h"

//
//  Checks for CRect intersection
//  NOTES: Static functions are just simpler
//
//  Parameters:  const CRect1
//               First normalised rectangle
//
//               const CRect& oCRect2
//               Second normalised rectangle
//
//  Returns:     BOOL
//               Intersection result
BOOL
CRect_Intersect ( const CRect& oCRect1, const CRect& oCRect2 )
{
    return !( oCRect2.left   > oCRect1.right  || 
              oCRect2.right  < oCRect1.left   || 
              oCRect2.top    > oCRect1.bottom ||
              oCRect2.bottom < oCRect1.top       );
}

