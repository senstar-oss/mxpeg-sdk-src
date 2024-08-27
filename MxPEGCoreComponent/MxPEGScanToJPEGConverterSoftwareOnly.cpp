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
//  $LastChangedDate: 2007-06-28 11:26:00 +0200 (Thu, 28 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/MxPEGScanToJPEGConverterSoftwareOnly.cpp $
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



#include "include/MxPEGScanToJPEGConverterSoftwareOnly.h"

#include <interfaces/ISegmentedStreamReceiver.h>

#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>



const unsigned char mx::MxPEGScanToJPEGConverterSoftwareOnly
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

const mx::MxPEGScanToJPEGConverterSoftwareOnly::HuffCompDC
       mx::MxPEGScanToJPEGConverterSoftwareOnly::HuffTbl_Y_UV_DC[2] = {
  
  {
    { 0, 1, 5, 1, 1, 1, 1, 1, 1, 0,  0,  0, 0, 0, 0, 0 },
     /*!< dc-bits[1..16]: index 0 is missing, 1..16 only ! */
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 },
     /*!< dc-val[0..11]: */
  },
  {
    { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1,  1,  0, 0, 0, 0, 0 },
     /*!< bits_dc[1..16]: index 0 is missing, 1..16 only ! */
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 },
     /*!< val_dc[0..11]: */
  }
};


const mx::MxPEGScanToJPEGConverterSoftwareOnly::HuffCompAC
       mx::MxPEGScanToJPEGConverterSoftwareOnly::HuffTbl_Y_UV_AC[2] = {
  
  {
    { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d },
    { 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
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
    },
  },
  {
    { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 },
    { 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
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
    },
  }
};



