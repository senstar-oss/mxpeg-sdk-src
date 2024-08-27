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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/MxPEGParser.cpp $
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



#include "include/MxPEGParser.h"

#include <interfaces/IStreamSource.h>
#include <interfaces/IUndecodedMxPEGFrameReceiver.h>

#include <mxm/core/mxmHuffmanTableId.h>
#include <mxm/core/mxmAudioBlockInfo.h>
#include <mxm/core/mxm_jpeg.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>

#include <cstdlib>
#include <cstring>



mx::MxPEGParser::MxPEGParser() {
   
   Source           = 0;
   FrameReceiver    = 0;
   Active           = false;
   
   ErrorEncountered = false;
   
   PrefetchBuffSize = 4 * 1024 * 1024;
   PrefetchedBytes = 0;
   
   PrefetchBuffer = new unsigned char[PrefetchBuffSize];
   
   // --- init state machine datafields... ---
   
   State = AwaitMarker;   // used to be : AwaitSOI
   
   CurrentBytePtr = 0;
   BytesLeft = 0;
   
   AwaitMarkerStage = 0;
   
   ReadMarkerStage = 0;
   ReadMarkerFirstLenByte = 0;
   MarkerBytesNeeded = 0;
   
   ScanStage = 0;
   
   MarkerPayload = 0;
   
   Width = Height = 0;
   
   
   Fresh = true;
}


mx::MxPEGParser::~MxPEGParser() {
   
   delete Source;
   delete FrameReceiver;
   
   delete[] PrefetchBuffer;
}



mxm::smart<mxmStringList> mx::MxPEGParser::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::MxPEGParser
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::MxPEGParser::setStreamSource(IStreamSource *stream_source) {
   
   if(   Active
      || Source) mxm::terminal("stream source is already set!",
                               this);
   
   Source = stream_source;
}


void mx::MxPEGParser
      ::setUndecodedMxPEGFrameReceiver(IUndecodedMxPEGFrameReceiver
                                        *frame_receiver)            {
   
   if(   Active
      || FrameReceiver) mxm::terminal("frame receiver is already set!",
                                      this);
   
   FrameReceiver = frame_receiver;
}


int mx::MxPEGParser::processStreamBytes(int num) {
  
  if(!Active) activate();
  
  int bytes_allowed;
  unsigned char *buff_ptr;
  int bytes_read, bytes_processed;
  
  // determine max no of bytes to process for this call...
  bytes_allowed = PrefetchBuffSize - PrefetchedBytes;
  if(bytes_allowed > num) bytes_allowed = num;
  if(bytes_allowed < 1) {
    mxm::sendStatusMessage(mxm::WarningMessage,
                           "prefetch buffer overflow!",
                           this);
    std::exit(666);
    // TODO !
  }
  
  bytes_processed = 0;
  while(bytes_allowed > 0) {
   
    buff_ptr = PrefetchBuffer + PrefetchedBytes;
    bytes_read = Source->fetchStreamBytes(buff_ptr, bytes_allowed); 
    if(!bytes_read) break;
    
    PrefetchedBytes += bytes_read;
   
    feedBytes(buff_ptr, bytes_read);   // might remove entire frame image and
                                       // thus modify _PrefetchedBytes_ variable
    
    bytes_processed += bytes_read;
    bytes_allowed   -= bytes_read;
  }
  
  if(Source->errorState()) setErrorState();
  
  return(bytes_processed);
}


bool mx::MxPEGParser::sourceStillUp() {
   
   if(!Active) activate();
   
   return(Source->stillUp());
}


bool mx::MxPEGParser::errorState() {
   
   if(!Active) activate();
   
   return(ErrorEncountered);
}


void mx::MxPEGParser::setErrorState() {
   
   if(!Active) activate();
   
   ErrorEncountered = true;
   if(FrameReceiver) FrameReceiver->setErrorState();
}


