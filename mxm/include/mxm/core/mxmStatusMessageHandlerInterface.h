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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmStatusMessageHandlerInterface.h $
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



#ifndef __MXM_STATUSMESSAGEHANDLERINTERFACE_H__
#define __MXM_STATUSMESSAGEHANDLERINTERFACE_H__



#include <mxm/core/mxmInterface.h>
#include <mxm/core/mxm_types.h>



class mxmString;
class mxmObject;



//! Interface to handlers for the central status message facility.
/*!
 *  \ingroup modularMX_core
 *
 *  An installed custom status message handler could for example output the
 *  messages to a nifty GUI widget.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmStatusMessageHandlerInterface : public mxmInterface {
  
 public:
  //! Processes the status message as it sees it fit.
  /*!
   *  Note that implementations for multithreaded environments need to be thread
   *  safe!
   */
  virtual void sendStatusMessage(mxm::StatusMessageType msg_type,
                                 const mxmString &msg,
                                 const mxmObject *sending_object = 0) = 0;
  //! Used (e.g. by the central status message facility) to switch the handler
  //! to a given verbosity level - the handler is free to ignore the request,
  //! though.
  virtual void setStatusMessageVerbosity(mxm::StatusMessageType msg_level) = 0;
  //! Returns the verbosity level currently supported/expected by the handler.
  /*!
   *  Through the return value the handler tells the calling context (the
   *  central status message facility, and thus application  code) which status
   *  messages it is interested in. By returning sensible values it can help
   *  application code to reduce overhead by only composing status messages that
   *  are actually needed. Application code can use mxm::statusMessageUseful()
   *  to determine whether or not a message for a given level is currently
   *  useful.
   */
  virtual mxm::StatusMessageType statusMessageVerbosity() = 0;
};



#endif   // __MXM_STATUSMESSAGEHANDLERINTERFACE_H__
