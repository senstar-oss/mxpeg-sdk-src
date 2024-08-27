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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IPPAcceleratedMxPEGComponent/trunk/include/MxPEGScanToJPEGConverterIPP.h $
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



#ifndef __MX_MXPEGSCANTOJPEGCONVERTERIPP_H__
#define __MX_MXPEGSCANTOJPEGCONVERTERIPP_H__



#define MX_MXPEGSCANTOJPEGCONVERTERIPP_ID \
         "MxPEGScanToJPEGConverterIPP"



#include "MxPEGScanDecoderIPPCore.h"

#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IMxPEGScanToJPEGConverter.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxm_jpeg.h>



class mxmUndecodedMxPEGFrameDescriptor;



namespace mx {
  
  class ISegmentedStreamReceiver;
  
  //! <tt>IPP</tt>-accelerated converter implementation (<tt>MxPEG</tt> to
  //! <tt>JPEG</tt>).
  /*!
   *  \ingroup mxsdk_ipp_mxpeg
   *
   *  The current implementation produces <tt>JPEG</tt> in <tt>Y22U11V11</tt>
   *  sampling mode, regardless of the sampling mode used in the input stream.
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class MxPEGScanToJPEGConverterIPP
         : public MxPEGScanDecoderIPPCore,
           public virtual IComponent,
           public virtual IMxPEGScanToJPEGConverter {
    
    //! These static methods provide interface and capability ids for
    //! registration with the framework (for both the component and its
    //! invoker).
   private:
    static mxm::smart<mxmStringList> _enumerateInterfaces() {
      return(new mxmStringList(
                   (mxmString(MX_MXPEGSCANTOJPEGCONVERTERIPP_ID),
                    MX_ICOMPONENT_ID,
                    MX_IMXPEGSCANTOJPEGCONVERTER_ID,
                    MX_IUNDECODEDMXPEGFRAMERECEIVER_ID,
                    MX_ISENDSSEGMENTEDSTREAM_ID,
                    MX_IERRORSTATE_ID)));
    }
    static mxm::smart<mxmStringList> _enumerateCapabilities(
                                        const mxmString &interface_id) {
      if(interface_id == mx::Framework::IdHintKey)
       return(new mxmStringList(MX_MXPEGSCANTOJPEGCONVERTERIPP_ID));
      if(interface_id == mx::Framework::ObsoletesHintKey)
       return(new mxmStringList("MxPEGScanToJPEGConverterSoftwareOnly"));
      else
      return(new mxmStringList());
    }
   private:
    class Invoker : public mxmObject, public virtual IComponentInvoker {
     public:
      mxm::smart<mxmStringList> enumerateInterfaces() {
        return(_enumerateInterfaces());
      }
      mxm::smart<mxmStringList> enumerateCapabilities(
                                  const mxmString &interface_id) {
        return(_enumerateCapabilities(interface_id));
      }
      IComponent *newComponent() {
        return(new MxPEGScanToJPEGConverterIPP());
      }
    };
   public:
    //! Generates an invoker singleton for the \ref MxPEGParser component.
    static IComponentInvoker *newInvoker() {
      return(new Invoker());
    }
    
   private:
    static const int BitstreamBufferSize = 512;
   private:
    static const unsigned char ZigZag_StoreMap[64];
    static const mxm::u8 HuffmanTable_Y_DC[],
                         HuffmanTable_Y_AC[],
                         HuffmanTable_UV_DC[],
                         HuffmanTable_UV_AC[];
    
   private:
    ISegmentedStreamReceiver *JPEGReceiver;
    Ipp16u                *ForwardQuantizationTables[4];
    Ipp16s *CoeffBuffer;
    int TileNumX, TileNumY;
    IppiEncodeHuffmanSpec *HuffmanEncodeSpec_Y_DC,
                          *HuffmanEncodeSpec_Y_AC,
                          *HuffmanEncodeSpec_UV_DC,
                          *HuffmanEncodeSpec_UV_AC;
    mxm::JPEG::MarkerLayoutAPP0 APP0_Block;
    mxm::JPEG::MarkerLayoutSOF0 SOF0_Block;
    mxm::JPEG::MarkerLayoutSOS  SOS_Block;
    int FramesProcessed;
    bool ErrorEncountered;
    
    // only valid while receiveUndecodedMxPEGFrame() executes...
    Ipp16u *ForwardQuantTableForMCU[MaxMCUsPerTile];
    
   public:
    MxPEGScanToJPEGConverterIPP();
    ~MxPEGScanToJPEGConverterIPP();
    
   public:
    //! (Re)implemented.
    mxm::smart<mxmStringList> enumerateInterfaces();
    //! (Re)implemented.
    mxm::smart<mxmStringList> enumerateCapabilities(
                                const mxmString &interface);
    
    //! (Re)implemented.
    void receiveUndecodedMxPEGFrame(mxmUndecodedMxPEGFrameDescriptor
                                     *frame_descriptor);
    //! (Re)implemented.
    void receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                                const mxm::u8 *sample_data);
    //! (Re)implemented.
    void setQuantizationTable(int target_table,
                              mxm::u8 *data_bytes);
    
    //! (Re)implemented.
    void setSegmentedStreamReceiver(ISegmentedStreamReceiver *receiver);
    
    //! (Re)implemented.
    bool errorState();
    //! (Re)implemented.
    void setErrorState();
  };
};



#endif   // __MX_MXPEGSCANTOJPEGCONVERTERIPP_H__
