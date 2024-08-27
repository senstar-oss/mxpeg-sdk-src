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
//  $LastChangedDate: 2007-06-29 15:34:53 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmList.h $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licenced under the BSD licence,
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



#ifndef __MXM_LIST_H__
#define __MXM_LIST_H__



#include <mxm/core/mxmContainer.h>
#include <mxm/core/mxmMutableBidirectionalIteratorInterface.h>



//! Doubly-linked list container.
/*!
 *  \ingroup modularMX_core
 * 
 *  We introduced this class as replacement for the respective memory management
 *  facilitiy when we abandoned <tt>libstdc++</tt>.
 *
 *  Have a look at what you can do to lists:
<pre>
  mxmList list(new mxmStringDecider());        // only allow mxmString s
  mxmString *da_string;                        //   as items
  char da_txt[100];
  int i;
  
  for(i = 0; i < 100; i++) {
    std::sprintf(da_txt, "TestText #%d", i);
    da_string = new mxmString(da_txt);
    list.addItem(da_string);                   // fill the list
  }
  
  std::printf("list size=%d\n", list.size());  // get # of items
  
  mxmList::Iterator iter = list.iterator();    // iterate over all items
  while(da_string = static_cast<mxmString *>(iter.nextItem())) {
    std::printf("Item : %s\n", da_string->text());
  }
  
  list.clear();                                // delete all items
</pre>
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmList : public mxmContainer {
 
 private:
  struct Node {
    struct Node *Prev;
    struct Node *Next;
    mxmObject *Item;
  };
 //! Iterator implementation for mxmList s.
 public:
  class MXM_CORE_API Iterator
                      : public mxmObject,
                        public mxmMutableBidirectionalIteratorInterface {
   private:
    mxmList *List;
    struct Node *CurrentNode;
    bool IterationCompleted;
    void *StableABIDataExtension;
   public:
    Iterator(const mxmList *list);
    Iterator();
   public:
    //! (Re)implemented.
    void resetIterator(void);
    //! (Re)implemented.
    mxmObject *nextItem(void);
    //! (Re)implemented.
    mxmObject *previousItem(void);
    //! (Re)implemented.
    mxmObject *removeCurrentItem(void);
  };
 
 private:
  friend class Iterator;
  
 private:
  struct Node Anchor;
  int ItemNum;
  void *StableABIDataExtension;
  
 public:
  //! Default Constructor.
  mxmList();
  //! Pass to this constructor an appropriate type decider if you wish to
  //! restrict the set of possible mxmObject types that can be added to this
  //! list.
  mxmList(mxmTypeDecider *item_type_decider);
 private:
  void init_mxmList(void);
 public:
  //! The list owns its items - all list items get destroyed along the way also.
  ~mxmList();
  
 public:
  //! Provides the calling context with an external iterator.
  mxmList::Iterator iterator(void) const;
  //! Adds the specified item to the end of the list.
  virtual void appendItemToTail(mxmObject *item);
  //! Adds the specified item to the head of the list.
  void prependItemToHead(mxmObject *item);
  //! Removes and returns the first item in the list.
  mxmObject *removeItemFromHead(void);
 
  //! (Re)implementation for interface mxmContainerInterface.
  void addItem(mxmObject *item);
  //! (Re)implementation for interface mxmContainerInterface.
  mxmObject *removeItem(void);
  //! (Re)implementation for interface mxmContainerInterface.
  int size(void) const;
};



#endif   // __MXM_LIST_H__

