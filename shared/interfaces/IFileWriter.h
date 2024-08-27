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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IFileWriter.h $
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



#ifndef __MX_IFILEWRITER_H__
#define __MX_IFILEWRITER_H__



#define MX_IFILEWRITER_ID "IFileWriter"



#include <interfaces/ISupportsFileSpecification.h>
#include <interfaces/ISegmentedStreamReceiver.h>



namespace mx {
   
   //! Encodes the concept of a file writer writing a potentially segmented
   //! byte stream into one or multiple files.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  In the multiple file case, each stream segment is written to a new,
    *  individual file. The files are automatically numbered.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class IFileWriter : public virtual ISupportsFileSpecification,
                       public virtual ISegmentedStreamReceiver    {
      
    public:
      //! Tells the writer to produce a single file.
      /*!
       *  This is the default behavior.
       *
       *  If set via \ref setSegmentSeparator(), subsequent stream segments will
       *  be separated by a separator string.
       */
      virtual void writeSingleConcatenatedFile() = 0;
      //! Sets the separator string for concatenated write mode, see
      //! \ref writeSingleConcatenatedFile().
      virtual void setSegmentSeparator(const mxmString &separator) = 0;
      //! Tells the writer to produce individual files for the stream segments.
      /*!
       *  \param num
       *  If set to <tt>1</tt>, all segments get written to the same one file.
       *  If set to <tt>num > 1</tt>, the segments are written to a set of
       *  <tt>num</tt> files, that cyclically get overwritten and are
       *  automatically numbered. Otherwise, the segments get written to an
       *  automatically numbered sequence of files, with no limitations on the
       *  number of files other than the precision of the counter's integer
       *  precision (default).
       */
      virtual void writeIndividualFiles(int num) = 0;
   };
};



#endif   // __MX_IFILEWRITER_H__

