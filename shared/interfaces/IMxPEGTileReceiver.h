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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IMxPEGTileReceiver.h $
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



#ifndef __MX_IMXPEGTILERECEIVER_H__
#define __MX_IMXPEGTILERECEIVER_H__



#define MX_IMXPEGTILERECEIVER_ID "IMxPEGTileReceiver"



#include <interfaces/IErrorState.h>

#include <mxm/core/mxmFrameInfo.h>
#include <mxm/core/mxmAudioBlockInfo.h>



namespace mx {
  
  //! Interface to entities receiving both <tt>MxPEG</tt> video and audio tiles.
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  Tile receivers decide for what is actually to be done with video and audio
   *  data decoded from an <tt>MxPEG</tt> stream, and thereby form the final
   *  stage in an <tt>MxPEG</tt> decoder configuration.
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class IMxPEGTileReceiver : public virtual IErrorState {
    
   public:
     //! To inform the receiver about details concerning the current frame.
     /*!
      *  Guaranteed to be called for each frame.
      */
     virtual void setFrameInfo(const mxmFrameInfo &frame_info) = 0;
     //! To inform the receiver that the stream switched to another video 
     //! resolution.
     /*!
      *  Guaranteed to be called at least once before the first tiles arrive.
      *
      *  Guaranteed to be called with both dimensions being positive.
      */
     virtual void videoResolutionChanged(int width, int height) = 0;
    
    // ! To inform the receiver that decoding of the current frame was finished
    //! and that - as a result - the following video/audio tiles belong to the
    //! next frame.
    /*!
     *  \note A call to this method does not mean that the frame was decoded 
     *  <i>successfully</i>.
     */
    virtual void frameComplete() = 0;
    
    //! By calling this method the sending entity asks the receiver where to
    //! store the decoded pixel data for the next video tile.
    /*!
     *  The pixel data will be written out <tt>RGBRGBRGB...</tt> with one byte
     *  per color channel. Between rows, a certain amount of memory is skipped
     *  according to the specified row <i>stepping which</i> is given in
     *  <tt>u32</tt> units. It is perfectly legal to specify negative values
     *  here, in fact this is how it will be handled in most cases when
     *  <tt>OpenGL</tt> is involved in the actual rendering, because here, the
     *  origin of 2D image data is usually located in the lower/left corner.
     */
    virtual void provideVideoTilePixelTarget(int tile_x, int tile_y,
                                             mxm::u32 **target_ptr,
                                             int *row_stepping_ptr)   = 0; 
    //! To inform the receiver about a transferred video tile.
    /*! It has been stored to the pixel target provided by the receiver 
     *  through a previous call to provideVideoTilePixelTarget(). 
     */
    virtual void videoTileTransferred() = 0;
    
    //! Receivers are required to increment an <tt>int</tt>-valued frame counter
    //! for each completed frame and to return it upon request via this method.
    /*!
     *  When wrapping around, this counter has to continue with <tt>0</tt>.
     */
    virtual int frameCounter() = 0;
    
    //! By calling this method the sending entity asks the backend where to
    //! store the audio data for the next audio block ("tile").
    /*!
     *  \return a buffer capable of holding at least that many bytes as reported
     *          by <tt>audio_block_info</tt>'s mxmAudioBlockInfo::size() method,
     *          or <tt>0</tt>, in case the receiver is not interested in audio
     */
    virtual mxm::u8 *provideAudioBlockTarget(const mxmAudioBlockInfo
                                                    &audio_block_info) = 0;
    //! To inform the backend that the current audio block has been copied.
    virtual void audioBlockTransferred() = 0;
  };
};



#endif   // __MX_IMXPEGTILERECEIVER_H__