mx::MxPEGScanToJPEGConverterSoftwareOnly
 ::MxPEGScanToJPEGConverterSoftwareOnly() {
  
  JPEGReceiver = 0;
  
  int max_size = sizeof(HuffCompDC);
  if(max_size < sizeof(HuffCompAC)) max_size = sizeof(HuffCompAC);
  HuffmanTableWriteBuffer = new mxm::u8[max_size];
  
  // Init the encoder buffer structure ...
  HuffmanEncoderBuffer = new mxm::u8[BitstreamBufferSize];
  initHuffmanEncoderBufferState();
  
  // Create encode huffman tables ...
  calcFoldedHuffTables();
  
  // Init member var ...
  CoeffBuffer            = 0;            // This buffer holds the DCT-JPEG image
  TileNumX = TileNumY    = 0;
  
  FramesProcessed        = 0;
  
  ErrorEncountered       = false;
    
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


mx::MxPEGScanToJPEGConverterSoftwareOnly
 ::~MxPEGScanToJPEGConverterSoftwareOnly() {
  
  delete JPEGReceiver;
  
  delete[] CoeffBuffer;
  delete[] HuffmanEncoderBuffer;
  delete[] HuffmanTableWriteBuffer;
}



mxm::smart<mxmStringList> mx::MxPEGScanToJPEGConverterSoftwareOnly
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::MxPEGScanToJPEGConverterSoftwareOnly
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::MxPEGScanToJPEGConverterSoftwareOnly
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
  
  bool use_bitmask;
  int tilebits_left;
  unsigned char *tilebit_ptr,
                tilebit_mask;
  int x, y;
  bool next_tile_right_here;
  const mxcpcHuffmanTree::Node *huffman_leaf_node;
  int additional_bit_num;
  int ac_coeffs_needed;
  int ac_coeffs_to_output;
  mxm::s16 *coeff_buffer = 0,
           *coeff_buffer2;
  mxm::s16 uv_tmp_buffer[64];
  int coeff_i;
  
  int i, j;
  
  // check coeff buffer and its dimensions...
  if(   (frame_descriptor->TileNumX != TileNumX)
     || (frame_descriptor->TileNumY != TileNumY)) {
  
    delete[] CoeffBuffer;
    
    CoeffBuffer = new mxm::s16[frame_descriptor->TileNumX 
                                * frame_descriptor->TileNumY
                                * 6 * 64
                              ];
    TileNumX = frame_descriptor->TileNumX;
    TileNumY = frame_descriptor->TileNumY;
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           "reallocated coeff buffer",
                           this);
  }
  
  // decide for bitmask mode...
  CurrentScanByte = frame_descriptor->ScanBytes;
  ScanMask        = 128;
  ScanBytesLeft   = frame_descriptor->ScanLength;
  if(!CurrentScanByte || !ScanBytesLeft) {
    setErrorState();
    return;
  }
  tilebits_left = frame_descriptor->TileNumX * frame_descriptor->TileNumY;
  tilebit_ptr   = frame_descriptor->TileBits;
  tilebit_mask  = 128;
  x = -1;
  y = 0;
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
        
        // --- decode the DC coeff... ---
        
        // recognize Huffman codeword...
        huffman_leaf_node = consumeHuffmanCodeword(HuffmanDCRootForMCU[i]);
        if(!huffman_leaf_node) {
          
          mxm::sendStatusMessage(mxm::WarningMessage,
                                 "huffman_leaf_node DC == 0",
                                 this);
          setErrorState();
          return;
        }
        
        // deal with additional bits to be read for this coeff...
        additional_bit_num = (int)(  (unsigned int)huffman_leaf_node->Value
                                   & (unsigned int)0x0f);
        if(additional_bit_num) {
          
          if(consumeAdditionalBits(additional_bit_num)) {
            
            mxm::sendStatusMessage(mxm::WarningMessage,
                                   "return val consumeAdditionalBits() == 0",
                                   this);
            setErrorState();
            return;
          }
          coeff_buffer[0] = CoeffInAdditionalBits;
        }
        else {
          
          coeff_buffer[0] = 0;
        }
        
        // do delta stuff...
        coeff_buffer[0] += *LastHuffmanDCPtrForMCU[i];
        *LastHuffmanDCPtrForMCU[i] = coeff_buffer[0];
        
        // --- decode 63 AC coeffs... ---
        coeff_i = 1;
        ac_coeffs_needed = 63;
        while(ac_coeffs_needed) {
          
          // recognize Huffman codeword...
          huffman_leaf_node = consumeHuffmanCodeword(HuffmanACRootForMCU[i]);
          if(!huffman_leaf_node) {
            
            mxm::sendStatusMessage(mxm::WarningMessage,
                                   "huffman_leaf_node AC == 0",
                                   this);
            setErrorState();
            return;
          }
          
          // check for EOB...
          if(huffman_leaf_node->Value == 0x00) {
            
            while(ac_coeffs_needed) {   // zero out the remaining matrix cells
              
              coeff_buffer[ZigZagMap[coeff_i]] = 0;
              coeff_i++;
              ac_coeffs_needed--;
            }
            continue;   // ac_coeffs_needed is 0
          }
          
          // deal with additional bits to be read for this coeff...
          additional_bit_num = (int)(  (unsigned int)huffman_leaf_node->Value
                                     & (unsigned int)0x0f);
          if(additional_bit_num) {
            
            if(consumeAdditionalBits(additional_bit_num)) {
              
              mxm::sendStatusMessage(mxm::WarningMessage,
                                     "return val consumeAdditionalBits() == 0",
                                     this);
              setErrorState();
              return;
            }
          }
          else {
            
            CoeffInAdditionalBits = 0;
          }
          
          // deal with AC coeffs to output and check break condition...
          ac_coeffs_to_output = huffman_leaf_node->Value/16 + 1;
          if(ac_coeffs_to_output > ac_coeffs_needed) {
            
            mxm::sendStatusMessage(mxm::WarningMessage,
                                   "too many AC coeffs!",
                                   this);
            setErrorState();
            return;
          }
          for(j = 0; j < ac_coeffs_to_output-1; j++) {   // fill zeroes...
            
            coeff_buffer[ZigZagMap[coeff_i]] = 0;
            coeff_i++;
          }
          coeff_buffer[ZigZagMap[coeff_i]] = CoeffInAdditionalBits;
          coeff_i++;
          if(coeff_i > 64) {
            
            mxm::sendStatusMessage(mxm::WarningMessage,
                                   "coeff buffer overflow!",
                                   this);
            setErrorState();
            return;
          }
          
          ac_coeffs_needed -= ac_coeffs_to_output;
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
          
          // perform dequantization...
          mxm::s16 *quant_src_ptr  = QuantTableForMCU[i],
                   *quant_dest_ptr = coeff_buffer;
          int k = 64;
          while(k) {
            
            *quant_dest_ptr++ = *quant_dest_ptr * *quant_src_ptr++;
            k--;
          }
          
          // perform inverse discrete cosine transform...
          idct((short *)coeff_buffer);
          
          // +128 level shift skipped
          
          if(y % 2) {   // lower halves
            
            // U & V: mix two sub-MCUs together into one (in location space),
            // then re-do DCT and quantization stuff...
            
            mxm::s16 *target_ptr = coeff_buffer2;
            
            mxm::s16 *mix_src_ptr1   = coeff_buffer2,              // upper half
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
            
            mix_src_ptr1   = uv_tmp_buffer;                        // lower half
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
            
            // -128 level shift skipped
            
            // perform forward discrete cosine transform...
            dct((short *)coeff_buffer2);
            
            // perform quantization...
            quant_src_ptr  = QuantTableForMCU[i],
            quant_dest_ptr = coeff_buffer2;
            k = 64;
            while(k) {
              *quant_dest_ptr++ = *quant_dest_ptr / *quant_src_ptr++;
              k--;
            }
          }
        }
      }
      
      // got all sub-MCUs!
    }
    
    tilebits_left--;
  }
  
  // ok, we've got a frame, generate some JPEG...
  
  // for handling the predictively coded dc ...
  mxm::s16 dc_coeffs[3]      = { 0, 0, 0} ;
  mxm::s16 *dc_coeff_ptrs[6] = { &dc_coeffs[0],
                                 &dc_coeffs[0],
                                 &dc_coeffs[0],
                                 &dc_coeffs[0],
                                 &dc_coeffs[1],
                                 &dc_coeffs[2]
                               };
  mxm::s16 *sub_mcu_coeffs;
  mxm::u8 quantization_table_to_write[64];
  mxm::s16 *quant_coeff;
  mxm::u8 write_buffer[10];
  int tiles_to_write;
  
  // write SOI...
  write_buffer[0] = 0xff;
  write_buffer[1] = mxm::JPEG::SOI;
  JPEGReceiver->receiveStreamBytes(write_buffer, 2);
  
  // write APP0...
  JPEGReceiver->receiveStreamBytes((mxm::u8 *)&APP0_Block, 
                                   sizeof(mxm::JPEG::MarkerLayoutAPP0));
  
  // write quantization tables, Y(0) and UV(1)...
  mxm::s16 *y_quant_table,
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
     quantization_table_to_write[ZigZag_StoreMap[j]] 
      = ((mxm::u8)*quant_coeff++);
    JPEGReceiver->receiveStreamBytes(quantization_table_to_write, 64);
  }
  
  // write SOF0...
  SOF0_Block.x_sz_hi = frame_descriptor->Width  / 256;
  SOF0_Block.x_sz_lo = frame_descriptor->Width  % 256;
  SOF0_Block.y_sz_hi = frame_descriptor->Height / 256;
  SOF0_Block.y_sz_lo = frame_descriptor->Height % 256;
  JPEGReceiver->receiveStreamBytes((mxm::u8 *)&SOF0_Block,
                                   sizeof(mxm::JPEG::MarkerLayoutSOF0));
  
  // write Huffman tables...
  {
    int huffman_table_to_write_size;
    int val_size;
    
    for(i = 0; i < 4; i++) {   // got four of them  
      
      write_buffer[0] = 0xff;
      write_buffer[1] = mxm::JPEG::DHT;
      if(i == 0) {
        write_buffer[4] = 0x00;
        val_size = sizeof(HuffTbl_Y_UV_DC[0].val_dc);
        std::memcpy(HuffmanTableWriteBuffer,
                    HuffTbl_Y_UV_DC[0].bits_dc,
                    16);
        std::memcpy(HuffmanTableWriteBuffer + 16,
                    HuffTbl_Y_UV_DC[0].val_dc,
                    val_size);
        huffman_table_to_write_size = 16 + val_size;
      }
      else if(i == 1) {
        write_buffer[4] = 0x10;
        val_size = sizeof(HuffTbl_Y_UV_AC[0].val_ac);
        std::memcpy(HuffmanTableWriteBuffer,
                    HuffTbl_Y_UV_AC[0].bits_ac,
                    16);
        std::memcpy(HuffmanTableWriteBuffer + 16,
                    HuffTbl_Y_UV_AC[0].val_ac,
                    val_size);
        huffman_table_to_write_size = 16 + val_size;
      }
      else if(i == 2) {
        write_buffer[4] = 0x01;
        val_size = sizeof(HuffTbl_Y_UV_DC[1].val_dc);
        std::memcpy(HuffmanTableWriteBuffer,
                    HuffTbl_Y_UV_DC[1].bits_dc,
                    16);
        std::memcpy(HuffmanTableWriteBuffer + 16,
                    HuffTbl_Y_UV_DC[1].val_dc,
                    val_size);
        huffman_table_to_write_size = 16 + val_size;
      }
      else {
        write_buffer[4] = 0x11;
        val_size = sizeof(HuffTbl_Y_UV_AC[1].val_ac);
        std::memcpy(HuffmanTableWriteBuffer,
                    HuffTbl_Y_UV_AC[1].bits_ac,
                    16);
        std::memcpy(HuffmanTableWriteBuffer + 16,
                    HuffTbl_Y_UV_AC[1].val_ac,
                    val_size);
        huffman_table_to_write_size = 16 + val_size;
      }
      write_buffer[2] = (huffman_table_to_write_size + 3) / 256;
      write_buffer[3] = (huffman_table_to_write_size + 3) % 256;
      JPEGReceiver->receiveStreamBytes(write_buffer, 5);
      JPEGReceiver->receiveStreamBytes(HuffmanTableWriteBuffer,
                                       huffman_table_to_write_size);
    }
  }
  
  // write SOS...
  JPEGReceiver->receiveStreamBytes((mxm::u8 *)&SOS_Block, 
                                   sizeof(mxm::JPEG::MarkerLayoutSOS));
  
  // define huffman code table list ..
  FoldedHuffTbl_DC_AC* huff_code_table_list[6];
  
  // Y -Table ...
  huff_code_table_list[0] = &Huff_Y_UV[0];
  huff_code_table_list[1] = &Huff_Y_UV[0];
  huff_code_table_list[2] = &Huff_Y_UV[0];
  huff_code_table_list[3] = &Huff_Y_UV[0];
  
  // UV -Table
  huff_code_table_list[4] = &Huff_Y_UV[1];
  huff_code_table_list[5] = &Huff_Y_UV[1];
  
  // write out scan...
  sub_mcu_coeffs = CoeffBuffer;
  tiles_to_write = TileNumX * TileNumY;
  
  int nb_of_bytes_to_write = 0;
  while(tiles_to_write) {
    
    // iterate over sub-MCUs of current tile YYYYUV ...
    for(i = 0; i < 6; i++) {
      
      encodeSubMCUTiles(sub_mcu_coeffs,
                        dc_coeff_ptrs[i],
                        huff_code_table_list[i],
                        &HuffmanBufferState);
      sub_mcu_coeffs += 64;
    }
    
    tiles_to_write--;
  }
  
  // write last byte...
  shutDownHuffAccu();
  
  // pump out data ...
  nb_of_bytes_to_write
   = HuffmanBufferState.buf_current_w_pos - HuffmanBufferState.buf_start;
  JPEGReceiver->receiveStreamBytes(HuffmanBufferState.buf_start,
                                   nb_of_bytes_to_write);
  
  // Reset global encoding buffer ...
  resetHuffmanEncoderBufferState();
  
  // write EOI...
  write_buffer[0] = 0xff;
  write_buffer[1] = mxm::JPEG::EOI;
  JPEGReceiver->receiveStreamBytes(write_buffer, 2);
   
  // ok, we've done it!
  JPEGReceiver->newStreamSegment();
  
  FramesProcessed++;
}


