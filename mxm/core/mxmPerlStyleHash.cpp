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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmPerlStyleHash.cpp $
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



#define MXM_PERLSTYLEHASH_CLASSNAME "mxmPerlStyleHash"



#include <mxm/core/mxmPerlStyleHash.h>
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>



mxmPerlStyleHash::mxmPerlStyleHash() {
  
  init_mxmPerlStyleHash();
}


mxmPerlStyleHash::mxmPerlStyleHash(const mxmPerlStyleHash &other_hash) {
  
  init_mxmPerlStyleHash();
  
  operator=(other_hash);
}


void mxmPerlStyleHash::init_mxmPerlStyleHash(void) {
  
  mxmTypeDecider *decider;
  
  decider = new KeyValuePairDecider();
  MXM_CHECK_NULL(decider);
  setItemTypeDecider(decider);
  
  CaseInsensitiveKeys = false;
}



mxmPerlStyleHash &mxmPerlStyleHash
                   ::operator=(const mxmPerlStyleHash &other_hash) {
  
  mxmPerlStyleHash::Iterator iter;
  KeyValuePair *pair,
               *new_pair;
  
  mxmList::clear();
  
  iter = other_hash.iterator();
  while(pair = static_cast<KeyValuePair *>(iter.nextItem())) {
    
    mxmString *new_key   = new mxmString(*pair->key());
    MXM_CHECK_NULL(new_key);
    mxmString *new_value = new mxmString(*pair->value());
    MXM_CHECK_NULL(new_value);
    new_pair = new KeyValuePair(new_key,
                                new_value);
    MXM_CHECK_NULL(new_pair);
    mxmList::appendItemToTail(new_pair);
  }
  
  CaseInsensitiveKeys = other_hash.CaseInsensitiveKeys;
  
  return(*this);
}


/*!
 *  In fact, this is the main hash class functionality. Use this operator to
 *  both set and retrieve key values. Setting key values happens by assigning to
 *  the string returned by the operator. If the string returned is a 
 *  <tt>null</tt>-string, as can be tested via mxmString::isNull(), the key is
 *  not in the hash.
 */
mxmString &mxmPerlStyleHash::operator[](const mxmString &key) {
  
  mxmList::Iterator iter;
  KeyValuePair *key_value_pair;
  mxmString *new_key, *new_value;
  
  iter = mxmList::iterator();
    // we need to use the mxmList iterator version, since we want to
    //   visit all entries - our own iterator implementation skips keys with
    //   null-string values
  while(key_value_pair = static_cast<KeyValuePair *>(iter.nextItem())) {
      // static cast feasible, since we've installed appropriate type decider.
    if(CaseInsensitiveKeys) {
      if(mxmString::areEqualIgnoringCase(key, *key_value_pair->key())) break;
    }
    else {
      if(key == *key_value_pair->key()) break;
    }
    
    if(key_value_pair->value()->isNull()) delete iter.removeCurrentItem();
      // purge potential <null>-string valued keys
  }
  
  if(key_value_pair) {   // key already present
    return(*key_value_pair->value());
  }
  else {                 // need to add new key
    
    new_key          = 0;
    new_value        = 0;
    key_value_pair   = 0;
    
    new_key        = new mxmString();
    MXM_CHECK_NULL(new_key);
    new_value      = new mxmString();
    MXM_CHECK_NULL(new_value);
    key_value_pair = new KeyValuePair(new_key, new_value);
    MXM_CHECK_NULL(key_value_pair);
    
    *new_key       = key;
    new_value->setNull();
    mxmList::appendItemToTail(key_value_pair);   // don't use our own version!
    
    return(*new_value);
  }
}


/*!
 *  For non-existent keys the method returns a <tt>null</tt>-string.
 */
