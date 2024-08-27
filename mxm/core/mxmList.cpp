//           ///          //                                  C++ Cross Platform
//          /////        ////
//         /// XXX     XXX ///            ///////////   /////////     ///   ///
//        ///    XXX XXX    ///         ///             ///    ///   ///  ///
//       ///       XXX       ///         /////////     ///      //  //////
//      ///      XXX XXX      ///               ///   ///    ///   ///  ///
//     ////    XXX     XXX    ////    ////////////  //////////    ///   ///
//    ////                     ////
//   ////  M  O  B  O  T  I  X  ////////////////////////////////////////////////
//  //// Security Vision Systems //////////////////////////////////////////////
//
//  $Author: khe_admin $
//  $LastChangedBy: khe_admin $
//  $LastChangedDate: 2007-06-29 12:31:37 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmList.cpp $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licensed under the BSD licence,
     //  http://www.opensource.org/licenses/bsd-license.php
     //
     //  Copyright (c) 2005 - 2007, MOBOTIX AG
     //  All rights reserved.
     //
     //  Redistribution and use in source and binary forms, with or without
     //  modification, are permitted provided that the following conditions are
     //  met:
     //
     //  - Redistributions of source code must retain the above copyright
     //    notice, this list of conditions and the following disclaimer.
     //
     //  - Redistributions in binary form must reproduce the above copyright
     //    notice, this list of conditions and the following disclaimer in the
     //    documentation and/or other materials provided with the distribution.
     //
     //  - Neither the name of MOBOTIX AG nor the names of its contributors may
     //    be used to endorse or promote products derived from this software
     //    without specific prior written permission.
     //
     //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     //  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     //  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     //  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     //  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     //  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     //  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     //  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     //  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     //
///////



#include <mxm/core/mxmList.h>
#include <mxm/core/mxmTypeDecider.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>



mxmList::mxmList() {
  
  init_mxmList();
}


/*!
 *  The list assumes ownership over the type decider.
 */
mxmList::mxmList(mxmTypeDecider *item_type_decider)
          : mxmContainer(item_type_decider)         {
  
  init_mxmList();
}


void mxmList::init_mxmList(void) {
  
  Anchor.Prev        = &Anchor;
  Anchor.Next        = &Anchor;
  Anchor.Item        = 0;
  
  ItemNum            = 0;
}


mxmList::~mxmList() {
  
  clear();
}


mxmList::Iterator mxmList::iterator(void) const {
  
  return Iterator(this);
}


/*!
 *  The list assumes ownership over the item.
 *
 */
void mxmList::appendItemToTail(mxmObject *item) {
  
  struct Node *node, *prev;
  
  if(!item) mxm::terminal("can't add null-item!",
                          this);
  
  // check item type...
  if(   ItemTypeDecider
     && !ItemTypeDecider->isOfProperType(item))
   mxm::terminal("type mismatch while adding to list."
                 "mxmList");
  
  node = new Node();
  MXM_CHECK_NULL(node);
  
  prev = Anchor.Prev;
  
  // establish linkage...
  prev->Next  = node;
  node->Prev  = prev;
  node->Next  = &Anchor;
  Anchor.Prev = node;
  
  node->Item = item;
  
  ItemNum++;
}


/*!
 *  The list assumes ownership over the item.
 */
void mxmList::prependItemToHead(mxmObject *item) {
  
  struct Node *node, *next;
  
  if(!item) mxm::terminal("can't add null-item!",
                          this);
  
  // check item type...
  if(   ItemTypeDecider
     && !ItemTypeDecider->isOfProperType(item))
   mxm::terminal("type mismatch while adding to list."
                 "mxmList");
  
  node = new Node();
  MXM_CHECK_NULL(node);
  
  next = Anchor.Next;
  
  // establish linkage...
  Anchor.Next = node;
  node->Prev  = &Anchor;
  node->Next  = next;
  next->Prev  = node;
  
  node->Item = item;
  
  ItemNum++;
}


/*!
 *  Ownership over the item is transferred back to the calling context.
 *
 *  Returns <tt>0</tt> if the list is empty.
 *
 */
mxmObject *mxmList::removeItemFromHead(void) {
  
  struct Node *node,
              *prev, *next;
  mxmObject *item;
  
  node = Anchor.Next;
  if(node == &Anchor) return(0);
  
  prev = node->Prev;
  next = node->Next;
  
  prev->Next = next;
  next->Prev = prev;
  
  item = node->Item;
  
  delete node;
  
  ItemNum--;
  
  return(item);
}


/*!
 */
void mxmList::addItem(mxmObject *item) {
  
  appendItemToTail(item);
}


/*!
 * 
 */
mxmObject *mxmList::removeItem(void) {
  
  return(removeItemFromHead());
}


int mxmList::size(void) const {
  
  return(ItemNum);
}


mxmList::Iterator::Iterator(const mxmList *list) {
  
  List = (mxmList *)list;   // casting away the _const_ is bad, but hey! - we
                            //   know what we're doing, aren't we?
  resetIterator();
}


mxmList::Iterator::Iterator() {
  
  List = 0;   // iterator nun-functional!
}



void mxmList::Iterator::resetIterator(void) {
  
  if(!List) return;
  
  CurrentNode = &List->Anchor;
  IterationCompleted = false;
}


mxmObject *mxmList::Iterator::nextItem(void) {
  
  if(!List) return(0);
  
  if(IterationCompleted) return(0);
  
  if(CurrentNode->Next != &List->Anchor) {
    CurrentNode = CurrentNode->Next;
    return(CurrentNode->Item);
  }
  else {
    CurrentNode = &List->Anchor;
    IterationCompleted = true;
    return(0);
  }
}


mxmObject *mxmList::Iterator::previousItem(void) {
  
  if(!List) return(0);
  
  if(IterationCompleted) return(0);
  
  if(CurrentNode->Prev != &List->Anchor) {
    CurrentNode = CurrentNode->Prev;
    return(CurrentNode->Item);
  }
  else {
    CurrentNode = &List->Anchor;
    IterationCompleted = true;
    return(0);
  }
}


mxmObject *mxmList::Iterator::removeCurrentItem(void) {
  
  struct Node *prev,
              *next;
  mxmObject *item;
  
  if(!List) return(0);
  
  if(CurrentNode == &List->Anchor) return(0);
  
  prev = CurrentNode->Prev;
  next = CurrentNode->Next;
  item = CurrentNode->Item;
  
  prev->Next = next;
  next->Prev = prev;
  
  delete CurrentNode;
  CurrentNode = prev;
  
  List->ItemNum--;
  
  return(item);
}

