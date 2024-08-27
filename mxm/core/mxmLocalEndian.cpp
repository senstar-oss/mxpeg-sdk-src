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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmLocalEndian.cpp $
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



#include <mxm/core/mxmLocalEndian.h>
#include <mxm/core/mxm_macros.h>

#include <cstring>



mxmLocalEndian::mxmLocalEndian() {
  
  mxm::u32 one32    = 0x00000001;
  mxm::u8  *one_ptr = (mxm::u8 *)&one32;
  if(*one_ptr) {
    
#ifdef __MXM_DEBUG__
    mxm::sendStatusMessage(mxm::StatusMessage,
                           "local endian is little.",
                           this);
#endif
    LocalEndian = LittleEndian;
  }
  else {
    
#ifdef __MXM_DEBUG__
    mxm::sendStatusMessage(mxm::StatusMessage,
                           "local endian is big.",
                           this);
#endif
    LocalEndian = BigEndian;
  }
  
  Buffer     = 0;
  BufferSize = 0;
}


mxmLocalEndian::~mxmLocalEndian() {
  
  delete[] Buffer;
}



void mxmLocalEndian::convertFromLittle(void *data, int size) {
  
  if(LocalEndian == BigEndian) toggleEndian(data, size);
}


void mxmLocalEndian::convertToLittle(void *data, int size) {
  
  if(LocalEndian == BigEndian) toggleEndian(data, size);
}


void mxmLocalEndian::convertFromBig(void *data, int size) {
  
  if(LocalEndian == LittleEndian) toggleEndian(data, size);
}


void mxmLocalEndian::convertToBig(void *data, int size) {
  
  if(LocalEndian == LittleEndian) toggleEndian(data, size);
}


void mxmLocalEndian::toggleEndian(void *data, int size) {
  
  if(size > BufferSize) {
    
    delete[] Buffer;
    Buffer = new mxm::u8[size];
    MXM_CHECK_NULL(Buffer);
    BufferSize = size;
  }
  
  std::memcpy(Buffer, data, size);
  mxm::u8 *src_ptr = Buffer + size - 1;
  mxm::u8 *data_ptr = (mxm::u8 *)data;
  while(size) {
    *data_ptr++ = *src_ptr--;
    size--;
  }
};
