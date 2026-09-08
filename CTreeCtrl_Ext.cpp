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
//  Description: MfcExtensions
//

#include "StdAfx.h"

#include "CTreeCtrl_ext.h"

//
//  Moves CTreeCtrl and associated childern to alternative location
//
//
//  Parameters:  CTreeCtrl *pCTreeCtrl
//               Tree control containing item to be copied
//
//               BOOL bCopyOnly
//               Full window copy flag
//
//  Returns:     HTREEITEM
//               Moved item as a copy of hItem
HTREEITEM
CTreeCtrl_MoveItem_ ( CTreeCtrl& oTreeCtrl, HTREEITEM hItem, HTREEITEM hItemTo
		               , BOOL bCopyOnly, PFNMTICOPYDATA pfnCopyData
		               , HTREEITEM hItemPos )
{
    // To be sure, to be sure
    if ( hItem   == nullptr ||
         hItemTo == nullptr    )
      return nullptr;
    if ( hItem   == hItemTo                        ||
         hItemTo == oTreeCtrl.GetParentItem(hItem)    )
      return hItem;
    // Block descendant movement
    HTREEITEM hItemParent = hItemTo;
    while ( hItemParent != TVI_ROOT                                     &&
           (hItemParent = oTreeCtrl.GetParentItem(hItemParent)) != NULL    )
    {
      if ( hItemParent == hItem )
        return NULL;
    }

    // Recursively copy items to new location and optionally remove original
    CString sText = oTreeCtrl.GetItemText(hItem);
    TVINSERTSTRUCT tvis;
    tvis.item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM
	                 | TVIF_SELECTEDIMAGE | TVIF_STATE;
    tvis.item.hItem = hItem;
  
    // Negate expanded and selected states
    tvis.item.stateMask = (UINT)-1 & ~(TVIS_DROPHILITED | TVIS_EXPANDED
		                    | TVIS_EXPANDEDONCE | TVIS_EXPANDPARTIAL | TVIS_SELECTED);
    oTreeCtrl.GetItem(&tvis.item);
    tvis.hParent = hItemTo;
    tvis.hInsertAfter = hItemPos;

    // Data item not transferred for copy only
    if ( bCopyOnly           && 
         pfnCopyData != NULL     )
      tvis.item.lParam = pfnCopyData ( oTreeCtrl, hItem, tvis.item.lParam );
    HTREEITEM hItemNew = oTreeCtrl.InsertItem(&tvis);
    oTreeCtrl.SetItemText(hItemNew, sText);

    // Recursively move children under new item
    HTREEITEM hItemChild = oTreeCtrl.GetChildItem(hItem);
    while (hItemChild != NULL)
    {
      HTREEITEM hItemNextChild = oTreeCtrl.GetNextSiblingItem(hItemChild);
      CTreeCtrl_MoveItem_(oTreeCtrl, hItemChild, hItemNew, bCopyOnly, pfnCopyData);
      hItemChild = hItemNextChild;
    }

    // Tidy up reminants
    // NOTES: Negate data field prior to removal
    if ( !bCopyOnly )
    {
      oTreeCtrl.SetItemData ( hItem, 0 );
      oTreeCtrl.DeleteItem  ( hItem );
    } 
    return hItemNew;
}

HTREEITEM
CTreeCtrl_MoveItem ( CTreeCtrl *pCTreeCtrl
                   , HTREEITEM hItem, HTREEITEM hItemParent, HTREEITEM hItemAfter
                   , fptrCTreeCtrl_MoveItem fptrMoveItemCB )
{
    // To be sure, to be sure
    if ( hItem       == NULL ||
         hItemParent == NULL    )
      return NULL;
    //if ( hItem == hItemTo || hItemTo == tree.GetParentItem(hItem))
    //  return hItem;

    // Block movement to a descendant
    HTREEITEM hItemCheck = hItemParent;
    while ( hItemCheck != TVI_ROOT &&
           (hItemCheck = pCTreeCtrl->GetParentItem(hItemCheck)) != NULL)
    {
      if (hItemCheck == hItem)
        return NULL;
    }

    // copy items to new location, recursively, then delete old heirarchy
    // get text, and other info
    CString sText = pCTreeCtrl->GetItemText(hItem);
    TVINSERTSTRUCT tvis;
    tvis.item.mask  = TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM | 
	        TVIF_SELECTEDIMAGE | TVIF_STATE;
    tvis.item.hItem = hItem;
    // we don't want to copy selection/expanded state etc
    tvis.item.stateMask = (UINT)-1 & ~(TVIS_DROPHILITED | TVIS_EXPANDED | 
	  		TVIS_EXPANDEDONCE | TVIS_EXPANDPARTIAL | TVIS_SELECTED);
    pCTreeCtrl -> GetItem ( &tvis.item );
    tvis.hParent      = hItemParent;
    tvis.hInsertAfter = hItemAfter;
    // if we're only copying, then ask for new data
    //if (bCopyOnly && pfnCopyData != NULL)
    tvis.item.lParam   = 0;//pfnCopyData(pCTreeCtrl, hItem, tvis.item.lParam);
    HTREEITEM hItemNew = pCTreeCtrl->InsertItem(&tvis);
    pCTreeCtrl->SetItemText(hItemNew, sText);
    if ( fptrMoveItemCB )
      (*fptrMoveItemCB) ( pCTreeCtrl, hItem, hItemNew );

    // Now move children to under new item
    HTREEITEM hItemChild = pCTreeCtrl->GetChildItem(hItem);
    while ( hItemChild != NULL )
    {
      HTREEITEM hItemNextChild = pCTreeCtrl->GetNextSiblingItem(hItemChild);
      CTreeCtrl_MoveItem ( pCTreeCtrl, hItemChild, hItemNew, TVI_LAST 
                         , fptrMoveItemCB );
      hItemChild = hItemNextChild;
    }

    //if (! bCopyOnly)
    {
      // clear item data, so nothing tries to delete stuff based on lParam
      pCTreeCtrl->SetItemData(hItem, 0);
      // no (more) children, so we can safely delete top item
      pCTreeCtrl->DeleteItem(hItem);
    }

    // Tidy up, and
    return hItemNew;
}