void mx::MxPEGParser::feedBytes(unsigned char *buffer,
                                int num)               {
  
  unsigned char marker_type;
  int width, height;
  bool need_to_copy_down;
  mxm::u8 u8_value;
  int quantization_table_target,
      huffman_table_class,
      huffman_table_target;
  mxm::u32 gen_no;
  mxm::u64 time_stamp;
  mxm::u32 audio_duration;
  
  if(num < 1) return;
  
  CurrentBytePtr = buffer;
  BytesLeft = num;
  while(BytesLeft) {
  
    need_to_copy_down = false;
    
#ifdef __MX_DEBUG__
    mxm::sendStatusMessage(mxm::DebugMessage,
                           mxmString("parser state=") + (int)State,
                           this);
#endif
    
    // this switch block encodes the decoder's state machine...
    switch(State) {
      
      // all these states can digest BytesLeft == 0
      
      // waits for new JPEG marker and dispatches to respective handler state...
      case AwaitMarker:
        if(!awaitMarker(&marker_type)) return;
#ifdef __MX_DEBUG__
        mxm::sendStatusMessage(mxm::DebugMessage,
                               mxmString("got marker : 0x")
                                + mxm::hex(&marker_type, sizeof(marker_type)),
                               this);
#endif
        switch(marker_type) {
          case mxm::JPEG::SOI:
            State = StartOfImage;
            break;
          case mxm::JPEG::APP0:
            State = APP0;
            break;
          case mxm::JPEG::COM:
            State = Comment;
            break;
          case mxm::JPEG::DQT:
            State = QuantizationTable;
            break;
          case mxm::JPEG::DHT:
            State = HuffmanTable;
            break;
          case mxm::JPEG::DRI:
            State = DefineRestartInterval;
            break;
          case mxm::JPEG::SOF0:
            State = StartOfFrame;
            break;
          case mxm::JPEG::SOS:
            State = StartOfScan;
            break;
          case mxm::JPEG::APP13:
            State = Audio;
            break;
          default:
            State = NoGoodState;
            break;
        }
        break;
      
      // marker has no payload, triggers scan decoding of last frame...
      case StartOfImage:
        if(Fresh)
          Fresh = false;
        else
         need_to_copy_down = true;
        State = RecordStartOfImage;
        break;
      
      // no payload to read, records SOI position to frame descriptor...
      case RecordStartOfImage:
        FrameDescriptor.StartOfImage = CurrentBytePtr - 2;
        State = AwaitMarker;
        break;
      
      // reads payload, does nothing...
      case APP0:
        if(!readMarkerPayload()) return;
        State = AwaitMarker;
        break;
      
      // reads payload, dispatches to handler states, or does nothing...
      case Comment:
        if(!readMarkerPayload()) return;
        if(!strncmp((char *)MarkerPayload, "#:M1IMG", 7))
          State = M1IMG;
        else if(!strncmp((char *)MarkerPayload, "MXF", 3))
          State = MxF;
        else if(!strncmp((char *)MarkerPayload, "MXM", 3))
          State = BitMap;
        else
          State = AwaitMarker;
        break;
      
      // JPEG comment: payload already read...
      case M1IMG:
        State = AwaitMarker;
        break;
      
      // JPEG comment: payload already read...
      case MxF:
        std::memcpy(&time_stamp,
                    (mxm::u8 *)MarkerPayload + 8,
                    sizeof(time_stamp));
        LocalEndian.convertFromLittle(&time_stamp, sizeof(time_stamp));
        std::memcpy(&gen_no,
                    (mxm::u8 *)MarkerPayload + 20,
                    sizeof(gen_no));
        LocalEndian.convertFromLittle(&gen_no, sizeof(gen_no));
        FrameDescriptor.setTimeStamp((mxm::frame_time)time_stamp);
        FrameDescriptor.setGenerationNumber(gen_no);
        State = AwaitMarker;
        break;
      
      // JPEG comment: payload already read - sets bitmap in frame descriptor
      case BitMap:
        FrameDescriptor.TileBits = &MarkerPayload[12];
        State = AwaitMarker;
        break;
      
      // reads payload, programs scan decoder's quantization table...
      case QuantizationTable:
        if(!readMarkerPayload()) return;
        {
           int payload_bytes_left = MarkerPayloadSize;
           mxm::u8 *current_table = MarkerPayload;
           while(payload_bytes_left >= 65) {
             
             quantization_table_target = (unsigned int)*current_table & 0x0f;
             mxm::clamp_int(&quantization_table_target, 0, 3);

             if(mxm::statusMessageUseful(mxm::DebugMessage))
              mxm::sendStatusMessage(mxm::DebugMessage,
                                     mxmString("setting quantization table ")
                                      + quantization_table_target,
                                     this);

             FrameReceiver->setQuantizationTable(quantization_table_target,
                                                 current_table + 1);
             current_table      += 65;
             payload_bytes_left -= 65;
           }
        }
        State = AwaitMarker;
        break;
      
      // reads payload, programs scan decoder's Huffman table...
      case HuffmanTable:
        if(!readMarkerPayload()) return;
        
        {
          int payload_bytes_left = MarkerPayloadSize;
          mxm::u8 *table_data    = MarkerPayload;
          for(;;) {
            
            if(payload_bytes_left < 17) break;
            
            huffman_table_class  = *table_data & 0xf0;
            mxm::clamp_int(&huffman_table_class, 0, 1);
            huffman_table_target = *table_data & 0x0f;
            mxm::clamp_int(&huffman_table_target, 0, 3);
            
            table_data++;
            int size = 0;
            for(int i = 0; i < 16; i++) size += *table_data++;
            payload_bytes_left -= 17;
            
            if(payload_bytes_left < size) break;
            
            if(mxm::statusMessageUseful(mxm::DebugMessage))
             mxm::sendStatusMessage(mxm::DebugMessage,
                                    mxmString("setting Huffman table, class=")
                                     + huffman_table_class 
                                     + ", target="
                                     + huffman_table_class,
                                     this);

            FrameReceiver->setHuffmanTable(
                             mxmHuffmanTableId(huffman_table_class,
                                               huffman_table_target),
                             table_data - 16);
            
            table_data         += size;
            payload_bytes_left -= size;
          }
        }
        
        State = AwaitMarker;
        break;

      case DefineRestartInterval:
        if(!readMarkerPayload()) return;
        State = AwaitMarker;
        break;

      // sets resolution in frame descriptor...
      case StartOfFrame:
        if(!readMarkerPayload()) return;
        width  = (int)MarkerPayload[3]*256 + (int)MarkerPayload[4];
        height = (int)MarkerPayload[1]*256 + (int)MarkerPayload[2];
        if(mxm::statusMessageUseful(mxm::DebugMessage))
         mxm::sendStatusMessage(mxm::DebugMessage,
                                mxmString("setting frame resolution to ")
                                 + width + "x"
                                 + height,
                                this);
        FrameDescriptor.setResolution(width, height);
        
        {
          int comp_num = MarkerPayload[5];
          FrameDescriptor.setComponentNum(comp_num);
          mxm::u8 *comp_param_ptr = &MarkerPayload[6];
          for(int i = 0; i < comp_num; i++) { // TODO : guard against bad values
            int component      = *comp_param_ptr++ - 1,
                horiz_sampling = *comp_param_ptr >> 4,
                vert_sampling  = *comp_param_ptr++ & (mxm::u8)0x0f,
                quant_table    = *comp_param_ptr++;
            FrameDescriptor.setComponentParameters(component,
                                                   horiz_sampling,
                                                   vert_sampling,
                                                   quant_table);
          }
          if(   (FrameDescriptor.ComponentNum == 3)
             && (FrameDescriptor.ComponentInfos[0].HorizSamplingFactor == 2)
             && (FrameDescriptor.ComponentInfos[0].VertSamplingFactor  == 2)
             && (FrameDescriptor.ComponentInfos[1].HorizSamplingFactor == 1)
             && (FrameDescriptor.ComponentInfos[1].VertSamplingFactor  == 1)
             && (FrameDescriptor.ComponentInfos[2].HorizSamplingFactor == 1)
             && (FrameDescriptor.ComponentInfos[2].VertSamplingFactor  == 1)) {
            
            FrameDescriptor.setSamplingMode(mxmUndecodedMxPEGFrameDescriptor
                                             ::Y22U11V11);
            if(mxm::statusMessageUseful(mxm::DebugMessage))
             mxm::sendStatusMessage(mxm::StatusMessage,
                                    "using sampling mode Y22U11V11 (MX)",
                                    this);
          }
          else {
            
            FrameDescriptor.setSamplingMode(mxmUndecodedMxPEGFrameDescriptor
                                             ::Y21U11V11);
            if(mxm::statusMessageUseful(mxm::DebugMessage))
             mxm::sendStatusMessage(mxm::StatusMessage,
                                    "using sampling mode Y21U11V11 (Axis, ...)",
                                    this);
          }
        }
        
        State = AwaitMarker;
        break;
      
      // sets scan starting position in frame descriptor...
      case StartOfScan:
        if(!readMarkerPayload()) return;
        FrameDescriptor.ScanBytes = CurrentBytePtr;
        
        {
          int comp_num = MarkerPayload[0];
          mxm::u8 *comp_param_ptr = &MarkerPayload[1];
          for(int i = 0; i < comp_num; i++) { // TODO : guard against bad values
            int component = *comp_param_ptr++ - 1,
                huffman_dc = *comp_param_ptr >> 4,
                huffman_ac = *comp_param_ptr++ & (mxm::u8)0x0f;
            FrameDescriptor.setMoreComponentParameters(component,
                                                       huffman_dc,
                                                       huffman_ac);
          }
        }
        
        State = Scan;
        break;
      
      // sets scan and image length in frame descriptor...
      case Scan:
        if(!doScan()) return;
        
        if(mxm::statusMessageUseful(mxm::DebugMessage))
         mxm::sendStatusMessage(mxm::DebugMessage,
                                mxmString("scan start=0x")
                                 + mxm::hex(FrameDescriptor.ScanBytes,
                                            sizeof(FrameDescriptor.ScanBytes))
                                 + ", end=0x"
                                 + mxm::hex(CurrentBytePtr,
                                            sizeof(CurrentBytePtr)),
                                this);
        
        FrameDescriptor.ScanLength
         = CurrentBytePtr - FrameDescriptor.ScanBytes;
        FrameDescriptor.ImageLength
         = CurrentBytePtr - FrameDescriptor.StartOfImage;
        State = AwaitMarker;
        break;
       
      case Audio:
        if(!readMarkerPayload()) return;

        if(mxm::statusMessageUseful(mxm::DebugMessage))
         mxm::sendStatusMessage(mxm::DebugMessage,
                                "received audio block",
                                this);
        
        std::memcpy(&audio_duration,
                    (mxm::u8 *)MarkerPayload + 0,
                    sizeof(audio_duration));
        LocalEndian.convertFromLittle(&audio_duration, sizeof(audio_duration));
        std::memcpy(&time_stamp,
                    (mxm::u8 *)MarkerPayload + 4,
                    sizeof(time_stamp));
        LocalEndian.convertFromLittle(&time_stamp, sizeof(time_stamp));
        
        FrameReceiver->receiveMxPEGAudioFrame(
                         mxmAudioBlockInfo((mxm::frame_time)time_stamp,
                                           (mxm::frame_time)audio_duration,
                                           MarkerPayloadSize - 12),
                         (mxm::u8 *)MarkerPayload + 12);
        
        State = AwaitMarker;
        break;
        
      case ReadPayloadAndReturn:
        if(!readMarkerPayload()) return;
        State = AwaitMarker;
        break;
        
      // terminal, internal decoder failure...
      case NoGoodState:
      default:
        if(!errorState()) {
          mxm::sendStatusMessage(mxm::WarningMessage,
                                 "no good state!",
                                 this);
          setErrorState();
        }
        CurrentBytePtr += BytesLeft;
        BytesLeft = 0;
        State = NoGoodState;                                    // stay grounded
        break;
    }
    
    if(need_to_copy_down) {
       
      // ---------- we've got one complete undecoded frame image ----------
      
      // check video resolution, do we have to signal change...?
      if(   (FrameDescriptor.Width != Width)
         || (FrameDescriptor.Height != Height)) {
         
        // resolution changes
        //BackEnd->videoResolutionChanged(FrameDescriptor.Width,
        //                                FrameDescriptor.Height);
        
        Width  = FrameDescriptor.Width;
        Height = FrameDescriptor.Height;
      }
      
      if(mxm::statusMessageUseful(mxm::DebugMessage))
       mxm::sendStatusMessage(mxm::DebugMessage,
                              mxmString("decoding scan, res=")
                               + FrameDescriptor.Width + "x"
                               + FrameDescriptor.Height
                               + ", scan_bytes="
                               + FrameDescriptor.ScanLength);
      
      FrameReceiver->receiveUndecodedMxPEGFrame(&FrameDescriptor);
      
      //BackEnd->frameComplete();
      
      // copy down remainder of prefetched bytes...
      //   (we also have to copy the 2-byte SOI marker that was just read)
      std::memmove(PrefetchBuffer, CurrentBytePtr - 2, BytesLeft + 2);
      CurrentBytePtr = PrefetchBuffer + 2;
      PrefetchedBytes = BytesLeft + 2;
        // _need_to_copy_down_ flag will be cleared first thing in the next
        // loop cycle...
      
      // prepare for new attack run...
      FrameDescriptor.resetForNextFrame();
    }
  }
}


