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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmString.cpp $
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



#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxmBuffer.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>

#include <cstring>
#include <cstdlib>



const char *mxmString::Base64EncodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                           "abcdefghijklmnopqrstuvwxyz"
                                           "0123456789"
                                           "+/";



/*!
 */
mxmString::mxmString() {
  
  TextBuffer   = 0;
  BufferedChar = EOF;
  CharBuffered = false;
  
  setText("");
}


/*!
 */
mxmString::mxmString(const char *txt) {
  
  TextBuffer   = 0;
  BufferedChar = EOF;
  CharBuffered = false;
  
  setText(txt);
}


/*!
 */
mxmString::mxmString(const mxmString &txt) {
  
  TextBuffer   = 0;
  BufferedChar = EOF;
  CharBuffered = false;
  
  if(txt.isNull())
   setNull();
  else
   setText(txt.text());
}


/*!
 */
mxmString::mxmString(int numba) {
  
  char txt[40];
  
  TextBuffer   = 0;
  BufferedChar = EOF;
  CharBuffered = false;
  
  std::sprintf(txt, "%d", numba);
  setText(txt);
}


mxmString::~mxmString() {
  
  delete[] TextBuffer;
}



/*!
 */
mxmString &mxmString::operator=(const char *txt) {
  
  setText(txt);
  
  return(*this);
}


/*!
 */
mxmString &mxmString::operator=(const mxmString &txt) {
  
  if(this != &txt) {
    if(!txt.isNull()) setText(txt.text());
    else              setNull();
  }
  
  return(*this);
}


/*!
 */
mxmString &mxmString::operator+=(const mxmString &txt) {

  append(txt);
  return(*this);
}


mxmString &mxmString::operator+=(const char *txt) {

  append(txt);
  return(*this);
}


/*!
 *  <tt>null</tt>-strings behave like empty strings.
 */
bool mxmString::operator==(const mxmString &txt) const {
  
  // handle null-strings...
  if(!TextBuffer || !txt.TextBuffer) {
    if(!TextBuffer && txt.TextBuffer) 
     return(std::strlen(txt.TextBuffer) == 0);
    else if(TextBuffer && !txt.TextBuffer)  
     return(std::strlen(TextBuffer) == 0);
    else
     return(true);
  }
  
  if(!std::strcmp(TextBuffer, txt.TextBuffer))
   return(true);
  else
   return(false);
}


bool mxmString::operator!=(const mxmString &txt) const {
  
  return(!operator==(txt));
}


/*!
 *  The empty or <tt>null</tt>-string is always prefix.
 */
bool mxmString::operator<=(const mxmString &txt) const {
  
  int len;
  
  len = length();
  if(!len)               return(true);
  if(len > txt.length()) return(false);
  
  if(!std::strncmp(TextBuffer, txt.TextBuffer, len))
    return(true);
  else
   return(false);
}


mxmStringList mxmString::operator,(const mxmString &other_string) {
  
  mxmStringList string_list;
  
  mxmString *new_txt = new mxmString(*this);
  MXM_CHECK_NULL(new_txt);
  string_list.appendItemToTail(new_txt);
  new_txt = new mxmString(other_string);
  MXM_CHECK_NULL(new_txt);
  string_list.appendItemToTail(new_txt);
  
  return(string_list);
}


/*!
 *  <tt>null</tt>-strings behave like empty strings.
 */
bool mxmString::areEqualIgnoringCase(const mxmString &txt1,
                                     const mxmString &txt2) {
  
  // handle null-strings...
  if(!txt1.TextBuffer || !txt2.TextBuffer) {
    if(!txt1.TextBuffer && txt2.TextBuffer) 
     return(std::strlen(txt2.TextBuffer) == 0);
    else if(txt1.TextBuffer && !txt2.TextBuffer)  
     return(std::strlen(txt1.TextBuffer) == 0);
    else
     return(true);
  }
  
// 
#ifndef MX_PLATFORM_WINDOWS
  if(!strcasecmp(txt1.TextBuffer, txt2.TextBuffer))
#else
  if(!_stricmp(txt1.TextBuffer, txt2.TextBuffer))
#endif
   return(true);
  else
   return(false);
}


/*!
 *  The text gets copied.
 */
