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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmApplication.h $
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



#ifndef __MXM_APPLICATION_H__
#define __MXM_APPLICATION_H__



#include <mxm/core/mxmSynchronizationFactoryInterface.h>
#include <mxm/core/mxmObject.h>
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmString.h>


//! Required central service object for <tt>mxm</tt> applications.
/*!
 *  \ingroup modularMX_core
 *
 *  <b>First thing to be created by an application</b>, in the <tt>main()</tt>
 *  scope, by the main thread and without any other threads already started.
 *  Will then be the last thing to be automatically deleted when the appication
 *  quits - make sure no threads other than the main thread are left around
 *  running when this happens. Same semantics apply for any potential
 *  subclass implementation.
 *
 *  Cleanup done upon object destruction:
 *  - a potentially installed custom status message handler is unregistered and
 *    disposed of
 *  - ...
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmApplication : public mxmObject {
  
 private:
  static mxmApplication *SingleInstance;
  
 private:
  mxm::smart<mxmSynchronizationFactoryInterface> SynchronizationFactory;
  void *StableABIDataExtension;
  
 public:
  //! Returns the application's synchronization factory.
  static mxm::smart<mxmSynchronizationFactoryInterface>
          synchronizationFactory();
  //! Returns the base path, where the application is installed.
  static mxmString basePath();
 public:
  //! The application object assumes ownership over the synchronization
  //! factory.
  mxmApplication(int argc,
                 char **argv,
                 mxmSynchronizationFactoryInterface *sync_factory = 0);
  ~mxmApplication();
};



#endif   // __MXM_APPLICATION_H__
