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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/MxPEGScanDecoderSoftwareOnly.h $
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



#ifndef __MX_MXPEGSCANDECODERSOFTWAREONLY_H__
#define __MX_MXPEGSCANDECODERSOFTWAREONLY_H__



#define MX_MXPEGSCANDECODERSOFTWAREONLY_ID "MxPEGScanDecoderSoftwareOnly"



#include "MxPEGScanDecoderSoftwareOnlyCore.h"

#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IMxPEGScanDecoder.h>

#include <Framework/Framework.h>



namespace mx {
  
  //! Software-only <tt>MxPEG</tt> scan decoder implementation.
  /*!
   *  \ingroup mxcpc_core
   *
   *  Through this portable scan decoder implementation we establish baseline
   *  <tt>MxPEG</tt> decoding on all three major target platforms in a fashion
   *  entirely independent of additional support libraries like Intel's
   *  <tt>IPP</tt>. However, where available the performance benefits of such
   *  support libraries can be exploited by plugging in another scan decoder
   *  implementation, for example an <tt>IPP</tt>-accelerated one.
   *
   *  \author Kai Hergenroether
   */
  class MxPEGScanDecoderSoftwareOnly : public MxPEGScanDecoderSoftwareOnlyCore,
                                       public virtual IComponent,
                                       public virtual IMxPEGScanDecoder        {
    
    //! These static methods provide interface and capability ids for
    //! registration with the framework (for both the component and its
    //! invoker).
   private:
    static mxm::smart<mxmStringList> _enumerateInterfaces() {
      return(new mxmStringList((mxmString(MX_MXPEGSCANDECODERSOFTWAREONLY_ID),
                               MX_ICOMPONENT_ID,
                               MX_IMXPEGSCANDECODER_ID,
                               MX_IUNDECODEDMXPEGFRAMERECEIVER_ID,
                               MX_ISENDSMXPEGTILES_ID,
                               MX_IERRORSTATE_ID)));
    }
    static mxm::smart<mxmStringList> _enumerateCapabilities(
                                        const mxmString &interface_id) {
      if(interface_id == mx::Framework::IdHintKey)
       return(new mxmStringList(MX_MXPEGSCANDECODERSOFTWAREONLY_ID));
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
        return(new MxPEGScanDecoderSoftwareOnly());
      }
    };
   public:
    //! Generates an invoker singleton for the \ref MxPEGParser component.
    static IComponentInvoker *newInvoker() {
      return(new Invoker());
    }
    
   private:
    IMxPEGTileReceiver *TileReceiver;
    bool Active;
    bool ErrorEncountered;
    int Width,
        Height;
    mxm::s16 *UpperHalfBuffer;
    
   public:
    MxPEGScanDecoderSoftwareOnly();
    ~MxPEGScanDecoderSoftwareOnly();
    
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
    void setMxPEGTileReceiver(IMxPEGTileReceiver *receiver);
    
    //! (Re)implemented.
    bool errorState();
    //! (Re)implemented.
    void setErrorState();
    
   private:
    void activate();
  };
};



#endif   // __MX_MXPEGSCANDECODERSOFTWAREONLY_H__