void mxmString::setText(const char *txt) {
  
  delete[] TextBuffer;
  TextBuffer = 0;
  
  TextBuffer = new char[std::strlen(txt) + 1];
  MXM_CHECK_NULL(TextBuffer);
  
  std::strcpy(TextBuffer, txt);
}


const char *mxmString::text(void) const {
  
  if(TextBuffer)
   return(TextBuffer);
  else
   return("");
}


void mxmString::setNull(void) {
  
  delete[] TextBuffer;
  TextBuffer = 0;
}


bool mxmString::isNull(void) const {

  if(!TextBuffer)
   return(true);
  else
   return(false);
}


int mxmString::length(void) const {

  if(TextBuffer)
   return(std::strlen(TextBuffer));
  else
   return(0);
}


void mxmString::setLength(int len, char fill_character) {
  
  // RAII...
  
  if(len < 1) {
    setText("");
    return;
  }
  if(length() == len) return;
  
  mxm::smart<mxmBuffer> buffer = new mxmBuffer(len + 1);
  MXM_CHECK_NULL(buffer);
  
  char *target_buffer = (char *)buffer->firstByte();
  std::strncpy(target_buffer, TextBuffer, len + 1);
  for(int i = length(); i < len; i++) target_buffer[i] = fill_character;
  target_buffer[len] = '\0';
  
  setText(target_buffer);
}


int mxmString::characterPosition(char c) const {

  int i, len;
  char *ptr;
  
  if(!TextBuffer) return(-1);
  
  ptr = TextBuffer;
  len = std::strlen(TextBuffer);
  for(i = 0; i < len; i++) {
    if(*ptr == c) break;
    ptr++;
  }
  if(i < len)
   return(i);
  else
   return(-1);
}


/*!
 */
void mxmString::append(const mxmString &txt) {
  
  int len, pre_len;
  char *text_buffer;
  
  if(!TextBuffer) setText(txt.text());
  else {
    
    pre_len = std::strlen(TextBuffer);
    len = pre_len + std::strlen(txt.text());
    
    text_buffer = new char[len + 1];
    MXM_CHECK_NULL(text_buffer);
    
    std::strcpy(text_buffer, TextBuffer);
    std::strcpy(text_buffer + pre_len, txt.text());
    
    delete[] TextBuffer;
    TextBuffer = text_buffer;
  }
}


/*!
 *  The split will be performed at the position of the first occurrence of one
 *  of the separators. If <tt>search_from_behind</tt> is set to <tt>true</tt>,
 *  then the split will be performed at the last separator occurrence. 
 *
 *  \returns <tt>true</tt> in case a separator was found and the split was
 *  performed, and <tt>false</tt> otherwise. If the string could not be split,
 *  it is copied to <tt>left</tt> and <tt>right</tt> is set to <tt>""</tt>.
 */
bool mxmString::split(mxmString &left, mxmString &right,
                      const mxmString &separators,
                      bool search_from_behind) const     {

  char *ptr;
  int len;
  char sep_c;
  
  if(!(len = length())) {
    left  = "";
    right = "";
    return(false);
  }
  
  if(!search_from_behind) {
    ptr = TextBuffer;
    while(len) {
      if(separators.characterPosition(*ptr) != -1) break;
      ptr++;
      len--;
    }
  }
  else {
    ptr = TextBuffer + len;
    while(len) {
      if(separators.characterPosition(*(ptr - 1)) != -1) break;
      ptr--;
      len--;
    }
    if(len) ptr--;
  }
  
  if(len) {
    
    sep_c = *ptr;
    *ptr = '\0';
    
    left.setText(TextBuffer);
    right.setText(ptr + 1);
    
    *ptr = sep_c;
    return(true);
  }
  else {
    left  = *this;
    right = "";
    return(false);
  }
}


/*!
 *  The split will be performed at the position of the first occurrence of one
 *  of the separator strings.
 *
 *  \returns <tt>true</tt> in case a separator was found and the split was
 *  performed, and <tt>false</tt> otherwise. If the string could not be split,
 *  it is copied to <tt>left</tt> and <tt>right</tt> is set to <tt>""</tt>.
 */
