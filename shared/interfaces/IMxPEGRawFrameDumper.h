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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IMxPEGRawFrameDumper.h $
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



#ifndef __MX_IMXPEGRAWFRAMEDUMPER_H__
#define __MX_IMXPEGRAWFRAMEDUMPER_H__



#define MX_IMXPEGRAWFRAMEDUMPER_ID "IMxPEGRawFrameDumper"



#include <interfaces/IMxPEGTileReceiver.h>
#include <interfaces/ISendsSegmentedStream.h>



namespace mx {
  
  //! <tt>MxPEG</tt> tile receiver dumping raw <tt>RGB</tt> and </tt>YUV</tt>
  //! frames.
  /*!
   *   \ingroup mxsdk_interfaces
   *
   *   Per default, the dumper writes raw <tt>RGB</tt> data with 3 bytes per
   *   pixel (being <tt>R</tt>, <tt>G</tt> and <tt>B</tt> values respectively).
   *   If <tt>RGB</tt>-to-<tt>YUV</tt> conversion is turned on, the frame is
   *   dumped in the planar <tt>yv12 YUV</tt> format. Note that this
   *   <tt>YUV</tt> format uses the <tt>YCbCr</tt> colorspace rather than plain
   *   <tt>YUV</tt>.
   *
   *   If <i>explicit dump mode</i> is enabled via enableExplicitDumps(),
   *   frames will not automatically be dumped on frameComplete() events, but
   *   only when dumpFrame() gets called.
   *
   *   \note The generated raw frame formats are digestible by the commonly used
   *   <tt>transcode</tt> and <tt>MPlayer/mencoder</tt> tools.
   *
   *   \author <tt>[khe]</tt> Kai Hergenroether
   */
  class IMxPEGRawFrameDumper : public virtual IMxPEGTileReceiver,
                               public virtual ISendsSegmentedStream {
    
   public:
    //! Explicitly causes the current contents of the frame buffer to be written
    //! to the frame receiver.
    virtual void dumpFrame() = 0;
    //! Toggles explicit dump mode.
    virtual void enableExplicitDumps(bool enabled) = 0;
    //! Toggles <tt>YUV</tt> output mode.
    virtual void enableYUVDumps(bool enabled) = 0;
  };
};



#endif   // __MX_IMXPEGRAWFRAMEDUMPER_H__

