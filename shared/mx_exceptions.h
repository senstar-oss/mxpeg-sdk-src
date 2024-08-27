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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/mx_exceptions.h $
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



#ifndef __MX_EXCEPTIONS_H__
#define __MX_EXCEPTIONS_H__



#include <mxm/core/mxmException.h>



namespace mx {
   
   //! Thrown in case an mx::IConfigSection is accessed via getter methods for
   //! configuration items that the context has neglected to retrieve before.
   class ConfigItemNotRetrievedException : public mxmException {
	public:
	  ConfigItemNotRetrievedException(const mxmString &initial_message,
                  const mxmObject *throwing_object = 0)
						: mxmException(initial_message, throwing_object) {}
	};
	//! Thrown in case an mx::IConfigSection is retrieved 
	//! that the context has neglected to set the host before.
   class HostNotSetException : public mxmException {
	public:
	  HostNotSetException(const mxmString &initial_message,
                  const mxmObject *throwing_object = 0)
						: mxmException(initial_message, throwing_object) {}
	};
   class CameraModelUnknownException : public mxmException {
	public:
	  CameraModelUnknownException(const mxmString &initial_message,
                  const mxmObject *throwing_object = 0)
						: mxmException(initial_message, throwing_object) {}
	};
	//! A dependent object could not be instantiated
   class DependentObjectNotInstantiated : public mxmException {
	public:
	  DependentObjectNotInstantiated(const mxmString &initial_message,
                  const mxmObject *throwing_object = 0)
						: mxmException(initial_message, throwing_object) {}
	};

	
   //! Thrown in case an interface is operated in violation of its capabilities.
   class InterfaceCapabilityViolationException : public mxmException {};
};



#endif   // __MX_EXCEPTIONS_H__
