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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxm_colorspaces.h $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licenced under the BSD licence,
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



#ifndef __MXM_COLORSPACES_H__
#define __MXM_COLORSPACES_H__



#include <mxm/core/mxm_core_platform_macros.h>



namespace mxm {
   
  //! Colorspace conversion from <tt>RGB</tt> to <tt>YUV</tt>.
  inline void convert_RGB_to_YUV(
                             mxm::u8  r, mxm::u8  g, mxm::u8  b,
                             mxm::u8 *y, mxm::u8 *u, mxm::u8 *v) {
    
    int int_r, int_g, int_b;
    int int_y, int_u, int_v;
    
    int_r = (int)r;
    int_g = (int)g;
    int_b = (int)b;
    
    // coefficients computed from "Video Demystified" by K. JACK...
    int_y = (( 263*int_r + 516*int_g + 100*int_b) >> 10)   +    16;
    int_u = ((-152*int_r - 298*int_g + 450*int_b) >> 10)   +   128;
    int_v = (( 450*int_r - 377*int_g -  73*int_b) >> 10)   +   128;
    
    mxm::clamp_int(&int_y, 0, 255);
    mxm::clamp_int(&int_u, 0, 255);
    mxm::clamp_int(&int_v, 0, 255);
    
    *y = (mxm::u8)int_y;
    *u = (mxm::u8)int_u;
    *v = (mxm::u8)int_v;
  }
  
  //! Colorspace conversion from <tt>RGB</tt> to <tt>YCbCr</tt> honoring
  //! <tt>ITU-R BT.601</tt>
  inline void convert_RGB_to_YCbCr(
                             mxm::u8  r, mxm::u8   g, mxm::u8   b,
                             mxm::u8 *y, mxm::u8 *Cb, mxm::u8 *Cr) {
    
    int int_r, int_g,  int_b;
    int int_y, int_Cb, int_Cr;
    
    int_r = (int)r;
    int_g = (int)g;
    int_b = (int)b;
    
    // coefficients computed from ITU-R BT.601...
    int_y  = (( 306*int_r + 601*int_g + 117*int_b) >> 10);
    int_Cb = ((-173*int_r - 339*int_g + 512*int_b) >> 10)   +   128;
    int_Cr = (( 512*int_r - 429*int_g -  83*int_b) >> 10)   +   128;
    
    mxm::clamp_int(&int_y,  0, 255);
    mxm::clamp_int(&int_Cb, 0, 255);
    mxm::clamp_int(&int_Cr, 0, 255);
    
    *y  = (mxm::u8)int_y;
    *Cb = (mxm::u8)int_Cb;
    *Cr = (mxm::u8)int_Cr;
  }
};



#endif   // __MXM_COLORSPACES_H__
