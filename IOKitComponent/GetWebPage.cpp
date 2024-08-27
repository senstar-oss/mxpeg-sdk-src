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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/GetWebPage.cpp $
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



#include "include/GetWebPage.h"

#include <Framework/Framework.h>

#include <mxmQt/mxm_qt.h>



mx::GetWebPage::GetWebPage() {
   
   WebPages = mxmString("/index.html");
   
   KeyChain = mx::Framework::getComponent(MX_IKEYCHAIN_ID);
   DialogsEnabled  = false;
   
   TimeoutInterval_ms = 0;
   
   Executing = false;
}



void mx::GetWebPage::setHost(const mxmIPAddress &host) {
   
   Host = host;
}


void mx::GetWebPage::setProxy(const mxmIPAddress &proxy) {
   
   Proxy = proxy;
}


void mx::GetWebPage::setWebPages(const mxmStringList &web_pages) {
   
   if(!web_pages.size()) {
      
      mxm::sendStatusMessage(mxm::WarningMessage,
                             "ignoring empty web pages specification!",
                             this);
      return;
   }
   
   WebPages = web_pages;
}


void mx::GetWebPage::useKeyChain(const mxmPerlStyleHash &chain_keys) {
   
   ChainKeys = chain_keys;
}


void mx::GetWebPage::useDialogs(bool enable) {
   
   DialogsEnabled = enable;
}


void mx::GetWebPage::useTimeouts(int interval_ms) {
   
   if(interval_ms > 0) {
      
      TimeoutTimer = new QTimer();
      TimeoutTimer->setSingleShot(true);
      QObject::connect(&*TimeoutTimer, SIGNAL(timeout()),
                       this,           SLOT(slotTimeout()));
      
      TimeoutInterval_ms = interval_ms;
   }
   else {
      
      TimeoutTimer = 0;
      TimeoutInterval_ms = 0;
   }
}


const mxmStringList &mx::GetWebPage::webPageBuffer() {
   
   return(LineBuffer);
}


void mx::GetWebPage::executeAction() {
   
   if(Executing) abortAction();
   
   LineBuffer.clear();
   CurrentLine = "";
   
   StreamSource = mx::Framework::getComponent(MX_IHTTPGETSTREAMSOURCE_ID);
   
   mxm::connect(StreamSource->streamBytesAvailableSignal(),
                this, SLOT(slotIncoming()));
   
   mxm::connect(StreamSource->authenticationRequestSignal(),
                KeyChain->acceptAuthenticationRequestSlot());
   mxm::connect(KeyChain->authenticationCommittedSignal(),
                StreamSource->processAuthenticationSlot());
   mxm::connect(KeyChain->authenticationCancelledSignal(),
                StreamSource->processAuthenticationCancellationSlot());
   
   KeyChain->chainKeys() = ChainKeys;
   
   if(DialogsEnabled) {
      
      AuthenticationDialog = mx::Framework
                              ::getComponent(MX_IAUTHENTICATIONDIALOG_ID);
      
      mxm::connect(KeyChain->authenticationRequestSignal(),
                   AuthenticationDialog->acceptAuthenticationRequestSlot());
      mxm::connect(AuthenticationDialog->authenticationCommittedSignal(),
                   KeyChain->processAuthenticationSlot());
      mxm::connect(AuthenticationDialog->authenticationCancelledSignal(),
                   KeyChain->processAuthenticationCancellationSlot());
      
      KeyChain->enableKeyForwarding(true);
   }
   else {
      
      KeyChain->enableKeyForwarding(false);
   }
   
   StreamSource->setHost(Host);
   StreamSource->setProxy(Proxy);
   StreamSource->setWebPages(WebPages);
   StreamSource->activate();
   
   if(!TimeoutTimer.null()) TimeoutTimer->start(TimeoutInterval_ms);
   
   Executing = true;
}


void mx::GetWebPage::abortAction() {
   
   if(!Executing) return;
   
   StreamSource         = 0;
   AuthenticationDialog = 0;
   
   LineBuffer.clear();
   CurrentLine = "";
   
   if(!TimeoutTimer.null()) TimeoutTimer->stop();
   
   Executing = false;
}


mxmQtSignal mx::GetWebPage::actionExecutedSignal() {
   
   return(mxmQtSignal(this,
                      SIGNAL(signalActionExecuted(bool))));
}


bool mx::GetWebPage::actionExecuting() {
   
   return(Executing);
}


void mx::GetWebPage::slotIncoming() {
   
   const int buffsize = 2048;
   
   char txt[buffsize];
   
   int bytes_read;
   while(bytes_read
          = StreamSource->fetchStreamBytes((mxm::u8 *)txt, buffsize - 1)) {
      
      int i = 0,
          line_start = 0;
      while(i < bytes_read) {
         
         if(txt[i] == '\n') {
            
            txt[i] = '\0';
            CurrentLine += mxmString(txt + line_start);
            CurrentLine.trim("\r");
            LineBuffer.add(CurrentLine);
            CurrentLine = "";
            
            line_start = i + 1;
         }
         
         i++;
      }
      if(line_start < bytes_read) {
         
         txt[bytes_read] = '\0';   // buffer's big enough, see above
         CurrentLine += mxmString(txt + line_start);
         CurrentLine.trim("\r");
         LineBuffer.add(CurrentLine);
      }
   }
   
   if(!StreamSource->stillUp())
    QMetaObject::invokeMethod(this,
                              "cleanUpAfterDownload",
                              Qt::QueuedConnection);  // will kill stream source
                                                      // from which control flow
                                                      // came
}


void mx::GetWebPage::cleanUpAfterDownload() {
   
   bool error_encountered;
   if(!StreamSource.null())
    error_encountered = StreamSource->errorState();
   else
    error_encountered = true;
   
   StreamSource         = 0;
   AuthenticationDialog = 0;
   
   CurrentLine = "";
   
   Executing = false;
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          mxmString("web page received, lines=")
                           + LineBuffer.size(),
                          this);
   
   if(!TimeoutTimer.null()) TimeoutTimer->stop();
   
   emit signalActionExecuted(!error_encountered);
}


void mx::GetWebPage::slotTimeout() {
   
   mxm::sendStatusMessage(mxm::WarningMessage,
                          "action execution timed out, aborting...",
                          this);
   
   abortAction();
   
   emit signalActionExecuted(false);
}
