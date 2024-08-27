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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/MxPEGScanDecoderSoftwareOnlyCore.cpp $
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



#include "include/MxPEGScanDecoderSoftwareOnlyCore.h"

#include <mxm/core/mxmUndecodedMxPEGFrameDescriptor.h>
#include <mxm/core/mxm_generic_stuff.h>



const int mx::MxPEGScanDecoderSoftwareOnlyCore
           ::ZigZagMap[64] = {  0,  1,  8, 16,  9,  2,  3, 10,
                               17, 24, 32, 25, 18, 11,  4,  5,
                               12, 19, 26, 33, 40, 48, 41, 34,
                               27, 20, 13,  6,  7, 14, 21, 28,
                               35, 42, 49, 56, 57, 50, 43, 36,
                               29, 22, 15, 23, 30, 37, 44, 51,
                               58, 59, 52, 45, 38, 31, 39, 46,
                               53, 60, 61, 54, 47, 55, 62, 63 
                             };
short mx::MxPEGScanDecoderSoftwareOnlyCore::iclip[1024];
short *mx::MxPEGScanDecoderSoftwareOnlyCore::iclp;



mx::MxPEGScanDecoderSoftwareOnlyCore
     ::MxPEGScanDecoderSoftwareOnlyCore() {
  
  for(int i = 0; i < 2; i++)
   for(int j = 0; j < 4; j++)
    HuffmanTrees[i][j] = 0;
  
  for(int i = 0; i < 4; i++)
   QuantizationTables[i] = 0;
  
  // init DCT stuff...
  iclp = iclip + 512;
  for(int i = -512; i < 512; i++)
   iclp[i] = (i < -256) ? -256 : ((i > 255) ? 255 : i);
  
  mxm::sendStatusMessage(mxm::StatusMessage,
                         "using software-only MxPEG decoding",
                         this);
}


mx::MxPEGScanDecoderSoftwareOnlyCore
     ::~MxPEGScanDecoderSoftwareOnlyCore() {
  
  for(int i = 0; i < 2; i++)
   for(int j = 0; j < 4; j++)
    delete HuffmanTrees[i][j];
  
  for(int i = 0; i < 4; i++)
   delete[] QuantizationTables[i];
}


void mx::MxPEGScanDecoderSoftwareOnlyCore
      ::setQuantizationTable(int target_table,
                             mxm::u8 *data_bytes) {
  
  mxm::clamp_int(&target_table, 0, 3);
  
  delete[] QuantizationTables[target_table];
  
  mxm::s16 *s16_ptr;
  QuantizationTables[target_table] = s16_ptr = new mxm::s16[64];
  for(int i = 0; i < 64; i++)
   s16_ptr[ZigZagMap[i]] = *data_bytes++;
}


void mx::MxPEGScanDecoderSoftwareOnlyCore
      ::setHuffmanTable(const mxmHuffmanTableId &target_table,
                        mxm::u8 *data_bytes)                {
  
  mxcpcHuffmanTree **tree = &HuffmanTrees[target_table.tableClass()]
                                         [target_table.tableTarget()];
  
  delete *tree;
  
  *tree = new mxcpcHuffmanTree();
  
  if((*tree)->configureFromTable(data_bytes)) {
    
    delete *tree;
    *tree = 0;
  }
   
#ifdef __MX_DEBUG__
  if(*tree)
   mxm::sendStatusMessage(
          mxm::DebugMessage,
          mxmString("updated Huffman tree (")
                      + (!target_table.tableClass() ? mxmString("dc")
                                                    : mxmString("ac"))+ ","
                      + target_table.tableTarget()
                      + "), leaves="
                      + (*tree)->countLeaves(),
          this);
#endif
}


