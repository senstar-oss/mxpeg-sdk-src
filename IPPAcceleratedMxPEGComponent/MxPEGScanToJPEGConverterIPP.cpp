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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IPPAcceleratedMxPEGComponent/trunk/MxPEGScanToJPEGConverterIPP.cpp $
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



#include "include/MxPEGScanToJPEGConverterIPP.h"

#include <interfaces/ISegmentedStreamReceiver.h>

#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>
#include <mxm/core/mxmException.h>
#include <mxm/core/mxm_generic_stuff.h>

#include <memory>



const unsigned char mx::MxPEGScanToJPEGConverterIPP
                     ::ZigZag_StoreMap[64] = {
  
  0,  1,  5,  6, 14, 15, 27, 28,
  2,  4,  7, 13, 16, 26, 29, 42,
  3,  8, 12, 17, 25, 30, 41, 43,
  9, 11, 18, 24, 31, 40, 44, 53,
  10, 19, 23, 32, 39, 45, 52, 54,
  20, 22, 33, 38, 46, 51, 55, 60,
  21, 34, 37, 47, 50, 56, 59, 61,
  35, 36, 48, 49, 57, 58, 62, 63
};


const mxm::u8
       mx::MxPEGScanToJPEGConverterIPP::HuffmanTable_Y_DC[]
        = { 0, 1, 5, 1, 1, 1, 1, 1, 1, 0,  0,  0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
          };
const mxm::u8
       mx::MxPEGScanToJPEGConverterIPP::HuffmanTable_UV_DC[]
        = { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1,  1,  0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
          };
const mxm::u8
       mx::MxPEGScanToJPEGConverterIPP::HuffmanTable_Y_AC[]
        = { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d,
            0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
            0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
            0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
            0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
            0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
            0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
            0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
            0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
            0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
            0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
            0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
            0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
            0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
            0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
            0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
            0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
            0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
            0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
            0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
            0xf9, 0xfa
          };
const mxm::u8
       mx::MxPEGScanToJPEGConverterIPP::HuffmanTable_UV_AC[]
        = { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77,
            0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
            0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
            0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
            0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
            0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
            0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
            0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
            0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
            0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
            0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
            0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
            0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
            0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
            0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
            0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
            0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
            0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
            0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
            0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
            0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
            0xf9, 0xfa
          };



