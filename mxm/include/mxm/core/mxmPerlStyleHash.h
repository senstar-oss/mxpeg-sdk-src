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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmPerlStyleHash.h $
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



#ifndef __MXM_PERLSTYLEHASH_H__
#define __MXM_PERLSTYLEHASH_H__



#include <mxm/core/mxmList.h>
#include <mxm/core/mxmTypeDecider.h>
#include <mxm/core/mxmString.h>



class mxmStringList;



//! Perl-style Hash storing key/value string pairs, especially useful for
//! processing configuration files.
/*!
 *  \ingroup modularMX_core
 *
 *  Since it is still an option to replace many of the camera software's
 *  <tt>Perl</tt> scripts by small and efficient <tt>C++</tt> applications, we
 *  tried to mimic with this class the convenient handling <tt>Perl</tt> offers
 *  for this kind of data type.
 *
 *  Note that we are perfectly aware of the true meaning of the term
 *  <tt>hash</tt>. However, within the context of this class we use it in the
 *  way <tt>Perl</tt> does.
 *
 *  Note also that in contrast to <tt>Perl</tt> hashes, for mxmHash es,
 *  the sequential ordering of the stored key/value pairs is significant -
 *  when iterating over a hash's key/value pairs, they are reproduced in the
 *  same order in which they were originally added to the hash.
 *
 *  Check out some stuff you can do to hashes:
<pre>
  mxmPerlStyleHash hash;
  mxmPerlStyleHash::KeyValuePair *key_value_pair;
  
  hash["kai"] = "funz";                           // add key/value pair
  hash["daniel"] = "kabs";
  hash["david"] = "gruys";
  hash["kai"] = "hergenroether";                  // overwrite value
  printf("kai is %s\n", hash["kai"].text());      // access a key's value
  if(hash["steve"].isNull())                      // check whether a key is
    printf("steve does not exist!\n");            //   present
  hash.deleteKey("kai");                          // purges a key
  printf("# hash entries : %d\n", hash.size());   // get number of keys
  
  mxmPerlStyleHash::Iterator iter                 // read out all key/value
                              = hash.iterator();  //   pairs
  while(key_value_pair
         = static_cast<mxmPerlStyleHash::KeyValuePair *>(iter.nextItem())) {
    printf("-->%s = %s\n", key_value_pair->key()->text(),
                           key_value_pair->value()->text());
  }
</pre>
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmPerlStyleHash : public mxmList {
  
 public:
  //! Iterator implementation for mxmPerlStyleHash es.
  class MXM_CORE_API Iterator : public mxmList::Iterator {
   public:
    Iterator(const mxmPerlStyleHash *hash);
    Iterator();
   public:
    //! (Re)implemented, to make sense with hashes.
    mxmObject *nextItem(void);
  };
  //! Represents one key/value string pair in a hash.
  class MXM_CORE_API KeyValuePair : public mxmObject {
   private:
    mxmString *Key;
    mxmString *Value;
   public:
    KeyValuePair(mxmString *key, mxmString *value);
    ~KeyValuePair();
   public:
    mxmString *key(void);
    mxmString *value(void);
  };
  //! Installed with the mxmList, so only key/value pairs can be added.
  class KeyValuePairDecider : public mxmTypeDecider {
   public:
    bool isOfProperType(const mxmObject *object);
  };
  
 private:
  bool CaseInsensitiveKeys;
  void *StableABIDataExtension;
  
 public:
  //! Returns an empty hash.
  static mxmPerlStyleHash emptyHash(void); 
  //! Produces a hash from an appropriate command line argument.
  static mxmPerlStyleHash fromCommandLineArg(const mxmString &arg);
  
 public:
  mxmPerlStyleHash();
  mxmPerlStyleHash(const mxmPerlStyleHash &other_hash);
 private:
  void init_mxmPerlStyleHash(void);
  
 public:
  mxmPerlStyleHash &operator=(const mxmPerlStyleHash &other_hash);
  
 public:
  //! Returns a reference to the value for a given key.
  mxmString &operator[](const mxmString &key);
  //! Also returns the value for a given key, but does not implicitly create the
  //! key if it is not present.
  mxmString value(const mxmString &key) const;
  //! If present, the key/value pair addressed by the given key is purged from
  //! the hash.
  void deleteKey(const mxmString &key);
  //! Specify whether or not you want your keys to be treated case sensitively.
  void setCaseInsensitiveKeysMode(bool enabled);
  //! Merges in keys from the other hash, but only those that are cleared by the
  //! <tt>allowed_key_mask</tt>.
  void mergeKeys(const mxmPerlStyleHash &other_hash,
                 const mxmStringList &allowed_key_mask,
                 bool treat_mask_as_prefixes = false,
                 const mxmString &target_key_prefix = "");
  //! Merges in key/value pairs contained in the specified string.
  void mergeKeysFromString(const mxmString &hash_string,
                           const mxmString &pair_delimiters,
                           const mxmString &key_value_separators,
                           const mxmString &trim_chars);
  //! Generates a list of all keys.
  mxm::smart<mxmStringList> enumerateKeys(void) const;
  //! Removes all occurances of the characters specified from the value for
  //! the given key.
  void purgeValueCharacters(const mxmString &key,
                            const mxmString &characters_to_purge);
  //! Converts the whole hash into a single string.
  mxmString concatenate(const mxmString &pair_separator = ",",
                        const mxmString &key_value_separator = "=") const;
  
  //! (Re)implementation, to make sense with hashes. 
  int size(void) const;
  
  //! Provides the calling context with an external iterator.
  mxmPerlStyleHash::Iterator iterator(void) const;
  //! (Re)implementation, to make sense with hashes.
  void appendItemToTail(mxmObject *item);
};



#endif   //  __MXM_PERLSTYLEHASH_H__