void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                               const mxm::u8 *sample_data)          {
}


void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::setSegmentedStreamReceiver(ISegmentedStreamReceiver *receiver) {
  
  delete JPEGReceiver;
  
  JPEGReceiver = receiver;
}


bool mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::errorState()                          {
  
  return(ErrorEncountered);
}


void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::setErrorState()                       {
  
  ErrorEncountered = true;
  
  if(JPEGReceiver) JPEGReceiver->setErrorState();
}


/*!
 * Calculates huffman tables a la mobotix!
 */
void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::calcFoldedHuffTables(void)            {
  
  int y_uv;
  
  static int firstcall = 0;
  if (firstcall) return;

  firstcall++;

  // Generate two sets of tables, for Y and UV 
  for (y_uv = 0; y_uv < 2; y_uv++) {
    
    /* first generate DC-table */
    generateHuffTables1(
     /* u8 * bits_tbl_off1, entries 0..15 count no. of symbols with 1..16 bits,
        respectively */ 
     &HuffTbl_Y_UV_DC[y_uv].bits_dc[0],
     /* u8 * val_tbl,       table of symbols that need to be encoded, 
        size = sum of entries in bits_tbl_off1 */
     &HuffTbl_Y_UV_DC[y_uv].val_dc[0],
     /* int swap_symbol, set if the swapped symbol is used to access the generated
        folded table (for ac-coefs) */
     0,
     &Huff_Y_UV[y_uv].dc_tbl[0]
    );
    
    /* then AC-table */
    generateHuffTables1(
     /* u8 * bits_tbl_off1, entries 0..15 count no. of symbols with 1..16 bits,
        respectively */ 
     &HuffTbl_Y_UV_AC[y_uv].bits_ac[0],
     /* u8 * val_tbl, table of symbols that need to be encoded, size = sum of 
        entries in bits_tbl_off1 */ 
     &HuffTbl_Y_UV_AC[y_uv].val_ac[0],
     /* int swap_symbol, set if the swapped symbol is used to access the 
        generated folded table (for ac-coefs) */
     1,
     /* jd_huf_code_tbl_entry_t * folded_tbl_p; */
     &Huff_Y_UV[y_uv].ac_tbl[0]
    );
  }
}


