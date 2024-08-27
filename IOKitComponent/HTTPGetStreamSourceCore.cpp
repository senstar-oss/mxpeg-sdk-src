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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/HTTPGetStreamSourceCore.cpp $
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



#include "include/HTTPGetStreamSourceCore.h"

#include <cstdio>



mx::HTTPGetStreamSourceCore
 ::HTTPGetStreamSourceCore() {
   
   Active                        = false;
   HostSet                       = false;
   
   Connected                     = false;
   Disconnected                  = false;
   Up                            = false;
   
   NegotiationState              = SendGET_NoAuth;
   HTTPNegotiationMode           = true;
   // negotiation will be initiated by the socket's connected() signal 
   // activating our processIncomingBytes() slot
   HTTPHeaderBytesRead           = 0;
   HTTPHeaderSize                = 0;
   
   AuthenticationEntered         = false;
   AuthenticationDialogRaised    = false;
   AuthenticationDialogWasRaised = false;
   
   FirstAuth                     = true;
   URLAttempt                    = 0;
   
   ErrorEncountered              = false;
}



mx::HTTPGetStreamSourceCore
 ::~HTTPGetStreamSourceCore() {
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          "signing off",
                          this);
}


void mx::HTTPGetStreamSourceCore::setHost(const mxmIPAddress &host) {
   
   if(Active) mxm::terminal("already active!",
                            this);
   
   Host         = host;
   if(host.port() == mxmIPAddress::NoPort)
    Host.setPort(80);
   
   HostSet      = true;
}


void mx::HTTPGetStreamSourceCore::setProxy(const mxmIPAddress &proxy) {
   
   if(Active) mxm::terminal("already active, can't set proxy!",
                            this);
   
   Proxy     = proxy;
   
   if(Proxy != mxmIPAddress::nullIP()) {
      
      Proxy.resolve();
      if(Proxy.resolved()) {
         
         if(Proxy.port() == mxmIPAddress::NoPort) {
            mxm::sendStatusMessage(mxm::WarningMessage,
                                   "no proxy port specified, substituting "
                                    "default...",
                                   this);
            Proxy.setPort(8080);
         }
      }
      else {
         mxm::sendStatusMessage(mxm::WarningMessage,
                                "proxy address can not be resolved, disabling "
                                 "proxy...",
                             this);
         Proxy = mxmIPAddress::nullIP();
      }
   }
}


void mx::HTTPGetStreamSourceCore
      ::setWebPages(const mxmStringList &web_pages) {
   
   if(Active) mxm::terminal("already active!",
                            this);
   
   URLList      = web_pages;
}


bool mx::HTTPGetStreamSourceCore::activate() {
   
   if(Active)          mxm::terminal("already active!",
                                     this);
   
   if(!HostSet)        mxm::terminal("host not set, can't activate!",
                                     this);
   if(!URLList.size()) mxm::terminal("no web page(s) set, can't activate!",
                                     this);
   
   establishConnection();
   
   Active = true;
   
   return(true);
}


mxmQtSignal mx::HTTPGetStreamSourceCore::streamBytesAvailableSignal() const {
   
   return(mxmQtSignal(this,
                      SIGNAL(bytesAvailable())));
}


bool mx::HTTPGetStreamSourceCore::errorState() {
  
   return(ErrorEncountered);
}


mxmQtSignal mx::HTTPGetStreamSourceCore::authenticationRequestSignal() const {
   
   return(mxmQtSignal(this,
                      SIGNAL(authenticationRequest(const QString&,
                                                   bool))));
}


mxmQtSlot mx::HTTPGetStreamSourceCore::processAuthenticationSlot() const {
   
   return(mxmQtSlot(this,
                    SLOT(processAuthentication(const QString&,
                                               const QString&))));
}


mxmQtSlot mx::HTTPGetStreamSourceCore
           ::processAuthenticationCancellationSlot() const {
   
   return(mxmQtSlot(this,
                    SLOT(processAuthenticationCancellation())));
}


void mx::HTTPGetStreamSourceCore::setErrorState() {
   
   ErrorEncountered = true;
}


bool mx::HTTPGetStreamSourceCore::awaitingAuthentication() {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   return(AuthenticationDialogRaised);
}


/*!
 *  If the dialog is currently raised the method will return <tt>true</tt> and
 *  the internal raise flag will not be reset.
 */
bool mx::HTTPGetStreamSourceCore::wasAwaitingAuthentication() {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   bool result;
   
   result = AuthenticationDialogWasRaised;
   
   if(!AuthenticationDialogRaised) AuthenticationDialogWasRaised = false;
   
   return(result);
}


