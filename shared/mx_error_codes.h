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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/mx_error_codes.h $
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



#ifndef __MX_ERROR_CODES_H__
#define __MX_ERROR_CODES_H__



namespace mx {
   
   /*! Error codes.
	 *  Error codes specified in this file are on a very common level. If more specific error codes
	 *  are available, they are specified in the interface itself. In this case the returned error
	 *  code is <tt>eExtendedError</tt> and a detailed error code is provided be an <tt>error</tt>
	 *  method.
	 */

	enum EErrorCode {
		eOK                                = 0,       //!< no error, we're happy
		eUnknownError,                                //!< unknown screw-up
		eIOFailureError,                              //!< an IO operation was not successful
		eInsufficientRightsError,                     //!< the user has not the privilege to do the desired operation
		eObjectNotPresentError,                       //!< 
		eFeatureNotSupportedError,                    //!< the desired feature is not (yet) supported
		eFeatureTemporarilyUnavailableError,          //!< try later
		eExtendedError,                               //!< get a more detailed error code via the error() method
		eInvalidValueError,                           //!< the value is not valid for that attribute
		eTimeOutError,                                //!< a timeout occurred
		eNotUniqueError,                              //!< a key value was not unique
		eInternalError,                               //!< you are doomed
		eNotFoundError,                               //!< an entry was not found
		eInterfaceNotSupportedError,                  //!< the desired interface is not supported by this component
      eInterfaceCapabilityNotSupportedError,        //!< interface is available, yet does not support requested capability
	};

	/*! common errors specific to the HTTP protocol
	 */
	enum EHttpError {
		eHttpNoError,						//!< no error occurred
		eHttpHostNotFound,				//!< host name lookup failed
		eHttpConnectionRefused,			//!< the server refused the connection
		eHttpUnexpectedClose,			//!< the server closed the connection unexpectedly
		eHttpAborted,						//!< the request was aborted (cancelAsyncRequest)
		eHttpPageNotFound,				//!< the requested page was not found on the server
		eHttpAuthenticationFailed,		//!< the authentication to the server failed (check user and password)
		eHttpServiceUnavailable,		//!< service from server unavailable
		eHttpTimedOut,						//!< timed out
		eHttpForbidden,               //!< access forbidden
		eHttpUnknownError					//!< an error other than those specified above occurred

	};


};



#endif   // __MX_ERROR_CODES_H__

