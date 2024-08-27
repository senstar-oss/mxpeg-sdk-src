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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/include/HTTPGetStreamSource.h $
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



#ifndef __MX_HTTPGETSTREAMSOURCE_H__
#define __MX_HTTPGETSTREAMSOURCE_H__



#define MX_HTTPGETSTREAMSOURCE_ID "HTTPGetStreamSource"



#include "HTTPGetStreamSourceCore.h"

#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>

#include <Framework/Framework.h>

#include <QTcpSocket>



namespace mx {
   
   //! Default <tt>HTTP GET</tt> stream source implementation using
   //! <tt>Qt</tt>'s <tt>TCP</tt> socket class.
   /*!
    *  \ingroup mxsdk_iokit
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class HTTPGetStreamSource : public HTTPGetStreamSourceCore,
                               public virtual IComponent       {
      
      Q_OBJECT
      
      //! These static methods provide interface and capability ids for
      //! registration with the framework (for both the component and its
      //! invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList((mxmString(MX_HTTPGETSTREAMSOURCE_ID),
                                   MX_ICOMPONENT_ID,
                                   MX_IHTTPGETSTREAMSOURCE_ID,
                                   MX_IASYNCSTREAMSOURCE_ID,
                                   MX_ISTREAMSOURCE_ID,
                                   MX_ISUPPORTSHOSTSPECIFICATION_ID,
                                   MX_ISUPPORTSPROXYSPECIFICATION_ID,
                                   MX_ISUPPORTSWEBPAGESPECIFICATION_ID,
                                   MX_IREQUIRESEXTERNALAUTHENTICATOR_ID,
                                   MX_IREQUIRESACTIVATION_ID,
                                   MX_IERRORSTATE_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface) {
         if(interface == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_HTTPGETSTREAMSOURCE_ID));
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
            return(new HTTPGetStreamSource());
         }
      };
    public:
      //! Generates an invoker singleton for the \ref StreamFile component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    private:
      QTcpSocket *AsyncSocket;
      
    public:
      HTTPGetStreamSource();
      ~HTTPGetStreamSource();
      
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
      int fetchStreamBytes(mxm::u8 *buffer, int num);
      //! (Re)implemented.
      bool stillUp();
      //! (Re)implemented.
      bool waitForStreamBytes(int timeout_ms = 0);
      
    private:
      //! (Re)implemented.
      void establishConnection();
      //! (Re)implemented.
      void abortConnection();
      //! (Re)implemented.
      bool readChar(char *c);
      //! (Re)implemented.
      void send(const mxmString &txt);
   };
};



#endif   // __MX_HTTPGETSTREAMSOURCE_H__
