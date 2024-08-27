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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmThreadLock.cpp $
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



#define MXM_THREADLOCK_CLASSNAME "mxmThreadLock"



#include <mxm/core/mxmThreadLock.h>
#include <mxm/core/mxm_generic_stuff.h>



mxmThreadLock::mxmThreadLock() {
  
#ifdef MX_PLATFORM_WINDOWS
  InitializeCriticalSection(&Lock);
#else
  if(pthread_mutex_init(&Lock, 0))
   mxm::terminal("failed to initialize POSIX mutex!",
                 this);
#endif
}


mxmThreadLock::~mxmThreadLock() {
  
#ifdef MX_PLATFORM_WINDOWS
  DeleteCriticalSection(&Lock);
#else
  if(pthread_mutex_destroy(&Lock))
   mxm::terminal("failed to release POSIX mutex!",
                 this);
#endif
}



void mxmThreadLock::acquire(void) {
  
  //std::puts("acquiring lock");
  
#ifdef MX_PLATFORM_WINDOWS
  EnterCriticalSection(&Lock);
#else
  if(pthread_mutex_lock(&Lock))
   mxm::terminal("failed to lock POSIX mutex!",
                 this);
#endif
}


void mxmThreadLock::release(void) {
  
  //std::puts("releasing lock");

#ifdef MX_PLATFORM_WINDOWS
  LeaveCriticalSection(&Lock);
#else
  if(pthread_mutex_unlock(&Lock))
   mxm::terminal("failed to unlock POSIX mutex!",
                 this);
#endif
}
