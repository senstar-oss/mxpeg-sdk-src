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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxm_jpeg.h $
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



#ifndef __MXM_JPEG_H__
#define __MXM_JPEG_H__



#include <mxm/core/mxm_types.h>



namespace mxm {
  
  //! Some <tt>JPEG</tt>-specific stuff.
  namespace JPEG {
    
    const u8 SOI   = 0xd8;
    const u8 APP0  = 0xe0;
    const u8 COM   = 0xfe;
    const u8 DQT   = 0xdb;
    const u8 DHT   = 0xc4;
    const u8 DRI   = 0xdd;
    const u8 SOF0  = 0xc0;
    const u8 SOS   = 0xda;
    const u8 EOI   = 0xd9;
    const u8 APP13 = 0xed;
     
#ifdef _MSC_VER
    #pragma pack(push,1)
#endif
    
    //! Assimilated from MxPEG Viewer (Codec2.h)
    typedef struct {
      
      mxm::u8 ff;               // Marker-leadin
      mxm::u8 app0_marker;      // APP0-Marker
      mxm::u8 len_hi, len_lo;   // Length of APP0 block (2 bytes)
      char id[5];               // Block ID (4 bytes - ASCII "JFIF" + \0)
      mxm::u8 major_vers, minor_vers;
                                // Version Major,Minor (2 bytes - major first)
      mxm::u8 units;            // Units (1 byte - 0x00 = none,
                                //                 0x01 = inch,
                                //                 0x02 = cm)
      mxm::u8 xdpu_hi, xdpu_lo; // Xdpu (2 bytes - dots per unit horizontal)
      mxm::u8 ydpu_hi, ydpu_lo; // Ydpu (2 bytes - dots per unit vertical)
      mxm::u8 thumbnail_x;      // Thumbnail X size (1 byte)
      mxm::u8 thumbnail_y;      // Thumbnail Y size (1 byte)
    }
#ifndef _MSC_VER
    __attribute__ ((packed))
#endif
    MarkerLayoutAPP0;
    
    //! Assimilated from MxPEG Viewer (Codec2.h)
    typedef struct {
      
      mxm::u8 ff;
      mxm::u8 marker;
      mxm::u8 len_hi, len_lo;
      mxm::u8 data_precision;
      mxm::u8 y_sz_hi, y_sz_lo;
      mxm::u8 x_sz_hi, x_sz_lo;
      mxm::u8 num_components;
      struct {
        mxm::u8 component_id;
        mxm::u8 sampl_fact;
        mxm::u8 quant_tbl_no;
      }
#ifndef _MSC_VER
      __attribute__ ((packed))
#endif
      components[3];
    }
#ifndef _MSC_VER
    __attribute__ ((packed))
#endif
    MarkerLayoutSOF0;
    
    //! Assimilated from MxPEG Viewer (Codec2.h)
    typedef struct {
      
      mxm::u8 ff;
      mxm::u8 sos_marker;
      mxm::u8 sz_hi;
      mxm::u8 sz_lo;
      mxm::u8 comps_in_scan;
      struct {
        mxm::u8 comp_id;
        mxm::u8 td_ta;
      }
 #ifndef _MSC_VER
      __attribute__ ((packed))
#endif
      components[3];
      
      mxm::u8 Ss;
      mxm::u8 Se;
      mxm::u8 Ah_Al;
    }
#ifndef _MSC_VER
    __attribute__ ((packed))
#endif
    MarkerLayoutSOS;

#ifdef _MSC_VER
    #pragma pack(pop)
#endif
  
  };
};



#endif   // __MXM_JPEG_H__
