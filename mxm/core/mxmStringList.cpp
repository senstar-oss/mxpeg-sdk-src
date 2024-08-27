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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmStringList.cpp $
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



#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringDecider.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>

#include <cstdio>



mxmStringList::mxmStringList() {
  
  init_mxmStringList();
}


mxmStringList::mxmStringList(const mxmStringList &other_string_list) {
  
  init_mxmStringList();
  
  operator=(other_string_list);
}


mxmStringList::mxmStringList(const mxmString &a_string) {
  
  init_mxmStringList();
  
  mxmString *new_txt = new mxmString(a_string);
  MXM_CHECK_NULL(new_txt);
  addItem(new_txt);
}


void mxmStringList::init_mxmStringList(void) {
  
  mxmStringDecider *string_decider;
  
  string_decider = new mxmStringDecider();
  MXM_CHECK_NULL(string_decider);
  setItemTypeDecider(string_decider);
}


/*!
 *  For missing string items empty strings will automatically be appended.
 *
 *  A negative <tt>index</tt> will be mapped to <tt>0</tt>.
 */
mxmString &mxmStringList::operator[](int index) {
  
  mxmStringList::Iterator iter;
  mxmString *txt;
  int strings_to_append,
      i;
  
  if(index < 0) index = 0;
  
  if(index >= size()) {
    
    strings_to_append = index - size() + 1;
    while(strings_to_append) {
      mxmString *new_txt = new mxmString("");
      MXM_CHECK_NULL(new_txt);
      appendItemToTail(new_txt);
      strings_to_append--;
    }
  }
  
  iter = iterator();
  for(i = 0; i <= index; i++)
   txt = static_cast<mxmString *>(iter.nextItem());
  
  return(*txt);
}

/*!
 *  If <tt>index</tt> is greater than the size of the list
 *  an empty string will be returned.
 *  A negative <tt>index</tt> will be mapped to <tt>0</tt>.
 */
mxmString mxmStringList::operator[](int index) const {
  
  mxmStringList::Iterator iter;
  mxmString* txt;
  int i;
  
  if(index < 0) index = 0;
  
  if(index >= size()) {
		return mxmString("");    
  }
  
  iter = iterator();
  for(i = 0; i <= index; i++)
   txt = static_cast<mxmString *>(iter.nextItem());
  
  return(*txt);
}

/*!
 */
mxmStringList &mxmStringList::operator=(const mxmStringList &other_string_list){
  
  mxmStringList::Iterator iter;
  mxmString *txt, *new_txt;
  
  if(this == &other_string_list) return(*this);
  
  if(size()) clear();
  
  iter = other_string_list.iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    new_txt = new mxmString(*txt);
    MXM_CHECK_NULL(new_txt);
    
    appendItemToTail(new_txt);
  }
  
  return(*this);
}


/*!
 */
mxmStringList mxmStringList::operator+(const mxmStringList &list_to_add) const {
  
  mxmStringList sum;
  mxmStringList::Iterator iter;
  mxmString *txt, *new_txt;
  
  
  sum = *this;
  
  iter = list_to_add.iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    new_txt = new mxmString(*txt);
    MXM_CHECK_NULL(new_txt);
    
    sum.appendItemToTail(new_txt);
  }
  
  return(sum);
}


/*!
 */
mxmStringList 
 mxmStringList::operator-(const mxmStringList &list_to_substract) const {
  
  mxmStringList difference;
  mxmStringList::Iterator iter;
  mxmString *txt, *new_txt;
  
  iter = iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    if(!list_to_substract.contains(*txt)) {
      
      new_txt = new mxmString(*txt);
      MXM_CHECK_NULL(new_txt);
      difference.appendItemToTail(new_txt);
    }
  }
  
  return(difference);
}


mxmStringList mxmStringList::operator,(const mxmString &yet_another_string) {
  
  mxmStringList string_list;
  
  string_list = *this;
  
  mxmString *new_txt = new mxmString(yet_another_string);
  MXM_CHECK_NULL(new_txt);
  string_list.appendItemToTail(new_txt);
  
  return(string_list);
}


void mxmStringList::add(const mxmString &txt) {
  
  mxmString *da_string = new mxmString(txt);
  MXM_CHECK_NULL(da_string);
  appendItemToTail(da_string);
}


/*!
 */
mxmString mxmStringList::concatenate(const mxmString &separator_string) const {
  
  mxmStringList::Iterator iter;
  bool da_first;
  mxmString txt;
  mxmString *more_txt;
  
  iter = iterator();
  da_first = true;
  while(more_txt = static_cast<mxmString *>(iter.nextItem())) {
    
    if(   !da_first
       && separator_string.length()) txt += separator_string;
    
    txt += *more_txt;
    
    da_first = false;
  }
  
  return(txt);
}


bool mxmStringList::contains(const mxmString &string) const {
  
  mxmString *txt;
  mxmStringList::Iterator iter;
  
  iter = iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    if(*txt == string) return(true);
  }
  
  return(false);
}


bool mxmStringList::containsPrefixOf(const mxmString &string) const {
  
  mxmString *txt;
  mxmStringList::Iterator iter;
  
  iter = iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    if(*txt <= string) return(true);
  }
  
  return(false);
}


int mxmStringList::containsElementsOf(const mxmStringList &list) const {
  
  mxmStringList::Iterator iter;
  int matches;
  mxmString *txt;
  
  matches = 0;
  iter = list.iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    if(contains(*txt)) matches++;
  }
  
  return(matches);
}


/*!
 * 
 */
bool mxmStringList::isSubsetOf(const mxmStringList &list) const {
  
  if(!size()) return(true);   // empty set is always subset
  
  return(list.containsElementsOf(*this) == size());
}


/*!
 *  Sequential ordering of the genuine strings in the list is preserved.
 */
void mxmStringList::removeDuplicates(void) {
  
  mxmStringList genuine_entries;
  mxmList::Iterator iter;
  mxmString *txt, *new_txt;
  
  iter = iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    
    if(!genuine_entries.contains(*txt)) {
      
      new_txt = new mxmString(*txt);
      MXM_CHECK_NULL(new_txt);
      
      genuine_entries.appendItemToTail(new_txt);
    }
  }
  
  *this = genuine_entries;
}


/*!
 */
bool mxmStringList::equalStringSets(const mxmStringList &other_set) {
  
  mxmStringList genuine_entries,
                genuine_other_entries;
  
  genuine_entries = *this;
  genuine_entries.removeDuplicates();
  
  genuine_other_entries = other_set;
  genuine_other_entries.removeDuplicates();
  
  if(!genuine_entries.isSubsetOf(genuine_other_entries))     return(false);
  if(genuine_entries.size() != genuine_other_entries.size()) return(false);
  
  return(true);
}


/*!
 *  No checking done for I/O failures!
 */
void mxmStringList::dump(std::FILE *outfile) const {
  
  mxmStringList::Iterator iter;
  mxmString *txt;
  
  std::fprintf(outfile, "mxmStringList@%x:\n", (unsigned int)this);
  
  iter = iterator();
  while(txt = static_cast<mxmString *>(iter.nextItem())) {
    std::fprintf(outfile, "  %s\n", txt->text());
  }
}


/*!
 *  No checking done for I/O failures!
 */
void mxmStringList::dump(const mxmString &filename) {
   
  std::FILE *outfile;
  
  outfile = std::fopen(filename.text(), "w");
  if(outfile) {
    
    dump(outfile);
    std::fclose(outfile);
  }
}

mxmStringList mxmStringList::emptyList(void) {
  
  mxmStringList string_list;
  
  return(string_list);
}