mx::MxPEGScanToJPEGConverterIPP
 ::MxPEGScanToJPEGConverterIPP() {
  
  for(int i = 0; i < 4; i++)
   ForwardQuantizationTables[i] = 0;
  
  HuffmanEncodeSpec_Y_DC  = 0;
  HuffmanEncodeSpec_Y_AC  = 0;
  HuffmanEncodeSpec_UV_DC = 0;
  HuffmanEncodeSpec_UV_AC = 0;
  
  int huffman_size;
  ippiEncodeHuffmanSpecGetBufSize_JPEG_8u(&huffman_size);
  HuffmanEncodeSpec_Y_DC  = (IppiEncodeHuffmanSpec *)std::malloc(huffman_size);
  HuffmanEncodeSpec_Y_AC  = (IppiEncodeHuffmanSpec *)std::malloc(huffman_size);
  HuffmanEncodeSpec_UV_DC = (IppiEncodeHuffmanSpec *)std::malloc(huffman_size);
  HuffmanEncodeSpec_UV_AC = (IppiEncodeHuffmanSpec *)std::malloc(huffman_size);
  
  if(   !HuffmanEncodeSpec_Y_DC
     || !HuffmanEncodeSpec_Y_AC
     || !HuffmanEncodeSpec_UV_DC
     || !HuffmanEncodeSpec_UV_AC) {
    
    std::free(HuffmanEncodeSpec_Y_DC);
    std::free(HuffmanEncodeSpec_Y_AC);
    std::free(HuffmanEncodeSpec_UV_DC);
    std::free(HuffmanEncodeSpec_UV_AC);
    
    mxm::terminal("failed to allocate IPP encoder tables!",
                  this);
  }
  
  try {
    
    if(ippiEncodeHuffmanSpecInit_JPEG_8u(&HuffmanTable_Y_DC[0],
                                         &HuffmanTable_Y_DC[16],
                                         HuffmanEncodeSpec_Y_DC)
        != ippStsNoErr) throw mxmLocalControlFlowException();
    if(ippiEncodeHuffmanSpecInit_JPEG_8u(&HuffmanTable_Y_AC[0],
                                         &HuffmanTable_Y_AC[16],
                                         HuffmanEncodeSpec_Y_AC)
        != ippStsNoErr) throw mxmLocalControlFlowException();
    if(ippiEncodeHuffmanSpecInit_JPEG_8u(&HuffmanTable_UV_DC[0],
                                         &HuffmanTable_UV_DC[16],
                                         HuffmanEncodeSpec_UV_DC)
        != ippStsNoErr) throw mxmLocalControlFlowException();
    if(ippiEncodeHuffmanSpecInit_JPEG_8u(&HuffmanTable_UV_AC[0],
                                         &HuffmanTable_UV_AC[16],
                                         HuffmanEncodeSpec_UV_AC)
        != ippStsNoErr) throw mxmLocalControlFlowException();
        
  } catch(mxmLocalControlFlowException) {
  
      std::free(HuffmanEncodeSpec_Y_DC);
      std::free(HuffmanEncodeSpec_Y_AC);
      std::free(HuffmanEncodeSpec_UV_DC);
      std::free(HuffmanEncodeSpec_UV_AC);
      
      mxm::terminal("failed to set up IPP encoder tables!",
                    this);throw std::bad_alloc();
    }
  
  JPEGReceiver        = 0;
  CoeffBuffer         = 0;
  TileNumX = TileNumY = 0;
  FramesProcessed     = 0;
  ErrorEncountered    = false;
  
  // init APP0 - taken from Klaus' jp-hdr.c (20050422)...
  APP0_Block.ff          = 0xff;
  APP0_Block.app0_marker = mxm::JPEG::APP0;
  APP0_Block.len_hi      = (sizeof(mxm::JPEG::MarkerLayoutAPP0) - 2) / 256;
  APP0_Block.len_lo      = (sizeof(mxm::JPEG::MarkerLayoutAPP0) - 2) % 256;
  std::strcpy(APP0_Block.id, "JFIF"); 
  APP0_Block.major_vers  = 0x01; 
  APP0_Block.minor_vers  = 0x01;
  APP0_Block.units       = 0x00;
  APP0_Block.xdpu_hi     = 0;
  APP0_Block.xdpu_lo     = 1;
  APP0_Block.ydpu_hi     = 0;
  APP0_Block.ydpu_lo     = 1;
  APP0_Block.thumbnail_x = 0;
  APP0_Block.thumbnail_y = 0;
  
  // init SOF0, image size is zeroed out...
  SOF0_Block.ff                         = 0xff;
  SOF0_Block.marker                     = mxm::JPEG::SOF0;
  SOF0_Block.len_hi           = (sizeof(mxm::JPEG::MarkerLayoutSOF0) - 2) / 256;
  SOF0_Block.len_lo           = (sizeof(mxm::JPEG::MarkerLayoutSOF0) - 2) % 256;
  SOF0_Block.data_precision             = 0x08;
  SOF0_Block.y_sz_hi                    = 0;
  SOF0_Block.y_sz_lo                    = 0;
  SOF0_Block.x_sz_hi                    = 0;
  SOF0_Block.x_sz_lo                    = 0;
  SOF0_Block.num_components             = 3;
  SOF0_Block.components[0].component_id = 1;   // Y
  SOF0_Block.components[0].sampl_fact   = 0x22;
  SOF0_Block.components[0].quant_tbl_no = 0;
  SOF0_Block.components[1].component_id = 2;   // U
  SOF0_Block.components[1].sampl_fact   = 0x11;
  SOF0_Block.components[1].quant_tbl_no = 1;
  SOF0_Block.components[2].component_id = 3;   // V
  SOF0_Block.components[2].sampl_fact   = 0x11;
  SOF0_Block.components[2].quant_tbl_no = 1;
  
  // init SOF0, image size is zeroed out...
  SOS_Block.ff                    = 0xff;
  SOS_Block.sos_marker            = mxm::JPEG::SOS;
  SOS_Block.sz_hi              = (sizeof(mxm::JPEG::MarkerLayoutSOS) - 2) / 256;
  SOS_Block.sz_lo              = (sizeof(mxm::JPEG::MarkerLayoutSOS) - 2) % 256;
  SOS_Block.comps_in_scan         = 3;
  SOS_Block.components[0].comp_id = 1;
  SOS_Block.components[0].td_ta   = 0x00;
  SOS_Block.components[1].comp_id = 2;
  SOS_Block.components[1].td_ta   = 0x11;
  SOS_Block.components[2].comp_id = 3;
  SOS_Block.components[2].td_ta   = 0x11;
  SOS_Block.Ss                    = 0x00;
  SOS_Block.Se                    = 0x3f;
  SOS_Block.Ah_Al                 = 0x00;
}


