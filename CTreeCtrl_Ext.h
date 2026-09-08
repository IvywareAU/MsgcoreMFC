// Copyright © 2002, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CTreeCtrl static function extensions
//

#pragma   once
#include "MsgcoreMFC.h"

//  Callback functions for movement of item data
typedef LPARAM(*fptrCTreeCtrl_MoveItem)(CTreeCtrl* pTreeCtrl, HTREEITEM hItemSource, HTREEITEM hItemDest);
typedef LPARAM(*PFNMTICOPYDATA)(const CTreeCtrl& oTreeCtrl, HTREEITEM, LPARAM);

//
//  CTreeCtrl movement helpers
//  NOTES: Static functions are just simpler
MsgcoreMFC_EXT HTREEITEM
CTreeCtrl_MoveItem_ ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem, HTREEITEM hItemTo 
		               , BOOL bCopyOnly = FALSE, PFNMTICOPYDATA pfnCopyData = NULL
		               , HTREEITEM hItemPos = TVI_LAST );
MsgcoreMFC_EXT HTREEITEM
CTreeCtrl_MoveItem ( CTreeCtrl *pCTreeCtrl
                   , HTREEITEM hItem, HTREEITEM hItemParent, HTREEITEM hItemAfter
                   , fptrCTreeCtrl_MoveItem fptrMoveItem = 0 );

//
//  Expansion and contraction of items
//  NOTES: Asynchronous operations suitable fo use from TVN handlers
MsgcoreMFC_EXT void
CTreeCtrl_AsyncExpand ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem );
MsgcoreMFC_EXT void
CTreeCtrl_AsyncCollapse ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem );

//
//  CTreeCtrl navigation helpers
MsgcoreMFC_EXT HTREEITEM
CTreeCtrl_FindItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItemParent, LPCTSTR lpszItemName );

//
//  CTreeCtrl selection helpers
MsgcoreMFC_EXT void
CTreeCtrl_DeselectItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem );

//
//  Bold state management
MsgcoreMFC_EXT HTREEITEM
CTreeCtrl_BoldItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItemBold, HTREEITEM hItemUnbold = 0 );

class MsgcoreMFC_EXT SafeBoldItem
{
    public:
      SafeBoldItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItemBold, HTREEITEM hItemUnbold = 0 );
     ~SafeBoldItem ( );
    private:
      CTreeCtrl& m_oCTreeCtrl;
      HTREEITEM  m_hItemBold;
      HTREEITEM  m_hItemRestore;
};
