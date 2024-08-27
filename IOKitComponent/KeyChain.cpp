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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/KeyChain.cpp $
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



#include "include/KeyChain.h"

#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxm_generic_stuff.h>



mx::KeyChain::KeyChain() {
   
   CurrentKey                = 0;
   ForwardingEnabled         = false;
   Forwarding                = false;
   AwaitingAuthentication    = false;
   WasAwaitingAuthentication = false;
}



mxmPerlStyleHash &mx::KeyChain::chainKeys() {
   
   return(ChainKeys);
}


void mx::KeyChain::enableKeyForwarding(bool enabled) {
   
   ForwardingEnabled = enabled;
}


mxmQtSlot mx::KeyChain::acceptAuthenticationRequestSlot() const {
   
   return(mxmQtSlot(this,
                    SLOT(slotAcceptAuthenticationRequest(const QString&,
                                                         bool))));
}


mxmQtSignal mx::KeyChain::authenticationCommittedSignal() const {
   
   return(mxmQtSignal(this,
                      SIGNAL(signalAuthenticationCommitted(const QString&,
                                                           const QString&))));
}


mxmQtSignal mx::KeyChain::authenticationCancelledSignal() const {
   
   return(mxmQtSignal(this,
                      SIGNAL(signalAuthenticationCancelled())));
}


void mx::KeyChain::slotAcceptAuthenticationRequest(const QString &msg,
                                                   bool first_try)       {
   
   if(first_try) {
      
      CurrentKey = 0;
      Forwarding = false;
   }
   
   if(Forwarding) {
      
      AwaitingAuthentication    = true;
      WasAwaitingAuthentication = true;
      
      emit signalAuthenticationRequest(msg, false);
   }
   else {
      
      mxm::smart<mxmStringList> keys = ChainKeys.enumerateKeys();
      if(CurrentKey < keys->size()) {   // serve from chain...
         
         mxmString username = (*keys)[CurrentKey],
                   password = ChainKeys[username];
         CurrentKey++;
         
         mxmString starred_password = password;
         char da_star = '*';
         starred_password.purgeCharacters("", true, &da_star);
         mxm::sendStatusMessage(mxm::StatusMessage,
                                mxmString("trying authentication credentials: ")
                                 + "username=" + username
                                 + ", password=" + starred_password,
                                this);
         
         QMetaObject::invokeMethod(this,
                                   "signalAuthenticationCommitted",
                                   Qt::QueuedConnection,
                                   Q_ARG(QString, username.text()),
                                   Q_ARG(QString, password.text()));
                                                        // need clean call stack
      }
      else {   // enable forwarding...?
         
         if(ForwardingEnabled) {
            
            mxm::sendStatusMessage(mxm::StatusMessage,
                                   "no more keys to try, "
                                    "enabling forwarding...",
                                   this);
            
            Forwarding                = true;
            AwaitingAuthentication    = true;
            WasAwaitingAuthentication = true;
            
            emit signalAuthenticationRequest(msg, true);
         }
         else {
            
            QMetaObject::invokeMethod(this,
                                      "signalAuthenticationCancelled",
                                      Qt::QueuedConnection);
                                                        // need clean call stack
         }
      }
   }
}


mxmQtSignal mx::KeyChain::authenticationRequestSignal() const {
   
   return(mxmQtSignal(this,
                      SIGNAL(signalAuthenticationRequest(const QString&,
                                                         bool))));
}


mxmQtSlot mx::KeyChain::processAuthenticationSlot() const {
   
   return(mxmQtSlot(this,
                    SLOT(processAuthentication(const QString&,
                                               const QString&))));
}

mxmQtSlot mx::KeyChain::processAuthenticationCancellationSlot() const {
   
   return(mxmQtSlot(this,
                    SLOT(processAuthenticationCancellation())));
}


void mx::KeyChain::processAuthentication(const QString &user,
                                         const QString &password) {
   
   AwaitingAuthentication = false;
   
   if(Forwarding)
    emit signalAuthenticationCommitted(user, password);
}


void mx::KeyChain::processAuthenticationCancellation() {
   
   AwaitingAuthentication = false;
   
   if(Forwarding)
    emit signalAuthenticationCancelled();
}


bool mx::KeyChain::awaitingAuthentication() {
   
   if(Forwarding)
    return(AwaitingAuthentication);
   else
    return(false);
}


bool mx::KeyChain::wasAwaitingAuthentication() {
   
   bool result = WasAwaitingAuthentication;
   
   WasAwaitingAuthentication = false;
   
   return(result);
}