mx::MxPEGScanToJPEGConverterIPP
 ::~MxPEGScanToJPEGConverterIPP() {
  
  for(int i = 0; i < 4; i++)
   delete[] ForwardQuantizationTables[i];
  
  std::free(HuffmanEncodeSpec_Y_DC);
  std::free(HuffmanEncodeSpec_Y_AC);
  std::free(HuffmanEncodeSpec_UV_DC);
  std::free(HuffmanEncodeSpec_UV_AC);
  
  delete[] CoeffBuffer;
  
  delete JPEGReceiver;
}



mxm::smart<mxmStringList> mx::MxPEGScanToJPEGConverterIPP
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::MxPEGScanToJPEGConverterIPP
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::MxPEGScanToJPEGConverterIPP
      ::receiveUndecodedMxPEGFrame(mxmUndecodedMxPEGFrameDescriptor
                                    *frame_descriptor)              {
  
  // guard against missing receiver
  if(!JPEGReceiver) {
    
    setErrorState();
    return;
  }
  
  // map core-managed tables...
  if(!mapTables(frame_descriptor)) {
    
    setErrorState();
    return;
  }
  // map our own forward quantization tables...
  switch(frame_descriptor->Sampling) {
    
    case mxmUndecodedMxPEGFrameDescriptor::Y21U11V11:        // some Axis models
      
      ForwardQuantTableForMCU[0] = ForwardQuantizationTables[frame_descriptor
                                                             ->ComponentInfos[0]
                                                                .QuantTable];
      ForwardQuantTableForMCU[1] = ForwardQuantTableForMCU[0];
      ForwardQuantTableForMCU[2] = ForwardQuantizationTables[frame_descriptor
                                                             ->ComponentInfos[1]
                                                                .QuantTable];
      ForwardQuantTableForMCU[3] = ForwardQuantizationTables[frame_descriptor
                                                             ->ComponentInfos[2]
                                                                .QuantTable];
      break;
      
      
    case mxmUndecodedMxPEGFrameDescriptor::Y22U11V11:                      // MX
    default:
      
      ForwardQuantTableForMCU[0] = ForwardQuantizationTables[frame_descriptor
                                                             ->ComponentInfos[0]
                                                                .QuantTable];
      ForwardQuantTableForMCU[1] = ForwardQuantTableForMCU[0];
      ForwardQuantTableForMCU[2] = ForwardQuantTableForMCU[0];
      ForwardQuantTableForMCU[3] = ForwardQuantTableForMCU[0];
      ForwardQuantTableForMCU[4] = ForwardQuantizationTables[frame_descriptor
                                                             ->ComponentInfos[1]
                                                                .QuantTable];
      ForwardQuantTableForMCU[5] = ForwardQuantizationTables[frame_descriptor
                                                             ->ComponentInfos[2]
                                                                .QuantTable];
      break;
  }
  
  for(int i = 0; i < MCUsPerTile; i++) {
    
    if(!ForwardQuantTableForMCU[i]) {
      
      setErrorState();
      return;
    }
  }
  
  Ipp16s *coeff_buffer,
         *coeff_buffer2;
  Ipp16s uv_tmp_buffer[64];
  int detected_marker = 0;
  Ipp32u prefetched_bits = 0;
  int prefetched_bit_num = 0;
  int pos = 0;
  Ipp8u bitstream_buffer[BitstreamBufferSize];
  int bitstream_pos = 0;
  IppStatus ipp_status;
  bool use_bitmask;
  int tilebits_left;
  unsigned char *tilebit_ptr,
                tilebit_mask;
  int x, y;
  bool next_tile_right_here;
  int i, j;
  
  if(   (frame_descriptor->TileNumX < 1)
     || (frame_descriptor->TileNumY < 1)) {
    
    setErrorState();
    return;
  }
  
  // check coeff buffer and its dimensions...
  if(   (frame_descriptor->TileNumX != TileNumX)
     || (frame_descriptor->TileNumY != TileNumY)) {
  
    delete[] CoeffBuffer;
    
    CoeffBuffer = new Ipp16s[frame_descriptor->TileNumX
                              * frame_descriptor->TileNumY
                              * 6 * 64];
    TileNumX = frame_descriptor->TileNumX;
    TileNumY = frame_descriptor->TileNumY;
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           "reallocated coeff buffer",
                           this);
  }
  
  // decide for bitmask mode...
  tilebits_left = frame_descriptor->TileNumX * frame_descriptor->TileNumY;
  tilebit_ptr   = frame_descriptor->TileBits;
  tilebit_mask  = 128;
  x = -1;
  y = 0;
  ipp_status = ippStsNoErr;
  next_tile_right_here = true;   // default for bitmask-less mode
  if(tilebit_ptr) 
    use_bitmask = true;
  else
    use_bitmask = false;
  
  if(   frame_descriptor->Sampling
     == mxmUndecodedMxPEGFrameDescriptor::Y21U11V11) {
    
    if(use_bitmask) {
      
      mxm::sendStatusMessage(mxm::WarningMessage,
                             "bitmask can not be used in Y21U11V11 "
                              "sampling mode!",
                             this);
      setErrorState();
      return;
    }
    
    tilebits_left *= 2;
  }
  
  // iterate over all tiles (MCUs)...
  while(tilebits_left) {
    
    // advance target tile coordinates...
    x++;
    if(x == frame_descriptor->TileNumX) {
      x = 0;
      y++;
    }
    
    // if in bitmask mode, consume and honor current tile bit...
    if(use_bitmask) {
      next_tile_right_here = (*tilebit_ptr & tilebit_mask) != 0;
      tilebit_mask /= 2;
      if(!tilebit_mask) {
        tilebit_mask = 128;
        tilebit_ptr++;
      }
    }
    
    if(next_tile_right_here) {   // decode tile...
      
      // iterate over the sub-MCUs...
      for(i = 0; i < MCUsPerTile; i++) {
        
        // determine coeff target...
        
        if(   frame_descriptor->Sampling
           == mxmUndecodedMxPEGFrameDescriptor::Y21U11V11) {
          
          if(!(y % 2)) {   // upper halves
            
            if(i < 2)
             coeff_buffer
              = &CoeffBuffer[y/2 * TileNumX * 6*64  +  x * 6*64  +  i * 64];
            else
             coeff_buffer
              = &CoeffBuffer[y/2 * TileNumX * 6*64  +  x * 6*64  +  (i+2) * 64];
          }
          else {           // lower halves
            
            if(i < 2) {
              
              coeff_buffer
              = &CoeffBuffer[y/2 * TileNumX * 6*64  +  x * 6*64  +  (i+2) * 64];
            }
            else {
              coeff_buffer = uv_tmp_buffer;
              coeff_buffer2
              = &CoeffBuffer[y/2 * TileNumX * 6*64  +  x * 6*64  +  (i+2) * 64];
            }
          }
        }
        else {
          
          coeff_buffer
           = &CoeffBuffer[y * TileNumX * 6*64  +  x * 6*64  +  i * 64];
        }
        
        ipp_status = ippiDecodeHuffman8x8_Direct_JPEG_1u16s_C1(
                      frame_descriptor->ScanBytes, &pos,
                      coeff_buffer,
                      LastHuffmanDCPtrForMCU[i],
                      &detected_marker,
                      &prefetched_bits, &prefetched_bit_num,
                      HuffmanDCForMCU[i], HuffmanACForMCU[i]);
        
        if(ipp_status != ippStsNoErr) {
          
          setErrorState();
          return;
        }
        
        // got the sub-MCU
        
        if(   (   frame_descriptor->Sampling
               == mxmUndecodedMxPEGFrameDescriptor::Y21U11V11)
           && (i > 1))                                         {
          
          // Converting from the Y21U11V11 to the Y22U11V11 (MX) sampling mode
          // requires us, per 16x16 tile, to convert 8 sub-MCUs (YYUVYYUV) into
          // 6 sub-MCUs (YYYYUV). Thus, for the U and V channels repectively, we
          // need to merge two sub-MCUs into one. Since we will interpolate in
          // location space, we need to perform dequantization and inverse DCT
          // for the U and V channels prior to the mixing. Afterwards we will
          // need to perform forward DCT and quantization on the resulting U and
          // V sub-MCUs to bring them back into the format that the JPEG writing
          // code expects.
          
          if(y % 2) {   // lower halves
            
            Ipp8u yuv_tmp_buffer[64];
            
            // get U and V data down into location space...
            ippiDCTQuantInv8x8LS_JPEG_16s8u_C1R(coeff_buffer2,
                                                YUV_TargetPtrs[i],
                                                YUV_TargetSteps[i],
                                                QuantTableForMCU[i]);
            ippiDCTQuantInv8x8LS_JPEG_16s8u_C1R(uv_tmp_buffer,
                                                yuv_tmp_buffer,
                                                YUV_TargetSteps[i],
                                                QuantTableForMCU[i]);
            
            // U & V: mix two sub-MCUs together into one (in location space)...
            
            Ipp8u *target_ptr = YUV_TargetPtrs[i];
            
            Ipp8u *mix_src_ptr1   = YUV_TargetPtrs[i],             // upper half
                  *mix_src_ptr2   = mix_src_ptr1 + 8;
            for(int l = 0; l < 4; l++) {
              
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              
              mix_src_ptr1 += 8;
              mix_src_ptr2 += 8;
            }
            
            mix_src_ptr1   = yuv_tmp_buffer;                       // lower half
            mix_src_ptr2   = mix_src_ptr1 + 8;
            for(int l = 0; l < 4; l++) {
              
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              *target_ptr++ = (*mix_src_ptr1++ >> 1) + (*mix_src_ptr2++ >> 1);
              
              mix_src_ptr1 += 8;
              mix_src_ptr2 += 8;
            }
            
            // get resulting sub-MCU back into frequency space, as it is
            // expected by the JPEG writing code below...
            ippiDCTQuantFwd8x8LS_JPEG_8u16s_C1R(YUV_TargetPtrs[i],
                                                YUV_TargetSteps[i],
                                                coeff_buffer2,
                                                ForwardQuantTableForMCU[i]);
          }
        }
      }
      
      // got all sub-MCUs!
    }
    
    tilebits_left--;
  }
  
  // ok, got a frame, write some JPEG...
  
  const IppiEncodeHuffmanSpec *dc_huffman_encode_specs[6]
                                = { HuffmanEncodeSpec_Y_DC,
                                    HuffmanEncodeSpec_Y_DC,
                                    HuffmanEncodeSpec_Y_DC,
                                    HuffmanEncodeSpec_Y_DC,
                                    HuffmanEncodeSpec_UV_DC,
                                    HuffmanEncodeSpec_UV_DC,
                                  };
  const IppiEncodeHuffmanSpec *ac_huffman_encode_specs[6]
                                = { HuffmanEncodeSpec_Y_AC,
                                    HuffmanEncodeSpec_Y_AC,
                                    HuffmanEncodeSpec_Y_AC,
                                    HuffmanEncodeSpec_Y_AC,
                                    HuffmanEncodeSpec_UV_AC,
                                    HuffmanEncodeSpec_UV_AC,
                                  };
  Ipp16s last_y_dc = 0, last_u_dc = 0, last_v_dc = 0;
  Ipp16s *last_dc_ptrs[6] = { &last_y_dc, &last_y_dc, &last_y_dc, &last_y_dc,
                              &last_u_dc, &last_v_dc
                            };
  int tiles_to_write;
  Ipp16s *sub_mcu_coeffs;
  mxm::u8 quantization_table_to_write[64];
  Ipp16u *quant_coeff;
  const unsigned char *huffman_table_to_write;
  int                  huffman_table_to_write_size;
  mxm::u8 write_buffer[10];
  
  // write SOI...
  write_buffer[0] = 0xff;
  write_buffer[1] = mxm::JPEG::SOI;
  JPEGReceiver->receiveStreamBytes(write_buffer, 2);
  
  // write APP0...
  JPEGReceiver->receiveStreamBytes((mxm::u8 *)&APP0_Block,
                                   sizeof(mxm::JPEG::MarkerLayoutAPP0));
  
  // write quantization tables, Y(0) and UV(1)...
  Ipp16u *y_quant_table,
         *uv_quant_table;
  switch(frame_descriptor->Sampling) {
    
    case mxmUndecodedMxPEGFrameDescriptor::Y21U11V11:
      y_quant_table  = QuantTableForMCU[0];
      uv_quant_table = QuantTableForMCU[2];
      break;
      
    case mxmUndecodedMxPEGFrameDescriptor::Y22U11V11:
    default:
      y_quant_table  = QuantTableForMCU[0];
      uv_quant_table = QuantTableForMCU[4];
      break;
  }
  for(i = 0; i < 2; i++) {   // got two of them
    
    write_buffer[0] = 0xff;
    write_buffer[1] = mxm::JPEG::DQT;
    write_buffer[2] = 0;
    write_buffer[3] = 67;
    write_buffer[4] = i;
    JPEGReceiver->receiveStreamBytes(write_buffer, 5);
  
    if(!i) quant_coeff = y_quant_table;
    else   quant_coeff = uv_quant_table;
    for(j = 0; j < 64; j++) 
      quantization_table_to_write[ZigZag_StoreMap[j]] = *quant_coeff++;
    JPEGReceiver->receiveStreamBytes(quantization_table_to_write, 64);
  }
  
  // write SOF0...
  SOF0_Block.x_sz_hi = frame_descriptor->Width / 256;
  SOF0_Block.x_sz_lo = frame_descriptor->Width % 256;
  SOF0_Block.y_sz_hi = frame_descriptor->Height / 256;
  SOF0_Block.y_sz_lo = frame_descriptor->Height % 256;
  JPEGReceiver->receiveStreamBytes((mxm::u8 *)&SOF0_Block,
                                   sizeof(mxm::JPEG::MarkerLayoutSOF0));
  
  // write huffman tables...
  for(i = 0; i < 4; i++) {   // got four of them
  
    write_buffer[0] = 0xff;
    write_buffer[1] = mxm::JPEG::DHT;
    if(i == 0) {
      write_buffer[4] = 0x00;
      huffman_table_to_write      = HuffmanTable_Y_DC;
      huffman_table_to_write_size = sizeof(HuffmanTable_Y_DC);
    }
    else if(i == 1) {
      write_buffer[4] = 0x10;
      huffman_table_to_write      = HuffmanTable_Y_AC;
      huffman_table_to_write_size = sizeof(HuffmanTable_Y_AC);
    }
    else if(i == 2) {
      write_buffer[4] = 0x01;
      huffman_table_to_write      = HuffmanTable_UV_DC;
      huffman_table_to_write_size = sizeof(HuffmanTable_UV_DC);
    }
    else {
      write_buffer[4] = 0x11;
      huffman_table_to_write      = HuffmanTable_UV_AC;
      huffman_table_to_write_size = sizeof(HuffmanTable_UV_AC);
    }
    write_buffer[2] = (huffman_table_to_write_size + 3) / 256;
    write_buffer[3] = (huffman_table_to_write_size + 3) % 256;
  
    JPEGReceiver->receiveStreamBytes(write_buffer, 5);
    JPEGReceiver->receiveStreamBytes((mxm::u8 *)huffman_table_to_write,
                                     huffman_table_to_write_size);
  }
  
  // write SOS...
  JPEGReceiver->receiveStreamBytes((mxm::u8 *)&SOS_Block, 
                                    sizeof(mxm::JPEG::MarkerLayoutSOS));
  
  // write out scan...
  last_y_dc = last_u_dc = last_v_dc = 0;
  sub_mcu_coeffs = CoeffBuffer;
  bitstream_pos  = 0;
  tiles_to_write = TileNumX * TileNumY;
  while(tiles_to_write) {
    
    // iterate over sub-MCUs of current tile...
    for(i = 0; i < 6; i++) {
      
      ippiEncodeHuffman8x8_Direct_JPEG_16s1u_C1(sub_mcu_coeffs,
                                                bitstream_buffer,
                                                &bitstream_pos,
                                                last_dc_ptrs[i],
                                                dc_huffman_encode_specs[i],
                                                ac_huffman_encode_specs[i]);
      
      if(bitstream_pos >= 8) {
        
        JPEGReceiver->receiveStreamBytes(bitstream_buffer, bitstream_pos/8);
        
        // copy down partially filled byte at the end...
        bitstream_buffer[0] = bitstream_buffer[bitstream_pos/8];
        bitstream_pos = bitstream_pos % 8;
      }
      
      sub_mcu_coeffs += 64;
    }
    
    tiles_to_write--;
  }
  
  // deal with last byte
  JPEGReceiver->receiveStreamBytes(bitstream_buffer, 1);     // pad unused bits?
  
  // write EOI...
  write_buffer[0] = 0xff;
  write_buffer[1] = mxm::JPEG::EOI;
  JPEGReceiver->receiveStreamBytes(write_buffer, 2);
  
  JPEGReceiver->newStreamSegment();
  
  FramesProcessed++;
}


