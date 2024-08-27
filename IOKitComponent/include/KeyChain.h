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
//  $LastChangedDate: 2007-06-29 15:34:53 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/include/KeyChain.h $
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



#ifndef __MX_KEYCHAIN_H__
#define __MX_KEYCHAIN_H__



#include <interfaces/IKeyChain.h>
#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmPerlStyleHash.h>

#include <QObject>



#define MX_KEYCHAIN_ID "KeyChain"



namespace mx {
   
   //! Key chain implementation.
   /*!
    *  \ingroup mxsdk_iokit
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class KeyChain : public QObject,
                    public mxmObject,
                    public virtual IComponent,
                    public virtual IKeyChain {
      
      Q_OBJECT
      
      //! These static methods provide interface and capability ids for
      //! registration with the framework (for both the component and its
      //! invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList((mxmString(MX_KEYCHAIN_ID),
                                   MX_ICOMPONENT_ID,
                                   MX_IKEYCHAIN_ID,
                                   MX_IREQUIRESEXTERNALAUTHENTICATOR_ID,
                                   MX_IEXTERNALAUTHENTICATOR_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface) {
         if(interface == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_KEYCHAIN_ID));
         else
          return(new mxmStringList());
      }
      
    private:
      class Invoker : public mxmObject, public virtual IComponentInvoker {
       public:
         mxm::smart<mxmStringList> enumerateInterfaces() {
            return(_enumerateInterfaces());
         }
         mxm::smart<mxmStringList> enumerateCapabilities(
                                     const mxmString &interface) {
            return(_enumerateCapabilities(interface));
         }
         IComponent *newComponent() {
            return(new KeyChain());
         }
      };
    public:
      //! Generates an invoker singleton for the \ref StreamFile component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    private:
      mxmPerlStyleHash ChainKeys;
      int CurrentKey;
      bool ForwardingEnabled;
      bool Forwarding;
      bool AwaitingAuthentication,
           WasAwaitingAuthentication;
      
    public:
      KeyChain();
      
    public:
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateInterfaces() {
         return(_enumerateInterfaces());
      }
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateCapabilities(
                                   const mxmString &interface) {
         return(_enumerateCapabilities(interface));
      }
      
      //! (Re)implemented.
      mxmPerlStyleHash &chainKeys();
      //! (Re)implemented.
      void enableKeyForwarding(bool enabled);
      
      //! (Re)implemented.
      mxmQtSlot acceptAuthenticationRequestSlot() const;
      //! (Re)implemented.
      mxmQtSignal authenticationCommittedSignal() const;
      //! (Re)implemented.
      mxmQtSignal authenticationCancelledSignal() const;
    public slots:
      //! Slot implementation.
      void slotAcceptAuthenticationRequest(const QString &msg,
                                           bool first_try);
    signals:
      //! Actual signal.
      void signalAuthenticationCommitted(const QString &username,
                                         const QString &password);
      //! Actual signal.
      void signalAuthenticationCancelled();
      
    public:
      //! (Re)implemented.
      mxmQtSignal authenticationRequestSignal() const;
      //! (Re)implemented.
      mxmQtSlot processAuthenticationSlot() const;
      //! (Re)implemented.
      mxmQtSlot processAuthenticationCancellationSlot() const;
    signals:
      //! (Re)implemented.
      void signalAuthenticationRequest(const QString &msg,
                                       bool first_try);
    public slots:
      //! Actual slot implementation.
      void processAuthentication(const QString &user,
                                 const QString &password);
      //! Actual slot implementation.
      void processAuthenticationCancellation();
    public:
      //! (Re)implemented.
      bool awaitingAuthentication();
      //! (Re)implemented.
      bool wasAwaitingAuthentication();
   };
};


#endif   // __MX_KEYCHAIN_H__
