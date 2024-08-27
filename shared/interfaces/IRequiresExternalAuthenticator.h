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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IRequiresExternalAuthenticator.h $
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



#ifndef __MX_IREQUIRESEXTERNALAUTHENTICATOR_H__
#define __MX_IREQUIRESEXTERNALAUTHENTICATOR_H__



#define MX_IREQUIRESEXTERNALAUTHENTICATOR_ID "IRequiresExternalAuthenticator"



#include <interfaces/IInterface.h>

#include <mxm/core/mxmQtSignal.h>
#include <mxm/core/mxmQtSlot.h>



class QString;



namespace mx {
   
   //! Interface to entities requiring an external authenticator being connected
   //! via <tt>Qt</tt> signals.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class IRequiresExternalAuthenticator : public virtual IInterface {
      
    public:
      //! Signal emitted when the object requires authentication credentials
      //! from an external place.
      /*!
       *  Signal signature is <tt>(const mxmString &, bool)</tt>. The string is
       *  a message elaborating on the purpose of the requested
       *  authentification. If the <tt>bool</tt> value is <tt>true</tt>
       *  that means that the request is the first one for a given
       *  authentication procedure.
       */
      virtual mxmQtSignal authenticationRequestSignal() const = 0;
      //! Slot to be activated from an external place after the object has
      //! emitted the \ref authenticationRequestSignal() signal, or,
      //! alternatively: the \ref processAuthenticationCancellationSlot() slot.
      /*!
       *  Slot signature is <tt>(const QString&, const QString&)</tt>.
       */
      virtual mxmQtSlot processAuthenticationSlot() const = 0;
      //! Slot to be activated from an external place after the object has
      //! emitted the \ref authenticationRequestSignal() signal, or,
      //! alternatively: the \ref processAuthenticationSlot() slot.
      /*!
       *  Slot signature is <tt>()</tt>.
       */
      virtual mxmQtSlot processAuthenticationCancellationSlot() const = 0;
      //! Actual slot implementation.
      virtual void processAuthenticationCancellation() = 0;
      
      //! Tells whether or not the object is currently waiting for
      //! authentication data.
      virtual bool awaitingAuthentication() = 0;
      //! Tells whether or not the object was in await-authentication-mode since
      //! the last call to this method.
      virtual bool wasAwaitingAuthentication() = 0;
   };
};



#endif   // __MX_IREQUIRESEXTERNALAUTHENTICATOR_H__