bool  mxmString::splitUsingSeparatorStrings(mxmString &left, mxmString &right,
                                            const mxmStringList
                                                   &separator_strings)         {
  
  left  = *this;
  right = "";                                                  // assume failure
  
  if(!length()) return(false);
  
  // locate first separator string...
  int first_sep_occurrence = -1;
  mxmString separator_used;
  mxmStringList::Iterator iter = separator_strings.iterator();
  while(mxmString *separator = static_cast<mxmString *>(iter.nextItem())) {
    
    int pos = findSubString(*separator);         // -1 if separator string empty
    if(pos == -1) continue;
    
    if(   (first_sep_occurrence == -1)
       || (pos < first_sep_occurrence)) {
       
       first_sep_occurrence = pos;
       separator_used       = *separator;
    }
  }
  if(first_sep_occurrence == -1) return(false);     // no separator string found
  
  left  = this->left(first_sep_occurrence);
  right = TextBuffer + left.length() + separator_used.length();
  return(true);
}


void mxmString::trim(const mxmString &delimiters) {

  char *ptr, *ptr2;
  int len;
  char *text_buffer;
  
  if(!TextBuffer) return;
  
  text_buffer = TextBuffer;  
  len = std::strlen(text_buffer);
  if(!len) return;
  
  ptr = text_buffer;
  while(ptr != '\0') {
    if(delimiters.characterPosition(*ptr) == -1) break;
    ptr++;
  }
  ptr2 = text_buffer + len - 1;
  while(ptr2 > ptr) {
    if(delimiters.characterPosition(*ptr2) == -1) break;
    *ptr2 = '\0';
    ptr2--;
  }
  
  // disconnect text buffer...
  TextBuffer = 0;
  // set trimmed text...
  setText(ptr);
  
  // dispose of old text buffer
  delete[] text_buffer;
}


/*!
 */
void mxmString::purgeCharacters(const mxmString &characters_to_purge,
                                bool invert_logic,
                                const char *replacement_character)          {
  
  char *buffer,
       *src_ptr, *dest_ptr;
  int len;
  
  len = length();
  if(!len) return;
  
  buffer = new char[len + 1];
  MXM_CHECK_NULL(buffer);
  src_ptr  = TextBuffer;
  dest_ptr = buffer;
  
  while(*src_ptr) {
    if(  invert_logic
       ^ (characters_to_purge.characterPosition(*src_ptr) == -1)) {
      *dest_ptr = *src_ptr;
      *dest_ptr++;
    }
    else {
      if(replacement_character) {
        *dest_ptr = *replacement_character;
        *dest_ptr++;
      }
    }
    src_ptr++;
  }
  
  *dest_ptr = '\0';
  
  setText(buffer);
  
  delete[] buffer;
}
/*!
 * Relies on findSubString() and as such does not replace anything if
 * <tt>search</tt> string is empty.
 */
void mxmString::replace(mxmString const &search, mxmString const & replacement) {
  int len = search.length();
  mxmString new_string;

  int found, from = 0;

  while((found = findSubString(search, from)) >= 0) {
    new_string += mid(from, found - from) + replacement;
    from = found + len;
  }
  new_string += mid(from, length() - from);
  *this = new_string;
}

/*!
 * Note: There is <b>no</b> corresponding unescape function as unescaping is done
 * through the browser's rendering engine.
 *
 * This function basically converts quotation marks and angle bracket open (&lt;)
 * to their equivalent HTML entities. This prevents intermingling of 
 * HTML attribute quotes and content containing quotes. Additionally,
 * the angle bracket avoids interpretation of content as HTML element.
 *
 * In addition to the above, the <tt>strict</tt> version escapes ampersand (&amp;)
 * so content which may contain HTML enties will not be interpreted through
 * browser engine.
 *
 * <b>Usage notes</b>
 *
 * Use \ref htmlEscaped() to escape user input that is printed out
 * on a HTML page. This prevents XSS (execution of arbitrary Javascript code).
 *
 * The <tt>strict</tt> option <b>must</b> be used to prevent browser engine
 * from interpreting HTML entities and as such changing the content of the
 * string, e.g. for
 * - data stored in <i>hidden form fields</i> that must be unchanged.
 * - <i>password fields</i> (e.g. to prevent &lt; from being converted)
 * - URLs that contain &, e.g. in HREF attribute of A element or
     SRC attribute of IMG element.
     (see http://ppewww.ph.gla.ac.uk/~flavell/www/formgetbyurl.html)
 *
 * Rules:
 * - <i>non-strict</i> escape user supplied data (e.g. form field input)
 *   as soon as is to be proccessed (e.g. printed on web page, used in attributes of
     HTML elements, fed to i8n functions).
 * - <i>non-strict</i> escape output of i8n functions if string is to be used
 *   as value of attributes of HTML elements (e.g. value of attribute
 *   <i>value</i> of INPUT form element) to prevent
     confusion of quotation marks but allow for HTML entities to be used.
 * - <i>strict</i> escape texual content that has to be transferred literally
 *   (see examples above).

 * Remember: Non-strict HTML escape can be applied repeatedly without mangling
   the string!
 */