bool mx::MxPEGScanDecoderSoftwareOnlyCore
      ::mapTables(mxmUndecodedMxPEGFrameDescriptor *frame_descriptor) {
  
  if(frame_descriptor->ComponentNum != 3) return(false);
  
  mxcpcHuffmanTree *huffman_dc_for_mcu[MaxMCUsPerTile],
                   *huffman_ac_for_mcu[MaxMCUsPerTile];
  
  switch(frame_descriptor->Sampling) {
    
    case mxmUndecodedMxPEGFrameDescriptor::Y21U11V11:        // some Axis models
      
      MCUsPerTile = 4;
      
      // Y1...
      huffman_dc_for_mcu[0]     = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanDC];
      huffman_ac_for_mcu[0]     = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanAC];
      QuantTableForMCU[0]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[0]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[0] = &LastHuffmanDCs[0];
      
      // Y2...
      huffman_dc_for_mcu[1]     = huffman_dc_for_mcu[0];
      huffman_ac_for_mcu[1]     = huffman_ac_for_mcu[0];
      QuantTableForMCU[1]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[1] = LastHuffmanDCPtrForMCU[0];
      
      // U...
      huffman_dc_for_mcu[2]     = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanDC];
      huffman_ac_for_mcu[2]     = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanAC];
      QuantTableForMCU[2]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[1]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[2] = &LastHuffmanDCs[1];
      
      // V...
      huffman_dc_for_mcu[3]     = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanDC];
      huffman_ac_for_mcu[3]     = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanAC];
      QuantTableForMCU[3]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[2]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[3] = &LastHuffmanDCs[2];
      
      break;
    
    
    case mxmUndecodedMxPEGFrameDescriptor::Y22U11V11:                      // MX
    default:
      
      MCUsPerTile = 6;
      
      // Y1...
      huffman_dc_for_mcu[0]     = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanDC];
      huffman_ac_for_mcu[0]     = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[0]
                                                   .HuffmanAC];
      QuantTableForMCU[0]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[0]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[0] = &LastHuffmanDCs[0];
      
      // Y2...
      huffman_dc_for_mcu[1]     = huffman_dc_for_mcu[0];
      huffman_ac_for_mcu[1]     = huffman_ac_for_mcu[0];
      QuantTableForMCU[1]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[1] = LastHuffmanDCPtrForMCU[0];
      // Y3...
      huffman_dc_for_mcu[2]     = huffman_dc_for_mcu[0];
      huffman_ac_for_mcu[2]     = huffman_ac_for_mcu[0];
      QuantTableForMCU[2]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[2] = LastHuffmanDCPtrForMCU[0];
      // Y4...
      huffman_dc_for_mcu[3]     = huffman_dc_for_mcu[0];
      huffman_ac_for_mcu[3]     = huffman_ac_for_mcu[0];
      QuantTableForMCU[3]       = QuantTableForMCU[0];
      LastHuffmanDCPtrForMCU[3] = LastHuffmanDCPtrForMCU[0];
      
      // U...
      huffman_dc_for_mcu[4]     = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanDC];
      huffman_ac_for_mcu[4]     = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[1]
                                                   .HuffmanAC];
      QuantTableForMCU[4]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[1]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[4] = &LastHuffmanDCs[1];
      
      // V...
      huffman_dc_for_mcu[5]     = HuffmanTrees[0]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanDC];
      huffman_ac_for_mcu[5]     = HuffmanTrees[1]
                                              [frame_descriptor
                                                ->ComponentInfos[2]
                                                   .HuffmanAC];
      QuantTableForMCU[5]       = QuantizationTables[frame_descriptor
                                                      ->ComponentInfos[2]
                                                         .QuantTable];
      LastHuffmanDCPtrForMCU[5] = &LastHuffmanDCs[2];
      
      break;
  }
  
  for(int i = 0; i < MCUsPerTile; i++) {
    
    if(   !huffman_dc_for_mcu[i]
       || !huffman_ac_for_mcu[i]
       || !QuantTableForMCU[i]) return(false);
    
    HuffmanDCRootForMCU[i] = huffman_dc_for_mcu[i]->getRoot();
    HuffmanACRootForMCU[i] = huffman_ac_for_mcu[i]->getRoot();
    *LastHuffmanDCPtrForMCU[i] = 0;
  }
  
  return(true);
}



// ---------- discrete cosine transform (inverse and forward) ------------------

void mx::MxPEGScanDecoderSoftwareOnlyCore::idctrow(short *blk) {

  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  /* shortcut */

  if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
      (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3]))) {

    blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;
    return;
  }

  x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */

  /* first stage */
  x8 = W7*(x4+x5);
  x4 = x8 + (W1-W7)*x4;
  x5 = x8 - (W1+W7)*x5;
  x8 = W3*(x6+x7);
  x6 = x8 - (W3-W5)*x6;
  x7 = x8 - (W3+W5)*x7;

  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2);
  x2 = x1 - (W2+W6)*x2;
  x3 = x1 + (W2-W6)*x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;

  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;

  /* fourth stage */
  blk[0] = (x7+x1)>>8;  
  blk[1] = (x3+x2)>>8;
  blk[2] = (x0+x4)>>8;
  blk[3] = (x8+x6)>>8;
  blk[4] = (x8-x6)>>8;
  blk[5] = (x0-x4)>>8;
  blk[6] = (x3-x2)>>8;
  blk[7] = (x7-x1)>>8;
}