mxmString mxmPerlStyleHash::value(const mxmString &key) const {
  
  mxmPerlStyleHash::Iterator iter;
  KeyValuePair *key_value_pair;
  
  iter = iterator();   // skips _null_-valued entries
  while(key_value_pair = static_cast<KeyValuePair *>(iter.nextItem())) {
    
    if(CaseInsensitiveKeys) {
      if(mxmString::areEqualIgnoringCase(key, *key_value_pair->key())) break;
    }
    else {
      if(key == *key_value_pair->key()) break;
    }
  }
  
  if(key_value_pair)
    return(*key_value_pair->value());
  else
   return(mxmString::nullString());
}


void mxmPerlStyleHash::deleteKey(const mxmString &key) {
  
  mxmList::Iterator iter;
  KeyValuePair *key_value_pair;
  
  iter = mxmList::iterator();
    // we need to use the mxmList iterator version, since we want to
    //   visit all entries - our own iterator implementation skips keys with
    //   null-string values
  while(key_value_pair = static_cast<KeyValuePair *>(iter.nextItem())) {
      // static cast feasible, since we've installed appropriate type decider.
    if(   (   CaseInsensitiveKeys
           && mxmString::areEqualIgnoringCase(key, *key_value_pair->key()))
       || (   !CaseInsensitiveKeys
           && (key == *key_value_pair->key()))) {
      delete iter.removeCurrentItem();
      return;
    }
  }
}


/*!
 *  The default is case sensitive keys.
 *
 *   \note This method will clear the hash.
 */
void mxmPerlStyleHash::setCaseInsensitiveKeysMode(bool enabled) {
  
  clear();
  
  CaseInsensitiveKeys = enabled;
}


/*!
 */
mxm::smart<mxmStringList> mxmPerlStyleHash::enumerateKeys(void) const {
  
  Iterator iter;
  KeyValuePair *pair;
  mxmString *new_str;
  
  mxm::smart<mxmStringList> key_list = new mxmStringList();
  MXM_CHECK_NULL(key_list);
  
  iter = iterator();
  while(pair = static_cast<KeyValuePair *>(iter.nextItem())) {
    new_str = new mxmString(*pair->key());
    MXM_CHECK_NULL(new_str);
    key_list->addItem(new_str);
  }
  
  return(key_list);
}


void mxmPerlStyleHash::mergeKeys(const mxmPerlStyleHash &other_hash,
                                 const mxmStringList &allowed_key_mask,
                                 bool treat_mask_as_prefixes,
                                 const mxmString &target_key_prefix)    {

  Iterator iter;
  KeyValuePair *pair; 
  mxmString da_value;
  
  iter = other_hash.iterator();
  while(pair = static_cast<KeyValuePair *>(iter.nextItem())) {
    
    if(treat_mask_as_prefixes) {
      if(!allowed_key_mask.containsPrefixOf(*pair->key())) continue;
    }
    else {
      if(!allowed_key_mask.contains(*pair->key())) continue;
    }
    
    da_value = other_hash.value(*pair->key());
    if(!da_value.isNull())
     (*this)[target_key_prefix + *pair->key()] = da_value;
  }
}


/*!
 */
void mxmPerlStyleHash
      ::mergeKeysFromString(const mxmString &hash_string,
                            const mxmString &pair_delimiters,
                            const mxmString &key_value_separators,
                            const mxmString &trim_chars)           {
  
  mxmString line, left, tmp_txt,
            key, value;
  bool split_result;
  
  tmp_txt = hash_string;
  
  do {
    
    line = tmp_txt;
    
    split_result = line.split(left, tmp_txt, pair_delimiters);
    
    left.split(key, value, key_value_separators);
    if(trim_chars.length()) {
      key.trim(trim_chars);
      value.trim(trim_chars);
    }
    
    if(key.length()) (*this)[key] = value;
    
  } while(split_result);
}


/*!
 */
void mxmPerlStyleHash
      ::purgeValueCharacters(const mxmString &key,
                             const mxmString &characters_to_purge) {
  
  mxmPerlStyleHash::Iterator iter;
  KeyValuePair *key_value_pair;
  
  iter = iterator();
  while(key_value_pair = static_cast<KeyValuePair *>(iter.nextItem())) {
    
    if(   (   CaseInsensitiveKeys
           && mxmString::areEqualIgnoringCase(key, *key_value_pair->key()))
       || (   !CaseInsensitiveKeys
           && (key == *key_value_pair->key()))) {
      
      key_value_pair->value()->purgeCharacters(characters_to_purge);
      return;
    }
  }
}


