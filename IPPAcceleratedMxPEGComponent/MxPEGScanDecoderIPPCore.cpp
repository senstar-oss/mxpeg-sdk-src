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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IPPAcceleratedMxPEGComponent/trunk/MxPEGScanDecoderIPPCore.cpp $
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



#include "include/MxPEGScanDecoderIPPCore.h"

#include <mxm/core/mxmHuffmanTableId.h>
#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>
#include <mxm/core/mxm_generic_stuff.h>

#include <cstdlib>



mx::MxPEGScanDecoderIPPCore::MxPEGScanDecoderIPPCore() {
  
  for(int i = 0; i < 2; i++)
   for(int j = 0; j < 4; j++)
    HuffmanTrees[i][j] = 0;
  
  for(int i = 0; i < 4; i++)
   QuantizationTables[i] = 0;
  
  ippiDecodeHuffmanSpecGetBufSize_JPEG_8u(&HuffmanSpecSize);
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         "using IPP-accelerated MxPEG decoding",
                         this);
}


mx::MxPEGScanDecoderIPPCore::~MxPEGScanDecoderIPPCore() {
  
  for(int i = 0; i < 2; i++)
   for(int j = 0; j < 4; j++)
    if(HuffmanTrees[i][j]) std::free(HuffmanTrees[i][j]);
  
  for(int i = 0; i < 4; i++)
   delete[] QuantizationTables[i];
}



void mx::MxPEGScanDecoderIPPCore::setQuantizationTable(int target_table,
                                                       mxm::u8 *data_bytes) {
  
  mxm::clamp_int(&target_table, 0, 3);
  
  delete[] QuantizationTables[target_table];
  
  Ipp16u      *s16_ptr;
  QuantizationTables[target_table] = s16_ptr = new Ipp16u[64];
  ippiQuantInvTableInit_JPEG_8u16u(data_bytes,
                                   s16_ptr);
}


void mx::MxPEGScanDecoderIPPCore
      ::setHuffmanTable(const mxmHuffmanTableId &target_table,
                        mxm::u8 *data_bytes)                {
  
  IppiDecodeHuffmanSpec **tree = &HuffmanTrees[target_table.tableClass()]
                                              [target_table.tableTarget()];
  
  if(*tree) std::free(*tree);
  
  *tree = (IppiDecodeHuffmanSpec *)std::malloc(HuffmanSpecSize);
  if(!*tree) mxm::terminal("failed to allocate memory for new Huffman table!",
                           this);
  
  if(   ippiDecodeHuffmanSpecInit_JPEG_8u((Ipp8u *)&data_bytes[0],
                                          (Ipp8u *)&data_bytes[16],
                                          *tree)
     != ippStsNoErr) {
    
    free(*tree);
    *tree = 0;
  }
}


