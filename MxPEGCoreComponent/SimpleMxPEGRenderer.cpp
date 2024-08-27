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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/SimpleMxPEGRenderer.cpp $
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



#include "include/SimpleMxPEGRenderer.h"



mx::SimpleMxPEGRenderer::SimpleMxPEGRenderer() {
   
   Active                   = false;
   
   Viewer                   = 0;
   
   CenterX = CenterY        = 0;
   OffsetX = OffsetY        = 0;
   
   FrameCount               = 0;
   
   FramerateMeasureInterval = -1;   // don't measure framerate
   FramerateMeasureTime.start();
   FramerateFrameCount      = 0;
   
   ScaleToFit               = false;
   
   SignalledFirstFrame      = false;
   
   ErrorEncountered         = false;
}


mx::SimpleMxPEGRenderer::~SimpleMxPEGRenderer() {
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          mxmString("signing off after having rendered ")
                           + FrameCount + " frames",
                          this);
}



void mx::SimpleMxPEGRenderer
     ::setGLWidget(QGLWidget *viewer_display) {
   
   if(Active) mxm::terminal("already active, can't set display!",
                            this);
   
   Viewer = viewer_display;
}



void mx::SimpleMxPEGRenderer
      ::setFramerateMeasureModeEnabled(bool enabled, int measure_interval_ms) {
   
   if(!Active) activate();
   
   if(!enabled) FramerateMeasureInterval = -1;
   else {
      FramerateMeasureInterval = measure_interval_ms;
      if(FramerateMeasureInterval < 0) FramerateMeasureInterval = 10000;  // 10s
      FramerateMeasureTime.start();
      FramerateFrameCount = 0;
   }
}


void mx::SimpleMxPEGRenderer::enableScaleToFit(bool enabled) {
   
   //if(Active) mxm::terminal("already active, can't set scale-to-fit!",
   //                         this);
   
   ScaleToFit = enabled;
}


bool mx::SimpleMxPEGRenderer::activate() {
   
   if(Active) mxm::terminal("already active!",
                            this);
   
   if(!Viewer) mxm::terminal("no display set, can't activate!",
                             this);
   
   Active = true;
   
   return(true);
}


void mx::SimpleMxPEGRenderer::videoResolutionChanged(int width,
                                                          int height) {
   
   if(!Active) activate();
   
   BufferedMxPEGDecoderBackEnd::videoResolutionChanged(width, height);
   
   if(PixelBuffer) {
      
      CenterX = Width/2;    OffsetX = Width  - CenterX;
      CenterY = Height/2;   OffsetY = Height - CenterY;
   }
   else {
      
      CenterX = CenterY = 0;
      OffsetX = OffsetY = 0;
   }
   
   Viewer->updateGL();
}


void mx::SimpleMxPEGRenderer::setErrorState(void) {
   
   if(!Active) activate();
   
   ErrorEncountered = true;
}


bool mx::SimpleMxPEGRenderer::errorState() {
   
   if(!Active) activate();
   
   return(ErrorEncountered);
}


void mx::SimpleMxPEGRenderer::frameComplete(void) {
   
   if(!Active) activate();
   
   int elapsed;
   float fps;
   
   // measure framerate, if enabled...
   fps = -1.0f;
   if(FramerateMeasureInterval >= 0) {
      
      FramerateFrameCount++;
      
      if((elapsed = FramerateMeasureTime.elapsed())
          >= FramerateMeasureInterval)              {
         
         fps = (float)FramerateFrameCount * 1000.0f / (float)elapsed;
         
         FramerateMeasureTime.start();
         FramerateFrameCount = 0;
      }
   }
   
   if(!PixelBuffer) {
      
      if(fps >= 0.0f) emit framerateMeasured(fps);
      
      return;
   }
   
   Viewer->makeCurrent();
   drawFrame();
   Viewer->swapBuffers();
   
   
   FrameCount++;
   if(FrameCount < 0) FrameCount = 0;   // overflow?
   
   if(fps >= 0.0f) emit framerateMeasured(fps);
   
   if(!SignalledFirstFrame) {
      
      SignalledFirstFrame = true;
      
      emit firstFrameRendered();
   }
}


void mx::SimpleMxPEGRenderer::videoTileTransferred() {
   
   //if(!Active) activate();
}


mxm::u8 *mx::SimpleMxPEGRenderer
          ::provideAudioBlockTarget(const mxmAudioBlockInfo &audio_block_info) {
   
   return(0);
}


void mx::SimpleMxPEGRenderer::audioBlockTransferred() {
   
}


int mx::SimpleMxPEGRenderer::frameCounter() {
   
   if(!Active) activate();
   
   return(FrameCount);
}


void mx::SimpleMxPEGRenderer::drawFrame() {
   
   if(!Active) activate();
   
   if(!PixelBuffer)      return;
   if(!Width || !Height) return;
   
   int win_width  = Viewer->width(),
       win_height = Viewer->height();
   
   if(   ScaleToFit
      && (   (abs(win_width - Width) > 2)
          || (abs(win_height - Height) > 2))) {
      
      // scale to fit display window, but keep original aspect ratio:
      GLfloat zoom_a = (GLfloat)win_width  / (GLfloat)Width,
              zoom_b = (GLfloat)win_height / (GLfloat)Height,
              zoom   = (zoom_a > zoom_b) ? zoom_b : zoom_a;
      
      int x = (win_width - Width*zoom) / 2;
      if(x < 0) x = 0;
      int y = (win_height+Height*zoom)/2;
      if(y > win_height) y = win_height;
      
      glRasterPos2i(x, y);
      glPixelZoom(zoom, zoom);
      
      glDrawPixels(Width, Height,
                   GL_RGB, GL_UNSIGNED_BYTE,
                   PixelBuffer + PixelBufferOffset);
      glPixelZoom(1.0f, 1.0f);
   } 
   else {   // center...
      
      glRasterPos2i(win_width/2, win_height/2);
      glBitmap(0,                 0,
               0.0f,              0.0f,
               (GLfloat)-CenterX, (GLfloat)-CenterY,
               0);
      glDrawPixels(Width, Height,
                   GL_RGB, GL_UNSIGNED_BYTE,
                   PixelBuffer + PixelBufferOffset);
   }
}


mxmQtSignal mx::SimpleMxPEGRenderer::framerateMeasuredSignal() {
   
   return(mxmQtSignal(this,
                      SIGNAL(framerateMeasured(float))));
}


mxmQtSignal mx::SimpleMxPEGRenderer::firstFrameRenderedSignal() {
   
   return(mxmQtSignal(this,
                      SIGNAL(firstFrameRendered())));
}