/*!
 *  Eats BytesLeft==0 
 *
 *  Result:
 *  - <tt>false</tt> : no marker yet, all bytes consumed
 *  - <tt>true</tt> : marker found, BytesLeft and CurrentBytePtr modified 
 *    accordingly (bytes may be consumed), AwaitMarkerStage == 0
 */
bool mx::MxPEGParser::awaitMarker(unsigned char *marker_type) {
  
  if(AwaitMarkerStage == 0) {   // nothing found yet
  
    while(BytesLeft) {
      if(*CurrentBytePtr == (unsigned char)0xff) {
        CurrentBytePtr++;
        BytesLeft--;
        if(BytesLeft) {
          *marker_type = *CurrentBytePtr;
          CurrentBytePtr++;
          BytesLeft--;
          return(true);   // found a marker!
        }
        else {
          // got the 0xff, but no byte left to determine marker type
          AwaitMarkerStage = 1;
          return(false);
        }
      }
      
      CurrentBytePtr++;
      BytesLeft--;
    }
    return(false);   // still nothing!
  }   // stage 0
  
  else {   // stage 1, still net type...
    if(BytesLeft) {
      *marker_type = *CurrentBytePtr;
      CurrentBytePtr++;
      BytesLeft--;
      AwaitMarkerStage = 0;
      return(true);
    }
    else 
      return(false);
  }
}