/*!
 *  Generate a run-length-coded huffman-code-tree for all symbols in
 *  val_tabl.
 *
 *  The huffman-codes for each symbol are stored in a "folded" table
 *  that can be indexed by a symbol and yields a word that contains
 *  the pre-shifted code in the lower 26 bits and the length of the combination
 *  of the rll-coded symbol and the mantissa in the upper 5 bits.
 *
 *  \param bits_tbl_off1
 *   Entries 0..15 count no. of symbols with 1..16 bits, respectively
 *   \param val_tbl
 *   Table of symbols that need to be encoded, size = sum of entries in
 *   bits_tbl_off1.
 *   \param swap_symbol
 *   set if the swapped symbol is used to access the generated
 *   folded table (for ac-coefs).
 */
void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::generateHuffTables1(const mxm::u8 * bits_tbl_off1,
                            const mxm::u8 * val_tbl,
                            int swap_symbol,
                            HuffCodeTblEntry * folded_tbl_p) {
  
  int len,
      codes_of_len,
      mantissa_len;
  
  int code = 0;
  for(len = 1; len <= 16; len ++) {
  
    for(codes_of_len = 1;
        codes_of_len <= bits_tbl_off1[len - 1];
        codes_of_len++)                         {
      
      int symbol = *val_tbl++;     /* get unswapped symbol from the table in the
                                      Jpeg-header */
      mantissa_len = symbol & 0xf; /* top nibble is run-length,
                                      low nibble is number of significant bits
                                      in coefficient */
      if(swap_symbol) {            /* 0xRS -> 0xSR */
        symbol = (symbol >> 4) + ((symbol & 0xf) << 4);
      }
      folded_tbl_p[symbol].code_msk  = code << mantissa_len;
      folded_tbl_p[symbol].fill_0bit = 0;
      folded_tbl_p[symbol].bit_len   = len + mantissa_len;
      code++;
    }
    
    code <<= 1;
  }
}


