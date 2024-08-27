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
//  $LastChangedDate: 2007-06-28 14:28:26 +0200 (Thu, 28 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/FileWriter.cpp $
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



#include "include/FileWriter.h"



mx::FileWriter::FileWriter() {
   
   HasBeenShutDown = false;
   
   reset();
}


mx::FileWriter::~FileWriter() {
   
}



mxm::smart<mxmStringList> mx::FileWriter
                           ::enumerateInterfaces() {
   
   return(_enumerateInterfaces());
}
mxm::smart<mxmStringList> mx::FileWriter
                           ::enumerateCapabilities(const mxmString &interface) {
   
   return(_enumerateCapabilities(interface));
}


void mx::FileWriter::setFilename(const mxmString &filename) {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   if(!filename.length()) return;
   
   reset(true);
   
   Filename       = filename;
   FileDescriptor = -1;
}


void mx::FileWriter::setFileDescriptor(int fd) {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   if(fd < 0) return;
   
   reset(true);
   
   Filename       = "";
   FileDescriptor = fd;
}


void mx::FileWriter::receiveStreamBytes(const mxm::u8 *bytes, int byte_num) {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   if(!byte_num) return;
   
   if(File.null()) openFile();
   
   // write separator string...?
   if(   (FreshSegment && !Fresh)
      && !IndividualFileMode
      && Separator.length()) {
      
      File->receiveStreamBytes((const mxm::u8 *)Separator.text(),
                               Separator.length());
   }
   
   File->receiveStreamBytes(bytes, byte_num);
   
   if(File->errorState())
    ErrorEncountered = true;
   
   Fresh        = false;
   FreshSegment = false;
}


void mx::FileWriter::newStreamSegment() {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   if(IndividualFileMode)
    closeFile();
   
   if(   IndividualFileMode
      && (IndividualFileNum != 1)) {
      
      IndividualFileCounter++;
      if(   (IndividualFileNum > 1)
         && (IndividualFileCounter >= IndividualFileNum))
       IndividualFileCounter = 0;
   }
   
   FreshSegment = true;
}


void mx::FileWriter::writeSingleConcatenatedFile() {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   reset(true);
   
   IndividualFileMode = false;
}


void mx::FileWriter::setSegmentSeparator(const mxmString &separator) {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   reset(true);
   
   Separator = separator;
}


void mx::FileWriter::writeIndividualFiles(int num) {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   reset(true);
   
   IndividualFileMode = true;
   
   if(num <= 0) num = 0;
   IndividualFileNum = num;
}


bool mx::FileWriter::errorState() {
   
   return(ErrorEncountered);
}


void mx::FileWriter::setErrorState() {
   
   if(HasBeenShutDown) {
      
      ErrorEncountered = true;
      return;
   }
   
   ErrorEncountered = true;
   
   if(!File.null()) File->setErrorState();
}


void mx::FileWriter::shutdown() {
   
   if(HasBeenShutDown) return;
   
   closeFile();
   
   HasBeenShutDown = true;
}


void mx::FileWriter::openFile() {
   
   File = new mx::StreamFile();
   File->setWriteMode(true);
   
   if(FileDescriptor >= 0) {
      
      File->setFileDescriptor(FileDescriptor);
   }
   else {
      
      mxmString filename = Filename;
      
      if(   IndividualFileMode
         && (IndividualFileNum != 1)) {
         
         mxmString left, right;
         filename.split(left, right, ".", true);
         
         filename = left + IndividualFileCounter + "." + right;
      }
      
      File->setFilename(filename);
   }
   
   File->activate();
   
   if(File->errorState()) ErrorEncountered = true;
}


void mx::FileWriter::closeFile() {
   
   if(!File.null()) {
      
      File->shutdown();
      if(File->errorState()) ErrorEncountered = true;
   }
   
   File = 0;
}


void mx::FileWriter::reset(bool safe_settings) {
   
   File                  = 0;                     // close potentially open file
   ErrorEncountered      = false;
   Fresh                 = true;
   FreshSegment          = true;
   
   if(!safe_settings) {
      
      Filename              = "";
      FileDescriptor        = -1;                               // filename mode
      
      IndividualFileMode    = false;
      IndividualFileNum     = 0;
      IndividualFileCounter = 0;
      
      Separator             = "";
   }
}
