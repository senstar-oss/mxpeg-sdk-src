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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IAsyncAction.h $
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



#ifndef __MX_IASYNCACTION_H__
#define __MX_IASYNCACTION_H__



#define MX_IASYNCACTION_ID "IAsyncAction"



#include <interfaces/IInterface.h>

#include <mxm/core/mxmQtSignal.h>
#include <mxm/core/mxmStringList.h>



namespace mx {
   
   //! Action that can be started in order to complete asynchronously.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  Upon completion of an action execution the client context gets notified
    *  via <tt>Qt</tt> signal.
    *
    *  \note This has nothing to do with multithreading - both client and
    *        action code execute within the same thread, moderated by the
    *        <tt>Qt</tt> event loop.
    *
    *  <b>Requirement:</b> When implementing objects get destroyed they are
    *  required to automatically abort a potential currently executing action.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class IAsyncAction : public virtual IInterface {
      
    public:
      //! Triggers the execution of the action - an action potentially executing
      //! already is aborted first.
      virtual void executeAction() = 0;
      //! Aborts execution of the action - in case the action is not currently
      //! executing, nothing happens.
      virtual void abortAction() = 0;
      //! Signal emitted when the action has been executed.
      /*!
       *  Signal signature is <tt>(bool success)</tt>.
       *
       *  \param success
       *  <tt>true</tt> in case the action has executed successfully, and
       *  <tt>false</tt> otherwise
       */
      virtual mxmQtSignal actionExecutedSignal() = 0;
      //! Tells whether or not the action is currently executing.
      virtual bool actionExecuting() = 0;
   };
};



#endif   // __MX_IASYNCACTION_H__