void mx::HTTPGetStreamSourceCore::processIncomingBytes() {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   int first_carriage_return;
   
   if(HTTPNegotiationMode) {
      
      for(;;) {

         mxm::sendStatusMessage(mxm::DebugMessage,
                                mxmString("new negotiation state : ")
                                 + (int)NegotiationState,
                                this);
         
         switch(NegotiationState) {
            
            case SendGET_NoAuth:
               sendGETRequest();
               NegotiationState = ReceiveResponseHeader_NoAuth;
               break;
            
            case ReceiveResponseHeader_NoAuth:
               
               if(!receiveHTTPHeader()) return;
               
               //for(int i = 0; i < HTTPHeaderSize; i++)
               // putchar(HTTPHeaderBuffer[i]);
               //putchar('\n');
               
               first_carriage_return = 0;
               while(   (HTTPHeaderBuffer[first_carriage_return] != '\r')
                     && (HTTPHeaderBuffer[first_carriage_return] != '\n'))
                                                                      // be safe
                first_carriage_return++;
               if(   (   (first_carriage_return >= 2)
                      && !std::strncmp(&HTTPHeaderBuffer
                                         [first_carriage_return - 2],
                                       "OK", 2))
                  || (   (first_carriage_return >= 4)
                      && !std::strncmp(&HTTPHeaderBuffer
                                         [first_carriage_return - 4],
                                       "Okay", 4)))                   {
                 NegotiationState = NegotiationSucceeded;
               }
               else if(   (first_carriage_return >= 9)
                       && !std::strncmp(&HTTPHeaderBuffer
                                          [first_carriage_return - 9],
                                        "Not Found", 9)) {
                  mxm::sendStatusMessage(mxm::StatusMessage,
                                         mxmString("web page not found : ")
                                          + URLList[URLAttempt],
                                         this);
                  abortConnection();
                  URLAttempt++;
                  if(URLAttempt < URLList.size()) {
                     mxm::sendStatusMessage(mxm::StatusMessage,
                                            mxmString("now trying : ")
                                             + URLList[URLAttempt] + "...",
                                            this);
                     establishConnection();
                     NegotiationState = SendGET_NoAuth;
                  }
                  else {
                     NegotiationState = NegotiationFailed;
                  }
               }
               else if(   (first_carriage_return >= 12)
                       && !std::strncmp(&HTTPHeaderBuffer
                                          [first_carriage_return - 12],
                                        "Unauthorized", 12)) {
                  abortConnection();
                  emit authenticationRequest(QString("Host <b>")
                                              + QString::number(Host.ip1())
                                              + QString(".")
                                              + QString::number(Host.ip2())
                                              + QString(".")
                                              + QString::number(Host.ip3())
                                              + QString(".")
                                              + QString::number(Host.ip4())
                                              + QString("</b> ")
                                              + QString("requires you to "
                                                         "authenticate."),
                                             FirstAuth);
                  FirstAuth        = false;
                  NegotiationState = RaisedAuthenticationDialog;
               }
               else {
                  NegotiationState = NegotiationFailed;
               }
               break;
               
            case RaisedAuthenticationDialog:
               AuthenticationEntered = false;
               AuthenticationDialogRaised = true;
               AuthenticationDialogWasRaised = true;
               NegotiationState = WaitForAuthenticationDialog;
               break;
               
            case WaitForAuthenticationDialog:
               if(!AuthenticationEntered) return;
               // AuthenticationDialogRaised is false again.
               establishConnection();
               NegotiationState = SendGET_EnteredAuth;
               return;  // SendGET_EnteredAuth will be reached by the socket's
                        // conected() signal activating us
                        // (processIncomingBytes()) again
               
            case SendGET_EnteredAuth:
               sendGETRequest(AuthenticationUser.text(),
                              AuthenticationPassword.text());
               NegotiationState = ReceiveResponseHeader_EnteredAuth;
               break;
               
            case ReceiveResponseHeader_EnteredAuth:
               if(!receiveHTTPHeader()) return;
               //for(int i = 0; i < HTTPHeaderSize; i++)
               // putchar(HTTPHeaderBuffer[i]);
               //putchar('\n');
               first_carriage_return = 0;
               while(   (HTTPHeaderBuffer[first_carriage_return] != '\r')
                     && (HTTPHeaderBuffer[first_carriage_return] != '\n'))
                                                                      // be safe
                first_carriage_return++;
               if(   (   (first_carriage_return >= 2)
                      && !std::strncmp(&HTTPHeaderBuffer
                                         [first_carriage_return - 2],
                                       "OK", 2))
                  || (   (first_carriage_return >= 4)
                      && !std::strncmp(&HTTPHeaderBuffer
                                         [first_carriage_return - 4],
                                       "Okay", 4)))                   {
                  mxm::sendStatusMessage(
                          mxm::StatusMessage,
                          mxmString("user '")
                           + AuthenticationUser
                           + "' successfully authenticated for HTTP GET access "
                              "to host "
                           + Host.ip1() + "." + Host.ip2() + "."
                           + Host.ip3() + "." + Host.ip4(),
                          this);
                  NegotiationState = NegotiationSucceeded;
               }
               else if(   (first_carriage_return >= 12)
                       && !std::strncmp(&HTTPHeaderBuffer
                                          [first_carriage_return - 12],
                                        "Unauthorized", 12))            {
                  abortConnection();
                  mxm::sendStatusMessage(
                          mxm::WarningMessage,
                          mxmString("user '")
                           + AuthenticationUser
                           + "' failed to authenticate for HTTP GET access to "
                              "host "
                           + Host.ip1() + "." + Host.ip2() + "."
                           + Host.ip3() + "." + Host.ip4(),
                          this);
                  emit authenticationRequest(
                          QString("Host <b>")
                           + QString::number(Host.ip1()) + QString(".")
                           + QString::number(Host.ip2()) + QString(".")
                           + QString::number(Host.ip3()) + QString(".")
                           + QString::number(Host.ip4()) + QString("</b> ")
                           + QString("rejected your authentication request. "
                                      "Please try again."),
                          FirstAuth);
                  FirstAuth        = false;   // not really needed :-)
                  NegotiationState = RaisedAuthenticationDialog;
               }
               else {
                  NegotiationState = NegotiationFailed;
               }
               break;
               
            case NegotiationSucceeded:
               HTTPNegotiationMode = false;
               Connected           = true;
               Up                  = true;
               emit bytesAvailable();
               return;
               
            case NegotiationFailed:
            default:
               mxm::sendStatusMessage(mxm::WarningMessage,
                                      "HTTP negotiations with host "
                                       + Host.toString() + " failed!",
                                      this);
               abortConnection();
               HTTPNegotiationMode = false;
               Connected           = true;
               Disconnected        = true;
               Up                  = false;
               ErrorEncountered    = true;
               emit bytesAvailable();
               return;
         }
      }
   }
   
   else {
      
      emit bytesAvailable();
   }
}


