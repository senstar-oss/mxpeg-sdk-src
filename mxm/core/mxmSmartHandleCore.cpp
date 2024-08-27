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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmSmartHandleCore.cpp $
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



#include <mxm/core/mxmLockInterface.h>
#include <mxm/core/mxmSmartHandleCore.h>
#include <mxm/core/mxm_generic_stuff.h>



mxmSmartHandleCore::mxmSmartHandleCore() {
  
  init();
}
void mxmSmartHandleCore::init(void) {
  
  Prev      = this;
  Next      = this;
  Lock      = 0;
  OwnedLock = 0;
  Reserved  = 0;
  
  Handlee = 0;
}


mxmSmartHandleCore::~mxmSmartHandleCore() {
  
  //reset();   // done by subclass destructor (also killed potential lock)
  
  if(   (Prev != this)
     || (Next != this)
     || Handlee
     || Lock)          {
    
    mxm::terminal("subclass implementation missing or incomplete.",
                  this);
        // subclass destructor must call reset(), otherwise we get problems with
        // properly activating the deleteHandlee() implementation
  }
  
  // no more handlee, no more lock, and we are alone
}



bool mxmSmartHandleCore::operator!(void) const {
  
  return(null());
}
bool mxmSmartHandleCore::null(void) const {
  
  if(Handlee)
   return(false);
  else
   return(true);
}


/*!
 *  This method can only be invoked on a smart handle that
 *  - does not yet have a lock (= is still unprotected)
 *  - is currently alone (= the only member of the handle group spawned by
 *    itself)
 *
 *  Failure to honor the above requirements is considered a programmer's
 *  screw-up, and is swiftly and decisively responded to with a call to
 *  mxm::terminal().
 *
 *  \param lock
 *  The lock object to be used for synchronization. Must be non-<tt>0</tt>.
 *
 *  \param assume_ownership
 *  If set to <tt>true</tt> the emerging handle group will assume ownership over
 *  the lock object. As a result, the lock will be automatically deleted at the
 *  time the last of the respective smart handles dies.
 */
void mxmSmartHandleCore::setLock(mxmLockInterface *lock,
                                 bool assume_ownership) {
  
  if(Lock)
   mxm::terminal("cannot set lock for already protected handle!",
                 this);
  if(this != Next)
   mxm::terminal("cannot set lock for handle that is not alone!",
                 this);
  if(!lock)
   mxm::terminal("attempt to set 0-lock for handle!",
                 this);
   
  Lock = lock;
  
  if(assume_ownership) OwnedLock = 1;
}


/*!
 *  Ownership over the handlee object is transferred back to the calling
 *  context.
 *
 *  \return
 *  <tt>false</tt> in case control over the handlee could not be released. This
 *  happens if there are other smart handles for the handlee object around. In
 *  this case everything is left unchanged. If on the other hand the handlee
 *  object could be properly released, the handle becomes a <tt>null</tt>-handle
 *  and the method returns <tt>true</tt>. Locking settings remain unchanged.
 */
bool mxmSmartHandleCore::releaseSmartlyHandledObject(void) {
  
  bool result;
  
  if(Lock) Lock->acquire();   // -----------------------------------------------
  
  if(this != Next) {
    
    result = false;                                 // we're not the only handle
  }
  else {
    
    Handlee = 0;
    
    result = true;
  }
  
  if(Lock) Lock->release();   // -----------------------------------------------
  
  return(result);
}


int mxmSmartHandleCore::groupSize() const {
  
  if(Lock) Lock->acquire();   // -----------------------------------------------
  
  int num                     = 1;
  mxmSmartHandleCore *current = this->Next;
  while(current != this) {
    
    num++;
    current = current->Next;
  }
  
  if(Lock) Lock->release();   // -----------------------------------------------

  return(num);
}


void mxmSmartHandleCore::reset() {
  
  mxmLockInterface *lock_to_release = 0,
                   *lock_to_delete  = 0;
  
  if(Lock) {
    
    Lock->acquire();   // ------------------------------------------------------
    lock_to_release = Lock;
  }
  
  if(Next != this) {
    
    // there are others, unlink...
    
    mxmSmartHandleCore *next = Next,
                       *prev = Prev;
    
    prev->Next = next;
    next->Prev = prev;
    
    Next = this;
    Prev = this;
  }
  else {
    
    // last man standing...
    
    if(Handlee)
     deleteHandlee();
    
    if(   Lock
       && (OwnedLock == 1))
     lock_to_delete = Lock;
  }
  
  Handlee   = 0;
  Lock      = 0;
  OwnedLock = 0;
  
  if(lock_to_release)
   lock_to_release->release();   // --------------------------------------------
  
  if(lock_to_delete)
   delete lock_to_delete;
}


/*!
 *  Must only be invoked on a handle in default state: <tt>null</tt>-handle
 *  (=> alone), no lock.
 *
 *  If the handle <tt>other</tt> is a <tt>null</tt>-handle, no group join is
 *  performed and we remain in default state. This can happen and is perfectly
 *  OK.
 *
 *  Copies dumb handlee pointer and locking semantics from <tt>other</tt>.
 */
void mxmSmartHandleCore::joinGroup(const mxmSmartHandleCore &other,
                                   void *handlee_override,
                                   bool lock_already_acquired)      {
  
  if(   Lock
     || Handlee)
   mxm::terminal("non-default-state handle attempted to join handle group!",
                 this);
  
  if(   !lock_already_acquired
     && other.Lock)
   other.Lock->acquire();   // -------------------------------------------------
  
  if(other.Handlee) {                        // _null_ handles are always alone!
    
    mxmSmartHandleCore *prev = other.Prev,        // to get around const problem
                       *next = prev->Next;
    
    // actually link in...
    prev->Next = this;
    Prev       = prev;
    Next       = next;
    next->Prev = this;
    
    // copy from others...
    if(handlee_override)
     Handlee   = handlee_override;
    else
     Handlee   = other.Handlee;
    Lock       = other.Lock;
    OwnedLock  = other.OwnedLock;
  }
  
  if(   !lock_already_acquired
     && other.Lock)
   other.Lock->release();   // -------------------------------------------------
}
