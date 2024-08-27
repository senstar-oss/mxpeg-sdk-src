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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmNumberSet.cpp $
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



#include <mxm/core/mxmNumberSet.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>



mxmNumberSet::mxmNumberSet() {
  
  Bucket *bucket = new Bucket();
  MXM_CHECK_NULL(bucket);
  
  bucket->NumbaCount = 0;
  NumbaBuckets.appendItemToTail(bucket);
  
  LastBucket = bucket;
  NumbaCount = 0;
  
  IteratorValid = false;
  CurrentBucket = 0;
  CurrentIndex  = 0;
}



/*!
 *  If a negative number is passed as argument, the method does nothing.
 *
 *  The inbuilt iterator is invalidated.
 */
void mxmNumberSet::addNumber(int numba) {
  
  Bucket *bucket;
  int i;
  
  IteratorValid = false;
  
  if(numba < 0)                  return;
  if(locate(numba, &bucket, &i)) return;
  
  // need to add da numba...
  if(LastBucket->NumbaCount < MXM_NUMBERSET_BUCKETSIZE) {
    LastBucket->Numbas[LastBucket->NumbaCount] = numba;
    LastBucket->NumbaCount++;
  }
  else {
    bucket = new Bucket();
    MXM_CHECK_NULL(bucket);
    
    bucket->Numbas[0]  = numba;
    bucket->NumbaCount = 1;
    NumbaBuckets.appendItemToTail(bucket);
    
    LastBucket = bucket;
  }
  
  NumbaCount++;
}


/*!
 *  Invalidates the inbuilt iterator.
 */
void mxmNumberSet::deleteNumber(int numba) {
  
  Bucket *bucket;
  int index, i;
  mxmList::Iterator bucket_iterator;
  
  IteratorValid = false;
  
  if(numba < 0)                                         return;
  if(!locate(numba, &bucket, &index, &bucket_iterator)) return;
  
  for(i = index; i < bucket->NumbaCount - 1; i++)
   bucket->Numbas[i] = bucket->Numbas[i + 1];
  bucket->NumbaCount--;
  
  // we've just used locate(), the returned iterator still points at the
  // right bucket
  if(   !bucket->NumbaCount
     && (NumbaBuckets.size() > 1)) delete bucket_iterator.removeCurrentItem();
                                     // leave at least one (maybe empty) bucket
  NumbaCount--;
}


int mxmNumberSet::size(void) {
  
  return(NumbaCount);
}


/*!
 *  Invalidates the inbuilt iterator.
 */
void mxmNumberSet::clear(void) {
  
  
  while(NumbaBuckets.size() > 1) delete NumbaBuckets.removeItemFromHead();
  BucketIterator = NumbaBuckets.iterator();
  
  LastBucket = (Bucket *)static_cast<Bucket *>(BucketIterator.nextItem());
  LastBucket->NumbaCount = 0;
  NumbaCount = 0;
  resetIterator();
  IteratorValid = false;
}


/*!
 *  Invalidates the crappy inbuilt iterator.
 */
void mxmNumberSet::addOffset(int offset) {

  int *numba_buffer,
      *numba_ptr;
  int num,
      i;
  
  num = size();
  if(!num) return;
  
  numba_ptr = numba_buffer = new int[num];
  MXM_CHECK_NULL(numba_buffer);
  
  resetIterator();
  for(i = 0; i < num; i++) *numba_ptr++ = nextNumber(); 
  
  numba_ptr = numba_buffer;
  clear();
  for(i = 0; i < num; i++) addNumber(*numba_ptr++ + offset);

  resetIterator();
  
  delete numba_buffer;
}


void mxmNumberSet::resetIterator(void) {
  
  BucketIterator = NumbaBuckets.iterator();
  CurrentBucket = static_cast<Bucket *>(BucketIterator.nextItem());
  CurrentIndex  = -1;
  IteratorValid = true;
}


int mxmNumberSet::nextNumber(void) {
  
  if(!IteratorValid) return(-1);
  
  for(;;) {
    CurrentIndex++;
    if(CurrentIndex >= CurrentBucket->NumbaCount) {
      CurrentBucket = static_cast<Bucket *>(BucketIterator.nextItem());
      if(!CurrentBucket) {
        resetIterator();
        IteratorValid = false;
        return(-1);
      }
      else {
        CurrentIndex = -1;
      }
    }
    else break;
  }
  
  return(CurrentBucket->Numbas[CurrentIndex]);
}


/*!
 *  If the number is in the set, the method returns <tt>true</tt> and writes
 *  its location info to <tt>bucket_ptr</tt> and <tt>index_ptr</tt>. If the
 *  number is not in the set, the method returns <tt>false</tt>..
 */
bool mxmNumberSet::locate(int numba,
                          Bucket **bucket_ptr, int *index_ptr,
                          mxmList::Iterator *bucket_iterator) {
  
  mxmList::Iterator iter;
  Bucket *bucket;
  int i;
  
  iter = NumbaBuckets.iterator();
  while(bucket = static_cast<Bucket *>(iter.nextItem())) {
    
    for(i = 0; i < bucket->NumbaCount; i++) {
      if(bucket->Numbas[i] == numba) {
        *bucket_ptr = bucket;
        *index_ptr  = i;
        if(bucket_iterator) *bucket_iterator = iter;
        return(true);
      }
    }
  }
  return(false);
}

