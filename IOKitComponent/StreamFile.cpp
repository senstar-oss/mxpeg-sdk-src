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
//  $LastChangedDate: 2007-06-28 17:55:38 +0200 (Thu, 28 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/StreamFile.cpp $
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



#include "include/StreamFile.h"

#include <mxm/core/mxm_generic_stuff.h>

#include <fcntl.h>
#ifdef MX_PLATFORM_WINDOWS
   #include <io.h>
   #include <windows.h>
#else
   #include <unistd.h>
#endif



mx::StreamFile::StreamFile() {
  
  FileDescriptor           = -1;
  WriteMode                = false;
  Online                   = false;
  
  FD                       = -1;
  StillUp                  = false;
  ErrorEncountered         = false;
  HasBeenShutDown          = false;
  CloseFileUponDestruction = false;
#ifdef MX_PLATFORM_WINDOWS
  RestoreMode              = -1;
  OSFHandle                = 0;
#endif
  TimedBlockMode           = false;
  BlockTimeout             = 1000;
}


mx::StreamFile::~StreamFile() {
  
  if(!HasBeenShutDown) closeFile();                    // shutdown() also closes
}



void mx::StreamFile::setFilename(const mxmString &filename) {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return;
  }
  
  if(Online) mxm::terminal("can't set file when already active!",
                           this);
  
  Filename       = filename;
  FileDescriptor = -1;
}


void mx::StreamFile::setFileDescriptor(int fd) {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return;
  }
  
  if(Online) mxm::terminal("can't set file when already active!",
                           this);
  
  if(fd < 0) mxm::terminal("bad file descriptor specified!",
                           this);
  
  Filename       = "";
  FileDescriptor = fd;
}


/*!
 *  Default is <tt>read</tt> mode.
 */
void mx::StreamFile::setWriteMode(bool enabled) {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return;
  }
  
  if(Online) mxm::terminal("can't toggle write mode when already active!",
                           this);
  
  WriteMode = enabled;
}


void mx::StreamFile::setTimedBlockModeEnabled(bool enabled, long timeout) {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return;
  }
  
  TimedBlockMode = enabled;
  
  if(TimedBlockMode) BlockTimeout = timeout;
}


int mx::StreamFile::fetchStreamBytes(mxm::u8 *buffer, int num) {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return(0);
  }
  
  if(!Online) mxm::terminal("not yet activated!",
                            this);
  
  if(!StillUp) return(0);
  
#ifdef MX_PLATFORM_WINDOWS
  if(TimedBlockMode) {
    if(WaitForSingleObject((HANDLE)OSFHandle, BlockTimeout) 
        == WAIT_TIMEOUT) return(0);
  }
#else
  if(TimedBlockMode) {

    long usecs = BlockTimeout * 1000;
    struct timeval timeout;
    timeout.tv_sec  = usecs / 1000000;
    timeout.tv_usec = usecs % 1000000;
    
    fd_set file_descriptors;
    FD_ZERO(&file_descriptors);
    FD_SET(FD, &file_descriptors);
    if(!select(FD + 1, &file_descriptors, 0, 0, &timeout)) return(0);
  }
#endif
  
  int actually_read = read(FD, buffer, num);
   
  if(actually_read <= 0) {
    
    StillUp = false;
    
    if(actually_read == -1) ErrorEncountered = true;
                                       // TODO : properly handle signal impacts?
    
    return(0);
  }
  
  return(actually_read);
}


bool mx::StreamFile::stillUp(void) {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return(false);
  }
  
  if(!Online) mxm::terminal("not yet activated!",
                            this);
  
  return(StillUp);
}


void mx::StreamFile::receiveStreamBytes(const mxm::u8 *bytes,
                                        int byte_num)         {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return;
  }
  
  if(!Online) mxm::terminal("not yet activated!",
                            this);
   
  if(ErrorEncountered) return;
  
  // have to do it tricky to properly handle short writes...
