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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IMxPEGParser.h $
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



#ifndef __MX_IMXPEGPARSER_H__
#define __MX_IMXPEGPARSER_H__



#define MX_IMXPEGPARSER_ID "IMxPEGParser"



#include "IProcessesStream.h"
#include "ISendsUndecodedMxPEGFrames.h"
#include "IErrorState.h"



namespace mx {
  
  class IStreamSource;
  class IUndecodedMxPEGFrameReceiver;
  
  //! Interface to <tt>MxPEG</tt> parsers.
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  The parser is the second stage in an <tt>MxPEG</tt> decoder setup, coming
   *  after an IStreamSource.
   *
   *  The new <tt>MxPEG</tt> decoder concept was designed to be portable and
   *  modular. By <i>portable</i> we understand the codebase to be phrased
   *  exclusively using standard C++ constructs, so that in contrast to the old
   *  implementation that was infested virtually from head to toe with
   *  <tt>MS Visual C++</tt> pecularities it will happily compile and run on all
   *  three major target platforms, being <tt>Windows</tt>, <tt>Linux</tt> and
   *  <tt>Mac OS X</tt>. By <i>modular</i> we refer to a clean object-oriented
   *  design, allowing the decoder to be easily configured and optimized for
   *  specific application settings by plugging together appropriate versions of
   *  decoding stages. A cellphone, for example, might be better off with a
   *  decoder stage optimized for minimum memory consumtion rather than for
   *  maximum decoder throughput, whereas workstation settings might rather
   *  enjoy the efficiency of a memory-hogging, all-out-performance version.
   *
   *  To be more specific, the current <tt>MxPEG</tt> decoder implementation
   *  uses a four-stage approach:
   *  - The first stage is an implementation of an IStreamSource
   *    representing - well - the source from where the decoder fetches the raw
   *    stream bytes.
   *  - The second stage is formed by an IMxPEGParser implementation,
   *    identifying in the stream the different portions of a single
   *    <tt>MxPEG</tt> frame and forwarding them to the next stage
   *  - the third stage, an IMxPEGScanDecoder implementation, processes
   *    the raw stream data in a fashion that it sees fit, and - as it goes
   *    along - transmits decoded video tiles and audio segments to the foruth
   *    stage.
   *  - The fourth and final stage, a specialized version of
   *    IMxPEGTileReceiver, decides what is actually to be done with the decoded
   *    video and audio data. The point of the whole thing is that various
   *    interesting scenarios are conceivable how in particular the decoded data
   *    can be processed further, each of which allowing for its own ways of
   *    streamlining the dataflow. To be most flexible in this respect, the
   *    final recever stage has also been given authority over the (memory)
   *    management of video and audio buffers.
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class IMxPEGParser: public virtual IProcessesStream,
                      public virtual ISendsUndecodedMxPEGFrames,
                      public virtual IErrorState {
  };
};



#endif   // __MX_IMXPEGPARSER_H__