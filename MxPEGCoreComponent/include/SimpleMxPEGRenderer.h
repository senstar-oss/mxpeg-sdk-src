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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/SimpleMxPEGRenderer.h $
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



#ifndef __MX_SIMPLEMXPEGRENDERER_H__
#define __MX_SIMPLEMXPEGRENDERER_H__



#define MX_SIMPLEMXPEGRENDERER_ID "SimpleMxPEGRenderer"



#include "BufferedMxPEGDecoderBackEnd.h"

#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/ISimpleMxPEGRenderer.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxm_smart.h>

#include <QObject>
#include <QTime>
#include <QGLWidget>



namespace mx {
   
   //! <tt>OpenGL</tt>-based <tt>MxPEG</tt> decoder backend internally buffering
   //! video tile updates and transmitting to the graphics hardware frame-wise
   //! and only as frames complete.
   /*!
    *  \ingroup mxsdk_mxpeg_core
    *
    *  This backend was meant only for demo purposes.
    */
   class SimpleMxPEGRenderer : public QObject,
                               public BufferedMxPEGDecoderBackEnd,
                               public virtual IComponent,
                               public virtual ISimpleMxPEGRenderer {
   
      Q_OBJECT
      
    private:
      bool Active;
      QGLWidget *Viewer;
      int CenterX, CenterY,
          OffsetX, OffsetY;
      int FrameCount;
      int FramerateMeasureInterval;
      QTime FramerateMeasureTime;
      int FramerateFrameCount;
      bool ScaleToFit;
      bool SignalledFirstFrame;
      bool ErrorEncountered;
      
      //! These static methods provide interface and capability ids for
      //! registration with the framework (for both the component and its
      //! invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList((mxmString(MX_SIMPLEMXPEGRENDERER_ID),
                                   MX_ICOMPONENT_ID,
                                   MX_ISIMPLEMXPEGRENDERER_ID,
                                   MX_IMXPEGTILERECEIVER_ID,
                                   MX_IERRORSTATE_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                         const mxmString &interface_id) {
         if(interface_id == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_SIMPLEMXPEGRENDERER_ID));
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
            return(new SimpleMxPEGRenderer());
         }
      };
    public:
      //! Generates an invoker singleton for the \ref StreamFile component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    public:
      //! The constructor does <b>not</b> assume ownership over the viewer
      //! widget!
      SimpleMxPEGRenderer();
      ~SimpleMxPEGRenderer();
      
    signals:
      //! Emitted if framerate measure mode is enabled and a new framerate
      //! measure becomes available (roughly every <i>n</i> milliseconds with
      //! <i>n</i> as it was specified to setFramerateMeasureModeEnabled()).
      void framerateMeasured(float fps);
      //! Emitted when the first frame has been rendered.
      void firstFrameRendered();
      
    public:
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateInterfaces() {
         return(_enumerateInterfaces());
      }
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateCapabilities(
                                   const mxmString &interface_id) {
         return(_enumerateCapabilities(interface_id));
      }
      
      //! (Re)implemented.
      void setGLWidget(QGLWidget *viewer_display);
      //! (Re)implemented.
      void setFramerateMeasureModeEnabled(bool enabled,
                                          int measure_interval_ms = 10000);
      //! (Re)implemented.
      void enableScaleToFit(bool enabled);
      //! (Re)implemented.
      void drawFrame();
      //! (Re)implemented.
      mxmQtSignal framerateMeasuredSignal();
      //! (Re)implemented.
      mxmQtSignal firstFrameRenderedSignal();
      
      //! (Re)implemented.
      void videoResolutionChanged(int width, int height);
      //! (Re)implemented.
      void errorEncountered();
      //! (Re)implemented.
      void frameComplete();
      //! (Re)implemented.
      void videoTileTransferred();
      //! (Re)implemented.
      mxm::u8 *provideAudioBlockTarget(const mxmAudioBlockInfo
                                       &audio_block_info);
      //! (Re)implemented.
      void audioBlockTransferred();
      //! (Re)implemented.
      int frameCounter();
      
      //! (Re)implemented.
      void setErrorState();
      //! (Re)implemented.
      bool errorState();
      
    private:
      bool activate();
   };
};



#endif   // __MX_SIMPLEMXPEGRENDERER_H__
