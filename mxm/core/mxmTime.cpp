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
//  $LastChangedDate: 2007-07-01 19:10:32 +0200 (Sun, 01 Jul 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmTime.cpp $
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



#include <mxm/core/mxmTime.h>
#include <mxm/core/mxm_generic_stuff.h>

#ifdef MX_PLATFORM_WINDOWS
  #include <windows.h>
#else
  #include <time.h>
#endif



mxmTime::mxmTime() {

  Time = getTimeSinceInitialization();
}


mxmTime::mxmTime(const mxmTime &other_time) {

  Time  = other_time.Time;
}



bool mxmTime::operator<(const mxmTime &other) const {
  
  return Time < other.Time;
}


/*!
 *  If <tt>earlier</tt> time is not earlier, the method returns <tt>0</tt>.
 */
int mxmTime::operator-(const mxmTime &earlier) const {

  if(!(Time > earlier.Time))
   return(0);
  else 
   return(int)((Time - earlier.Time) / 1000);
}


mxmTime::operator mxm::frame_time() const {
    
  return Time;
}


/*!
 *  To reasonably respond to the two possible problems of
 *  - externally induced clock adjustments while executing
 *  - timer wrap-arounds (unlikely)
 *
 *  we consider the timeout as elapsed if actually the <i>time difference</i>
 *  between both specified time stamps exceeds <tt>timeout_ms</tt>.
 */
bool mxmTime::timedOut(const mxmTime &earlier,
                       int timeout_ms) const   {

  if(earlier < *this)
   return((*this - earlier) > timeout_ms);
  else
   return((earlier - *this) > timeout_ms);
}


mxmTime mxmTime::now(void) {
  
  mxmTime now;
  
  return(now);
}

bool mxmTime::IsInitialized = false;
time_t mxmTime::RefTime;
mxm::frame_time mxmTime::RefTicks = 0;
#ifdef MX_PLATFORM_WINDOWS
double mxmTime::CounterFrequencyFactor = 0.0;
bool mxmTime::IsPerformanceCounterAvailable = false;
#else
#endif

/*! Initializes the mxmTime subsystem. This method is called once, preferably from 
 *  mxmApplication, when the application is started. This method fills all static
 *  members and is the only method which has write access to them.
 *  
 */

void mxmTime::initialize() {
#ifdef MX_PLATFORM_WINDOWS
	// check if performance counter is available, otherwise use GetTickCount()
	LARGE_INTEGER counterFrequency, refCounterTicks;
	if(QueryPerformanceFrequency(&counterFrequency)) {
		IsPerformanceCounterAvailable = true;
		CounterFrequencyFactor = 1000000.0 / double(counterFrequency.QuadPart);
		QueryPerformanceCounter(&refCounterTicks);
		RefTicks = (double)refCounterTicks.QuadPart * CounterFrequencyFactor;
	} else {
		IsPerformanceCounterAvailable = false;
		RefTicks = GetTickCount() * 1000;
	}
#else
	struct timeval ticks;
	gettimeofday(&ticks, 0);
	RefTicks = (mxm::frame_time)ticks.tv_sec * 1000000 + (mxm::frame_time)ticks.tv_usec;
#endif
	time(&RefTime);
	IsInitialized = true;
}

/*! Returns the time in microseconds since the mxmTime subsystem was initialized by
 *  the <tt>initialize()</tt> method.
 */
mxm::frame_time mxmTime::getTimeSinceInitialization() {
	mxm::frame_time elapsed;
	if(!IsInitialized)
		mxm::terminal("mxmTime subsystem not initialized.", 0);
	
	mxm::frame_time microseconds;
#ifdef MX_PLATFORM_WINDOWS
	if(IsPerformanceCounterAvailable) {
		LARGE_INTEGER ticks;
		QueryPerformanceCounter(&ticks);
		microseconds = (double)ticks.QuadPart * CounterFrequencyFactor;
	} else {
		microseconds = (mxm::frame_time)GetTickCount() * 1000;
	}
#else
	timeval ticks;
	gettimeofday(&ticks, 0);
	microseconds = (mxm::frame_time)ticks.tv_sec * 1000000 + (mxm::frame_time)ticks.tv_usec;
#endif
	elapsed = microseconds - RefTicks;
	return elapsed;
}