void mx::MxPEGScanDecoderSoftwareOnlyCore::idctcol(short *blk) {

  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
      (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3]))) {
      
    blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]=blk[8*6]=blk[8*7]
     = iclp[(blk[8*0]+32)>>6];

    return;
  }

  x0 = (blk[8*0]<<8) + 8192;

  /* first stage */
  x8 = W7*(x4+x5) + 4;
  x4 = (x8+(W1-W7)*x4)>>3;
  x5 = (x8-(W1+W7)*x5)>>3;
  x8 = W3*(x6+x7) + 4;
  x6 = (x8-(W3-W5)*x6)>>3;
  x7 = (x8-(W3+W5)*x7)>>3;

  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2) + 4;
  x2 = (x1-(W2+W6)*x2)>>3;
  x3 = (x1+(W2-W6)*x3)>>3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;

  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;

  /* fourth stage */
  blk[8*0] = iclp[(x7+x1)>>14];
  blk[8*1] = iclp[(x3+x2)>>14];
  blk[8*2] = iclp[(x0+x4)>>14];
  blk[8*3] = iclp[(x8+x6)>>14];
  blk[8*4] = iclp[(x8-x6)>>14];
  blk[8*5] = iclp[(x0-x4)>>14];
  blk[8*6] = iclp[(x3-x2)>>14];
  blk[8*7] = iclp[(x7-x1)>>14];
}


/*
 *  Credits (as could be reproduced):
 *  - <tt>Chen-Wang</tt> (algorithm)
 */
void mx::MxPEGScanDecoderSoftwareOnlyCore::idct(short *block) {

  int i;

  for(i = 0; i < 8; i++) idctrow(block+8*i);
  for(i = 0; i < 8; i++) idctcol(block+i);
}


/*
 *  Credits (as could be reproduced):
 *  - <tt>Robert.Danielsen@nta.no</tt>
 */
void mx::MxPEGScanDecoderSoftwareOnlyCore::dct(short *block) {
  
  int j1, i, j, k;
  float b[8];
  float b1[8];
  float d[8][8];
  float f0 = .7071068f;
  float f1 = .4903926f;
  float f2 = .4619398f;
  float f3 = .4157348f;
  float f4 = .3535534f;
  float f5 = .2777851f;
  float f6 = .1913417f;
  float f7 = .0975452f;
  
  short da_block[64];
  for(int i = 0; i < 64; i++) da_block[i] = block[i];
  
  for (i = 0, k = 0; i < 8; i++, k += 8)
  {
    for (j = 0; j < 8; j++)
    {
      b[j] = (float) da_block[k + j];
    }
    /* Horizontal transform */
    for (j = 0; j < 4; j++)
    {
      j1 = 7 - j;
      b1[j] = b[j] + b[j1];
      b1[j1] = b[j] - b[j1];
    }
    b[0] = b1[0] + b1[3];
    b[1] = b1[1] + b1[2];
    b[2] = b1[1] - b1[2];
    b[3] = b1[0] - b1[3];
    b[4] = b1[4];
    b[5] = (b1[6] - b1[5]) * f0;
    b[6] = (b1[6] + b1[5]) * f0;
    b[7] = b1[7];
    d[i][0] = (b[0] + b[1]) * f4;
    d[i][4] = (b[0] - b[1]) * f4;
    d[i][2] = b[2] * f6 + b[3] * f2;
    d[i][6] = b[3] * f6 - b[2] * f2;
    b1[4] = b[4] + b[5];
    b1[7] = b[7] + b[6];
    b1[5] = b[4] - b[5];
    b1[6] = b[7] - b[6];
    d[i][1] = b1[4] * f7 + b1[7] * f1;
    d[i][5] = b1[5] * f3 + b1[6] * f5;
    d[i][7] = b1[7] * f7 - b1[4] * f1;
    d[i][3] = b1[6] * f3 - b1[5] * f5;
  }
  /* Vertical transform */
  for (i = 0; i < 8; i++)
  {
    for (j = 0; j < 4; j++)
    {
      j1 = 7 - j;
      b1[j] = d[j][i] + d[j1][i];
      b1[j1] = d[j][i] - d[j1][i];
    }
    b[0] = b1[0] + b1[3];
    b[1] = b1[1] + b1[2];
    b[2] = b1[1] - b1[2];
    b[3] = b1[0] - b1[3];
    b[4] = b1[4];
    b[5] = (b1[6] - b1[5]) * f0;
    b[6] = (b1[6] + b1[5]) * f0;
    b[7] = b1[7];
    d[0][i] = (b[0] + b[1]) * f4;
    d[4][i] = (b[0] - b[1]) * f4;
    d[2][i] = b[2] * f6 + b[3] * f2;
    d[6][i] = b[3] * f6 - b[2] * f2;
    b1[4] = b[4] + b[5];
    b1[7] = b[7] + b[6];
    b1[5] = b[4] - b[5];
    b1[6] = b[7] - b[6];
    d[1][i] = b1[4] * f7 + b1[7] * f1;
    d[5][i] = b1[5] * f3 + b1[6] * f5;
    d[7][i] = b1[7] * f7 - b1[4] * f1;
    d[3][i] = b1[6] * f3 - b1[5] * f5;
  }
  for (i = 0; i < 8; i++)
  {
    for (j = 0; j < 8; j++)
    {
      *(block + j + i*8) = (short)(d[i][j]);
    }
  }
}