void mx::HTTPGetStreamSourceCore::processSocketDisconnected() {
   
   if(!Connected) return;
   
   Disconnected = true;
   
   emit bytesAvailable();       // contexts must get a chance to read the source
                                // "empty" and then find stillUp() to be _false_
}


void mx::HTTPGetStreamSourceCore
      ::processAuthentication(const QString& user, const QString& password) {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   AuthenticationUser         = user.toLatin1().constData();
   AuthenticationPassword     = password.toLatin1().constData();
   
   AuthenticationDialogRaised = false;
   AuthenticationEntered      = true;
   
   processIncomingBytes();
}


void mx::HTTPGetStreamSourceCore
      ::processAuthenticationCancellation(void) {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          "authentication cancelled",
                          this);
   
   AuthenticationDialogRaised = false;
   
   abortConnection();
   HTTPNegotiationMode = false;
   Connected           = true;
   Disconnected        = true;
   Up                  = false;
   ErrorEncountered    = true;
   emit bytesAvailable();
}


void mx::HTTPGetStreamSourceCore
      ::sendGETRequest(const char *user,
                       const char *password) {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   mxmString request;
   
   // compose HTTP GET request... 
   if(Proxy == mxmIPAddress::nullIP())   // no proxy...
    request = mxmString("GET ")
               + URLList[URLAttempt]
               + " HTTP/1.0\r\n";
   else                                  // use proxy...
    request = mxmString("GET http://")
               + Host.toString()
               + URLList[URLAttempt]
               + " HTTP/1.0\r\n";
   if(   user
      && password)
    request += mxmString("Authorization: Basic ")
                + (mxmString(user) + ":" + password).base64() + "\r\n";
   if(Proxy != mxmIPAddress::nullIP())   // proxy case
    request += mxmString("Host: ") + Host.toString() + "\r\n";
   
   
   // TODO : testing, remove!
   request += mxmString("Connection: close") + "\r\n";
   
   
   request += "\r\n";
   
   // send GET request...
   mxm::sendStatusMessage(mxm::DebugMessage,
                          mxmString("sending GET request \"")
                           + request  + "\"",
                          this);
   send(request);
}


/*!
 *  \returns <tt>true</tt> if the complete header has successfully been read
 *           into the buffer, or <tt>false</tt> if header bytes are still
 *           missing. 
 */
bool mx::HTTPGetStreamSourceCore::receiveHTTPHeader(void) {
   
   char c;
   
   if(HTTPHeaderBytesRead
       == MX_HTTPGETSTREAMSOURCECORE_HEADERBUFFER_SIZE)
    return(false);
          // buffer is full - "dead end" will be resolved by timeout strategy of
          // superior context
   
   while(readChar(&c)) {
      
      HTTPHeaderBuffer[HTTPHeaderBytesRead] = c;
      HTTPHeaderBytesRead++;
      
      if(   (   (HTTPHeaderBytesRead >= 2) 
             && !std::strncmp(&HTTPHeaderBuffer[HTTPHeaderBytesRead - 2],
                              "\n\n", 2))   // old camera software does it wrong
         || (   (HTTPHeaderBytesRead >= 4)
             && !std::strncmp(&HTTPHeaderBuffer[HTTPHeaderBytesRead - 4],
                              "\r\n\r\n", 4)))                            {
         
         HTTPHeaderSize = HTTPHeaderBytesRead;
         HTTPHeaderBytesRead = 0;
         return(true);
      }
      else {
         if(HTTPHeaderBytesRead 
             == MX_HTTPGETSTREAMSOURCECORE_HEADERBUFFER_SIZE)
          return(false);
      }
   }
   
   return(false);
}
