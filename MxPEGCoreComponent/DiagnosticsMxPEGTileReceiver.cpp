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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/DiagnosticsMxPEGTileReceiver.cpp $
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



#include "include/DiagnosticsMxPEGTileReceiver.h"



mx::DiagnosticsMxPEGTileReceiver::DiagnosticsMxPEGTileReceiver() {

  ReceivedFrames   = 0;
  TileNum          = 0;
  AudioBlockNum    = 0;
  
  ErrorEncountered = false;
}


mx::DiagnosticsMxPEGTileReceiver::~DiagnosticsMxPEGTileReceiver() {
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         mxmString("signing off after having received ")
                          + ReceivedFrames + " frames and "
                          + AudioBlockNum + " audio blocks",
                         this);
}



mxm::smart<mxmStringList> mx::DiagnosticsMxPEGTileReceiver
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::DiagnosticsMxPEGTileReceiver
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::DiagnosticsMxPEGTileReceiver::setFrameInfo(const mxmFrameInfo
                                                           &frame_info) {
  
  // TODO properly honor integer type ranges!
  
  FrameInfo = frame_info;
  int secs  = (FrameInfo.timeStamp() / 1000) / 1000,
      msecs = (FrameInfo.timeStamp() / 1000) % 1000;
  char txt[80];
  std::sprintf(txt, "%d:%03d", secs, msecs);
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         mxmString("received frame info: ")
                          + "res=" + FrameInfo.width() 
                                      + "x" + FrameInfo.height()
                          + ", time_stamp=" + txt
                          + ", seqno=" + (int)FrameInfo.seqNo(),
                         this);
}


void mx::DiagnosticsMxPEGTileReceiver::videoResolutionChanged(int width, 
                                                              int height) {
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         mxmString("video resolution changed to ")
                          + width + "x" + height,
                         this);
}


void mx::DiagnosticsMxPEGTileReceiver::frameComplete() {
  
  mxmString err_txt;
  if(ErrorEncountered) err_txt = ", *** ERROR state set";
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         mxmString("------ video frame ") + ReceivedFrames
                          + " complete, tile_num=" + TileNum
                          + err_txt,
                         this);
  
  ReceivedFrames++;
  if(ReceivedFrames < 0) ReceivedFrames = 0;
  
  TileNum = 0;
}


void mx::DiagnosticsMxPEGTileReceiver::provideVideoTilePixelTarget(
                                         int tile_x, int tile_y,
                                         mxm::u32 **target_ptr,
                                         int *row_stepping_ptr)     {

  *target_ptr       = (mxm::u32 *)TilePixelBuffer;
  *row_stepping_ptr = 0;
}


void mx::DiagnosticsMxPEGTileReceiver::videoTileTransferred() {

  TileNum++;
}


int mx::DiagnosticsMxPEGTileReceiver::frameCounter() {

  return(ReceivedFrames);
}


mxm::u8 *mx::DiagnosticsMxPEGTileReceiver
          ::provideAudioBlockTarget(const mxmAudioBlockInfo
                                           &audio_block_info) {
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         mxmString("audio block, size=")
                          + audio_block_info.size(),
                         this);
  
  AudioBlockNum++;
  
  return(0);
}


void mx::DiagnosticsMxPEGTileReceiver::audioBlockTransferred() {
  
  // nop
}


bool mx::DiagnosticsMxPEGTileReceiver::errorState() {

  return(ErrorEncountered);
}


void mx::DiagnosticsMxPEGTileReceiver::setErrorState() {
  
  mxm::sendStatusMessage(mxm::WarningMessage,
                         "*** error state set!",
                         this);
  
  ErrorEncountered = true;
}
