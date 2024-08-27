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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IPPAcceleratedMxPEGComponent/trunk/include/MxPEGScanDecoderIPPCore.h $
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



#ifndef __MX_MXPEGSCANDECODERIPPCORE_H__
#define __MX_MXPEGSCANDECODERIPPCORE_H__



#include <interfaces/IUndecodedMxPEGFrameReceiver.h>

#include <mxm/core/mxmObject.h>

#include <ippj.h>



class mxmHuffmanTableId;



namespace mx {
  
  //! Manages Huffman and quantization tables for derived <tt>IPP</tt>-accelerated
  //! scan decoders.
  /*!
   *  \ingroup mxsdk_ipp_mxpeg
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class MxPEGScanDecoderIPPCore : public mxmObject,
                                  public virtual IUndecodedMxPEGFrameReceiver {
    
   protected:
    static const int MaxMCUsPerTile = 8;
    
   private:
    IppiDecodeHuffmanSpec *HuffmanTrees[2][4];
    Ipp16u                *QuantizationTables[4];
    int HuffmanSpecSize;
    
   protected:
    //! The following fields' state is defined to only be valid during scan
    //! sweeps - so they won't show up in the constructor...
    int MCUsPerTile;
    const IppiDecodeHuffmanSpec *HuffmanDCForMCU[MaxMCUsPerTile],
                                *HuffmanACForMCU[MaxMCUsPerTile];
    Ipp16u *QuantTableForMCU[MaxMCUsPerTile];
    Ipp16s LastHuffmanDCs[3];
    Ipp16s *LastHuffmanDCPtrForMCU[MaxMCUsPerTile];
    Ipp16s CoeffBuffers[MaxMCUsPerTile][64];
    Ipp8u YUV_ColorValues[MaxMCUsPerTile * 8*8];
    Ipp8u *YUV_TargetPtrs[MaxMCUsPerTile];
    int YUV_TargetSteps[MaxMCUsPerTile];
    const Ipp8u *YUV_SrcPtrs[3];
    int YUV_SrcSteps[3];
    IppiSize YUV_RGB_RoiSize;
    
   public:
    MxPEGScanDecoderIPPCore();
    ~MxPEGScanDecoderIPPCore();
    
   public:
    //! (Re)implemented.
    void setQuantizationTable(int target_table,
                              mxm::u8 *data_bytes);
    //! (Re)implemented.
    void setHuffmanTable(const mxmHuffmanTableId &target_table,
                         mxm::u8 *data_bytes);
    
   protected:
    //! Used by derived implementations to compute the mappings for the Huffman
    //! and quantization tables managed by the core.
    bool mapTables(mxmUndecodedMxPEGFrameDescriptor *frame_descriptor);
  };
};



#endif   // __MX_MXPEGSCANDECODERIPPCORE_H__