//
//  Finds and selects child item by name
//
//  Parameters:  CTreeCtrl *pCTreeCtrl
//               Tree control containing item to be located
//
//               HTREEITEM hItemParent
//               Parent item from which child is to selected
//
//               LPCTSTR lpszItemName
//               Name of item to be selected
//
HTREEITEM
CTreeCtrl_FindItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItemParent, LPCTSTR lpszItemName)
{
    // Fetch first child item
	  HTREEITEM hChild = oCTreeCtrl.GetChildItem ( hItemParent );
    while ( hChild )
	  {
		  // Comparison
      CString strChildName = oCTreeCtrl.GetItemText ( hChild );
      if ( strChildName.CompareNoCase(lpszItemName) == 0 )
		    break;

		  // Fetch next sibling
		  hChild = oCTreeCtrl.GetNextSiblingItem(hChild);
	  } 

	  // Tidy up, and
    if ( hChild )
      oCTreeCtrl.SelectItem ( hChild );
	  return hChild;
}

//
//  CTreeCtrl selection helpers
MsgcoreMFC_EXT void
CTreeCtrl_DeselectItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem )
{
    if ( hItem == NULL )
      hItem = oCTreeCtrl.GetSelectedItem();
    if ( hItem ) {
      oCTreeCtrl.SetItemState ( hItem, ~(UINT)TVIS_SELECTED, TVIS_SELECTED );
      return;
    }
    //int i = 0;     // Defensive 
    //while ( (hItem=oCTreeCtrl.GetSelectedItem()) != NULL && i++ < 1000 )
    //  CTreeCtrl_DeselectItem ( oCTreeCtrl, hItem );
}

//
//  Bold state management
MsgcoreMFC_EXT HTREEITEM
CTreeCtrl_BoldItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItemBold, HTREEITEM hItemUnbold )
{
    // Unbold existing item
    if ( hItemUnbold )
      oCTreeCtrl.SetItem ( hItemUnbold, TVIF_STATE, NULL, 0, 0, 0, TVIS_BOLD, 0 );

    // Bold item
    if ( hItemBold )
      oCTreeCtrl.SetItem ( hItemBold, TVIF_STATE, NULL, 0, 0, TVIS_BOLD, TVIS_BOLD, 0 );

    return hItemBold;
}

//
//  Safe bolded item class
//  NOTES: Bolds item upon instanciation and restores previous state when popped from stack
SafeBoldItem::SafeBoldItem ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItemBold, HTREEITEM hItemRestore )
            : m_oCTreeCtrl(oCTreeCtrl)
{
    m_hItemBold    = hItemBold;
    m_hItemRestore = hItemRestore;
    CTreeCtrl_BoldItem ( oCTreeCtrl, hItemBold, hItemRestore );
}
SafeBoldItem::~SafeBoldItem ( )
{
    CTreeCtrl_BoldItem ( m_oCTreeCtrl, m_hItemRestore, m_hItemBold );
}

//
//  Asynchronously expends/contracts nominated item
//  NOTES: Suitable for use from within TVN_GETDISPINFO handlers
//
//  Parameters:  CTreeCtrl& oCTreeCtrl
// 
//               HTREEITEM hItem
//               Item to be expanded/contracted
//               
void
CTreeCtrl_AsyncExpand ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem )
{
    if ( hItem )
    {
      CRect rc;
      TVHITTESTINFO tvhti{};
      const UINT state = oCTreeCtrl.GetItemState ( hItem, TVIS_EXPANDED );
      if ( (state & TVIS_EXPANDED) == 0)
      {
        oCTreeCtrl.GetItemRect(hItem, &rc, TRUE);
        tvhti.pt = { rc.CenterPoint() };
        oCTreeCtrl.HitTest(&tvhti);
        ASSERT(tvhti.flags == TVHT_ONITEMLABEL);
        oCTreeCtrl.PostMessage ( WM_LBUTTONDOWN, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
        oCTreeCtrl.PostMessage ( WM_LBUTTONUP, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
        oCTreeCtrl.PostMessage ( WM_LBUTTONDBLCLK, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
        oCTreeCtrl.PostMessage ( WM_LBUTTONUP, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
      }
    }
}
void
CTreeCtrl_AsyncCollapse ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem )
{
    if ( hItem )
    {
      CRect rc;
      TVHITTESTINFO tvhti{};
      const UINT state = oCTreeCtrl.GetItemState ( hItem, TVIS_EXPANDED );
      if ( (state & TVIS_EXPANDED) != 0)
      {
        oCTreeCtrl.GetItemRect ( hItem, &rc, TRUE );
        tvhti.pt = { rc.CenterPoint() };
        oCTreeCtrl.HitTest ( &tvhti );
        ASSERT(tvhti.flags == TVHT_ONITEMLABEL);
        oCTreeCtrl.PostMessage ( WM_LBUTTONDOWN, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
        oCTreeCtrl.PostMessage ( WM_LBUTTONUP, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
        oCTreeCtrl.PostMessage ( WM_LBUTTONDBLCLK, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
        oCTreeCtrl.PostMessage ( WM_LBUTTONUP, 0, MAKELPARAM(tvhti.pt.x, tvhti.pt.y) );
      }
    }
}