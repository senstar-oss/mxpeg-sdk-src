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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/BufferedMxPEGDecoderBackEnd.cpp $
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



#include "include/BufferedMxPEGDecoderBackEnd.h"

#include <cstdio>
#include <memory>



mx::BufferedMxPEGDecoderBackEnd::BufferedMxPEGDecoderBackEnd() {
   
   Width = Height = 0;
   TileNumX = TileNumY = 0;
   
   PixelBuffer = 0;
   PixelBufferOffset = 0;
}


mx::BufferedMxPEGDecoderBackEnd::~BufferedMxPEGDecoderBackEnd() {
   
   delete[] PixelBuffer;
}



/*!
 *  \return <tt>true</tt> in case of success, and <tt>false</tt> if no frame
 *          buffer is currently attached. Only in the success case are the data
 *          indicated by the argument pointers valid.
 */
bool mx::BufferedMxPEGDecoderBackEnd
      ::getFrameBufferInfo(int *width, int *height,
                           mxm::u32 **pixel_buffer,
                           int *pixel_buffer_offset) {
   
   if(PixelBuffer) {
      
      *width               = Width;
      *height              = Height;
      *pixel_buffer        = PixelBuffer;
      *pixel_buffer_offset = PixelBufferOffset;
      return(true);
   }
   else {
      
      return(false);
   }
}


mxmFrameInfo mx::BufferedMxPEGDecoderBackEnd::frameInfo() {
   
   return(FrameInfo);
}


void mx::BufferedMxPEGDecoderBackEnd::setFrameInfo(const mxmFrameInfo
                                                          &frame_info) {
   
   FrameInfo = frame_info;
}


void mx::BufferedMxPEGDecoderBackEnd::videoResolutionChanged(int width, 
                                                             int height) {
   
   int tile_num_x = width/16;
   if(width % 16) tile_num_x++;
   int tile_num_y = height/16;
   if(height % 16) tile_num_y++;
   
   int unused_line_num = height % 16;
   if(unused_line_num)
    PixelBufferOffset = unused_line_num * (tile_num_x * 3 * 16/4); 
   else
    PixelBufferOffset = 0;
   
   delete[] PixelBuffer;
   PixelBuffer = new mxm::u32[tile_num_x*tile_num_y * 16*16*3/4];
   
   Width  = width;
   Height = height;
   
   TileNumX = tile_num_x;
   TileNumY = tile_num_y;
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          mxmString("switched video resolution to ")
                           + width + "x" + height,
                          this);
}


void mx::BufferedMxPEGDecoderBackEnd
      ::provideVideoTilePixelTarget(int tile_x, int tile_y,
                                    mxm::u32 **target_ptr,
                                    int *row_stepping_ptr)   {
   
   int rowsize;
   
   if(!PixelBuffer) {
      
      *target_ptr = DummyTilePixelBuffer;
      *row_stepping_ptr
                  = 0;
   }
   else {
      
      rowsize     = 16*3/4;   // in u32 units
      
      *target_ptr = PixelBuffer 
                     + (((TileNumY - 1 - tile_y)*16 + 15)*TileNumX + tile_x)
                        * rowsize;
      *row_stepping_ptr
                  = -(1 + TileNumX) * rowsize; 
   }
}