void mx::MxPEGScanToJPEGConverterIPP
      ::setQuantizationTable(int target_table,
                             mxm::u8 *data_bytes) {
  
  MxPEGScanDecoderIPPCore::setQuantizationTable(target_table,
                                                data_bytes);
  
  mxm::clamp_int(&target_table, 0, 3);
  
  delete[] ForwardQuantizationTables[target_table];
  
  Ipp16u *s16_ptr;
  ForwardQuantizationTables[target_table] = s16_ptr = new Ipp16u[64];
  ippiQuantFwdTableInit_JPEG_8u16u(data_bytes,
                                   s16_ptr);
}


void mx::MxPEGScanToJPEGConverterIPP
      ::receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                               const mxm::u8 *sample_data)          {
}


void mx::MxPEGScanToJPEGConverterIPP
      ::setSegmentedStreamReceiver(ISegmentedStreamReceiver *receiver) {
  
  delete JPEGReceiver;
  
  JPEGReceiver = receiver;
}


bool mx::MxPEGScanToJPEGConverterIPP::errorState() {
  
  return(ErrorEncountered);
}


void mx::MxPEGScanToJPEGConverterIPP::setErrorState() {
  
  ErrorEncountered = true;
  
  if(JPEGReceiver) JPEGReceiver->setErrorState();
}
