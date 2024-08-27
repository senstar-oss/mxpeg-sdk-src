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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmThreadConditionVariable.cpp $
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



#define MXM_THREADCONDITIONVARIABLE_CLASSNAME "mxmThreadConditionVariable"



#include <mxm/core/mxmThreadConditionVariable.h>
#include <mxm/core/mxmThreadLock.h>
#include <mxm/core/mxm_generic_stuff.h>



mxmThreadConditionVariable
 ::mxmThreadConditionVariable(mxmThreadLock *lock) {
  
  Lock = lock;
  
#ifdef MX_PLATFORM_WINDOWS
  WaitersCount = 0;
  WaitGenerationCount = 0;
  ReleaseCount = 0;
  InitializeCriticalSection(&WaitersCountLock);
  // no security, manual-reset, non-signalled initially, unnamed
  Event = CreateEvent(NULL, TRUE, FALSE, NULL);
#else
  if(pthread_cond_init(&ConditionVariable, 0))
   mxm::terminal("failed to initialize POSIX condition variable!",
                 this);
#endif
}


mxmThreadConditionVariable::~mxmThreadConditionVariable() {
  
#ifdef MX_PLATFORM_WINDOWS
	CloseHandle(Event);
	DeleteCriticalSection(&WaitersCountLock);
#else
  if(pthread_cond_destroy(&ConditionVariable))
   mxm::terminal("failed to release POSIX condition variable!",
                 this);
#endif
}



void mxmThreadConditionVariable::wait() {
  
  //std::puts("waiting on condition variable...");
  
#ifdef MX_PLATFORM_WINDOWS
	EnterCriticalSection(&WaitersCountLock);
	WaitersCount++;
	// store current generation
	int generation = WaitGenerationCount;
	LeaveCriticalSection(&WaitersCountLock);
	Lock->release();

	for(;;) {
		// Wait until the event is signalled
		WaitForSingleObject(Event, INFINITE);
		EnterCriticalSection(&WaitersCountLock);
		// exit the loop when the event is signalled and there are still
		// waiting threads from this generation that haven't been released 
		// from this wait yet.
		bool waitDone = (ReleaseCount > 0) && (WaitGenerationCount != generation);
		LeaveCriticalSection(&WaitersCountLock);
		if(waitDone)
			break;
	}
	Lock->acquire();
	EnterCriticalSection(&WaitersCountLock);
	WaitersCount--;
	ReleaseCount--;
	bool lastWaiter = ReleaseCount == 0;
	LeaveCriticalSection(&WaitersCountLock);
	if(lastWaiter)
		ResetEvent(Event);
#else
  if(pthread_cond_wait(&ConditionVariable,
                       &(Lock->Lock)))
   mxm::terminal("failed to wait on POSIX condition variable! - "
                  "maybe associated lock wasn't held?",
                 this);
#endif
  //std::puts("awoke from condition variable...");
}


void mxmThreadConditionVariable::signal() {
  
  //std::puts("signalling on condition variable...");
#ifdef MX_PLATFORM_WINDOWS
	EnterCriticalSection(&WaitersCountLock);
	if(WaitersCount > ReleaseCount) {
		SetEvent(Event);
		ReleaseCount++;
		WaitGenerationCount++;
	}
	LeaveCriticalSection(&WaitersCountLock);
#else  
  if(pthread_cond_signal(&ConditionVariable))
   mxm::terminal("failed to signal one thread sleeping on POSIX condition "
                  "variable!",
                 this);
#endif
}


void mxmThreadConditionVariable::signalAll() {
  
  //std::puts("signalling all on condition variable...");
#ifdef MX_PLATFORM_WINDOWS
	EnterCriticalSection(&WaitersCountLock);
	if(WaitersCount > 0) {
		SetEvent(Event);
		// release all threads in this generation
		ReleaseCount = WaitersCount;
		// start a new generation
		WaitGenerationCount++;
	}
	LeaveCriticalSection(&WaitersCountLock);
#else
  if(pthread_cond_broadcast(&ConditionVariable))
   mxm::terminal("failed to signal all threads sleeping on POSIX condition "
                  "variable!",
                 this);
#endif
}
