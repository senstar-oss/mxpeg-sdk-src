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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/MxPEGParser.h $
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



#ifndef __MX_MXPEGPARSER_H__
#define __MX_MXPEGPARSER_H__



#define MX_MXPEGPARSER_ID "MxPEGParser"



#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IMxPEGParser.h>
#include <interfaces/IErrorState.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>
#include <mxm/core/mxmLocalEndian.h>
#include <mxm/core/mxmObject.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringList.h>



namespace mx {
   
   //! Versatile <tt>MxPEG</tt> parser.
   /*!
    *  \ingroup mxsdk_mxpeg_core
    *
    *  Follows the conventional framewise approach.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class MxPEGParser : public mxmObject,
                       public virtual IComponent,
                       public virtual IMxPEGParser {
      
      //! These static methods provide interface and capability ids for
      //! registration with the framework (for both the component and its
      //! invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList((mxmString(MX_MXPEGPARSER_ID),
                                   MX_ICOMPONENT_ID,
                                   MX_IMXPEGPARSER_ID,
                                   MX_IPROCESSESSTREAM_ID,
                                   MX_ISENDSUNDECODEDMXPEGFRAMES_ID,
                                   MX_IERRORSTATE_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface) {
         if(interface == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_MXPEGPARSER_ID));
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
            return(new MxPEGParser());
         }
      };
    public:
      //! Generates an invoker singleton for the \ref MxPEGParser component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    public:
      enum DecoderState { AwaitMarker,
                          StartOfImage,
                          RecordStartOfImage,
                          APP0,
                          Comment,
                          M1IMG,
                          MxF,
                          QuantizationTable,
                          HuffmanTable,
                          DefineRestartInterval,
                          BitMap,
                          StartOfFrame,
                          StartOfScan,
                          Scan,
                          Audio,
                          ReadPayloadAndReturn,
                          NoGoodState
                        };
      
    private:
      IStreamSource                *Source;
      IUndecodedMxPEGFrameReceiver *FrameReceiver;
      bool                         Active;
      
      // state machine datafields...
      DecoderState State;
      unsigned char *CurrentBytePtr;
      int BytesLeft;
      int AwaitMarkerStage;
      int ReadMarkerStage;
      unsigned char ReadMarkerFirstLenByte;
      int MarkerPayloadSize,
          MarkerBytesNeeded;
      int ScanStage;
      unsigned char *MarkerPayload;
      
      mxmUndecodedMxPEGFrameDescriptor FrameDescriptor;
      int Width, Height;
      
      unsigned char *PrefetchBuffer;
      int PrefetchBuffSize;
      int PrefetchedBytes;
      
      bool Fresh;
      mxmLocalEndian LocalEndian;
      
      bool ErrorEncountered;
      
    public:
      MxPEGParser();
      ~MxPEGParser();
    
    public:
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateInterfaces();
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateCapabilities(
                                   const mxmString &interface);
      
      //! (Re)implemented.
      void setStreamSource(IStreamSource *stream_source);
      //! (Re)implemented.
      void setUndecodedMxPEGFrameReceiver(IUndecodedMxPEGFrameReceiver
                                           *frame_receiver);
      //! (Re)implemented.
      int processStreamBytes(int num);
      //! (Re)implemented.
      bool sourceStillUp();
     
      //! (Re)implemented.
      bool errorState();
      //! (Re)implemented.
      void setErrorState();
      
    private:
      //! Internally used to pump raw MxPEG stream data into the parsr's state
      //! machine.
      void feedBytes(unsigned char *buffer, int num);
      //! Private helper method.
      bool awaitMarker(unsigned char *marker_type);
      //! Private helper method.
      bool readMarkerPayload(void);
      //! Private helper method.
      bool doScan(void);
      //! Helper.
      void activate();
   };
};

#endif   // __MX_MXPEGPARSER_H__
