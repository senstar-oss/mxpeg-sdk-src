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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxm_types.h $
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



#ifndef __MXM_TYPES_H__
#define __MXM_TYPES_H__



#undef u8
#undef u16
#undef s16
#undef u32
#undef u64



namespace mxm {
  
  typedef unsigned char      u8;
  typedef unsigned short     u16;
  typedef short              s16;
  typedef unsigned int       u32;
  typedef unsigned long long u64;
  typedef unsigned int       flags;
  
  //! Time type used with <tt>MX</tt> frames, in microseconds since the epoch.
  typedef u64      frame_time;
  
  //! Known video formats.
  enum EVideoFormat { eNoVideo          = 0,
                      eMxPEGVideo       = 1,
                      eMJPGVideo        = 2,
                      eUnknownVideo     = 666
                    };
  //! Known audio formats.
  enum EAudioFormat { eNoAudio          = 0,
                      ePCMAlaw8kHzAudio = 1,
                      eUnknownAudio     = 666
                    };

  enum Direction { LeftToRightDirection,
                   TopToBottomDirection
                 };
  
  //! Verbosity level of central status message facility.
  enum StatusMessageType { DesperateDebugMessage   =  10,
                           DebugMessage            =  20,
                           DebugWarningMessage     =  30,
                           StatusMessage           =  40,
                           ImportantStatusMessage  =  45,
                           SuccessMessage          =  50,
                           WarningMessage          =  60,
                           FailureMessage          =  70,
                           TerminalMessage         =  80,
                           NullMessage             = 666  };
};



#endif   // __MXM_TYPES_H__