/*!
 *  Take care of initialisierung!
 */
void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::initHuffmanEncoderBufferState()       {
  
  HuffmanBufferState.buf_start         = &HuffmanEncoderBuffer[0];
  HuffmanBufferState.buf_current_w_pos = &HuffmanEncoderBuffer[0];
  HuffmanBufferState.buf_end1
   = &HuffmanEncoderBuffer[BitstreamBufferSize + 1];
  
  HuffmanBufferState.huf_accu          = 0;
  HuffmanBufferState.huf_bits_used     = 0;
}


/*!
 *   Reset HuffmanEncoderBufferState in order to fetch next frame!
 */
void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::resetHuffmanEncoderBufferState()      {
  
  HuffmanBufferState.buf_start         = &HuffmanEncoderBuffer[0];
  HuffmanBufferState.buf_current_w_pos = &HuffmanEncoderBuffer[0];
  
  HuffmanBufferState.huf_accu          = 0;
  HuffmanBufferState.huf_bits_used     = 0;
}


/*!
 *  \param coef_matrix
 *  The 8x8 input coefficient matrix!
 *  \param last_dc_coeff
 *  Ptr to a list of the last DC coefficients.
 *  \param huff_code_tbl_entry
 *  A huffman table.
 *  \param hbuf_state_ptr
 *  Ptr to global buffer state.
 */
