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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/include/HTTPGetStreamSourceCore.h $
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



#ifndef __MX_HTTPGETSTREAMSOURCECORE_H__
#define __MX_HTTPGETSTREAMSOURCECORE_H__



#define MX_HTTPGETSTREAMSOURCECORE_HEADERBUFFER_SIZE 1024



#include <interfaces/IHTTPGetStreamSource.h>

#include <mxm/core/mxmIPAddress.h>
#include <mxm/core/mxmStringList.h>

#include <QObject>



namespace mx {
   
   //! Common functionality for <tt>HTTP GET</tt> stream sources. 
   /*!
    *  \ingroup mxsdk_iokit
    *
    *  Subclasses just have to provide the actual network I/O implementation.
    *
    *  Subclasses might differ in whether or not they support <tt>SSL</tt>
    *  connections or in the efficiency of their network access implementation.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class HTTPGetStreamSourceCore : public QObject,
                                   public mxmObject,
                                   public virtual IHTTPGetStreamSource {
      
      Q_OBJECT
     
    private:
      enum HTTPNegotiationState { SendGET_NoAuth,
                                  ReceiveResponseHeader_NoAuth,
                                  RaisedAuthenticationDialog,
                                  WaitForAuthenticationDialog,
                                  SendGET_EnteredAuth,
                                  ReceiveResponseHeader_EnteredAuth,
                                  NegotiationSucceeded,
                                  NegotiationFailed
                                };
      
    protected:
      bool Active;
      mxmIPAddress Host;
      mxmIPAddress Proxy;
      
    private:
      bool HostSet;
      mxmStringList URLList;         // bad name, should habe been _WebPageList_
      
      bool                 HTTPNegotiationMode;
      HTTPNegotiationState NegotiationState;
      char                 HTTPHeaderBuffer
                            [MX_HTTPGETSTREAMSOURCECORE_HEADERBUFFER_SIZE];
      int                  HTTPHeaderBytesRead;
      int                  HTTPHeaderSize;
      
      bool      AuthenticationEntered,
                AuthenticationDialogRaised,
                AuthenticationDialogWasRaised;
      mxmString AuthenticationUser,
                AuthenticationPassword;
      
      bool FirstAuth;
      int URLAttempt;
      
    protected:
      bool Connected,                     // "final data connection established"
           Disconnected;        // "final data connection has been disconnected"
      bool Up;
      bool ErrorEncountered;
      
    public:
      HTTPGetStreamSourceCore();
      ~HTTPGetStreamSourceCore();
      
    signals:
      //! Emitted when bytes have become available to be read.
      void bytesAvailable();
      //! Emitted when during negotiation with the host, need arises to
      //! get authentication information about the user.
      /*!
       *  Originally indended to be connected to an mycpcAuthenticationDialog's
       *  raise() slot.
       *
       *  By using the component-oriented signal/slot-mechanism, we avoid doing
       *  an upcall to the higher layer of a GUI lib.
       */
      void authenticationRequest(const QString &msg,
                                 bool first_request_for_current_procedure);
      
    public slots:
      //! To be activated from an external place after the source core has
      //! emitted the authenticationRequest() signal, or, alternatively:
      //! the processAuthenticationCancellation() slot.
      void processAuthentication(const QString &user, const QString &password);
      //! To be activated from an external place after the source core has
      //! emitted the authenticationRequest() signal, or, alternatively:
      //! the processAuthentication() slot.
      void processAuthenticationCancellation();
      
    public:
      //! (Re)implemented.
      void setHost(const mxmIPAddress &host);
      //! (Re)implemented.
      void setWebPages(const mxmStringList &web_pages);
      //! (Re)implemented.
      void setProxy(const mxmIPAddress &proxy);
      //! (Re)implemented.
      bool activate();
      
      //! (Re)implemented.
      mxmQtSignal streamBytesAvailableSignal() const;
      
      //! (Re)implemented.
      mxmQtSignal authenticationRequestSignal() const;
      //! (Re)implemented.
      mxmQtSlot processAuthenticationSlot() const;
      //! (Re)implemented.
      mxmQtSlot processAuthenticationCancellationSlot() const;
      
      //! (Re)implemented.
      bool errorState();
      //! (Re)implemented.
      void setErrorState();
      
      //! (Re)implemented.
      bool awaitingAuthentication();
      //! (Re)implemented.
      bool wasAwaitingAuthentication();
      
    private slots:
      //! Connected to the encapsuled socket's "incoming-bytes-ready" signal.
      void processIncomingBytes();
      //! Connected to the encapsuled socket's "disconnected" signal.
      void processSocketDisconnected();
      
    private:
      //! To be implemented by subclasses.
      virtual void establishConnection() = 0;
      //! To be implemented by subclasses.
      virtual void abortConnection() = 0;
      //! To be implemented by subclasses.
      /*!
       *  \return <tt>true</tt> in case a character has been read, and
       *          <tt>false</tt> otherwise (no more characters were available)
       */
      virtual bool readChar(char *c) = 0;
      //! To be implemented by subclasses.
      virtual void send(const mxmString &txt) = 0;
      
      //! Internal helper method.
      void sendGETRequest(const char *user = 0, const char *password = 0);
      //! Internal helpermethod.
      bool receiveHTTPHeader();

   };
};



#endif   // __MX_HTTPGETSTREAMSOURCECORE_H__

