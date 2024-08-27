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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmTime.h $
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



#ifndef __MXM_TIME_H__
#define __MXM_TIME_H__



#include <mxm/core/mxmObject.h>
#include <mxm/core/mxm_types.h>

#ifdef MX_PLATFORM_WINDOWS
  #include <windows.h>
  #include <time.h>
#else
  #include <sys/time.h>
#endif



//! Identifies a point in time. 
/*!
 *  \ingroup modularMX_core
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmTime : public mxmObject {
  
 public:
  static mxmTime now(void);
  //! Initializes the mxmTime subsystem.
  static void initialize();
  //! Returns the time in microseconds since initialization.
  static mxm::frame_time getTimeSinceInitialization();
  
 private:
  static time_t RefTime;
  static bool IsInitialized;
  static mxm::frame_time RefTicks;
#ifdef MX_PLATFORM_WINDOWS
  static double CounterFrequencyFactor;  // us / ticks
  static bool IsPerformanceCounterAvailable;
#else
#endif
  mxm::frame_time Time;
  void *StableABIDataExtension;
  
 public:
  //! Time object will represent the time of its construction.
  mxmTime();
  mxmTime(const mxmTime &other_time);
  
 public:
  //! "Earlier" operator.
  bool operator<(const mxmTime &other) const;
  //! Computes time difference in milliseconds to specified earlier point in
  //! time.
  int operator-(const mxmTime &earlier) const;
  //! Conversion to <tt>mxm::frame_time</tt>.
  operator mxm::frame_time() const;
  
 public:
  //! Tells whether or not the time specified by <tt>timeout_ms</tt> has elapsed
  //! between <tt>earlier</tt> and the time called upon.
  bool timedOut(const mxmTime &earlier,
                int timeout_ms) const;
};


#endif   // __MXM_TIME_H__
