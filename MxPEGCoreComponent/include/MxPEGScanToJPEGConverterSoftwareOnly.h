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
//  $LastChangedDate: 2007-06-28 11:26:00 +0200 (Thu, 28 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/MxPEGScanToJPEGConverterSoftwareOnly.h $
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



#ifndef __MX_MXPEGSCANTOJPEGCONVERTERSOFTWAREONLY_H__
#define __MX_MXPEGSCANTOJPEGCONVERTERSOFTWAREONLY_H__



#define MX_MXPEGSCANTOJPEGCONVERTERSOFTWAREONLY_ID \
         "MxPEGScanToJPEGConverterSoftwareOnly"



#include "MxPEGScanDecoderSoftwareOnlyCore.h"
#include "mxcpcHuffmanTree.h"

#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IMxPEGScanToJPEGConverter.h>

#include <Framework/Framework.h>

#include <mxm/core/mxm_jpeg.h>



class mxmUndecoderMxPEGFrameDescriptor;



namespace mx {
  
  class ISegmentedStreamReceiver;
  
  //! Converts an <tt>MxPEG</tt> stream to a sequence of full <tt>JPEG</tt>
  //! images using no external hardware-optimized acceleration routines.
  /*!
   *  \ingroup mxsdk_mxpeg_core
   *
   *  The current implementation produces <tt>JPEG</tt> in <tt>Y22U11V11</tt>
   *  sampling mode, regardless of the sampling mode used in the input stream.
   *
   *  After multiple revisions, the class should be threadable now.
   *
   *  \author <tt>[dgr]</tt>, <tt>[khe]</tt>
  */
  class MxPEGScanToJPEGConverterSoftwareOnly
         : public MxPEGScanDecoderSoftwareOnlyCore,
           public virtual IComponent,
           public virtual IMxPEGScanToJPEGConverter {
    
    //! These static methods provide interface and capability ids for
    //! registration with the framework (for both the component and its
    //! invoker).
   private:
    static mxm::smart<mxmStringList> _enumerateInterfaces() {
      return(new mxmStringList(
                   (mxmString(MX_MXPEGSCANTOJPEGCONVERTERSOFTWAREONLY_ID),
                    MX_ICOMPONENT_ID,
                    MX_IMXPEGSCANTOJPEGCONVERTER_ID,
                    MX_IUNDECODEDMXPEGFRAMERECEIVER_ID,
                    MX_ISENDSSEGMENTEDSTREAM_ID,
                    MX_IERRORSTATE_ID)));
    }
    static mxm::smart<mxmStringList> _enumerateCapabilities(
                                        const mxmString &interface_id) {
      if(interface_id == mx::Framework::IdHintKey)
       return(new mxmStringList(MX_MXPEGSCANTOJPEGCONVERTERSOFTWAREONLY_ID));
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
        return(new MxPEGScanToJPEGConverterSoftwareOnly());
      }
    };
   public:
    //! Generates an invoker singleton for the \ref MxPEGParser component.
    static IComponentInvoker *newInvoker() {
      return(new Invoker());
    }
    
   private:
    static const int BitstreamBufferSize = 0x100000;
    static const unsigned char ZigZag_StoreMap[64];
    
    //! One DC huffman table - rules for one component (Y or UV) 
    typedef struct {
      //! bits_dc[x] = how many length-codes are encoded with a symbol of length
      //!              x. note that 0 is missing!
      mxm::u8 bits_dc[16];
      //! the actual codes for coefficient-lengths from 0 to 11 bits. The length
      //! is the number of bits that are not zero
      mxm::u8 val_dc[12];
    } HuffCompDC;
    
    //! One AC huffman table - rules for one component (Y or UV) 
    typedef struct {
      //! max length of code is 16 bits - in fact, 119 of 162 possible 
      //! combinations are 16 bits !
      //! - including up to 11 significant data bits, 
      //! - a single non-zero dc-coefficient could therefore 
      //! - generate up to 27 bits of huffman data
      mxm::u8 bits_ac[16];
      mxm::u8 val_ac[162];
    } HuffCompAC;
    
    static const HuffCompDC HuffTbl_Y_UV_DC[2];
    static const HuffCompAC HuffTbl_Y_UV_AC[2];
    
    //!  Single entry in table
    typedef struct {
      //! mask, made of rll-encoded symbol(2..16bits) shifted left by (num. of
      //! mantissa-bits)
      unsigned int code_msk:  26;
      //! always 0
      unsigned int fill_0bit:  1;
      //! 2..26, sum of len of rll-encoded symbol + len of mantissa-bits 
      unsigned int bit_len:    5;
    } HuffCodeTblEntry;
    
    //! Used to convert a HuffCodeTblEntry -> mxm::u32 !
    typedef union {
      HuffCodeTblEntry table_entry;
      mxm::u32         u32_word;
    } u32t;
    
