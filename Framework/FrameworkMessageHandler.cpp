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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/Framework/trunk/FrameworkMessageHandler.cpp $
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



#include "include/Framework/FrameworkMessageHandler.h"
#include "include/Framework/Framework.h"

#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxmDate.h>

#ifdef MX_PLATFORM_WINDOWS
	#include <windows.h>
#endif



mx::FrameworkMessageHandler::FrameworkMessageHandler(bool bUseLock) {
	// default
	m_msgLevel = mxm::StatusMessage;
	m_lSizeLimit = 1000000; // 1MB size of logfile
	m_bUseLock = bUseLock;
	
	if(bUseLock)
		m_lock = mxmApplication::synchronizationFactory()->newLock();
	else
		m_lock = 0;

	// open the logfile, append mode
	mxmString logName = mxmApplication::basePath() + "/Framework.log";
	m_logStream.open(logName.text(), std::ios_base::app | std::ios_base::out);
	if(m_logStream.fail()) {
		mxmString errtxt = mxmString("Cannot open logfile ") + logName;
		mxm::terminal(errtxt, this);
	}

	m_logStream << "\n=========================================\n" 
		<< mxmDate::currentDate().toString(mxmDate::eEuropean2Time).text() << ": Message handler created. ";
}

mx::FrameworkMessageHandler::~FrameworkMessageHandler() {

	m_logStream << "\n" << mxmDate::currentDate().toString(mxmDate::eEuropean2Time).text() 
		<< ": Message handler destroyed. \n=========================================\n ";
	m_logStream.close();

	delete m_lock;
}

void mx::FrameworkMessageHandler::setStatusMessageVerbosity(mxm::StatusMessageType msg_level) {
	m_msgLevel = msg_level;
}

void mx::FrameworkMessageHandler::sendStatusMessage(mxm::StatusMessageType /*msg_type*/,
																const mxmString &message,
																const mxmObject *object) {

	if(m_bUseLock)
		m_lock->acquire();

	// check if actual size exceeds the size limit
	int iSize = m_logStream.tellp();
	if(iSize > m_lSizeLimit)
		reopenLogfile();

	mxmString classInfo("class:(unknown) : ");
	if(object)
		classInfo = mxm::rttiClassName(object);

	m_logStream << "\n" << mxmDate::currentDate().toString(mxmDate::eEuropean2Time).text() << ": " 
		<< classInfo.text() << " : " << message.text();
	
	if(m_bUseLock)
		m_lock->release();
}

void mx::FrameworkMessageHandler::reopenLogfile() {

	m_logStream << "\n" << mxmDate::currentDate().toString(mxmDate::eEuropean2Time).text() 
		<< ": Changing logfile. \n=========================================\n ";
	m_logStream.close();

	mxmString oldLogfile = mxmApplication::basePath() + "/Framework_old.log";
	mxmString newLogfile = mxmApplication::basePath() + "/Framework.log";

	// remove old File
	remove(oldLogfile.text());
	rename(newLogfile.text(), oldLogfile.text());

	m_logStream.open(newLogfile.text(), std::ios_base::app | std::ios_base::out);
	if(m_logStream.fail()) {
		mxmString errtxt = mxmString("Cannot open logfile ") + newLogfile;
		mxm::terminal(errtxt, this);
	}

	m_logStream << "\n=========================================\n" 
		<< mxmDate::currentDate().toString(mxmDate::eEuropean2Time).text() << ": New logfile created. ";
	
}