mxmString mxmString::HTMLEscaped(bool const strict) const {
  mxmString result = *this;
  result.escapeHTML(strict);
  return result;
}

void mxmString::escapeHTML(bool const strict) {
  mxmString result = *this;
  if (strict) {
    this->replace("&","&amp;");
  }
  this->replace("\"", "&quot;");
  this->replace("<" , "&lt;"  );
  this->replace("'" , "&#39;" );
}

mxmString mxmString
           ::urlPercentSymbolEscaped(const mxmString &characters_to_escape) 
              const                                                         {
  
  mxmString result;
  int len;
 
  len = length();
  
  if(len) {
    
    mxmString to_escape;
    char *ptr;
    int replacements_to_make;
    int i;
    
    to_escape = characters_to_escape + "%";
    
    // count...
    ptr = TextBuffer;
    replacements_to_make = 0;
    for(i = 0; i < len; i++)
     if(to_escape.characterPosition(*ptr++) != -1)
      replacements_to_make++;
    
    if(replacements_to_make) {
      
      int target_len;
      char *target_buffer,
           *target_ptr;
      
      target_len = len + replacements_to_make*2;
      
      target_buffer = new char[target_len + 1];
      MXM_CHECK_NULL(target_buffer);
      
      ptr        = TextBuffer;
      target_ptr = target_buffer;
      while(*ptr) {
        
        if(to_escape.characterPosition(*ptr) != -1) {
          
          unsigned int c_as_uint;
          char txt[10];
          
          c_as_uint = (unsigned char)*ptr; 
          std::sprintf(txt, "%02x", c_as_uint);

          *target_ptr++ = '%';
          *target_ptr++ = txt[0];
          *target_ptr++ = txt[1];
        }
        else {
          
          *target_ptr++ = *ptr;
        }
        
        ptr++;
      }
      *target_ptr = '\0';
      
      result.setText(target_buffer);
      
      delete[] target_buffer;
    }
    else {
      
      return(*this);
    }
  }
  
  return(result);
}

void mxmString::escapeURLPercentSymbol(mxmString const & characters_to_escape) {
  mxmString result = this->urlPercentSymbolEscaped(characters_to_escape);
  *this = result;
}


/*!
 * \return true on success.
 *
 */
bool mxmString::unescapeURLPercentSymbols() {
  
  int len, i;
  char *buffer,
       *src_ptr, *dest_ptr;
  bool problem;
  
  len = length();
  if(!len) return(true);
  
  buffer   = new char[len + 1];
  MXM_CHECK_NULL(buffer);
  src_ptr  = TextBuffer;
  dest_ptr = buffer;
  problem  = false;
  
  for(i = 0; i < len; i++) {
    
    if(*src_ptr == '%') {
      if(len - i - 1 < 2) {
        
        *dest_ptr++ = '_';
        problem     = true;
        break;
      }
      else {
        
        char hex[3];
        unsigned int code;
        
        src_ptr++;
        hex[0] = *src_ptr++;
        hex[1] = *src_ptr++;
        hex[3] = '\0';
        i += 2;
        
        std::sscanf(hex, "%x", &code);
        
        if(code > 255) {
          *dest_ptr++ = '_';
          problem     = true;
        }
        else {
          *dest_ptr++ = (char)code;
        }
      }
    }
    else {
      *dest_ptr++ = *src_ptr++;
    }
  }
  
  *dest_ptr = '\0';
  
  setText(buffer);
  delete[] buffer;
  
  return(!problem);
}


