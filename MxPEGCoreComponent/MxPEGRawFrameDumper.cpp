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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/MxPEGRawFrameDumper.cpp $
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



#include "include/MxPEGRawFrameDumper.h"

#include <interfaces/ISegmentedStreamReceiver.h>

#include <mxm/core/mxm_colorspaces.h>



mx::MxPEGRawFrameDumper::MxPEGRawFrameDumper() {
  
  FrameReceiver    = 0;
  ConvertToYUV     = false;
  
  YUVBuffer        = 0;
  
  ExplicitDumpMode = false;
  
  FrameCounter     = 0;
  
  ErrorEncountered = false;
}


mx::MxPEGRawFrameDumper::~MxPEGRawFrameDumper() {
  
  delete FrameReceiver;
  
  delete[] YUVBuffer;
}



mxm::smart<mxmStringList> mx::MxPEGRawFrameDumper
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::MxPEGRawFrameDumper
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::MxPEGRawFrameDumper
      ::setSegmentedStreamReceiver(ISegmentedStreamReceiver *receiver) {
  
  delete FrameReceiver;
  
  FrameReceiver = receiver;
}


void mx::MxPEGRawFrameDumper::enableExplicitDumps(bool enabled) {
  
  ExplicitDumpMode = enabled;
}

void mx::MxPEGRawFrameDumper::enableYUVDumps(bool enabled) {
  
  ConvertToYUV = enabled;
}


void mx::MxPEGRawFrameDumper::dumpFrame(void) { 
  
  if(!FrameReceiver) {
    
    setErrorState();
    return;
  }
  
  if(!PixelBuffer) {
    
    setErrorState();
    return;
  }
  if(   ConvertToYUV
     && !YUVBuffer)  {
    
    setErrorState();
    return;
  }
  
  int row_size;
  int x, y;
  mxm::u32 *line_start_ptr;
  mxm::u8 *ptr;
  int yuv_bytenum,
      yuv_uv_bytenum;
  int yuv_Cr_offset,
      yuv_Cb_offset;
  mxm::u8 col_y,
          col_Cb,
          col_Cr;
  mxm::u8 *yuv_Cr_ptr,
          *yuv_Cb_ptr;
  int i;
  
  row_size = TileNumX*16*3/4;                                    // in u32 units
  
  line_start_ptr = PixelBuffer 
                    + PixelBufferOffset
                    + (Height - 1) * row_size;
  
  if(!ConvertToYUV) {
  
    for(y = 0; y < Height; y++) {
    
      ptr = (mxm::u8 *)line_start_ptr;
    
      FrameReceiver->receiveStreamBytes(ptr, Width * 3);
    
      line_start_ptr -= row_size;
    }
  }
  
  else {
    
    yuv_bytenum = Width*Height + 2 * Width*Height/4;       // Width, Height even
    yuv_uv_bytenum = 2 * Width*Height/4;
    yuv_Cr_offset = Width*Height;
    yuv_Cb_offset = yuv_Cr_offset + Width*Height/4;
    yuv_Cr_ptr = YUVBuffer + yuv_Cr_offset;
    yuv_Cb_ptr = YUVBuffer + yuv_Cb_offset;
    
    for(i = 0; i < yuv_uv_bytenum; i++) YUVBuffer[yuv_Cr_offset + i] = 0;
    
    for(y = 0; y < Height; y++) {
      
      ptr = (mxm::u8 *)line_start_ptr;
      for(x = 0; x < Width; x++) {
      
        mxm::convert_RGB_to_YCbCr(ptr[0], ptr[1], ptr[2],
                                  &col_y, &col_Cb, &col_Cr);
        ptr += 3;
        
        YUVBuffer[y*Width + x] =  col_y;
        *yuv_Cb_ptr            += (col_Cb >> 2);
        *yuv_Cr_ptr            += (col_Cr >> 2);

        if(x & 1) {
          yuv_Cb_ptr++;
          yuv_Cr_ptr++;
        }
      }
      
      line_start_ptr -= row_size;
      if(!(y & 1)) {
        yuv_Cb_ptr -= (Width >> 1);
        yuv_Cr_ptr -= (Width >> 1);
      }
    }
    
    FrameReceiver->receiveStreamBytes(YUVBuffer, yuv_bytenum);
  }
  
  FrameReceiver->newStreamSegment();
}


void mx::MxPEGRawFrameDumper::videoResolutionChanged(int width, int height) {
  
  mx::BufferedMxPEGDecoderBackEnd::videoResolutionChanged(width, height);
  
  delete[] YUVBuffer;
  YUVBuffer = 0;
  
  if(PixelBuffer) {
    
    if(width%2 || height%2) {
      
      mxm::sendStatusMessage(mxm::WarningMessage,
                             "cannot allocate YUV buffer for odd image "
                              "dimensions!!!",
                             this);
      setErrorState();
      return;
    }
    
    YUVBuffer = new mxm::u8[width*height + 2 * width*height/4];
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           mxmString("resized YUV buffer to ")
                            + width + "x" + height,
                           this);
  }
}


void mx::MxPEGRawFrameDumper::videoTileTransferred(void) {

}


mxm::u8 *mx::MxPEGRawFrameDumper
          ::provideAudioBlockTarget(const mxmAudioBlockInfo &audio_block_info) {
  
   return(0);
}


void mx::MxPEGRawFrameDumper::audioBlockTransferred() {
  
}


void mx::MxPEGRawFrameDumper::frameComplete(void) {

  if(!ExplicitDumpMode) dumpFrame();
  
  FrameCounter++;
  if(FrameCounter < 0) FrameCounter = 0;
}


int mx::MxPEGRawFrameDumper::frameCounter(void) {
  
  return(FrameCounter);
}


void mx::MxPEGRawFrameDumper::setErrorState() {
  
  ErrorEncountered = true;
  
  if(FrameReceiver) FrameReceiver->setErrorState();
}


bool mx::MxPEGRawFrameDumper::errorState() {
  
  return(ErrorEncountered);
}