bool mx::MxPEGScanDecoderIPPCore
      ::mapTables(mxmUndecodedMxPEGFrameDescriptor *frame_descriptor) {
  
  if(frame_descriptor->ComponentNum != 3) return(false);
  
  switch(frame_descriptor->Sampling) {
    
    case mxmUndecodedMxPEGFrameDescriptor::Y21U11V11:        // some Axis models
      
      MCUsPerTile = 4;
      
      // Y1...
      HuffmanDCForMCU[0]        = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanDC];
      HuffmanACForMCU[0]        = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanAC];
      QuantTableForMCU[0]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[0]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[0] = &LastHuffmanDCs[0];
      
      // Y2...
      HuffmanDCForMCU[1]        = HuffmanDCForMCU[0];
      HuffmanACForMCU[1]        = HuffmanACForMCU[0];
      QuantTableForMCU[1]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[1] = LastHuffmanDCPtrForMCU[0];
      
      // U...
      HuffmanDCForMCU[2]        = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanDC];
      HuffmanACForMCU[2]        = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanAC];
      QuantTableForMCU[2]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[1]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[2] = &LastHuffmanDCs[1];
      
      // V...
      HuffmanDCForMCU[3]        = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanDC];
      HuffmanACForMCU[3]        = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanAC];
      QuantTableForMCU[3]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[2]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[3] = &LastHuffmanDCs[2];
      
      YUV_TargetPtrs[0]         = &YUV_ColorValues[0];
      YUV_TargetPtrs[1]         = &YUV_ColorValues[8];
      YUV_TargetPtrs[2]         = &YUV_ColorValues[128];
      YUV_TargetPtrs[3]         = &YUV_ColorValues[192];
      
      YUV_TargetSteps[0]        = 16;
      YUV_TargetSteps[1]        = 16;
      YUV_TargetSteps[2]        =  8;
      YUV_TargetSteps[3]        =  8;
      
      YUV_SrcPtrs[0]            = &YUV_ColorValues[0];
      YUV_SrcPtrs[1]            = &YUV_ColorValues[128];
      YUV_SrcPtrs[2]            = &YUV_ColorValues[192];
       
      YUV_SrcSteps[0]           = 16;
      YUV_SrcSteps[1]           =  8;
      YUV_SrcSteps[2]           =  8;
      
      YUV_RGB_RoiSize.width     = 16;
      YUV_RGB_RoiSize.height    =  8;
      
      break;
      
      
    case mxmUndecodedMxPEGFrameDescriptor::Y22U11V11:                      // MX
    default:
      
      MCUsPerTile = 6;
      
      // Y1...
      HuffmanDCForMCU[0]        = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanDC];
      HuffmanACForMCU[0]        = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanAC];
      QuantTableForMCU[0]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[0]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[0] = &LastHuffmanDCs[0];
      
      // Y2...
      HuffmanDCForMCU[1]        = HuffmanDCForMCU[0];
      HuffmanACForMCU[1]        = HuffmanACForMCU[0];
      QuantTableForMCU[1]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[1] = LastHuffmanDCPtrForMCU[0];
      // Y3...
      HuffmanDCForMCU[2]        = HuffmanDCForMCU[0];
      HuffmanACForMCU[2]        = HuffmanACForMCU[0];
      QuantTableForMCU[2]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[2] = LastHuffmanDCPtrForMCU[0];
      // Y4...
      HuffmanDCForMCU[3]        = HuffmanDCForMCU[0];
      HuffmanACForMCU[3]        = HuffmanACForMCU[0];
      QuantTableForMCU[3]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[3] = LastHuffmanDCPtrForMCU[0];
      
      // U...
      HuffmanDCForMCU[4]        = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanDC];
      HuffmanACForMCU[4]        = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanAC];
      QuantTableForMCU[4]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[1]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[4] = &LastHuffmanDCs[1];
      
      // V...
      HuffmanDCForMCU[5]        = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanDC];
      HuffmanACForMCU[5]        = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanAC];
      QuantTableForMCU[5]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[2]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[5] = &LastHuffmanDCs[2];
      
      YUV_TargetPtrs[0]         = &YUV_ColorValues[0];
      YUV_TargetPtrs[1]         = &YUV_ColorValues[8];
      YUV_TargetPtrs[2]         = &YUV_ColorValues[128 + 0];
      YUV_TargetPtrs[3]         = &YUV_ColorValues[128 + 8];
      YUV_TargetPtrs[4]         = &YUV_ColorValues[256];
      YUV_TargetPtrs[5]         = &YUV_ColorValues[320];
      
      YUV_TargetSteps[0]        = 16;
      YUV_TargetSteps[1]        = 16;
      YUV_TargetSteps[2]        = 16;
      YUV_TargetSteps[3]        = 16;
      YUV_TargetSteps[4]        =  8;
      YUV_TargetSteps[5]        =  8;
      
      YUV_SrcPtrs[0]            = &YUV_ColorValues[0];
      YUV_SrcPtrs[1]            = &YUV_ColorValues[256];
      YUV_SrcPtrs[2]            = &YUV_ColorValues[320];
       
      YUV_SrcSteps[0]           = 16;
      YUV_SrcSteps[1]           =  8;
      YUV_SrcSteps[2]           =  8;
      
      YUV_RGB_RoiSize.width     = 16;
      YUV_RGB_RoiSize.height    = 16;
      
      break;
  }
  
  for(int i = 0; i < MCUsPerTile; i++) {
    
    if(   !HuffmanDCForMCU[i]
       || !HuffmanACForMCU[i]
       || !QuantTableForMCU[i]) return(false);
    
    *LastHuffmanDCPtrForMCU[i] = 0;
  }
  
  return(true);
}
