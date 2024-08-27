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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IComponent.h $
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



#ifndef __MX_ICOMPONENT_H__
#define __MX_ICOMPONENT_H__



#define MX_ICOMPONENT_ID "IComponent"



// interface includes
#include "IInterface.h"
// SDK includes
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmStringList.h>



namespace mx {

   //! Common base-interface for all <tt>MxSDK</tt> framework components.
   /*!
       \ingroup mxsdk_interfaces
       
       The <tt>MxSDK</tt> framework manages dynamically loaded SDK components that
       present themselves to the framework via this interface. Using it, the
       framework can query a given loaded component for the IInterface s it
       services and furthermore for so-called interface capabilities that
       describe certain details about the particular interface implementation.
       
       \author [agu]
   */
   class IComponent : public virtual IInterface {
      
     public:
      //! Informs about the interfaces implemented by the component, so that
      //! the calling context knows what it can do with it.
      /*!
       *  Since components always implement at least one interface, the returned
       *  list is never empty.
       */
      virtual mxm::smart<mxmStringList> enumerateInterfaces() = 0;
      //! For a particular interface, this method informs about so-called
      //! capabilities that in more detail describe how the interface can do the
      //! stuff it is meant for.
      /*!
       *  You can think of interface capabilities as modifiers, features, ...
       *
       *  If the specified interface is not supported, the method returns a
       *  smart null pointer.
       */
      virtual mxm::smart<mxmStringList> enumerateCapabilities(const mxmString& InterfaceId) = 0;
   };
};  // namespace mx



#endif  // __MX_ICOMPONENT_H__
