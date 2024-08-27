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
//  $LastChangedDate: 2007-06-29 16:29:06 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/Framework/trunk/include/Framework/FrameworkMessageHandler.h $
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



/*!
	\file FrameworkMessagehandler

	\author AGu

*/

#ifndef __MX_FRAMEWORKMESSAGEHANDLER_H__
#define __MX_FRAMEWORKMESSAGEHANDLER_H__

// MS Visual C++ stuff...
#if defined(_MSC_VER) && !defined(MX_STATIC_LIB)
	#ifdef MX_FRAMEWORK_API_WINDOWS_EXPORTS
		#define MX_FRAMEWORK_API __declspec(dllexport)
	#else
		#define MX_FRAMEWORK_API __declspec(dllimport)
	#endif
#else 
	#define MX_FRAMEWORK_API
#endif    // _MSC_VER

#include <mxm/core/mxmStatusMessageHandlerInterface.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxmObject.h>

#include <fstream>



namespace mx {

//! Message handler writing status messages to file.
/*!
   This message handler writes the incoming status messages to a file. The file
   can grow to a certain size. If the file reached this size, it is copied and a
   new file is created.
   This message handler must be added via <tt>addStatusMessageHandler</tt> from
   the <tt>mxm</tt> message handler framework to activate it.
*/
class MX_FRAMEWORK_API FrameworkMessageHandler : public mxmStatusMessageHandlerInterface,
                                                 public mxmObject {
   private:
      bool m_bUseLock;
      mxm::StatusMessageType m_msgLevel;
      long m_lSizeLimit;
      std::ofstream m_logStream;
      mxmLockInterface* m_lock;
      void *StableABIDataExtension;
      
	public:
		FrameworkMessageHandler(bool bUseLock = false);
		~FrameworkMessageHandler();
		//! (re-)implemented
		void sendStatusMessage(mxm::StatusMessageType msg_type, 
									  const mxmString &message, 
									  const mxmObject *object = 0);
		//! (re-)implemented
		void setStatusMessageVerbosity(mxm::StatusMessageType msg_level);
		//! (re-)implemented
		mxm::StatusMessageType statusMessageVerbosity() { return m_msgLevel; }
      
   private:
		void reopenLogfile();
};

}; // namespace mx
#endif // __MX_FRAMEWORKMESSAGEHANDLER_H__