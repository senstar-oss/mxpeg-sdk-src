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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxm_generic_stuff.h $
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



/*!
    \defgroup modularMX_core modularMX Runtime Platform Core

    \brief
    <tt>MxSDK</tt> Runtime Platform Core
*/



#ifndef __MXM_NAMESPACE_H__
#define __MXM_NAMESPACE_H__



#undef u8
   // steve, steve, steve...



#include <mxm/core/mxmString.h>
#include <mxm/core/mxm_types.h>



class mxmPerlStyleHash;
class mxmStringList;
class mxmStatusMessageHandlerInterface;



//! Namespace housing some <tt>modularMX</tt> generic stuff.
namespace mxm {
  
  const int DialogSpacing             = 10;
  const int StatusMessageHandlerSlots = 10;
  
  extern MXM_CORE_API const char *ValidFilenameCharacters;
  extern MXM_CORE_API const char *WhitespaceCharacters;
  
  extern MXM_CORE_API mxmStatusMessageHandlerInterface
                          *StatusMessageHandlers[StatusMessageHandlerSlots + 1];
  extern MXM_CORE_API StatusMessageType DefaultStatusMessageLevel;
  
  //! Does the obvious thing.
  inline void clamp_s16(s16 *numba, s16 min, s16 max) {
    if(*numba < min) *numba = min;
    if(*numba > max) *numba = max;
  }
  //! Does the obvious thing.
  inline void clamp_int(int *numba, int min, int max) {
    if(*numba < min) *numba = min;
    if(*numba > max) *numba = max;
  }
  //! Does the obvious thing.
  inline void clamp_float(float *numba, float min, float max) {
    if(*numba < min) *numba = min;
    if(*numba > max) *numba = max;
  }
  
  //! Returns an integer random number from <tt>{ 0, 1, 2, ..., max}</tt>.
  int MXM_CORE_API randomNumber(int max);
  
  //! Substitutes <tt>$n</tt> argument placeholders by the arguments they
  //! reference in the specified string list.
  MXM_CORE_API void substituteArguments(mxmString &txt,
                                        const mxmStringList &args);
  
  //! Produces hexdump representation of the given memory block.
  MXM_CORE_API mxmString hex(void *mem_block, int block_size);
  
  //! Tells whether or not the <tt>modularMX</tt> debug mode is enabled.
  MXM_CORE_API bool debugMode(void);
  
  //! <b>[Callable anytime]</b> Outputs a message via the central status message
  //! facility.
  MXM_CORE_API void sendStatusMessage(mxm::StatusMessageType msg_type,
                                      const mxmString &message,
                                      const mxmObject *sending_object = 0);
  //! <b>[Not synchronized!]</b> Installs a new custom status message handler.
  MXM_CORE_API void addStatusMessageHandler(mxmStatusMessageHandlerInterface
                                             *msg_handler);
  //! <b>[Not synchronized!]</b> Uninstalls a given custom status message
  //! handler.
  MXM_CORE_API void removeStatusMessageHandler(mxmStatusMessageHandlerInterface
                                               *msg_handler);
  //! <b>[Not synchronized!]</b> Uninstalls all custom status message handlers.
  MXM_CORE_API void removeStatusMessageHandlers();
  //! <b>[Not synchronized!]</b> Sets the verbosity level for the central status
  //! message facility.
  MXM_CORE_API void setStatusMessageVerbosity(StatusMessageType msg_level);
  //! <b>[Not synchronized!]</b> Returns <tt>true</tt> if and only if there
  //! is currently interest in messages of the level specified.
  MXM_CORE_API bool statusMessageUseful(StatusMessageType msg_level);
  
  //! Returns the C++ implementation's runtime type information class name for
  //! the <tt>mxm</tt> object.
  MXM_CORE_API mxmString rttiClassName(const mxmObject *sending_object);
  
  //! Called when terminal condition encountered - was introduced when we turned
  //! away from exceptions.
  MXM_CORE_API void terminal(const mxmString &msg,
                             const mxmObject *sending_object = 0);
};



#endif   // __MXM_NAMESPACE_H__