void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::encodeSubMCUTiles(mxm::s16* coef_matrix,
                          mxm::s16* last_dc_coeff,
                          FoldedHuffTbl_DC_AC* huff_code_tbl_entry,
                          HuffmanBufferStateStructure* hbuf_state_ptr) {
  
      // Fetch settings state from global context!
      unsigned int huff_bits_used = hbuf_state_ptr->huf_bits_used;
      mxm::u32     huff_accu_32   = hbuf_state_ptr->huf_accu;
      u32t union_accu_tmp;
      int total_bits = 0;
      int bits = 0;
      
      /*
       *     Convert coefficients in Huffman codewords ...
       *
       *    ___ DC coefficient
       *    |
       *    v
       *    34 12 3 0 0 5 0 4 0 1 ....
       *    ^        ^
       *    |        |_______ "3" AC coefficients != 0,  
       *    |                 two(R) 0-coefficients before that -> RB = 23
       *    |                 Bitlength(B) from 5=101_b = 3
       *    |
       *    |
       *    |___ "1" AC coefficient
       */
      
      //------------------------------------------------------------------------
      // first start encoding DC component...
      //------------------------------------------------------------------------
      
      // Container for the current huffman codeword indexed by the value of
      // delta.
      mxm::u32 dc_encoded_word;
      // First element in list is always the DC coefficient!
      mxm::s16 delta = coef_matrix[0] - (*last_dc_coeff);
      *last_dc_coeff = coef_matrix[0];
      
      // Delta should be in boundaries ...
      if((delta < -2046) || (delta > 2046)) {
        
        mxm::terminal("out of range", this);
      }
      
      /* 0000 bbbb , 1 <= b <= 10 */
      if(delta == 0) bits = 0;
      if(delta  > 0) bits = returnBitsInCoef( delta);
      if(delta  < 0) bits = returnBitsInCoef(-delta);
      
      union_accu_tmp.table_entry = (*huff_code_tbl_entry).dc_tbl[bits];
      dc_encoded_word = union_accu_tmp.table_entry.code_msk
                         + (union_accu_tmp.table_entry.bit_len << 27);
      
      /* build one's complement */
      if(delta  < 0) delta = (delta  -1 ) & (0x7fffffff >> (31 - bits));
        /* 1111 1011 -> ____ _010 */
      
      /* tttt.t0hh_hhhh.hhhh_hhhh.hhoo.oooo.oooo 
       *    ^      ^                   ^
       *    |      |                   |___ooo = <bits> zeros, will be
       *    |      |                       overwritten with mantissa later
       *        hhhhh=huffman-code
       *
       * tttt.t _____ number of hhh + ooo bits, 
       *              2 <= h <= 16, 0 <= o <= 10, 2 <= ttttt <= 26
       */
      
      dc_encoded_word = dc_encoded_word | delta ;    /* ...000hhhhhccc */
      total_bits   = dc_encoded_word >> 27;
      
      // kill tttt.t(32-total_bits always > 5) and left align the new bit stream
      dc_encoded_word = dc_encoded_word << (32 - total_bits);
        /* hhhh.hccc_0000.0000_0000.0000_0000.000 */
      
      /*   ####.## -> huff_bits_used = 6           */       
      huff_accu_32 = huff_accu_32 | (dc_encoded_word >> huff_bits_used);
       /* aaaa.aaoo_oooo.oooo_oooo.oooo_oooo.oooo *
        * | 0000.00hh_hhhc.cc00_0000.0000_0000.0000 
        * -------------------------------------------
        * = aaaa.aahh_hhhc.cc00_0000.0000_0000.0000
        *   ####.####_####.## -> new huff_bits_used = 6+8 = 14
        */
      
      huff_bits_used += total_bits;
      
      // Special case accu full ...
      if(huff_bits_used >= 32) {
        
        // Write accu (32 bit) to output_bitstream_buff ...
        // this may generate more than 4 bytes becuase of zero-stuffing
        copyAccuToGlobalHuffBuffer(huff_accu_32, hbuf_state_ptr);
        huff_bits_used -= 32;
        huff_accu_32 = dc_encoded_word << (total_bits - huff_bits_used);
         /* (hhhh) <-  hccc.oooo_oooo.oooo_oooo.oooo_oooo.oooo */
      }
      
      //------------------------------------------------------------------------
      // Now start encoding AC coeff...
      //------------------------------------------------------------------------
      
      int run_len = 0;   /* count zeros */
      for(int ac_coef = 1; ac_coef <= 63; ac_coef++) {
        
        mxm::u32 new_bit_accu;   // used to "or" symbol and mantissa, must be
                                 // u32 rather than int, so a
                                 // (new_bit_accu >> n) never shifts in 1 from
                                 // bit 31.
        bits = 0;                // number of bits in coef, 0000 bbbb,
                                 //  1 <= b <= 10 
        int symbol = 0;          // symbol,
                                 // (run_len << 4) | bits = ..00 rrrr bbbb,
                                 // 0 <= rr <= 15
        int c = coef_matrix[ZigZagMap[ac_coef]];
        
        if(c == 0) {
          
          run_len +=1;
        }
        else {
          
          // max number of zeros to encoding table is 15
          
          while(run_len > 15) {
            
            mxm::u32 u32_zero_lenght;
            union_accu_tmp.table_entry = (*huff_code_tbl_entry).ac_tbl[0x0f];
            u32_zero_lenght = union_accu_tmp.table_entry.code_msk
                               + (union_accu_tmp.table_entry.bit_len << 27);
            total_bits = u32_zero_lenght >> 27;
            u32_zero_lenght = u32_zero_lenght << (32 - total_bits);
            huff_accu_32 = huff_accu_32 | ( u32_zero_lenght >> huff_bits_used);
            huff_bits_used += total_bits;
            if(huff_bits_used >= 32) {
              copyAccuToGlobalHuffBuffer(huff_accu_32, hbuf_state_ptr);
                /* ex: huff_bits_used = 4, total_bits=8 */
              huff_bits_used -= 32;
                /* (hhhh) <-  hccc.oooo_oooo.oooo_oooo.oooo_oooo.oooo */
              huff_accu_32 = u32_zero_lenght << (total_bits - huff_bits_used);
            }
            run_len -= 16;
          }
            
          /* 0000 bbbb , 1 <= b <= 10 */
          if(c >= 0)
           bits = returnBitsInCoef(c);
          else
           bits = returnBitsInCoef(-c);
           
          /* symbol, (run_len << 4) | bits = ..00 rrrr bbbb , 0 <= rr <= 15 */
          symbol = (bits << 4) + run_len;
          
          /*   tttt.t0hh_hhhh.hhhh_hhhh.hhoo.oooo.oooo
           *          ^      ^                   ^
           *          |      |                   |___ooo = <bits> zeros, will be
           *          |      |                       overwritten with mantissa
           *          |      |                       later 
           *          hhhhh=huffman-code,
           *   tttt.t _____ number of hhh + ooo bits, 
           *                2 <= h <= 16, 0 <= o <= 10, 
           *                2 <= ttttt <= 26 
           */
          
          union_accu_tmp.table_entry = (*huff_code_tbl_entry).ac_tbl[symbol];
          new_bit_accu = union_accu_tmp.table_entry.code_msk
                          + (union_accu_tmp.table_entry.bit_len << 27);
          
          /* build one's complement */
          if(c < 0) c = (c -1 ) & (0x7fffffff >> (31 - bits));
            /* 1111 1011 -> ____ _010 */
          new_bit_accu = new_bit_accu | c; /* ...000hhhhhccc */
          total_bits = new_bit_accu >> 27; /* total_bits = ..00t.tttt (2..26) */
          
          // kill tttt.t(32-total_bits always > 5) and left align the new bit
          // stream ...
          new_bit_accu = new_bit_accu << (32 - total_bits);
            /*   hhhh.hccc_0000.0000_0000.0000_0000.000 */
          
          /* now we must append the new bit-stream of <total_bits> length in 
             new_bit_accu to the huffman bitstream */
          /*   ####.## -> huff_bits_used = 6 */
          huff_accu_32 = huff_accu_32 | (new_bit_accu >> huff_bits_used);
            /*   aaaa.aaoo_oooo.oooo_oooo.oooo_oooo.oooo *
             * | 0000.00hh_hhhc.cc00_0000.0000_0000.0000
             * -------------------------------------------
             * = aaaa.aahh_hhhc.cc00_0000.0000_0000.0000
             *   ####.####_####.## -> new huff_bits_usded = 6+8 = 14
             */
          
          huff_bits_used += total_bits;
          if(huff_bits_used >= 32) {
            
            copyAccuToGlobalHuffBuffer(huff_accu_32, hbuf_state_ptr);
            huff_bits_used -= 32;
              /* ex: huff_bits_used = 4, total_bits=8 */
            huff_accu_32 = new_bit_accu << (total_bits - huff_bits_used);
              /* (hhhh) <-  hccc.oooo_oooo.oooo_oooo.oooo_oooo.oooo */
          }
            
          run_len = 0;
        }
      }
      
      // write EOB ...
      if(run_len != 0) {
        
        mxm::u32 eob_tag;
        union_accu_tmp.table_entry = (*huff_code_tbl_entry).ac_tbl[0x00];
        eob_tag = union_accu_tmp.table_entry.code_msk
                   + (union_accu_tmp.table_entry.bit_len << 27);
        total_bits = eob_tag >> 27;
        eob_tag = eob_tag << (32 - total_bits);
        huff_accu_32 = huff_accu_32 | (eob_tag >> huff_bits_used);
        huff_bits_used += total_bits;
        if(huff_bits_used >= 32) {
          
          copyAccuToGlobalHuffBuffer(huff_accu_32, hbuf_state_ptr);
          huff_bits_used -= 32;
            /* ex: huff_bits_used = 4, total_bits=8 */
          huff_accu_32 = eob_tag << (total_bits - huff_bits_used);
            /* (hhhh) <-  hccc.oooo_oooo.oooo_oooo.oooo_oooo.oooo */
        }
      }
      
      // update global buffer ..
      hbuf_state_ptr->huf_bits_used = huff_bits_used;
      hbuf_state_ptr->huf_accu      = huff_accu_32;
}


