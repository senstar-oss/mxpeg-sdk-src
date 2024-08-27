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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxm_generic_stuff.cpp $
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



#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxmPerlStyleHash.h>
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxmStatusMessageHandlerInterface.h>
#include <mxm/core/mxmException.h>
#include <mxm/core/mxm_macros.h>

#include <typeinfo>
#include <cstdlib>
#include <cstdio>
#include <cstring>



const char *mxm::ValidFilenameCharacters = "abcdefghijklmnopqrstuvwxyz"
                                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                           "1234567890"
                                           "_-.";
const char *mxm::WhitespaceCharacters    = " \t\r";

mxmStatusMessageHandlerInterface
 *mxm::StatusMessageHandlers[StatusMessageHandlerSlots + 1] = { 0 };
mxm::StatusMessageType
 mxm::DefaultStatusMessageLevel = mxm::StatusMessage;



int mxm::randomNumber(int max) {
  
#ifndef MX_PLATFORM_WINDOWS
  long rand_val = random();
  int result = (int)((double)rand_val
                      / 2147483647.0   // 2^31 - 1
                      * (double)max   +   .5);
#else
  long rand_val = rand();
  int result = (int)((double)rand_val
                      / (double)RAND_MAX
                      * (double)max   +   .5);
#endif
  
  mxm::clamp_int(&result, 0, max);
  
  return(result);
}


/*!
 *  \param txt
 *  Text containing argument placeholders of the form <tt>$n</tt> where
 *  <tt>n</tt> is whole number.
 *
 *  \param args
 *  List of strings providing the arguments for substitution of the
 *  aforementioned <tt>$n</tt> placeholders.
 *
 *  Argument substitution is safe,
 *  that is, missing agruments in the list will not crash the application - an
 *  <tt>"[?]"</tt> is substituted instead. You are furthermore entirely
 *  free to change the sequential ordering of argument placeholders in the
 *  text string - the proper argument in the argument list is addressed via the
 *  whole number <tt>n</tt> in the respective <tt>$n</tt> placeholder. 
 */
void mxm::substituteArguments(mxmString &txt, const mxmStringList &args) {
  
  mxmList::Iterator iter;
  mxmString left, right, numba,
            result;
  int arg_index, i;
  mxmString *arg;
  bool did_split;
  
  did_split = txt.split(left, right, "$");
  result = left;
  txt = right;
  
  while(did_split) {
      
    did_split = txt.split(left, right, "$");
    
    numba = left.left("1234567890");
    numba.toInt(arg_index);
    arg_index--;
    
    i = 0;
    iter = args.iterator();
    while(arg = static_cast<mxmString *>(iter.nextItem())) {
      
      if(i == arg_index) {
        result += *arg;
        break;
      }
      
      i++;
    }
    if(!arg) result += "[?]";
    
    if(left.length())
     result += (left.text() + 1);
    
    txt = right;
  }
  
  txt = result;
}


mxmString mxm::hex(void *mem_block, int block_size) {
  
  mxmString result;
  
  char *target_buffer,
       *target_ptr;
  char *source_ptr;
  char txt[10];
  
  target_buffer = new char[block_size*2 + 1];
  MXM_CHECK_NULL(target_buffer);
  
  source_ptr = (char *)mem_block;
  target_ptr = target_buffer;
  while(block_size) {
    std::sprintf(txt, "%02x", (unsigned int)((unsigned char)*source_ptr++));
    *target_ptr++ = txt[0];
    *target_ptr++ = txt[1];
    block_size--;
  }
  *target_ptr = '\0'; 
  
  result.setText(target_buffer);
  
  delete[] target_buffer;
  
  return(result);
}


bool mxm::debugMode(void) {
  
  return(true);
}