/*!
 *  If method returns <tt>true</tt>, the payload is recognized to be completely
 *  present and the <tt>MarkerPayload</tt> pointer is set accordingly.
 *
 *  Analogous behavior as waitMarker().
 */
bool mx::MxPEGParser::readMarkerPayload(void) {

  for(;;) {
    
    if(ReadMarkerStage == 0) {   // got nothing
      if(!BytesLeft) return(false);
      else if(BytesLeft == 1) {
        ReadMarkerFirstLenByte = *CurrentBytePtr;
        CurrentBytePtr++;
        BytesLeft--;
        ReadMarkerStage = 1;
        return(false);
      }
      else {  
        MarkerPayloadSize = MarkerBytesNeeded = 256*(int)CurrentBytePtr[0] 
                                                 + (int)CurrentBytePtr[1]
                                                 - 2;
        CurrentBytePtr += 2;
        BytesLeft -= 2;
        MarkerPayload = CurrentBytePtr;
        ReadMarkerStage = 2;
        continue;
      }
    }   // stage 0
    
    else if(ReadMarkerStage == 1) {   // got the 1 length byte, waiting for 2nd 
      if(!BytesLeft) return(false);
      MarkerPayloadSize = MarkerBytesNeeded = 256*(int)ReadMarkerFirstLenByte 
                                               + (int)*CurrentBytePtr
                                               - 2;
      CurrentBytePtr++;
      BytesLeft--;
      MarkerPayload = CurrentBytePtr;
      ReadMarkerStage = 2;
      continue;
    }   // stage 1
    
    else if(ReadMarkerStage == 2) {   // reading marker payload 
      
      if(!BytesLeft) return(false);
      if(BytesLeft >= MarkerBytesNeeded) {
        CurrentBytePtr += MarkerBytesNeeded;
        BytesLeft -= MarkerBytesNeeded;
        MarkerBytesNeeded = 0;
        ReadMarkerFirstLenByte = 0;
        ReadMarkerStage = 0;
        return(true);
      }
      else {
        MarkerBytesNeeded -= BytesLeft;
        CurrentBytePtr += BytesLeft;
        BytesLeft = 0;
        return(false);
      }
    }
  }
}


