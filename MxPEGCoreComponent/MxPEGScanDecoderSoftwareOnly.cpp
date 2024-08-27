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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/MxPEGScanDecoderSoftwareOnly.cpp $
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



#include "include/MxPEGScanDecoderSoftwareOnly.h"

#include <interfaces/IMxPEGTileReceiver.h>

#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>



mx::MxPEGScanDecoderSoftwareOnly::MxPEGScanDecoderSoftwareOnly() {
  
  TileReceiver      = 0;
  Active            = false;
  
  ErrorEncountered  = false;
  
  Width             = -1;
  Height            = -1;
  
  UpperHalfBuffer   = 0;
}


mx::MxPEGScanDecoderSoftwareOnly::~MxPEGScanDecoderSoftwareOnly() {
  
  delete TileReceiver;
  
  delete[] UpperHalfBuffer;
}



mxm::smart<mxmStringList> mx::MxPEGScanDecoderSoftwareOnly
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::MxPEGScanDecoderSoftwareOnly
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::MxPEGScanDecoderSoftwareOnly
      ::receiveUndecodedMxPEGFrame(mxmUndecodedMxPEGFrameDescriptor
                                    *frame_descriptor)              {
  
  if(!Active) activate();
  
  // send frame info and signal resolution change (if applicable)...
  int width  = frame_descriptor->Width,
      height = frame_descriptor->Height;
  int tile_xnum = width / 16;
  if(width % 16) tile_xnum++;
  if(   (width  != Width)
     || (height != Height)) {
    
    Width  = width;
    Height = height;
    TileReceiver->videoResolutionChanged(Width, Height);
    
    delete[] UpperHalfBuffer;
    UpperHalfBuffer = new mxm::s16[tile_xnum * 64*4];
  }
  mxmFrameInfo frame_info(frame_descriptor->TimeStamp,
                          frame_descriptor->GenerationNumber,
                          (mxm::u64)0xffffffff);
  frame_info.setDimensions(width,
                           height);
  TileReceiver->setFrameInfo(frame_info);
  // TODO : check if time stamp, gen_no valid?
  
  // map core-managed tables...
  if(!mapTables(frame_descriptor)) {
    
    ErrorEncountered = true;
    TileReceiver->setErrorState();
    return;
  }
  
  bool use_bitmask;
  int tilebits_left;
  unsigned char *tilebit_ptr, tilebit_mask;
  int x, y;
  bool next_tile_right_here;
  const mxcpcHuffmanTree::Node *huffman_leaf_node;
  int additional_bit_num;
  int ac_coeffs_needed;
  int ac_coeffs_to_output;
  
  mxm::s16 *coeff_buffer;
  mxm::s16 *src_ptr, *dest_ptr;
  int coeff_i;
  int color_y, color_u, color_v,
      color_r, color_g, color_b;
  mxm::u32 *pixel_target;
  int target_row_stepping;
  unsigned char *target_ptr;
  int i, j;
  unsigned int ii, jj;
  
  if(   (frame_descriptor->TileNumX < 1)
     || (frame_descriptor->TileNumY < 1)) {
    
    setErrorState();
    return;
  }
  
  // initialize and decide for bitmask mode...
  CurrentScanByte = frame_descriptor->ScanBytes;
  ScanMask        = 128;
  ScanBytesLeft   = frame_descriptor->ScanLength;
  if(!CurrentScanByte || !ScanBytesLeft) {
    ErrorEncountered = true;
    TileReceiver->setErrorState();
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
      
      if(   (   frame_descriptor->Sampling
             == mxmUndecodedMxPEGFrameDescriptor::Y21U11V11)
         && !(y % 2))
       coeff_buffer = &UpperHalfBuffer[x * 64*4];
      else
       coeff_buffer = &CoeffBuffers[0][0];
      
      // iterate over the sub-MCUs...
      for(i = 0; i < MCUsPerTile; i++) {
        
        // --- decode the DC coeff... ---
        
        // recognize Huffman codeword...
        huffman_leaf_node = consumeHuffmanCodeword(HuffmanDCRootForMCU[i]);
        if(!huffman_leaf_node) {
          ErrorEncountered = true;
          TileReceiver->setErrorState();
          return;
        }
 
        // deal with additional bits to be read for this coeff...
        additional_bit_num = (int)(  (unsigned int)huffman_leaf_node->Value
                                   & (unsigned int)0x0f);
        if(additional_bit_num) {
          if(consumeAdditionalBits(additional_bit_num)) {
            ErrorEncountered = true;
            TileReceiver->setErrorState();
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
            ErrorEncountered = true;
            TileReceiver->setErrorState();
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
              ErrorEncountered = true;
              TileReceiver->setErrorState();
              return;
            }
          }
          else
            CoeffInAdditionalBits = 0;
          
          // deal with AC coeffs to output and check break condition...
          ac_coeffs_to_output = huffman_leaf_node->Value/16 + 1;
          if(ac_coeffs_to_output > ac_coeffs_needed) {
            mxm::sendStatusMessage(mxm::WarningMessage,
                                   "too many AC coeffs!",
                                   this);
            ErrorEncountered = true;
            TileReceiver->setErrorState();
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
            ErrorEncountered = true;
            TileReceiver->setErrorState();
            return;
          }
          
          ac_coeffs_needed -= ac_coeffs_to_output;
        }
        
        // we have all coefficients for this 8x8 coeff block
        
        // perform dequantization...
        src_ptr  = QuantTableForMCU[i];
        dest_ptr = coeff_buffer;
        j = 64;
        while(j) {
          *dest_ptr++ = *dest_ptr * *src_ptr++;
          j--;
        }
        
        // perform inverse discrete cosine transform...
        idct((short *)coeff_buffer);
        
        // perform level shift...
        dest_ptr = coeff_buffer;
        j = 64;
        while(j) {
          mxm::clamp_s16(dest_ptr, -128, 127);
          *dest_ptr++ += (mxm::s16)128;
          j--;
        }
         
        coeff_buffer += 64;
      }
      
      // got all sub-MCUs
      
      // --- finally negotiate with backend and write decoded pixel data... ---
      //     (we're doing the colorspace conversion on the fly)
      
      switch(frame_descriptor->Sampling) {
        
        // uses 8 MCUs per tile...
        case mxmUndecodedMxPEGFrameDescriptor::Y21U11V11:
          
          if(y % 2) {
            
            TileReceiver->provideVideoTilePixelTarget(x, y/2,
                                                      &pixel_target,
                                                      &target_row_stepping);
            target_ptr = (unsigned char *)pixel_target;
            
            for(jj = 0; jj < 16; jj++) {
              
              for(ii = 0; ii < 16; ii++) {
                
                // cunningly, shiftingly, retrieve YUV color values...
                color_y = (int)(*(&CoeffBuffers[0][0]
                                   + ((((jj>>3)<<1) + (ii>>3)) << 6)
                                   + ((jj & 0x07) << 3)
                                   + (ii & 0x07)
                                 )
                               );
                if(jj >> 3) {
                  color_y = (int)(*(CoeffBuffers[0]
                                     + ((ii >> 3) << 6)
                                     + ((jj & 0x07) << 3)
                                     + (ii & 0x07)
                                   )
                                 );
                  color_u = (int)(*(CoeffBuffers[2]
                                     + ((jj >> 3) << 3)
                                     + (ii >> 1)
                                   )
                                 ) - 128;
                  color_v = (int)(*(CoeffBuffers[3]
                                     + ((jj >> 3) << 3)
                                     + (ii >> 1)
                                   )
                                 ) - 128;
                }
                else {
                  color_y = (int)(*(UpperHalfBuffer
                                     + ((unsigned int)x << 8)
                                     + ((ii >> 3) << 6)
                                     + ((jj & 0x07) << 3)
                                     + (ii & 0x07)
                                   )
                                 );
                  color_u = (int)(*(UpperHalfBuffer
                                     + ((unsigned int)x << 8) + 2*64
                                     + ((jj & 0x07) << 3)
                                     + (ii >> 1)
                                   )
                                 ) - 128;
                  color_v = (int)(*(UpperHalfBuffer
                                     + ((unsigned int)x << 8) + 3*64
                                     + ((jj & 0x07) << 3)
                                     + (ii >> 1)
                                   )
                                 ) - 128;
                }
                
                // the coefficients used here result from conversion to our 
                // fixed-point integer arithmetic situation and consequently
                // are unlikely to show up in any literature consulted...
                color_r = ((color_y<<10)                + 1167*color_v) >> 10;
                color_g = ((color_y<<10) -  403*color_u -  595*color_v) >> 10;
                color_b = ((color_y<<10) + 2081*color_u               ) >> 10;
                
                mxm::clamp_int(&color_r, 0, 255);
                mxm::clamp_int(&color_g, 0, 255);
                mxm::clamp_int(&color_b, 0, 255);
                
                *target_ptr++ = color_r;
                *target_ptr++ = color_g;
                *target_ptr++ = color_b;
              }
              
              target_ptr += target_row_stepping*4;
            }
            
            TileReceiver->videoTileTransferred();
          }
          break;
        
        
        // uses 6 MCUs per tile...
        case mxmUndecodedMxPEGFrameDescriptor::Y22U11V11:
        default:
          
          TileReceiver->provideVideoTilePixelTarget(x, y,
                                                    &pixel_target,
                                                    &target_row_stepping);
          target_ptr = (unsigned char *)pixel_target;
          
          for(jj = 0; jj < 16; jj++) {
            for(ii = 0; ii < 16; ii++) {
            
              // cunningly, shiftingly, retrieve YUV color values...
              color_y = (int)(*(&CoeffBuffers[0][0]
                                 + ((((jj>>3)<<1) + (ii>>3)) << 6)
                                 + ((jj & 0x07) << 3)
                                 + (ii & 0x07)
                               )
                             );
              color_u = (int)(*(CoeffBuffers[4]
                                 + ((jj>>1) << 3)
                                 + (ii>>1)
                               )
                             ) - 128;
              color_v = (int)(*(CoeffBuffers[5]
                                 + ((jj>>1) << 3)
                                 + (ii >> 1)
                               )
                             ) - 128;
              
              // the coefficients used here result from conversion to our 
              // fixed-point integer arithmetic situation and consequently are 
              // unlikely to show up in any literature consulted...
              color_r = ((color_y<<10)                + 1167*color_v) >> 10;
              color_g = ((color_y<<10) -  403*color_u -  595*color_v) >> 10;
              color_b = ((color_y<<10) + 2081*color_u               ) >> 10;
              
              mxm::clamp_int(&color_r, 0, 255);
              mxm::clamp_int(&color_g, 0, 255);
              mxm::clamp_int(&color_b, 0, 255);
              
              *target_ptr++ = color_r;
              *target_ptr++ = color_g;
              *target_ptr++ = color_b;
            }
            
            target_ptr += target_row_stepping*4;
          }
          
          TileReceiver->videoTileTransferred();
          
          break;
          
      }   // switch(SamplingMode)
    }
    
    tilebits_left--;
  }
  
  TileReceiver->frameComplete();
}


