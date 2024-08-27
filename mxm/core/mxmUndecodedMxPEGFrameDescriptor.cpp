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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmUndecodedMxPEGFrameDescriptor.cpp $
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



#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>



mxmUndecodedMxPEGFrameDescriptor::mxmUndecodedMxPEGFrameDescriptor() {
  
  init(640, 480);
}



void mxmUndecodedMxPEGFrameDescriptor::setResolution(int width, int height) {
  
  if(width < 1)  width = 640;
  if(height < 1) height = 480;
  
  Width  = width;
  Height = height;
  
  TileNumX = width/16;
  if(width % 16) TileNumX++;
  TileNumY = height/16;
  if(height % 16) TileNumY++;
}


void mxmUndecodedMxPEGFrameDescriptor
      ::setTimeStamp(mxm::frame_time time_stamp) {
  
  TimeStamp      = time_stamp;
  TimeStampValid = true;
}


void mxmUndecodedMxPEGFrameDescriptor::setGenerationNumber(mxm::u32 gen_no) {
  
  GenerationNumber      = gen_no;
  GenerationNumberValid = true;
}


void mxmUndecodedMxPEGFrameDescriptor::setComponentNum(int component_num) {
  
  mxm::clamp_int(&component_num, 0, MaxComponents - 1);
  
  ComponentNum = component_num;
  
  for(int i = 0; i < ComponentNum ; i++) {
    
    struct ComponentInfo *comp_info = &ComponentInfos[i];
    comp_info->HorizSamplingFactor  = 0;
    comp_info->HorizSamplingFactor  = 0;
    comp_info->QuantTable           = 0;
    comp_info->HuffmanDC            = 0;
    comp_info->HuffmanAC            = 0;
  }
}


void mxmUndecodedMxPEGFrameDescriptor
       ::setComponentParameters(int component,
                                int horiz_sampling, int vert_sampling,
                                int quant_table)                       {
  
  if(   (component < 0)
     || (component >= ComponentNum)) {
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           "component id out of range!",
                           this);
    return;
  }
  
  struct ComponentInfo *comp_info = &ComponentInfos[component];
  mxm::clamp_int(&horiz_sampling, 0, 16);
  comp_info->HorizSamplingFactor  = horiz_sampling;
  mxm::clamp_int(&vert_sampling,  0, 16);
  comp_info->VertSamplingFactor   = vert_sampling;
  mxm::clamp_int(&quant_table,    0, 3);
  comp_info->QuantTable           = quant_table;

  if(mxm::statusMessageUseful(mxm::DebugMessage))
   mxm::sendStatusMessage(mxm::DebugMessage,
                          mxmString("component ") + component + " : "
                           + "horiz_sampling=" + horiz_sampling
                           + ", vert_sampling=" + vert_sampling
                           + ", quant_table=" + quant_table,
                          this);
}


void mxmUndecodedMxPEGFrameDescriptor
       ::setMoreComponentParameters(int component,
                                    int huffman_dc, int huffman_ac) {
  
  if(   (component < 0)
     || (component >= ComponentNum)) {
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           "component id out of range!",
                           this);
    return;
  }
  
  struct ComponentInfo *comp_info = &ComponentInfos[component];
  mxm::clamp_int(&huffman_dc,     0, 3);
  comp_info->HuffmanDC            = huffman_dc;
  mxm::clamp_int(&huffman_ac,     0, 3);
  comp_info->HuffmanAC            = huffman_ac;
  
  if(mxm::statusMessageUseful(mxm::DebugMessage))
   mxm::sendStatusMessage(mxm::DebugMessage,
                          mxmString("component ") + component + " : "
                           + "huffman_dc=" + huffman_dc
                           + ", huffman_ac=" + huffman_ac,
                          this);
}


void mxmUndecodedMxPEGFrameDescriptor::setSamplingMode(SamplingMode sampling) {
  
  Sampling = sampling;
}


void mxmUndecodedMxPEGFrameDescriptor::init(int width, int height) {
  
  setResolution(width, height);
  
  ComponentNum          = 0;
  Sampling              = Y22U11V11;   // MOBO
  
  resetForNextFrame();
}


void mxmUndecodedMxPEGFrameDescriptor::resetForNextFrame() {
  
  ScanBytes             = 0;
  ScanLength            = 0;
  
  TileBits              = 0;
  
  StartOfImage          = 0;
  ImageLength           = 0;
  
  TimeStamp             = 0;
  TimeStampValid        = false;
  
  GenerationNumber      = 0;
  GenerationNumberValid = false;
}
