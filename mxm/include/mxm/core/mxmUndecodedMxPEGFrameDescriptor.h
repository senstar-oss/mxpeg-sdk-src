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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmUndecodedMxPEGFrameDescriptor.h $
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



#ifndef __MXM_UNDECODEDFRAMEDESCRIPTOR_H__
#define __MXM_UNDECODEDFRAMEDESCRIPTOR_H__



#include <mxm/core/mxmObject.h>
#include <mxm/core/mxm_types.h>
#include <mxm/core/mxm_core_platform_macros.h>



//! Describes a parsed, but still undecoded <tt>MxPEG</tt> frame in memory.
/*!
 *  \ingroup modularMX_core
 *
 *  \note As a resulkt of being backported from early <tt>mxcpc</tt> code, the
 *  data members can be accessed directly, without the need of going through
 *  accessor functions. This should be changed.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmUndecodedMxPEGFrameDescriptor : public mxmObject {
  
 private:
  static const int MaxComponents = 4;
  
 public:
  enum SamplingMode { Y22U11V11 = 0,   // MOBO
                      Y21U11V11 = 1    // Axis
                    };
  
 public:
  struct ComponentInfo {
    int HorizSamplingFactor,
        VertSamplingFactor;
    int QuantTable;
    int HuffmanDC,
        HuffmanAC;
  };
  
 public:
  int             Width,
                  Height;
  int             TileNumX,
                  TileNumY;
  unsigned char   *ScanBytes;
  int             ScanLength;
  unsigned char   *TileBits;
  unsigned char   *StartOfImage;
  int             ImageLength;
  mxm::frame_time TimeStamp;
  bool            TimeStampValid;
  mxm::u32        GenerationNumber;
  bool            GenerationNumberValid;
  int             ComponentNum;
  ComponentInfo   ComponentInfos[MaxComponents];
  SamplingMode    Sampling;
 private:
  void *StableABIDataExtension;
  
 public:
  mxmUndecodedMxPEGFrameDescriptor();
  void setResolution(int width, int height);
  void setTimeStamp(mxm::frame_time time_stamp);
  void setGenerationNumber(mxm::u32 gen_no);
  void setComponentNum(int component_num);
  void setComponentParameters(int component,
                              int horiz_sampling, int vert_sampling,
                              int quant_table);
  void setMoreComponentParameters(int component,
                                  int huffman_dc, int huffman_ac);
  void setSamplingMode(SamplingMode sampling);
  void init(int width, int height);
  //! Component information and resolution settings are not reset.
  void resetForNextFrame();
};



#endif   // __MXM_UNDECODEDFRAMEDESCRIPTOR_H__
