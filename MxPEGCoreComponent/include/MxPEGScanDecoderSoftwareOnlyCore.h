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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/MxPEGScanDecoderSoftwareOnlyCore.h $
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



#ifndef __MX_MXPEGSCANDECODERSOFTWAREONLYCORE_H__
#define __MX_MXPEGSCANDECODERSOFTWAREONLYCORE_H__


#include "mxcpcHuffmanTree.h"

#include <interfaces/IUndecodedMxPEGFrameReceiver.h>

#include <mxm/core/mxmObject.h>
#include <mxm/core/mxmHuffmanTableId.h>
#include <mxm/core/mxm_generic_stuff.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>



namespace mx {
  
  //! Core functionality for software-only <tt>MxPEG</tt> decoding.
  /*!
   *  \ingroup mxcpc_core
   *
   *  \author Kai Hergenroether
   */
  class MxPEGScanDecoderSoftwareOnlyCore : public mxmObject,
                                           public virtual
                                            IUndecodedMxPEGFrameReceiver {
    
   protected:
    static const int MaxMCUsPerTile = 8;
   protected:
    static const int ZigZagMap[64];
   private:
    static const int W1 = 2841;
    static const int W2 = 2676;
    static const int W3 = 2408;
    static const int W5 = 1609;
    static const int W6 = 1108;
    static const int W7 =  565;
    static short iclip[1024];
    static short *iclp;
  
   private:
    mxcpcHuffmanTree *HuffmanTrees[2][4];
    mxm::s16         *QuantizationTables[4];
    
   protected:
    //! The following fields' state is defined to only be valid during scan
    //! sweeps - so they won't show up in the constructor...
    unsigned char *CurrentScanByte,
                  ScanMask;
    int ScanBytesLeft;
    int CoeffInAdditionalBits;
    int MCUsPerTile;
    const mxcpcHuffmanTree::Node *HuffmanDCRootForMCU[MaxMCUsPerTile],
                                 *HuffmanACRootForMCU[MaxMCUsPerTile];
    mxm::s16 *QuantTableForMCU[MaxMCUsPerTile];
    mxm::s16 LastHuffmanDCs[3];
    mxm::s16 *LastHuffmanDCPtrForMCU[MaxMCUsPerTile];
    mxm::s16 CoeffBuffers[MaxMCUsPerTile][64];
  
    
   public:
    MxPEGScanDecoderSoftwareOnlyCore();
    ~MxPEGScanDecoderSoftwareOnlyCore();
    
   public:
    //! (Re)implemented.
    void setQuantizationTable(int target_table,
                              mxm::u8 *data_bytes);
    //! (Re)implemented.
    void setHuffmanTable(const mxmHuffmanTableId &target_table,
                         mxm::u8 *data_bytes);
    
   protected:
    //! Used by derived implementations to compute the mappings for the Huffman
    //! and quantization tables managed by the core.
    bool mapTables(mxmUndecodedMxPEGFrameDescriptor *frame_descriptor);
     
    //! We need this often in inner loops of the decoder routine, so we 
    //! request inline substitution and hope the compiler sufficiently rocks.
    /*!
     *  Only to be called if <tt>ScanBytesLeft > 0</tt>.
     *
     *  Returns whether or not the bit just consumed was set.
     */
    inline bool consumeScanBit(void) {
      
      bool bit_was_set;
      unsigned char last_scan_byte;
      
      bit_was_set = (*CurrentScanByte & ScanMask) ? true : false;
      
      ScanMask /= 2;
      if(!ScanMask) {
        
        last_scan_byte = *CurrentScanByte;
        
        CurrentScanByte++;
        ScanBytesLeft--;
        if(last_scan_byte == 0xff) {
          if(*CurrentScanByte == 0x00) {
            if(!ScanBytesLeft) {
              mxm::sendStatusMessage(mxm::FailureMessage,
                                     "not more scan bytes!",
                                     this);
              //std::exit(666);
            }
            else {
              CurrentScanByte++;
              ScanBytesLeft--;
            }
          }
          else {
            mxm::sendStatusMessage(mxm::FailureMessage,
                                   "unexpected marker encountered!",
                                   this);
            //std::exit(666);
          }
        
          //mxcpc::sendStatusMsg("warning: 0xff in datastream, feeding now, checking later!");
        }
        
        ScanMask = 128;
      }
      
      return(bit_was_set);
    }
    
    //! We need this often in inner loops of the decoding routine, so we 
    //! request inline substitution and hope the compiler sufficiently rocks.
    /*!
     *  Specify as argument the root of the Huffman tree to be used for codeword
     *  recognition.
     *
     *  Internally checks for <tt>ScanBytesLeft > 0</tt>.
     *
     *  Returns the Huffman leaf node corresponding to the codeword just
     *  consumed or <tt>0</tt> in case of failure. Failure conditions can be
     *  - an invalid bitstring was encountered
     *  - all scan bytes were cosumed before a valid codeword was detected 
     */
    inline const mxcpcHuffmanTree::Node
                  *consumeHuffmanCodeword(
                     const mxcpcHuffmanTree::Node *current_huffman_node) {
      
      const mxcpcHuffmanTree::Node *last_huffman_node;
      
      do {
         
        if(!ScanBytesLeft) return(0);
        
        last_huffman_node = current_huffman_node;
        
        if(consumeScanBit())
         current_huffman_node = current_huffman_node->Child1;
        else
         current_huffman_node = current_huffman_node->Child0;
        
        if(current_huffman_node == last_huffman_node) {
          
          mxm::sendStatusMessage(mxm::FailureMessage,
                                 " !!! invalid codeword !!!",
                                 this);
          //std::exit(666);
          return(0);
        }
      } while(!current_huffman_node->IsLeaf);
      
      return(current_huffman_node);
    }
    
    //! We need this often in inner loops of the decoding routine, so we 
    //! request inline substitution and hope the compiler sufficiently rocks.
    /*!
     *  Specify as argument the number <tt>n >= 1</tt> of additional bits to 
     *  read.
     *
     *  The coefficient represented by the read bits will be stored to the
     *  class member variable <tt>CoeffInAdditionalBits</tt>.
     *
     *  The method internally checks for <tt>ScanBytesLeft > 0</tt>.
     *
     *  It returns <tt>false</tt> on success, and <tt>true</tt> if all scan
     *  bytes were cosumed before a valid codeword was detected.
     */
    inline bool consumeAdditionalBits(int n) {
      
      bool positive;
      int base;
      static int bases[] = { -666, -666, 2, 4, 8, 16, 32, 64, 128, 256, 512 };
      
      // read first bit...
      if(!ScanBytesLeft) return(true);
      positive = consumeScanBit();
      if(n == 1) {
        
        if(positive) CoeffInAdditionalBits =  1;
        else         CoeffInAdditionalBits = -1;
        return(false);
      }
      
      // read remaining bits...
      base = bases[n];
      if(!positive) {
        base *= 2;
        base = -base;
        base++;
      }
      n--;
      CoeffInAdditionalBits = 0;
      while(n) {
        
        if(!ScanBytesLeft) return(true);
        
        CoeffInAdditionalBits *= 2;
        
        if(consumeScanBit())
         CoeffInAdditionalBits += 1;
        
        n--;
      }
      CoeffInAdditionalBits += base;  
      
      return(false);
    }
    
    //! Inverse discrete cosine transform (8x8).
    void idct(short *blk);
    //! Forward discrete cosine transform (8x8).
    void dct(short *blk);
    
   private:
    void idctrow(short *blk);
    void idctcol(short *blk);
    
  };
};



#endif   // __MX_MXPEGSCANDECODERSOFTWAREONLYCORE_H__