/*!
 *  \param huff_accu_32 
 *  Input word.
 *  \param huff_buf_state_ptr
 *  Ptr. to global buffer.
 */
void mx::MxPEGScanToJPEGConverterSoftwareOnly
      ::copyAccuToGlobalHuffBuffer(mxm::u32 huff_accu_32,
                                   HuffmanBufferStateStructure
                                    *huff_buf_state_ptr)       {
      
      mxm::u8  b;
      mxm::u8* p = huff_buf_state_ptr->buf_current_w_pos;
      
      if((p+8) >= huff_buf_state_ptr->buf_end1) {
        
        mxm::terminal("Huffman buffer overflow!", this);
      }
      
      /* Output four bytes from huff_accu_32 with MSByte first, pad each 0xff
       * with 0x00 (zero-stuffing) so that control-codes beginning with 0xff can
       * be distinguished from bitstream.
       */
      
      b = (huff_accu_32 >> 24);
      *p++ = b;   /* b = 0x76 */
      if(b == 0xff) *p++ = 0x00;
      
      b = (huff_accu_32 >> 16) & 0xff;
      *p++ = b;   /* b = 0x54 */
      if(b == 0xff) *p++ = 0x00;
      
      b = (huff_accu_32 >>  8) & 0xff;
      *p++ = b;   /* b = 0x32 */
      if (b == 0xff) *p++ = 0x00;   
      
      b = (huff_accu_32      ) & 0xff;
      *p++ = b;   /* b = 0x10 */
      if (b == 0xff) *p++ = 0x00;
      huff_buf_state_ptr->buf_current_w_pos = p;
}


