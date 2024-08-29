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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxgconv/trunk/mxgconv.cpp $
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



#ifndef MX_PLATFORM_WINDOWS
  #include <unistd.h>
#else
  #define STDIN_FILENO 0
  #define STDOUT_FILENO 1
  #define STDERR_FILENO 2
#endif

#include <mxpeg_sdk/static_component_bootstrap.h>
                                    // for statically linked MxPEG SDK demo case

#include <interfaces/IStreamSourceFile.h>
#include <interfaces/IMxPEGParser.h>
#include <interfaces/IMxPEGScanDecoder.h>
#include <interfaces/IMxPEGScanToJPEGConverter.h>
#include <interfaces/IMxPEGRawFrameDumper.h>
#include <interfaces/IFileWriter.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmApplication.h>
#include <mxm/core/mxmNopStatusMessageHandler.h>
#include <mxm/core/mxmPerlStyleHash.h>
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxm_generic_stuff.h>

#include <cstdio>
#include <cstdlib>

#include "mxpeg-decoder.h"

#include "../IOKitComponent/include/FileWriter.h"
#include "../IOKitComponent/include/StreamFile.h"

#include "../MxPEGCoreComponent/include/DiagnosticsMxPEGTileReceiver.h"
#include "../MxPEGCoreComponent/include/MxPEGParser.h"
#include "../MxPEGCoreComponent/include/MxPEGRawFrameDumper.h"
#include "../MxPEGCoreComponent/include/MxPEGScanDecoderSoftwareOnly.h"
#include "../MxPEGCoreComponent/include/MxPEGScanToJPEGConverterSoftwareOnly.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

// BOOL APIENTRY DllMain( HMODULE hModule,
// 					  DWORD  ul_reason_for_call,
// 					  LPVOID lpReserved
// 					  )
// {
// 	return TRUE;
// }
BOOL APIENTRY DllMain( HMODULE /*hModule*/,
					  DWORD  ul_reason_for_call,
					  LPVOID /*lpReserved*/
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

// MessageHandler that outputs to Windows debug

class DebugMessageHandler : public mxmStatusMessageHandlerInterface
{
	mxm::StatusMessageType m_level = mxm::StatusMessageType::DesperateDebugMessage;
public:
	//! Processes the status message as it sees it fit.

	void sendStatusMessage(mxm::StatusMessageType /*msg_type*/,
		const mxmString & msg,
		const mxmObject * /*sending_object = 0*/)
	{
		mxmString text("mxm: ");
		text += msg;
		OutputDebugStringA(text.text());
	}

	//! Used (e.g. by the central status message facility) to switch the handler
	//! to a given verbosity level - the handler is free to ignore the request,
	//! though.
	void setStatusMessageVerbosity(mxm::StatusMessageType msg_level)
	{
		m_level = msg_level;
	}

	//! Returns the verbosity level currently supported/expected by the handler.

	mxm::StatusMessageType statusMessageVerbosity()
	{
		return m_level;
	}
};

class CMxpegDecode
{
public:
	CMxpegDecode(){};
	~CMxpegDecode(){};

	void DecodeMxpeg(BYTE * pIn,int len,BYTE * pOut,int * outlen) ;
	void UnInitMxpeg() ;
	void InitMxpeg();
protected:
	mxm::smart<mx::IMxPEGParser> parser;
	mx::IFileWriter *writer;
};

void CMxpegDecode::DecodeMxpeg(BYTE * pIn, int len, BYTE * pOut, int * outlen) 
{
	mxm::sendStatusMessage(mxm::DebugMessage, mxmString("Decode"));

	int outBufSize = *outlen;
	*outlen = 0;

	parser->setBuffer(pIn);
	writer->setOutLength(outBufSize);
	writer->setOutBuffer(pOut);
	writer->setOutLengthPointer(outlen);
	parser->processStreamBytes(len);
}

void CMxpegDecode::UnInitMxpeg() 
{
	mxm::sendStatusMessage(mxm::DebugMessage, mxmString("UnInit"));

	writer->shutdown();

	// we're done, smart pointer will handle clean-up...

	if(writer->errorState()) {

		std::fprintf(stderr, "*** failed!\n");
		//std::exit(2);
	}
	else {

		//std::exit(0);
	}
	mxm::removeStatusMessageHandlers();
}


//int Testmain(int argc, char **argv,char * pIn) {
void CMxpegDecode::InitMxpeg() 
{
/*   mxm::addStatusMessageHandler(new mxmNopStatusMessageHandler());*/
                       // make sure we won't get mxm status messages on _stdout_

	// Un-comment these 2 following lines to get all messages out via OutputDebugStringA, which you can see using sysinternals DbgView.exe
	//mxm::DefaultStatusMessageLevel = mxm::StatusMessageType::DesperateDebugMessage;
	//mxm::addStatusMessageHandler(new DebugMessageHandler);

	mxm::sendStatusMessage(mxm::DebugMessage, mxmString("Init"));

	writer = dynamic_cast<mx::IFileWriter *>(dynamic_cast<mxmObject *>( new mx::FileWriter()));

	mx::IUndecodedMxPEGFrameReceiver *frame_receiver;
	mxmString format ="yuv",outfile_extension;

	mx::IMxPEGRawFrameDumper
		*dumper  
		= dynamic_cast<mx::IMxPEGRawFrameDumper *>(dynamic_cast<mxmObject *>( new mx::MxPEGRawFrameDumper()));

	dumper->setSegmentedStreamReceiver(writer);

	mx::IMxPEGScanDecoder *scan_decoder = dynamic_cast<mx::IMxPEGScanDecoder *>(dynamic_cast<mxmObject *>( new mx::MxPEGScanDecoderSoftwareOnly()));

	scan_decoder->setMxPEGTileReceiver(dumper);
	frame_receiver = scan_decoder;

	writer->writeIndividualFiles(0);

	dumper->enableYUVDumps(true);

	outfile_extension = ".yv12";

	writer->setFileDescriptor(STDOUT_FILENO);

	mx::IMxPEGParser * p = dynamic_cast<mx::IMxPEGParser *>(dynamic_cast<mxmObject *>( new mx::MxPEGParser()));
	parser = p;

	parser->setUndecodedMxPEGFrameReceiver(frame_receiver);
}

__declspec(dllexport) void * InitMxpeg()
{
	CMxpegDecode * p = new CMxpegDecode();
	p->InitMxpeg();
	return (void *)p;
}

__declspec(dllexport) void DecodeMxpeg(void * pObject,BYTE * pIn,int len,BYTE * pOut,int * outlen)
{
	CMxpegDecode * p = (CMxpegDecode*)pObject;
	p->DecodeMxpeg(pIn,len,pOut,outlen);
}

__declspec(dllexport) void UnInitMxpeg(void * pObject)
{
	CMxpegDecode * p = (CMxpegDecode*)pObject;
	p->UnInitMxpeg();
	delete p;
}