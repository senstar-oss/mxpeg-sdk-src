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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmException.h $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licenced under the BSD licence,
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



#ifndef __MXM_EXCEPTION_H__
#define __MXM_EXCEPTION_H__



#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringList.h>

#include <exception>



//! <b>[Not available on Camera.]</b> Base to all <tt>MxSDK</tt> exceptions.
/*!
 *  \ingroup modularMX_core
 *
 *  Remember that as runtime platform for camera services, <tt>modularMX</tt>
 *  (<tt>mxm</tt>) does not use exceptions.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmException : public std::exception {
  
 private:
  mxmStringList Messages;
  void *StableABIDataExtension;
  
 public:
  mxmException(const mxmString &initial_message,
               const mxmObject *throwing_object = 0);
  ~mxmException() throw ();
  
 public:
  //! Via this method a catching context can add its context information before
  //! rethrowing the exception upwards, thus building an exception-stack unwind
  //! log.
  void addMessage(const mxmString &message,
                  const mxmObject *adding_object = 0);
  //! Retrieves the messages stored in the exception.
  mxmStringList messages() const;
};



//! Thrown in terminal conditions - must lead immediately to the termination of
//! the  application.
class MXM_CORE_API mxmTerminalException : public mxmException {
  
 public:
  mxmTerminalException(const mxmString &initial_message,
                       const mxmObject *throwing_object = 0);
};


//! Only for shaping the local control flow inside a method - must <b>never</b>
//! escape a method.
class MXM_CORE_API mxmLocalControlFlowException : public mxmException {
  
 public:
  mxmLocalControlFlowException(const mxmString &initial_message = "",
                               const mxmObject *throwing_object = 0);
};



#endif   // __MXM_EXCEPTION_H__