void mxm::sendStatusMessage(mxm::StatusMessageType msg_type,
                            const mxmString &msg,
                            const mxmObject *sending_object) {
  
  mxmStatusMessageHandlerInterface **handler = StatusMessageHandlers;
  
  if(*handler) {
    
    while(*handler) {
      
      if(msg_type >= (*handler)->statusMessageVerbosity())
       (*handler)->sendStatusMessage(msg_type,
                                     msg,
                                     sending_object);
      handler++;
    }
    
    return;
  }
  
  // default status message processing...
  // TODO : treat platforms differently
  
  if(msg_type < DefaultStatusMessageLevel) return;
  
#ifndef MX_PLATFORM_WINDOWS
  mxmString class_name;
  if(sending_object) class_name = mxmString("(")
                                   + rttiClassName(sending_object)
                                   + ") ";
  std::puts((mxmString("mxm ") + class_name + ": " + msg).text());
#endif
}


/*!
 *  The central status message facility assumes ownership over the handler.
 */
void mxm::addStatusMessageHandler(mxmStatusMessageHandlerInterface
                                   *msg_handler)                   {
  
  mxmStatusMessageHandlerInterface **handler = StatusMessageHandlers;
  int handler_num = 0;
  while(*handler) {
    
    handler_num++;
    handler++;
  }
  
  if(handler_num == StatusMessageHandlerSlots)
   mxm::terminal("no more status message handler slots!");
  
  *handler++ = msg_handler;
  *handler   = 0;
  
  msg_handler->setStatusMessageVerbosity(DefaultStatusMessageLevel);
}


/*!
 *  The handler gets deleted.
 *
 *  If the specified handler is not installed, the method does nothing.
 */
void mxm::removeStatusMessageHandler(mxmStatusMessageHandlerInterface
                                     *msg_handler)                   {
  
  mxmStatusMessageHandlerInterface **handler = StatusMessageHandlers;
  while(*handler) {
    
    if(*handler == msg_handler) break;
    
    handler++;
  }
  
  if(*handler) {
    
    delete (*handler);
    
    do {
      
      *handler = *(handler + 1);
      handler++;
    } while(*handler);
  }
}


/*!
 *  The handlers get deleted.
 */
void mxm::removeStatusMessageHandlers() {
  
  mxmStatusMessageHandlerInterface **handler = StatusMessageHandlers;
  while(*handler) {
    
    delete (*handler);
    *handler = 0;
    
    handler++;
  }
}


void mxm::setStatusMessageVerbosity(StatusMessageType msg_level) {
  
  mxmStatusMessageHandlerInterface **handler = StatusMessageHandlers;
  while(*handler) {
    
    (*handler)->setStatusMessageVerbosity(msg_level);
    handler++;
  }
  
  DefaultStatusMessageLevel = msg_level;
}


bool mxm::statusMessageUseful(StatusMessageType msg_level) {
  
  StatusMessageType current_level;
  
  mxmStatusMessageHandlerInterface **handler = StatusMessageHandlers;
  if(*handler) {
    
    current_level = mxm::NullMessage;
    
    StatusMessageType handler_level;
    while(*handler) {
      
      handler_level = (*handler)->statusMessageVerbosity();
      if(handler_level < current_level) current_level = handler_level;
      
      handler++;
    }
  }
  else {
    
    current_level = DefaultStatusMessageLevel;
  }
  
  return(msg_level >= current_level);
}


mxmString mxm::rttiClassName(const mxmObject *sending_object) {
  
  if(sending_object)
   return(typeid(*sending_object).name());
  else
   return("<null>");
}


void mxm::terminal(const mxmString &msg,
                   const mxmObject *sending_object) {
  
  std::puts((mxmString("*** TERMINAL FAILURE (")
              + rttiClassName(sending_object)
              + ") : "
              + msg).text());
  
  mxm::removeStatusMessageHandlers();      // give the handlers a chance to shut
                                           // down proeprly
  
  throw mxmTerminalException(msg, sending_object);  // TODO : only if not on cam
  
  //std::exit(666);
}
