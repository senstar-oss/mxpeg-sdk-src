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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmNumberSet.h $
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



#ifndef __MXM_NUMBERSET_H__
#define __MXM_NUMBERSET_H__



#define MXM_NUMBERSET_BUCKETSIZE 16



#include <mxm/core/mxmList.h>



//! Leightweight Container holding non-negative <tt>int</tt>-valued numbers.
/*!
 *  \ingroup modularMX_core
 *
 *  Note that since the items managed by this class are not subclassed from
 *  mxmObject, the class is not a true <tt>modularMX</tt> container, and as such
 *  it is not subclassed from mxmContainer.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmNumberSet : public mxmObject {
  
 private:
  class Bucket : public mxmObject {
   public:
    int NumbaCount;
    int Numbas[MXM_NUMBERSET_BUCKETSIZE];
  };
  
 private:
  mxmList NumbaBuckets;
  Bucket *LastBucket;
  int NumbaCount;
  bool IteratorValid;
  mxmList::Iterator BucketIterator;
  Bucket *CurrentBucket;
  int CurrentIndex;
  void *StableABIDataExtension;
  
 public:
  mxmNumberSet();
  
 public:
  //! If the number is not already in the set, it is added.
  void addNumber(int numba);
  //! If the number is in the set, it is removed.
  void deleteNumber(int numba);
  //! Returns the number of numbers stored in the container.
  int size(void);
  //! Well, clears the container.
  void clear(void);
  //! Adds the given offset to all numbers in the set. 
  void addOffset(int offset);
  
  //! Prepares container for number readout.
  void resetIterator(void);
  //! Returns next number or <tt>-1</tt>, if iteration finished or iterator
  //! otherwise invalidated.
  int nextNumber(void);
  
 private:
  // Locates the specified number in the set.
  bool locate(int numba,
              Bucket **bucket_ptr, int *index_ptr,
              mxmList::Iterator *bucket_iterator = 0);
};



#endif   // __MXM_NUMBERSET_H__

