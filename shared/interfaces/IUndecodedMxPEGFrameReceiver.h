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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IUndecodedMxPEGFrameReceiver.h $
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



#ifndef __MX_IUNDECODEDMXPEGFRAMERECEIVER_H__
#define __MX_IUNDECODEDMXPEGFRAMERECEIVER_H__



#define MX_IUNDECODEDMXPEGFRAMERECEIVER_ID "IUndecodedMxPEGFrameReceiver"



#include "IErrorState.h"

#include <mxm/core/mxm_types.h>



class mxmHuffmanTableId;
class mxmUndecodedMxPEGFrameDescriptor;
class mxmAudioBlockInfo;



namespace mx {
  
  //! Interface to entities receiving parsed, but still undecoded <tt>MxPEG</tt>
  //! frames - these form the third stage in <tt>MxPEG</tt> stream decoding.
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class IUndecodedMxPEGFrameReceiver : public virtual IErrorState {
    
   public:
     //! Copies 64 bytes from the specified location to one of the receiver's
     //! 4 quantization tables.
     virtual void setQuantizationTable(int target_table,
                                       mxm::u8 *data_bytes) = 0;
     //! Copies bytes from the specified location to one of the receiver's
     //! 8 huffman tables.
     virtual void setHuffmanTable(const mxmHuffmanTableId &target_table,
                                  mxm::u8 *data_bytes) = 0;
     //! Hands an entire parsed, yet undecoded <tt>MxPEG</tt> (video) frame over
     //! to the receiver.
     /*!
      *  The frame descriptor is assumed to contain valid information.
      */
     virtual void receiveUndecodedMxPEGFrame(
                    mxmUndecodedMxPEGFrameDescriptor *frame_descriptor) = 0;
     //! Hands an (unprocessed) <tt>MxPEG</tt> audio frame (block) over to the
     //! receiver.
     virtual void receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                                         const mxm::u8 *sample_data) = 0;
  };
};


#endif   // __MX_IUNDECODEDMXPEGFRAMERECEIVER_H__

