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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmString.h $
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



#ifndef __MXM_STRING_H__
#define __MXM_STRING_H__



#include <mxm/core/mxmObject.h>
#include <mxm/core/mxm_smart.h>

#include <cstdio>



class mxmStringList;



//! String of characters, featuring methods greatly facilitating string
//! handling.
/*!
 *  \ingroup modularMX_core
 *
 *  We introduced this class as a replacement for the standard C++ string
 *  processing facilities we abandoned with <tt>libstdc++</tt>. You can do all
 *  sorts of nifty stuff to strings, such as:
<pre>
     mxmString test_txt, another_txt,
               key, value;
     
     test_txt.setText("fudel");                   // set text
     test_txt = "funz";                           // assignment operator
     another_txt = test_txt;                      // deep copy
     test_txt.append(another_txt);                // append method
     test_txt.append("4567");                     //   -- dito --
     another_txt = another_txt + test_txt;        // concatenation operator
     another_txt = test_txt + "one-two-three";    //   -- dito --
     another_txt = "one-two-three" + test_txt;    //   -- dito --
     another_txt += test_txt;                     // appending operator
     another_txt += "<<<<";                       //   -- dito --
     std::printf("txt=%s\n", test_txt.text());    // actually use text
     std::printf("len=%d\n", test_txt.length());  // get string length
     test_txt.split(key, value, "|");             // split string in two
     key.trim(" \r\n"); value.trim(" \r\n");      // trim whitespaces and stuff
     std::printf("key=%s, value=%s\n",            // process key/value pair
                 key.text(), value.text());
     std::printf("Pos of char '1' is %d\n",       // locate a character
                 test_txt.characterPosition('1'));
</pre>
 *  Note that this class is intended for <i>strings</i>, not for larger amounts
 *  of text - the class' method performance will degrade with growing number of
 *  characters stored.
 *
 *  <b>Empty Strings and <tt>null</tt>-Strings</b><br>
 *  Internally, there is a distinction between an empty string and a 
 *  <tt>null</tt>-string. However, to the user both variants behave exactly the
 *  same way, so typically you won't notice unless you are looking for it by
 *  calling isNull().
 *  Note in this context, that <tt>null</tt>-strings are perfectly legal and
 *  will deconstruct properly when <tt>delete</tt>d. The whole 
 *  <tt>null</tt>-string stuff is interesting, apart from historical
 *  considerations, because it presents you with a special state that can be
 *  exploited and given meaning in certain scenarios, such as with
 *  mxmPerlStyleHash es.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmString : public mxmObject {
  
 private:
  static const char *Base64EncodeTable;
   
 private:
  char *TextBuffer;
  int BufferedChar;
  bool CharBuffered;
  void *StableABIDataExtension;
  
 public:
  //! Returns a <tt>null</tt>-string.
  static mxmString nullString(void);
  
 public:
  //! Constructs empty <tt>""</tt> string.
  mxmString();
  //! The text is copied.
  mxmString(const char *txt);
  //! Copy contructor, text is copied.
  mxmString(const mxmString &txt);
  //! Number is converted using <tt>std::sprintf(buffer, "%d", numba);</tt>.
  mxmString(int numba);
  ~mxmString();
  
 public:
  mxmString &operator=(const char *txt);
  mxmString &operator=(const mxmString &txt);
  mxmString &operator+=(const mxmString &txt);
  mxmString &operator+=(const char *txt);
  bool operator==(const mxmString &txt) const;
  bool operator!=(const mxmString &txt) const;
  //! Is-prefix-of operator.
  bool operator<=(const mxmString &txt) const;
  //! Returns an mxmStringList with the string as first entry and the
  //! <tt>other_string</tt> as second entry.
  mxmStringList operator,(const mxmString &other_string);
  
 public:
  //! The text is copied.
  void setText(const char *txt);
  //! Only valid until the next operation on the string.
  const char *text(void) const;
  //! Explicitly makes the string a <tt>null</tt> string.
  void setNull(void);
  //! Tells whether or not the string is a <tt>null</tt> string.
  bool isNull(void) const;
  //! Returns the number of characters currently in the string.
  int length(void) const;
  //! Either removes trailing characters or appends fill characters to make the
  //! string have the specified length.
  void setLength(int len, char fill_character = ' ');
  //! Returns the position of the first occurance of the specified character,
  //! counting from <tt>0</tt>, or <tt>-1</tt> in case the character is not in
  //! the string.
  int characterPosition(char c) const;
  //! Appends the specified string to the end of the current string.
  void append(const mxmString &txt);
  //! Non-destructively splits the string in two parts.
  bool split(mxmString &left, mxmString &right,
             const mxmString &separators,
             bool search_from_behind = false) const;
  //! Non-destructively splits the string in two parts, using string-valued
  //! separators.
  bool splitUsingSeparatorStrings(mxmString &left, mxmString &right,
                                  const mxmStringList &separator_strings);
  //! Destructively removes delimiter characters from either side of the string.
  void trim(const mxmString &delimiters);
  //! Removes from the string all occurances of the characters specified.
  void purgeCharacters(const mxmString &characters_to_purge,
                       bool invert_logic = false,
                       const char *replacement_character = 0);
  //! Searches for all occurrences of <tt>search</tt> and replaces them
  //! by <tt>replacement</tt>.
  void replace(mxmString const &search, mxmString const & replacement);
  //! Escapes delicate characters when string is to appear on HTML web page.
  mxmString HTMLEscaped(bool const strict = false) const;
  //! Same as HTMLEscaped() but escapes the string on which it is called upon.
  void escapeHTML(bool const strict = false);
  //! Returns version of string where the <tt>characters_to_escape</tt> are
  //! replaced by the respective <tt>%xy</tt> substitutions used in URL
  //! encoding.
  mxmString urlPercentSymbolEscaped(const mxmString &characters_to_escape)
      const;
  //! Same as urlPercentSymbolEscaped() but works on the string on which it
  //! is called upon.
  void escapeURLPercentSymbol(mxmString const & characters_to_escape);
  //! Unescapes <tt>\%xy</tt> symbols as used in URL encoding.
  bool unescapeURLPercentSymbols();
  //! Returns version of string where <tt>\%xy</tt> symbols as used in URLs
  //! are unescaped.
  mxmString urlPercentSymbolsUnescaped(void) const;
  //! Unescapes URL-like character substitutions.
  bool unescapeURL();
  //! Returns URL unescaped version of the string.
  mxmString urlUnescaped(void) const;
  //! Encodes the string as <tt>Base 64</tt>.
  mxmString base64(void) const;
  //! Returns the first <tt>character_num</tt> characters as substring.
  mxmString left(int character_num) const;
  //! Starting from position <tt>character_start</tt>, returns <tt>character_len</tt>
  //! characters as <tt>mxmString</tt>.
  mxmString mid(int character_start, int character_len) const;
  //! Returns the longest prefix exclusively consisting of the specified valid
  //! characters.
  mxmString left(const mxmString &valid_characters) const;
  //! Returns the position of the first occurrence of <tt>search</tt> 
  //! at or after position <tt>start_from</tt> in the string
  //! upon which this method it invoked.
  int findSubString(mxmString const & search, int start_from = 0) const;
  //! Tells whether or not the specified string is a substring of the string
  //! upon which this method is invoked.
  bool subString(const mxmString &another_string) const;
  //! Splits the string into tokens, honoring the separator characters
  //! specified.
  mxm::smart<mxmStringList> tokenize(const mxmString &separators,
                                     const mxmString &characters_to_trim = "")
                             const;
  //! Splits the string into tokens, using string-valued separators.
  mxm::smart<mxmStringList> tokenizeUsingSeparatorStrings(
                              const mxmStringList &separator_strings,
                              const mxmString &characters_to_trim = "") const;
  
  //! Does the obvious thing, returns <tt>false</tt> in case the string is empty
  //! or null, and <tt>true</tt> otherwise.
  bool toInt(int &target_int);
  
 public:
  //! Case-insensitively compares two strings.
  static bool areEqualIgnoringCase(const mxmString &txt1,
                                   const mxmString &txt2);
};


MXM_CORE_API mxmString operator+(const mxmString &txt1, const mxmString &txt2);
MXM_CORE_API mxmString operator+(const char *txt1, const mxmString &txt2);



#endif   // __MXM_STRING_H__