/*!
 *  Analogous behavior as waitMarker().
 */
bool mx::MxPEGParser::doScan(void) {

label_doScan_begin:

  if(ScanStage == 0) {   // nuffin'
    while(BytesLeft) {
      if(*CurrentBytePtr == 0xff) {
        CurrentBytePtr++;
        BytesLeft--;
        if(!BytesLeft) {
          ScanStage = 1;
          return(false);
        }
        else {
          if(*CurrentBytePtr == 0x00) {
            CurrentBytePtr++;
            BytesLeft--;
            continue;
          }
          else if(*CurrentBytePtr == mxm::JPEG::EOI) {
            CurrentBytePtr++;
            BytesLeft--;
            return(true);
          }
          else {
            mxm::sendStatusMessage(mxm::FailureMessage,
                                   "!!! MxPEG decoder failure "
                                    "(invalid scan termination) !!!",
                                   this);
            std::exit(666);
          }
        }
      }
      else {
        CurrentBytePtr++;
        BytesLeft--;
      }
    }
    return(false);
  }   // stage 0
  
  else {   // stage 1 - read a 0xff, decide whether marker or padding
    if(!BytesLeft) return(false);
    if(*CurrentBytePtr == 0x00) {
      CurrentBytePtr++;
      BytesLeft--;
      ScanStage = 0;
      goto label_doScan_begin;   // nasty, but what da hell...
    }
    else if(*CurrentBytePtr == mxm::JPEG::EOI) {
      CurrentBytePtr++;
      BytesLeft--;
      ScanStage = 0;
      return(true);
    }
    else {
      mxm::sendStatusMessage(mxm::FailureMessage,
                             "!!! MxPEG decoder failure "
                              "(invalid scan termination) !!!",
                             this);
      std::exit(666);
    }
  }   // stage 1
}


void mx::MxPEGParser::activate() {
   
   if(Active)         mxm::terminal("already active!",
                                    this);
   if(!Source)        mxm::terminal("no stream source configured!",
                                    this);
   if(!FrameReceiver) mxm::terminal("no frame receiver configured!",
                                    this);
   
   Active = true;
}
