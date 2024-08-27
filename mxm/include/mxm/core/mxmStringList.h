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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmStringList.h $
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



#ifndef __MXM_STRINGLIST_H__
#define __MXM_STRINGLIST_H__



#include <mxm/core/mxmList.h>

#include <cstdio>



class mxmString;



//! List of strings, offering some nice convenience functions.
/*!
 *  \ingroup modularMX_core
 *
 *  Only mxmString s can be added to this list - attempts to add items of other
 *  type will result in terminal failure.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmStringList : public mxmList {
  
 public:
  mxmStringList();
  mxmStringList(const mxmStringList &other_string_list);
  mxmStringList &operator=(const mxmStringList &other_string_list);
  mxmStringList(const mxmString &a_string);
 private:
  void init_mxmStringList(void);
  
 public:
  //! Returns a reference to the <tt>i</tt>th string in the list.
  mxmString &operator[](int index);
  //! Returns a copy to the <tt>i</tt>th string in a const list.
  mxmString operator[](int index) const;
  //! Appends to the list the strings stored in the <tt>list_to_add</tt>.
  mxmStringList operator+(const mxmStringList &list_to_add) const;
  //! Removes from the list all strings that are also present in the 
  //! <tt>list_to_substract</tt>.
  mxmStringList operator-(const mxmStringList &list_to_substract) const;
  //! Returns a string list resulting from appending to the tail of the local
  //! string list the string specified as argument.
  mxmStringList operator,(const mxmString &yet_another_string);
  
 public:
  //! For your convenience.
  void add(const mxmString &txt);
  //! Returns the concatenation string of all strings in the list, separated by
  //! the specified <tt>separator_string</tt>.
  mxmString concatenate(const mxmString &separator_string) const;
  //! Tells whether or not the specified string is contained in the list.
  bool contains(const mxmString &string) const;
  //! Tells whether or not the list contains a prefix of the specified string.
  bool containsPrefixOf(const mxmString &string) const;
  //! Tells how many members of the specified string list are contained in the
  //! list the method is invoked for.
  int containsElementsOf(const mxmStringList &list) const;
  //! Tells whether or not the local string list is a subset of the specified
  //! one.
  bool isSubsetOf(const mxmStringList &list) const;
  //! Removes duplicate item strings from the list.
  void removeDuplicates(void);
  //! Tests for string set equality.
  bool equalStringSets(const mxmStringList &other_set);
  //! For testing purposes.
  void dump(std::FILE *outfile) const;
  //! For testing purposes.
  void dump(const mxmString &filename);
  
 public:
  //! Returns an empty string list.
  static mxmStringList emptyList(void);
};



#endif   // __MXM_STRINGLIST_H__