void mx::MxPEGScanDecoderSoftwareOnly
      ::receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                               const mxm::u8 *sample_data)          {
  
  mxm::u8 *audio_target_buffer
            = TileReceiver->provideAudioBlockTarget(audio_info);
  
  if(audio_target_buffer) {
    
    std::memcpy(audio_target_buffer,
                sample_data,
                audio_info.size());
    TileReceiver->audioBlockTransferred();
  }
}


void mx::MxPEGScanDecoderSoftwareOnly
      ::setMxPEGTileReceiver(IMxPEGTileReceiver *receiver) {
  
  if(   Active
     || TileReceiver) mxm::terminal("tile receiver is already set!",
                                    this);
  
  TileReceiver = receiver;
}


bool mx::MxPEGScanDecoderSoftwareOnly::errorState() {
  
  if(!Active) activate();
  
  return(ErrorEncountered);
}


void mx::MxPEGScanDecoderSoftwareOnly::setErrorState() {
  
  if(!Active) activate();
  
  ErrorEncountered = true;
  TileReceiver->setErrorState();
}


void mx::MxPEGScanDecoderSoftwareOnly::activate() {
  
  if(Active)        mxm::terminal("can only be activated once!",
                                  this);
  if(!TileReceiver) mxm::terminal("no tile receiver set!",
                                  this);
  
  Active = 1;
}