int mx::MxPEGScanToJPEGConverterSoftwareOnly
     ::shutDownHuffAccu(void)                {
      
      mxm::u8  b;
      mxm::u8* p = HuffmanBufferState.buf_current_w_pos;
      
      if((p + 8) >= HuffmanBufferState.buf_end1) {
        
        mxm::terminal("Huffman buffer overflow!");
      }
      
      if(   (HuffmanBufferState.huf_bits_used == 2)
         && (HuffmanBufferState.huf_accu == 0))     {
        
        b = 0x0;
        *p++ = b;
        HuffmanBufferState.buf_current_w_pos = p;
      }
      else {
        
        while(HuffmanBufferState.huf_accu) {
          
          b = (HuffmanBufferState.huf_accu >> 24);
          *p++ = b;
          if(b == 0xff) *p++ = 0x00;
          HuffmanBufferState.huf_accu <<= 8;
        }
        
        HuffmanBufferState.buf_current_w_pos = p;
      }
      
      return(0);
}



/*!
 *  \param c
 *  <- this!
 */
int mx::MxPEGScanToJPEGConverterSoftwareOnly
     ::returnBitsInCoef(mxm::s16 c)          {
  
  int bits = 1;
  
  while(c >>= 1 ) bits += 1;
  
  return(bits);
}