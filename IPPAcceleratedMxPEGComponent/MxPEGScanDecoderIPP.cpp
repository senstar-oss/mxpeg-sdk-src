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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IPPAcceleratedMxPEGComponent/trunk/MxPEGScanDecoderIPP.cpp $
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



#include "include/MxPEGScanDecoderIPP.h"

#include <interfaces/IMxPEGTileReceiver.h>

#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>

#include <ippcc.h>

#include <cstring>



mx::MxPEGScanDecoderIPP::MxPEGScanDecoderIPP() {
  
  TileReceiver     = 0;
  
  Width            = -1;
  Height           = -1;
  
  UpperHalfBuffer  = 0;
  
  ErrorEncountered = false;
}


mx::MxPEGScanDecoderIPP::~MxPEGScanDecoderIPP() {
  
  delete TileReceiver;
  
  delete[] UpperHalfBuffer;
}



mxm::smart<mxmStringList> mx::MxPEGScanDecoderIPP
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::MxPEGScanDecoderIPP
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::MxPEGScanDecoderIPP
      ::receiveUndecodedMxPEGFrame(mxmUndecodedMxPEGFrameDescriptor
                                    *frame_descriptor)              {
  
  if(!TileReceiver) {
    
    mxm::sendStatusMessage(mxm::FailureMessage,
                           "receiveUndecodedMxPEGFrame() : "
                            "no tile receiver set!",
                           this);
    setErrorState();
    return;
  }
  
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
    UpperHalfBuffer = new mxm::u32[tile_xnum * (16*16*3/4/2)];
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
    
    setErrorState();
    return;
  }
  
  int detected_marker    = 0;
  Ipp32u prefetched_bits = 0;
  int prefetched_bit_num = 0;
  int pos                = 0;
  
  // remaining local data...
  Ipp16s *coeff_buffer;
  mxm::u32 mini_buffer[16*16*3/4];
  Ipp8u *mini_target;
  mxm::u32 *final_copy_source,
           *final_copy_target;
  int target_row_stepping;
  IppStatus ipp_status;
  bool use_bitmask;
  int tilebits_left;
  unsigned char *tilebit_ptr,
                tilebit_mask;
  int x, y;
  bool next_tile_right_here;
  int i;
  
  if(   (frame_descriptor->TileNumX < 1)
     || (frame_descriptor->TileNumY < 1)) {
    
    setErrorState();
    return;
  }
  
  // initialize and decide for bitmask mode...
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
      
      coeff_buffer = &CoeffBuffers[0][0];
      
      // iterate over sub-MCUs...
      for(i = 0; i < MCUsPerTile; i++) {
       
        if(ipp_status != ippStsNoErr) {   // decoding of previous sub-MCU caused
                                          // an error - better stop...
          setErrorState();
          return;
        }
        
        ipp_status = ippiDecodeHuffman8x8_Direct_JPEG_1u16s_C1(
                       frame_descriptor->ScanBytes, &pos,
                       coeff_buffer,
                       LastHuffmanDCPtrForMCU[i],
                       &detected_marker,
                       &prefetched_bits, &prefetched_bit_num,
                       HuffmanDCForMCU[i], HuffmanACForMCU[i]
                     );
        
        ippiDCTQuantInv8x8LS_JPEG_16s8u_C1R(coeff_buffer,
                                            YUV_TargetPtrs[i],
                                            YUV_TargetSteps[i],
                                            QuantTableForMCU[i]);
        
        coeff_buffer += 64;
      }
      
      // got all sub-MCUs!
       
      // now perform colorspace conversion...
      if(   (   frame_descriptor->Sampling
             == mxmUndecodedMxPEGFrameDescriptor::Y21U11V11)
         && !(y % 2))
       mini_target = (Ipp8u *)&UpperHalfBuffer[x * (16*16*3/4/2)];
      else
       mini_target = (Ipp8u *)mini_buffer;
      
      ippiYUV420ToRGB_8u_P3C3R(YUV_SrcPtrs, YUV_SrcSteps,
                               mini_target,
                               16*3, YUV_RGB_RoiSize);
      
      // --- finally negotiate with backend and write decoded pixel data... ---
      switch(frame_descriptor->Sampling) {
        
        // uses 8 MCUs per tile...
        case mxmUndecodedMxPEGFrameDescriptor::Y21U11V11:
          
          if(y % 2) {
            
            TileReceiver->provideVideoTilePixelTarget(x, y/2,
                                                      &final_copy_target,
                                                      &target_row_stepping);
            
            // 16 upper rows
            final_copy_source = &UpperHalfBuffer[x * (16*16*3/4/2)];
            for(i = 0; i < 8; i++) {
              
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              final_copy_target += target_row_stepping;
            }
            
            // 16 lower rows...
            final_copy_source = mini_buffer;
            for(i = 0; i < 8; i++) {
              
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              *final_copy_target++ = *final_copy_source++;
              final_copy_target += target_row_stepping;
            }
            
            TileReceiver->videoTileTransferred();
          }
          
          break;
        
        // uses 6 MCUs per tile...
        case mxmUndecodedMxPEGFrameDescriptor::Y22U11V11:
        default:
          
          TileReceiver->provideVideoTilePixelTarget(x, y,
                                                    &final_copy_target,
                                                    &target_row_stepping);
          final_copy_source = mini_buffer;
          for(i = 0; i < 16; i++) {   // 16 rows
            
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            *final_copy_target++ = *final_copy_source++;
            final_copy_target += target_row_stepping;
          }
          
          TileReceiver->videoTileTransferred();
          
          break;
      }
      
    }
    
    tilebits_left--;
  }
  
  TileReceiver->frameComplete();
}


void mx::MxPEGScanDecoderIPP
      ::receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                               const mxm::u8 *sample_data)          {
  
  if(!TileReceiver) {
    
    mxm::sendStatusMessage(mxm::FailureMessage,
                           "receiveMxPEGAudioFrame() : "
                            "no tile receiver set!",
                           this);
    setErrorState();
    return;
  }
  
  mxm::u8 *audio_target_buffer
            = TileReceiver->provideAudioBlockTarget(audio_info);
  if(audio_target_buffer) {
    
    std::memcpy(audio_target_buffer,
                sample_data,
                audio_info.size());
    TileReceiver->audioBlockTransferred();
  }
}


void mx::MxPEGScanDecoderIPP
      ::setMxPEGTileReceiver(IMxPEGTileReceiver *receiver) {
  
  delete TileReceiver;
  
  TileReceiver = receiver;
}


bool mx::MxPEGScanDecoderIPP::errorState() {
  
  return(ErrorEncountered);
}


void mx::MxPEGScanDecoderIPP::setErrorState() {
  
  ErrorEncountered = true;
  
  if(TileReceiver) TileReceiver->setErrorState();
}

