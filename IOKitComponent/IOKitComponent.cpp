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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/IOKitComponent.cpp $
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



#if defined(_MSC_VER) && !defined(MX_STATIC_LIB)
	#ifdef MX_IOKIT_API_WINDOWS_EXPORTS
		#define MX_IOKIT_API __declspec(dllexport)
	#else
		#define MX_IOKIT_API __declspec(dllimport)
	#endif
#else 
	#define MX_IOKIT_API
#endif   // _MSC_VER



#include "include/FileWriter.h"
#include "include/GetConfigSection.h"
#include "include/GetWebPage.h"
#include "include/HTTPGetStreamSource.h"
#include "include/KeyChain.h"
#include "include/StreamFile.h"

#include <interfaces/IComponent.h>

#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxm_generic_stuff.h>



extern "C" {
   MX_IOKIT_API mxmStringList *enumerateComponents();
   MX_IOKIT_API mx::IComponent *createComponent(const char *id);
}



mxmStringList *enumerateComponents() {
   
   return(new mxmStringList((mxmString(MX_FILEWRITER_ID),
                             MX_GETCONFIGSECTION_ID,
                             MX_GETWEBPAGE_ID,
                             MX_HTTPGETSTREAMSOURCE_ID,
                             MX_KEYCHAIN_ID,
                             MX_STREAMFILE_ID)));
}


mx::IComponent *createComponent(const char *id) {
   
   if(mxmString(id) == MX_FILEWRITER_ID)
    return(mx::FileWriter::newInvoker());
   else if(mxmString(id) == MX_GETCONFIGSECTION_ID)
    return(mx::GetConfigSection::newInvoker());
   else if(mxmString(id) == MX_GETWEBPAGE_ID)
    return(mx::GetWebPage::newInvoker());
   else if(mxmString(id) == MX_HTTPGETSTREAMSOURCE_ID)
    return(mx::HTTPGetStreamSource::newInvoker());
   else if(mxmString(id) == MX_KEYCHAIN_ID)
    return(mx::KeyChain::newInvoker());
   else if(mxmString(id) == MX_STREAMFILE_ID)
    return(mx::StreamFile::newInvoker());
   else
    mxm::terminal("invalid component singleton identifier!");
};