mxmString mxmString::urlPercentSymbolsUnescaped(void) const {
  
  mxmString result = *this;
  result.unescapeURLPercentSymbols();
  return(result);
}


/*!
 *  Currently, URL unescaping does the following:
 *  - it ressolves <tt>+</tt> characters to spaces
 *  - it unescapes the URL-style percent symbols of the form <tt>\%xy</tt>
 */
bool mxmString::unescapeURL() {
  
  char space;
  
  space = ' ';
  purgeCharacters("+", false, &space);
  
  return(unescapeURLPercentSymbols());
}


mxmString mxmString::urlUnescaped(void) const {
  
  mxmString result = *this;
  result.unescapeURL();
  return(result);
}


mxmString mxmString::base64(void) const {

  unsigned char *data;
  int byte_num;
  unsigned char in[3], out[4];
  int i, len;
  unsigned char *target_buffer,
                *encoded;
  mxmString base64_string;
  
  byte_num             = length();
  target_buffer        = new unsigned char[(byte_num/3 + 1)*4 + 1 + 10];
                                                               // +10 = be safe
  MXM_CHECK_NULL(target_buffer);
  data                 = (unsigned char *)TextBuffer;
  encoded              = target_buffer;
  
  while(byte_num) {
    
    // get next 3-byte block of binary input data...
    len = 0;
    for(i = 0; i < 3; i++ ) {
      if(byte_num) {
        in[i] = (unsigned char) *data++;
        byte_num--;
        len++;
      }
      else {
        in[i] = 0;
      }
    }
    
    // generate corresponding 4-byte base64 block...
    out[0] = Base64EncodeTable[in[0] >> 2];
    out[1] = Base64EncodeTable[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
    out[2] 
     = (unsigned char)(
         len > 1 
         ? Base64EncodeTable[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] 
         : '='
       );
    out[3] = (unsigned char) (len > 2 ? Base64EncodeTable[in[2] & 0x3f] : '=');
    
    // ... and write it to target buffer...
    *target_buffer++ = out[0];
    *target_buffer++ = out[1];
    *target_buffer++ = out[2];
    *target_buffer++ = out[3];
  }
  
  // properly terminate base64-string in target buffer
  *target_buffer = 0;
  
  base64_string.setText((char *)encoded);
  
  delete encoded;
  
  return(base64_string);
}


mxmString mxmString::left(int character_num) const {
  
  char *txt;
  int len, i;
  mxmString left;
  
  if (character_num <= 0) return left; // we could call terminal() instead,
                                       // but then the test suite would abort.

  txt = new char[character_num + 1];
  MXM_CHECK_NULL(txt);
  if(len = length()) {
    if(character_num < len) len = character_num;
    for(i = 0; i < len; i++) txt[i] = TextBuffer[i];
    txt[i] = '\0';
  }
  else {
    txt[0] = '\0';
  }
  
  left.setText(txt);
  
  delete txt;
  
  return(left);
}

/*!
 * First character is at position 0.
 *
 * If <tt>character_start</tt> is negative, starts that far from the
 * end of the string.
 */
mxmString mxmString::mid(int character_start, int character_len) const {
  char *txt;
  int endpos, i;

  if (character_len <= 0 || length() <= 0)
    return mxmString("");

  txt = new char[character_len + 1];
  MXM_CHECK_NULL(txt);
  endpos = length();
  if (character_start < 0) {
    // character_start counts from the end of the string, starting with 1.
    if (- character_start > endpos) {
      return mxmString("");
    } else {
      character_start = endpos + character_start;
    }
  }
  if(character_start + character_len < endpos) {
    endpos = character_start + character_len;
  }
  for(i = character_start; i < endpos; i++) {
    txt[i - character_start] = TextBuffer[i];
  }
  txt[i - character_start] = '\0';

  mxmString mid;
  mid.setText(txt);
  
  delete txt;
  
  return(mid);
}

mxmString mxmString::left(const mxmString &valid_characters) const {
  
  int i, len;
  
  if(!length()) return("");
  
  len = std::strlen(TextBuffer);
  for(i = 0; i < len; i++)
   if(valid_characters.characterPosition(TextBuffer[i]) == -1) break;
  
  return(left(i));
}
/*!
 * if <tt>start_from</tt> is omitted, starts searching from the beginning of
 * the string.
 * First character is at position 0.
 * \return
 * - -1 if the <tt>search</tt> string is not found or empty.
 * - otherwise the position of the first occurrence at or after 
 *   position <tt>start_from</tt>
 */
int mxmString::findSubString (mxmString const & search, int start_from) const {
  if (!search.length()) return -1;

  if (start_from < 0) start_from = 0; // if we call terminal() instead
                                      // how to handle this in test suite?
  if (start_from + 1 > length()) return -1;
  
  const char * found = strstr(text() + start_from, search.text());
  if (found) {
    return (found - text());
  } else {
    return -1;
  }
}

/*!
 *  subString() conforms to the GLIBC string search <tt>strstr()</tt>.
 *  \return
 *  <tt>true</tt> if <tt>another_string</tt> is empty.
 */
bool mxmString::subString(const mxmString &another_string) const {
  
  // offering two shortcuts...
  if(!another_string.length())           return(true);
  if(length() < another_string.length()) return(false);
  
  if(std::strstr(text(), another_string.text()))
   return(true);
  else
   return(false);
}


mxmString operator+(const mxmString &txt1, const mxmString &txt2) {
  
  mxmString result;
  
  result = txt1;
  result.append(txt2);
  return(result);
}


mxmString operator+(const char *txt1, const mxmString &txt2) {
  
  mxmString result;
  
  result = txt1;
  result.append(txt2);
  return(result);
}


/*!
 *  The method returns a list of mxmString s representing the tokens
 *  generated from the string. The whole data structure is dynamically generated
 *  on the heap.
 *
 *  The function will never generate empty token strings.
 */
mxm::smart<mxmStringList>
      mxmString::tokenize(const mxmString &separators,
                          const mxmString &characters_to_trim) const {
  
  mxm::smart<mxmStringList> token_list;
  mxmString txt, left, right;
  bool did_split;
  mxmString *token;
  
  token_list = new mxmStringList();
  MXM_CHECK_NULL(token_list);
  
  if(length()) {
  
    txt = *this;
    do {
      
      txt.trim(separators);
      did_split = txt.split(left, right, separators);
      
      if(characters_to_trim.length()) left.trim(characters_to_trim);
      
      if(left.length()) {
        
        token = new mxmString(left);
        MXM_CHECK_NULL(token);
        token_list->appendItemToTail(token);
      }
      
      txt = right;
      
    } while(did_split);
  }
  
  return(token_list);
}


/*!
 *  Note that analogously to the character-valued separator version of split(),
 *  this method will not produce any empty tokens for concatenations of multiple
 *  separator strings.
 */
mxm::smart<mxmStringList> mxmString::tokenizeUsingSeparatorStrings(
                                       const mxmStringList &separator_strings,
                                       const mxmString &characters_to_trim)
                           const                                               {
  
  mxm::smart<mxmStringList> token_list;
  mxmString txt, left, right;
  bool did_split;
  mxmString *token;
  
  token_list = new mxmStringList();
  MXM_CHECK_NULL(token_list);
  
  if(length()) {
  
    txt = *this;
    do {
      
      did_split = txt.splitUsingSeparatorStrings(left, right,
                                                 separator_strings);
      
      if(characters_to_trim.length()) left.trim(characters_to_trim);
      
      if(left.length()) {
        
        token = new mxmString(left);
        MXM_CHECK_NULL(token);
        token_list->appendItemToTail(token);
      }
      
      txt = right;
      
    } while(did_split);
  }
  
  return(token_list);
}


/*!
 *  The return value is either the <tt>atoi()</tt> result of a non-empty,
 *  non-<tt>null</tt> string or <tt>0</tt>.
 */
bool mxmString::toInt(int &target_int) {
  
  target_int = 0;
  
  if(!TextBuffer) return(false);
  if(!std::strlen(TextBuffer)) return(false);
  
  target_int = std::atoi(TextBuffer);
  return(true);
}


mxmString mxmString::nullString(void) {
   
  mxmString null;
  
  null.setNull();
  
  return(null);
}
