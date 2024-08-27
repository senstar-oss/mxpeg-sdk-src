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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/include/GetWebPage.h $
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



#ifndef __MX_GETWEBPAGE_H__
#define __MX_GETWEBPAGE_H__



#define MX_GETWEBPAGE_ID "GetWebPage"



#include <interfaces/IGetWebPage.h>
#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IHTTPGetStreamSource.h>
#include <interfaces/IKeyChain.h>
#include <interfaces/IAuthenticationDialog.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmIPAddress.h>
#include <mxm/core/mxmPerlStyleHash.h>

#include <QObject>
#include <QTimer>



namespace mx {
   
   //! Action downloading a web page.
   /*!
    *  \ingroup mxsdk_iokit
    *
    *  Note that it supports authentication via key chain and authentication
    *  dialog.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class GetWebPage : public QObject,
                      public mxmObject,
                      public virtual IComponent,
                      public virtual IGetWebPage {
      
      Q_OBJECT
      
      // These static methods provide interface and capability ids for
      // registration with the framework (for both the component and its
      // invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList((mxmString(MX_GETWEBPAGE_ID),
                                   MX_ICOMPONENT_ID,
                                   MX_IGETWEBPAGE_ID,
                                   MX_IASYNCACTION_ID,
                                   MX_ISUPPORTSHOSTSPECIFICATION_ID,
                                   MX_ISUPPORTSPROXYSPECIFICATION_ID,
                                   MX_ISUPPORTSWEBPAGESPECIFICATION_ID,
                                   MX_ICANUSEKEYCHAIN_ID,
                                   MX_ICANUSEDIALOGS_ID,
                                   MX_ICANUSETIMEOUTS_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface) {
         if(interface == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_GETWEBPAGE_ID));
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
            return(new GetWebPage());
         }
      };
    public:
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    private:
      mxmIPAddress Host,
                   Proxy;
      mxmStringList WebPages;
      mxm::smart<IKeyChain> KeyChain;
    protected:
      mxm::smart<IAuthenticationDialog> AuthenticationDialog;
      mxm::smart<QTimer> TimeoutTimer;
      int TimeoutInterval_ms;
    private:
      mxmPerlStyleHash ChainKeys;
      bool DialogsEnabled;
      
    protected:
      bool Executing;
      
      mxm::smart<IHTTPGetStreamSource> StreamSource;
      mxmStringList LineBuffer;
      mxmString CurrentLine;
      
    public:
      GetWebPage();
      
    signals:
      //! Signal implementation for \ref actionExecutedSignal().
      void signalActionExecuted(bool);
      
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
      void setHost(const mxmIPAddress &host);
      //! (Re)implemented.
      void setProxy(const mxmIPAddress &proxy);
      //! (Re)implemented.
      void setWebPages(const mxmStringList &web_pages);
      
      //! (Re)implemented.
      void useKeyChain(const mxmPerlStyleHash &chain_keys);
      //! (Re)implemented.
      void useDialogs(bool enable);
      
      //! (Re)implemented.
      void useTimeouts(int interval_ms);
      
      //! (Re)implemented.
      const mxmStringList &webPageBuffer();
      
      //! (Re)implemented.
      void executeAction();
      //! (Re)implemented.
      void abortAction();
      //! (Re)implemented.
      mxmQtSignal actionExecutedSignal();
      //! (Re)implemented.
      bool actionExecuting();
      
    private slots:
      void slotIncoming();
      virtual void cleanUpAfterDownload();
      void slotTimeout();
   };
};



#endif   // __MX_GETWEBPAGE_H__