//   int written,
//       to_write;
//   to_write = byte_num;
  //while(to_write > 0) 
  {
  
	 BYTE * buffer = this->getOutBuffer();
	 int * p_pos = this->getOutLengthPointer();
	 int max = this->getOutLength() - *p_pos;
	 memcpy(buffer+(*p_pos), bytes, min(max(0, max), byte_num));
	 *p_pos += byte_num;
//    written = write(FD, bytes, to_write);
//     if(written == -1) {
//       
//       // TODO : handle signal impacts?
//       
//       ErrorEncountered = true;
//       break;
//     }
//     
	//written = to_write;
    //to_write -= written;
    bytes += byte_num;
  }
}


bool mx::StreamFile::errorState() {
  
  if(!Online) mxm::terminal("not yet activated!",
                            this);
  
  return(ErrorEncountered);
}


void mx::StreamFile::setErrorState() {
  
  if(!Online) mxm::terminal("not yet activated!",
                            this);
  
  ErrorEncountered = true;
  StillUp          = false;
}


void mx::StreamFile::shutdown() {
  
  if(HasBeenShutDown) return;
  
  if(!Online) mxm::terminal("not yet activated!",
                            this);
  
  closeFile();
  
  HasBeenShutDown = true;
}


bool mx::StreamFile::activate() {
  
  if(HasBeenShutDown) {
    
    ErrorEncountered = true;
    return(false);
  }
  
  if(Online)                    mxm::terminal("can only be activated once!",
                                              this);
  //if(   !Filename.length()
  //   && (FileDescriptor == -1)) mxm::terminal("file not specified!",
  //                                            this);
  
  mxmString mode_txt = "read mode";
  if(WriteMode) mode_txt = "write mode";
  
  if(FileDescriptor == -1) {   // filename mode...
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           mxmString("opening stream file \"")
                            + Filename + "\" in "
                            + mode_txt + "...",
                           this);
    
    if(WriteMode) {
#ifdef MX_PLATFORM_WINDOWS
      FD = open(Filename.text(),
                O_CREAT | O_WRONLY | O_TRUNC | O_BINARY);
#else
      FD = open(Filename.text(),
                O_CREAT | O_WRONLY | O_TRUNC,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
    }
    else {
#ifdef MX_PLATFORM_WINDOWS
      FD = open(Filename.text(), O_RDONLY | O_BINARY);
#else
      FD = open(Filename.text(), O_RDONLY);
#endif
    }
    
    if(FD == -1) {
      
      mxmString fd_text;
      if(FileDescriptor == -1) fd_text = "<not specified>";
      else                     fd_text = FileDescriptor;
      mxm::sendStatusMessage(mxm::WarningMessage,
                             mxmString("failed to open file, filename=\"")
                              + Filename + "\", file descriptor="
                              + fd_text,
                             this);
      
      ErrorEncountered = true;
      // defaults: CloseFileUponDestruction = false;
      // defaults: StillUp = false;
    }
    else {
#ifdef MX_PLATFORM_WINDOWS
      OSFHandle = _get_osfhandle(FD);
#endif
      CloseFileUponDestruction = true;
      StillUp = true;
    }
    
#ifdef MX_PLATFORM_WINDOWS
    // defaults: RestoreMode = -1;
#endif
  }
  
  else {   // file descriptor mode...
    
    mxm::sendStatusMessage(mxm::StatusMessage,
                           mxmString("opening stream file (descriptor ")
                            + FileDescriptor + ") in "
                            + mode_txt + "...",
                           this);
    
    FD = FileDescriptor;
    
#ifdef MX_PLATFORM_WINDOWS
    if((RestoreMode = setmode(FD, O_BINARY)) != -1) {
      OSFHandle = _get_osfhandle(FD);
      StillUp = true;
    }
    else {
      ErrorEncountered = true;
      // defaults: StillUp = false;
    }
#else
    StillUp = true;
#endif
    
    // defaults: CloseFileUponDestruction = false;
  }
  
  Online = true;
  
  if(ErrorEncountered)
   return(false);
  else
   return(true);
}


void mx::StreamFile::closeFile() {
  
  #ifdef MX_PLATFORM_WINDOWS
  if(RestoreMode != -1) {
    
    if(setmode(FD, RestoreMode) == -1) ErrorEncountered = true;
  }
#endif

  if(CloseFileUponDestruction) {
    
    if(close(FD)) ErrorEncountered = true;
  }
}
