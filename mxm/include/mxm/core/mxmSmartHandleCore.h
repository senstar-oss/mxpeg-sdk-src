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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmSmartHandleCore.h $
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



#ifndef __MXM_SMARTHANDLECORE_H__
#define __MXM_SMARTHANDLECORE_H__



#include <mxm/core/mxmObject.h>
#include <mxm/core/mxm_types.h>



class mxmLockInterface;
namespace mxm {
  template<class T> class smart;
}



//! Implements all \ref mxm::smart<T> code that does not have to go into the
//! template - hopefully minimizing code expansion.
/*!
 *  \ingroup modularMX_core
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmSmartHandleCore : public mxmObject {
  
 private:
  template<class T> friend class mxm::smart;        // we're closely related :-)
  
 private:
  mxmSmartHandleCore *Prev,
                     *Next;
  void               *Handlee;
  mxmLockInterface   *Lock;
  mxm::u32           OwnedLock:1;   // only valid if lock attached
  mxm::u32           Reserved:31;
  
 private:
  //! Initializes to default state: <tt>null</tt>-handle, no lock.
  mxmSmartHandleCore();
  //! <i>[Not synchronized]</i> Helper.
  void init();
  virtual ~mxmSmartHandleCore();
  //! <i>[Not synchronized]</i> Helper, to be implemented by subclasses.
  virtual void deleteHandlee() = 0;
  
 public:
  //! <i>[Not synchronized]</i> To provide typesafe and syntactically acceptable
  //! way of testing for (non-) <tt>null</tt>-ness.
  bool operator!(void) const;
  //! <i>[Not synchronized]</i> Returns <tt>true</tt> if handle is
  //!  <tt>null</tt>-handle, and <tt>false</tt> otherwise.
  bool null(void) const;
  //! <b>[Synchronized]</b> Returns the number of handles in the smart pointer
  //! group the handle is member of.
  int groupSize() const;
  //! <i>[Not synchronized]</i> This method sets up the synchronization
  //! mechanism to be used to protect the emerging handle group's collective
  //! datastructure.
  void setLock(mxmLockInterface *lock, bool assume_ownership = false);
  //! <b>[Synchronized]</b> Releases control over potential handlee and becomes
  //! a <tt>null</tt>-handle again.
  bool releaseSmartlyHandledObject(void);
  
 private:
  //! <b>[Synchronized]</b> Helper, leaves potential group and returns to
  //! default state: <tt>null</tt>-handle without any lock attached.
  void reset();
  //! <b>[Synchronized, via target group's lock!]</b>
  //! Helper, makes default-state handle join the specified target group.
  void joinGroup(const mxmSmartHandleCore &other,
                 void *handlee_override = 0,
                 bool lock_already_acquired = false);
};



#endif   // __MXM_SMARTHANDLECORE_H__
