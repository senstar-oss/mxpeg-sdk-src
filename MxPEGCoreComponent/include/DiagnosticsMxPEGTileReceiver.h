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
//  $LastChangedDate: 2007-06-29 15:34:53 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/DiagnosticsMxPEGTileReceiver.h $
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



#ifndef __MX_DIAGNOSTICSMXPEGTILERECEIVER_H__
#define __MX_DIAGNOSTICSMXPEGTILERECEIVER_H__



#define MX_DIAGNOSTICSMXPEGTILERECEIVER_ID "DiagnosticsMxPEGTileReceiver"



#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IMxPEGTileReceiver.h>
#include <interfaces/IErrorState.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmFrameInfo.h>



namespace mx {
  
  //! MxPEG tile receiver for testing/diagnostics purposes.
  /*!
   *  \ingroup mxsdk_mxpeg_core
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class DiagnosticsMxPEGTileReceiver : public mxmObject,
                                       public virtual IComponent,
                                       public virtual IMxPEGTileReceiver {
    
    //! These static methods provide interface and capability ids for
    //! registration with the framework (for both the component and its
    //! invoker).
   private:
    static mxm::smart<mxmStringList> _enumerateInterfaces() {
      return(new mxmStringList((mxmString(MX_DIAGNOSTICSMXPEGTILERECEIVER_ID),
                                MX_ICOMPONENT_ID,
                                MX_IMXPEGTILERECEIVER_ID,
                                MX_IERRORSTATE_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface) {
         if(interface == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_DIAGNOSTICSMXPEGTILERECEIVER_ID));
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
                                     const mxmString &interface) {
            return(_enumerateCapabilities(interface));
         }
         IComponent *newComponent() {
            return(new DiagnosticsMxPEGTileReceiver());
         }
      };
    public:
      //! Generates an invoker singleton for the \ref StreamFile component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
    
   private:
    int ReceivedFrames;
    int TileNum,
        AudioBlockNum;
    bool ErrorEncountered;
    mxm::u32 TilePixelBuffer[16*16*3/4];
    mxmFrameInfo FrameInfo;
    
   public:
    DiagnosticsMxPEGTileReceiver();
    ~DiagnosticsMxPEGTileReceiver();
    
   public:
    //! (Re)implemented.
    mxm::smart<mxmStringList> enumerateInterfaces();
    //! (Re)implemented.
    mxm::smart<mxmStringList> enumerateCapabilities(const mxmString &interface);
    
    //! (Re)implemented.
    void setFrameInfo(const mxmFrameInfo &frame_info);
    //! (Re)implemented.
    void videoResolutionChanged(int width, int height);
    //! (Re)implemented.
    void frameComplete();
    //! (Re)implemented.
    void provideVideoTilePixelTarget(int tile_x, int tile_y,
                                     mxm::u32 **target_ptr,
                                     int *row_stepping_ptr);
    //! (Re)implemented.
    void videoTileTransferred();
    //! Returns the number of frames that have so far been decoded.
    int frameCounter();
    
    //! (Re)implemented.
    mxm::u8 *provideAudioBlockTarget(const mxmAudioBlockInfo
                                            &audio_block_info);
    //! (Re)implemented.
    virtual void audioBlockTransferred();
    
    //! (Re)implemented.
    void setErrorState();
    //! (Re)implemented.
    bool errorState();
  };
};



#endif   // __MX_DIAGNOSTICSMXPEGTILERECEIVER_H__
