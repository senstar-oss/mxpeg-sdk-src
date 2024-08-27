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
//  $LastChangedDate: 2007-06-28 15:51:01 +0200 (Thu, 28 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxpeg_sdk/trunk/static_component_bootstrap.cpp $
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



// Note that dependencies on Qt and IPP were commented out. Feel free to
// reactivate them in case you have Qt and/or IPP at your disposal.



#include <mxpeg_sdk/static_component_bootstrap.h>

#include <mxm/core/mxmList.h>

// Note that including headers for specific component interface implementations
// as we do it here is for bootstrapping statically linked components only
// (simple MxPEG SDK cases). Application code always instantiates and uses
// components via the mx::Framework and the global I* component interfaces
// defined in the <SDK_ROOT>/shared/interfaces folder.
#include "../IOKitComponent/include/FileWriter.h"
//#include "../IOKitComponent/include/GetConfigSection.h"
//#include "../IOKitComponent/include/GetWebPage.h"
//#include "../IOKitComponent/include/HTTPGetStreamSource.h"
//#include "../IOKitComponent/include/KeyChain.h"
#include "../IOKitComponent/include/StreamFile.h"

#include "../MxPEGCoreComponent/include/DiagnosticsMxPEGTileReceiver.h"
#include "../MxPEGCoreComponent/include/MxPEGParser.h"
#include "../MxPEGCoreComponent/include/MxPEGRawFrameDumper.h"
#include "../MxPEGCoreComponent/include/MxPEGScanDecoderSoftwareOnly.h"
#include "../MxPEGCoreComponent/include/MxPEGScanToJPEGConverterSoftwareOnly.h"
//#include "../MxPEGCoreComponent/include/SimpleMxPEGRenderer.h"

// uncomment this if you wish to enable IPP support...
//#include "../IPPAcceleratedMxPEGComponent/include/MxPEGScanDecoderIPP.h"
//#include "../IPPAcceleratedMxPEGComponent/include/MxPEGScanToJPEGConverterIPP.h"



/*!
 *  The returned datastructure is created dynamically on the heap and is
 *  intended for immediately being passed into the constructor of the
 *  <tt>mx::Framework</tt> plugin manager.
 */
mxmList *mxpeg_sdk::createStaticallyLinkedComponents() {
   
   mxmList *components = new mxmList();
   
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::FileWriter::newInvoker()));
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::GetConfigSection::newInvoker()));
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::GetWebPage::newInvoker()));
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::HTTPGetStreamSource::newInvoker()));
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::KeyChain::newInvoker()));
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::StreamFile::newInvoker()));
   
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::DiagnosticsMxPEGTileReceiver::newInvoker()));
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::MxPEGParser::newInvoker()));
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::MxPEGRawFrameDumper::newInvoker()));
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::MxPEGScanDecoderSoftwareOnly::newInvoker()));
   components->addItem(dynamic_cast<mxmObject *>
                        (mx::MxPEGScanToJPEGConverterSoftwareOnly::newInvoker()));
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::SimpleMxPEGRenderer::newInvoker()));
   
   // uncomment this if you wish to enable IPP support...
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::MxPEGScanDecoderIPP::newInvoker()));
   //components->addItem(dynamic_cast<mxmObject *>
   //                     (mx::MxPEGScanToJPEGConverterIPP::newInvoker()));
   
   return(components);
}