mxmString mxmPerlStyleHash::concatenate(const mxmString &pair_separator,
                                        const mxmString &key_value_separator)
                             const                                            {
  
  mxmString result;
  bool fresh = true;
  Iterator iter = iterator();
  while(KeyValuePair *pair = static_cast<KeyValuePair *>(iter.nextItem())) {
    
    if(!fresh)
     result += pair_separator;
    else
     fresh = false;
    
    result += *pair->key()
               + key_value_separator
               + *pair->value();
  }
  
  return(result);
}


/*!
 *  This reimplementation is slow.
 */
int mxmPerlStyleHash::size(void) const {
  
  mxmPerlStyleHash::Iterator iter;
  int num;
  
  num = 0;
  iter = iterator();
  while(iter.nextItem()) num++;
    // our own iterator implementation properly skips the null-string valued
    //   keys.
  
  return(num);
}



/*!
 *  A previous occurance of the respective key is wasted. The new key/value pair
 *  is appended to the tail of the list.
 *
 *  Not recommended not be used, however. Obsoleted by the <tt>[]</tt> operator
 *  anyways
 *
 *  \attention In violation of what was said for mxmList::appendItemToTail(),
 *             the previous key occurance, if present, might be lost. Don't use.
 */
void mxmPerlStyleHash::appendItemToTail(mxmObject *item) {
  
  KeyValuePair *key_value_pair;
  
  key_value_pair = dynamic_cast<KeyValuePair *>(item);
  if(!key_value_pair) mxm::terminal("type mismatch while adding.",
                                    this);
  
  deleteKey(*key_value_pair->key());
  (*this)[*key_value_pair->key()] = *key_value_pair->value();
  
  delete key_value_pair;
}


mxmPerlStyleHash::Iterator mxmPerlStyleHash::iterator(void) const {
  
  return Iterator(this);
}


mxmPerlStyleHash::Iterator::Iterator(const mxmPerlStyleHash *hash)
                             : mxmList::Iterator(hash)             {
}


mxmPerlStyleHash::Iterator::Iterator() {
}


/*!
 *  Since this is the iterator used by hash users to read out all key/value
 *  pairs, this reimplemented version skips those pairs that have a
 *  <tt>null</tt>-string value since these have a special, hash-implemtational
 *  meaning ("key not present").
 */
mxmObject *mxmPerlStyleHash::Iterator::nextItem(void) {
  
  KeyValuePair *key_value_pair;
  
  while(key_value_pair
         = static_cast<KeyValuePair *>(mxmList::Iterator::nextItem())) {
  
    if(key_value_pair->value()->isNull())
     continue;
    else
     return(key_value_pair);
  }
  
  return(0);
}


mxmPerlStyleHash::KeyValuePair::KeyValuePair(mxmString *key,
                                             mxmString *value) {
  Key = key;
  Value = value;
}


mxmPerlStyleHash::KeyValuePair::~KeyValuePair() {
  
  delete Key;
  delete Value;
}


mxmString *mxmPerlStyleHash::KeyValuePair::key(void) {
  
  return(Key);
}


mxmString *mxmPerlStyleHash::KeyValuePair::value(void) {
  
  return(Value);
}


bool mxmPerlStyleHash::KeyValuePairDecider
      ::isOfProperType(const mxmObject *object) {
  
  if(dynamic_cast<const KeyValuePair *>(object)) return(true);
  else                                           return(false);
}


mxmPerlStyleHash mxmPerlStyleHash::emptyHash(void) {
   
  mxmPerlStyleHash empty;
  
  return(empty);
}


mxmPerlStyleHash mxmPerlStyleHash::fromCommandLineArg(const mxmString &arg) {
  
  mxmPerlStyleHash args;
  
  args.mergeKeysFromString(arg, ",", "=", "");
  return(args);
}