    /*! One AC huffman table for either Y or UV */
    typedef struct {
      //! We index this with symbols 00, 0F, 10..AF and hope gcc won't notice!
      HuffCodeTblEntry ac_tbl[1];
      //! Add 1 to length to be able to use same ptr as for ac-table
      HuffCodeTblEntry dc_tbl[12];
      //! Ununsed entry  
      int free_13;
      //! Second unused entry [14] is still free
      int free_14;
      //! The valid entries for the ac-table continue here at [15]
      HuffCodeTblEntry dc_tbl_0FtoAF[161];
    } FoldedHuffTbl_DC_AC;
    
    //! Array of two tables, size = 2*0xB0 words = 1408 bytes : */
    FoldedHuffTbl_DC_AC Huff_Y_UV[2];
    
    //! This structure holds the buffer state for huffman encoding ...
    typedef struct {
      mxm::u8* buf_start;         // Addr of buf in memory, word-aligned
      mxm::u8* buf_end1;          // End of buffer + 1, word-aligned */
      mxm::u8* buf_current_w_pos; // Current buffer pointer, word-aligned if
                                  //  huf_accu is valid,
                                  //  may be odd after jd_close_huf_buf_stateb
                                  //  was called */
      mxm::u32 huf_accu;          // Left justified bits not yet emitted
      unsigned int huf_bits_used; // Number of valid bits in huf_accu not yet
                                  //  emitted
    } HuffmanBufferStateStructure;
    
   private:
    ISegmentedStreamReceiver *JPEGReceiver;
    HuffmanBufferStateStructure HuffmanBufferState;
    mxm::u8 *HuffmanTableWriteBuffer;
    mxm::u8 *HuffmanEncoderBuffer;
    mxm::s16 *CoeffBuffer;        // Ptr. to decoded frame!
    int TileNumX, TileNumY;       // Nb. of tiles to handle per frame!
    mxm::JPEG::MarkerLayoutAPP0 APP0_Block;
    mxm::JPEG::MarkerLayoutSOF0 SOF0_Block;
    mxm::JPEG::MarkerLayoutSOS  SOS_Block;
    int FramesProcessed;
    bool ErrorEncountered;
    
   public:
    //! The converter assumes ownership over the JPEG receiver.
    MxPEGScanToJPEGConverterSoftwareOnly();
    ~MxPEGScanToJPEGConverterSoftwareOnly();
    
   public:
    //! (Re)implemented.
    mxm::smart<mxmStringList> enumerateInterfaces();
    //! (Re)implemented.
    mxm::smart<mxmStringList> enumerateCapabilities(
                                const mxmString &interface);
    
    //! (Re)implemented.
    void receiveUndecodedMxPEGFrame(mxmUndecodedMxPEGFrameDescriptor
                                     *frame_descriptor);
    //! (Re)implemented.
    void receiveMxPEGAudioFrame(const mxmAudioBlockInfo &audio_info,
                                const mxm::u8 *sample_data);
    
    //! (Re)implemented.
    void setSegmentedStreamReceiver(ISegmentedStreamReceiver *receiver);
    
    //! (Re)implemented.
    bool errorState();
    //! (Re)implemented.
    void setErrorState();
  	 
   private:
    //! Calculate the huffman tables ..
    void calcFoldedHuffTables(void);
    //! Create a huffman lookup table.
    /*!
     * \param bits_tbl_off1 Entries 0..15 count no. of symbols 
     *        with 1..16 bits, respectively.
     * \param val_tbl Table of symbols that need to be encoded, 
     *        size = sum of entries in bits_tbl_off1.
     * \param swap_symbol Set if the swapped symbol is used to access the 
     *        generated folded table (for ac-coefs).
     * \param folded_tbl_p Holds the generated huffman tables!
     */
    void generateHuffTables1(const mxm::u8 * bits_tbl_off1,
                             const mxm::u8 * val_tbl,
                             int swap_symbol,
                             HuffCodeTblEntry * folded_tbl_p);
    //! Init the Buffer.
    void initHuffmanEncoderBufferState();
    void resetHuffmanEncoderBufferState();
    
    //! This Method takes a 8x8 sub-MCU and preforms a swift Huffman Encoding.
    void encodeSubMCUTiles(mxm::s16* coef_matrix,
                           mxm::s16* last_dc_coeff,
                           FoldedHuffTbl_DC_AC* huff_code_tbl_entry,
                           HuffmanBufferStateStructure* hbuf_state_ptr);
    
    //! Copies a word into the global huffman encoding buffer!
    void copyAccuToGlobalHuffBuffer(mxm::u32 huff_accu_32,
                                    HuffmanBufferStateStructure
                                    *huff_buf_state_ptr);
    
    //! Closes down huffman encoding stream.
    int shutDownHuffAccu(void);
    
    //! Number of bits from ....
    int returnBitsInCoef(mxm::s16 c);
  };
};


#endif   // __MX_MXPEGSCANTOJPEGCONVERTERSOFTWAREONLY_H__
