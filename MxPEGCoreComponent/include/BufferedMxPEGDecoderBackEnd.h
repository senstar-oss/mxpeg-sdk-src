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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/BufferedMxPEGDecoderBackEnd.h $
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



#ifndef __MX_BUFFEREDMXPEGDECODERBACKEND_H__
#define __MX_BUFFEREDMXPEGDECODERBACKEND_H__



#include <interfaces/IMxPEGTileReceiver.h>

#include <mxm/core/mxmObject.h>



namespace mx {
   
   //! Baseclass to <tt>MxPEG</tt> decoder backends internally buffering entire
   //! <tt>RGB</tt> frames.
   /*!
    *  \ingroup mxsdk_mxpeg_core
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class BufferedMxPEGDecoderBackEnd : public mxmObject,
                                       public virtual IMxPEGTileReceiver {
      
    protected:
      int Width, Height;
      int TileNumX, TileNumY;
      mxm::u32 *PixelBuffer;
      int PixelBufferOffset;
      mxmFrameInfo FrameInfo;
      
    private:
      mxm::u32 DummyTilePixelBuffer[16*16*3/4];
      
    public:
      BufferedMxPEGDecoderBackEnd();
      ~BufferedMxPEGDecoderBackEnd();
      
    public:
      //! Retrieves information about the frame currently buffered in the
      //! backend.
      bool getFrameBufferInfo(int *width, int *height,
                              mxm::u32 **pixel_buffer,
                              int *pixel_buffer_offset);
      //! Retrieves information about the last frame that came in.
      mxmFrameInfo frameInfo();
      
      //! (Re)implemented.
      void setFrameInfo(const mxmFrameInfo &frame_info);
      //! (Re)implemented.
      void videoResolutionChanged(int width, int height);
      //! (Re)implemented.
      void provideVideoTilePixelTarget(int tile_x, int tile_y,
                                       mxm::u32 **target_ptr,
                                       int *row_stepping_ptr);
   };
};



#endif   // __MX_BUFFEREDMXPEGDECODERBACKEND_H__

