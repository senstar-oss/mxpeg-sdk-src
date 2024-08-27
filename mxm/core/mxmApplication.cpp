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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmApplication.cpp $
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



#include <mxm/core/mxmApplication.h>
#include <mxm/core/mxmDefaultSynchronizationFactory.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>

#ifdef MX_PLATFORM_WINDOWS
#undef UNICODE
#include <windows.h>
#endif

#include <cstring>



mxmApplication *mxmApplication::SingleInstance = 0;



mxmApplication::mxmApplication(int argc,
                               char **argv,
                               mxmSynchronizationFactoryInterface
                                *sync_factory)                    {
  
  if(SingleInstance)
   mxm::terminal("central application object already instantiated!",
                 this);
  
  SingleInstance = this;
  
  if(!sync_factory) {
    
    sync_factory = new mxmDefaultSynchronizationFactory();
    MXM_CHECK_NULL(sync_factory);
  }
  
  SynchronizationFactory = sync_factory;
  
  // protect factory's smart handle (group) by matching sync mechanism...
  SynchronizationFactory.setLock(sync_factory->newLock(), true);
              // _true_ => smart handle group assumes ownership over lock object
}


mxmApplication::~mxmApplication() {
  
  // shut down status message facility...
  mxm::removeStatusMessageHandlers();
  
  // TODO : other cleaning up stuff...
  
  SingleInstance = 0;
}



/*!
 *  Note that the synchronization factory is threadsafe, and that the returned
 *  smart handle is protected by a lock of the kind matching the synchronization
 *  mechanisms that the factory provides.
 */
mxm::smart<mxmSynchronizationFactoryInterface>
 mxmApplication::synchronizationFactory()      {
  
  if(!SingleInstance) mxm::terminal("central application object missing!");
  
  return(SingleInstance->SynchronizationFactory);
}

mxmString mxmApplication::basePath() {

	char pathName[512];

#ifdef MX_PLATFORM_WINDOWS
	DWORD ret = ::GetModuleFileName(NULL, pathName, 512);
	if(ret == 0) {
		ret = ::GetLastError();
		void* pMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, ret, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsgBuf, 0, NULL);
      
		mxmString errtxt((const char*)pMsgBuf);
		::LocalFree(pMsgBuf);
		::MessageBox(NULL, errtxt.text(), "Internal Error", MB_OK | MB_ICONSTOP);
		mxm::terminal(errtxt, SingleInstance);
	}
#else
	// TODO: non-windows implementation
	pathName[0] = '\0';
#endif
	// remove application name
	for(int i=std::strlen(pathName); i>0; i--) {
		if(pathName[i] == '/' || pathName[i] == '\\') {
			pathName[i] = '\0';
			break;
		}
	}
	return mxmString(pathName);
}
