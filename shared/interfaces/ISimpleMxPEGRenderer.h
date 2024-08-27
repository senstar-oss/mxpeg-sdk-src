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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleMxPEGRenderer.h $
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



#ifndef __MX_ISIMPLEMXPEGRENDERER_H__
#define __MX_ISIMPLEMXPEGRENDERER_H__



#define MX_ISIMPLEMXPEGRENDERER_ID "ISimpleMxPEGRenderer"



#include <interfaces/IMxPEGTileReceiver.h>

#include <mxm/core/mxmQtSignal.h>



class QGLWidget;



namespace mx {
   
   //! Interface to the <tt>OpenGL</tt>-based <tt>MxPEG</tt> decoder demo 
   //! backend internally buffering video tile updates and transmitting to the
   //! graphics hardware frame-wise and only as frames complete.
   /*!
    *  \ingroup mxsdk_mxpeg_core
    *
    *  This backend was meant only for demo purposes.
    */
   class /*MXCPC_TEK_API*/ ISimpleMxPEGRenderer
                            : public virtual IMxPEGTileReceiver {
      
    public:
      //! <b>[Config Phase]</b> Does not assume ownership over the display
      //! widget.
      virtual void setGLWidget(QGLWidget *viewer_display) = 0;
      //! <b>[Config Phase]</b> Configures framerate measure mode.
      /*!
       *   Per default, the framerate measure mode is disabled.
       */
      virtual void setFramerateMeasureModeEnabled(bool enabled,
                                                  int measure_interval_ms
                                                       = 10000) = 0;
      //! <b>[Config Phase]</b> Toggles whether or not the frame should be
      //! scaled to match the current window dimensions.
      /*!
       *   Per default, scale-to-fit is disabled.
       */
      virtual void enableScaleToFit(bool enabled) = 0;
      //! Issues the <tt>OpenGL</tt> calls copying the current frame's pixel
      //! data to the viewer window.
      virtual void drawFrame() = 0;
      //! Returns the signal that is emitted if the framerate measure mode is
      //! enabled and a new framerate measure becomes available (roughly every
      //!  <tt>n</tt> milliseconds with <tt>n</tt> as it was specified to
      //! \ref setFramerateMeasureModeEnabled().
      virtual mxmQtSignal framerateMeasuredSignal() = 0;
      //! Returns the signal that gets emitted when the first frame has been
      //! rendered.
      virtual mxmQtSignal firstFrameRenderedSignal() = 0;
   };
};



#endif   // __MX_ISIMPLEMXPEGRENDERER_H__
