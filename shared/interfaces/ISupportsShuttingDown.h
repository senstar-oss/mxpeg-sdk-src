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
//  $LastChangedDate: 2007-06-28 14:28:26 +0200 (Thu, 28 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsShuttingDown.h $
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



#ifndef __MX_ISUPPORTSSHUTTINGDOWN_H__
#define __MX_ISUPPORTSSHUTTINGDOWN_H__



#define MX_ISUPPORTSSHUTTINGDOWN_ID "ISupportsShuttingDown"



#include "IErrorState.h"



namespace mx {
  
  //! Interface to entities supporting controlled shutdown.
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  Some objects that implement IErrorState need to finalize things when they
   *  get deleted, such as closing files for example. Such operations could go
   *  wrong, and the calling context would have no way of knowing about it
   *  unless there were a mechanism allowing for controlled shutdown prior to
   *  the actual object deletion. This is what this interface is for. Client
   *  contexts call call \ref shutdown() and test errorState() afterwards to
   *  find out about potential errors that might have occured.
   *
   *  <b>Error detection semantics:</b>
   *  - if errorState() returns <tt>false</tt> after \ref shutdown() has been
   *    called, that means that the entity has been shut down properly and that
   *    everything has gone fine up until then
   *  - if errorState() returns <tt>true</tt> after \ref shutdown() has been
   *    called, that means that something has gone wrong before the shutdown or
   *    while performing the shutdown itself
   *  - attempts to use the object after \ref shutdown() has been called are
   *    considered illegal and are required to set the object's error state
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class ISupportsShuttingDown : public virtual IErrorState {
    
   public:
    //! Shuts down the implementing entity.
    virtual void shutdown() = 0;
  };
};



#endif   // __MX_ISUPPORTSSHUTTINGDOWN_H__

