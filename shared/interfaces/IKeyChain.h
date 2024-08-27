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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IKeyChain.h $
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



#ifndef __MX_IKEYCHAIN_H__
#define __MX_IKEYCHAIN_H__



#define MX_IKEYCHAIN_ID "IKeyChain"



#include "IExternalAuthenticator.h"
#include "IRequiresExternalAuthenticator.h"



class mxmPerlStyleHash;



namespace mx {
   
   //! Encodes the concept of a key chain.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  A key chain holds multiple <tt>(username, password)</tt> pairs and
    *  serves them to an <tt>IRequiresExternalAuthentication</tt>-entity. If all
    *  of these keys are tried for a given authentication procedure, the key
    *  chain can switch further transactions through to another external
    *  authenticator, if such is attached (that's why it also implements
    *  <tt>IRequiresExternalAuthenticator</tt>).
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class IKeyChain : public virtual IExternalAuthenticator,
                     public virtual IRequiresExternalAuthenticator {
      
    public:
      //! Returns a reference to an encapsuled hash holding the actual keys
      //! (user/password) pairs - use it to set up your key chain.
      virtual mxmPerlStyleHash &chainKeys() = 0;
      //! Toggles whether or not key transactions should be forwarded to
      //! another external authenticator in case all keys have already been
      //! tried.
      /*!
       *  Default is <tt>disabled</tt>.
       */
      virtual void enableKeyForwarding(bool enabled) = 0;
   };
};



#endif   // __MX_IKEYCHAIN_H__

