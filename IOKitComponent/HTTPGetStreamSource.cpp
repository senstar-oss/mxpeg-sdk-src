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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/HTTPGetStreamSource.cpp $
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



#include "include/HTTPGetStreamSource.h"



mx::HTTPGetStreamSource::HTTPGetStreamSource() {
   
   AsyncSocket = new QTcpSocket();
   
   QObject::connect(AsyncSocket, SIGNAL(connected()),
                    this,        SLOT(processIncomingBytes()));
   QObject::connect(AsyncSocket, SIGNAL(readyRead()),
                    this,        SLOT(processIncomingBytes()));
   QObject::connect(AsyncSocket, SIGNAL(disconnected()),
                    this,        SLOT(processSocketDisconnected()));
}


mx::HTTPGetStreamSource::~HTTPGetStreamSource() {
   
   AsyncSocket->blockSignals(true);   // otherwise the socket might emit a
                                      // _disconnected()_ signal just before
                                      // biting it, and we would emit our
                                      // _bytesAvailable()_ signal despite we're
                                      // already going down ourselves...
   
   delete AsyncSocket;
}



int mx::HTTPGetStreamSource
     ::fetchStreamBytes(mxm::u8 *buffer, int num) {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   if(!Connected) return(0);
   if(!Up)        return(0);
   
   int result;
   int bytes_read = AsyncSocket->read((char *)buffer, num);
   if(bytes_read < 0) {
      
      ErrorEncountered = true;
      result = 0;
   }
   else {
      
      result = bytes_read;
   }
   
   if(   !result
      && Disconnected) {
      
      mxm::sendStatusMessage(mxm::StatusMessage,
                             "no more data, source down",
                             this);
      Up = false;
   }
   
   return(result);
}


bool mx::HTTPGetStreamSource::stillUp() {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   if(!Connected)
    return(true);
   else
    return(Up);
}


bool mx::HTTPGetStreamSource::waitForStreamBytes(int timeout_ms) {
   
   if(!Active) mxm::terminal("not yet activated!",
                             this);
   
   return(AsyncSocket->waitForReadyRead(timeout_ms));
}


void mx::HTTPGetStreamSource::establishConnection() {
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          "attempting to establish connection to "
                           + Host.toString() + "...",
                          this);
   
   mxmIPAddress ip;
   if(Proxy != mxmIPAddress::nullIP()) ip = Proxy;
   else                                ip = Host;
   
   AsyncSocket->connectToHost((mxmString(ip.ip1()) + "."
                                + ip.ip2() + "."
                                + ip.ip3() + "."
                                + ip.ip4()).text(),
                              ip.port());
}


void mx::HTTPGetStreamSource::abortConnection() {
   
   AsyncSocket->blockSignals(true);
   AsyncSocket->abort();
   AsyncSocket->blockSignals(false);
}


bool mx::HTTPGetStreamSource::readChar(char *c) {
   
   return(AsyncSocket->read(c, 1) == 1);
}


void mx::HTTPGetStreamSource::send(const mxmString &txt) {
   
   AsyncSocket->write(txt.text(), txt.length());
   
   // We don't do any error checking here. Errors will be detected via the
   // stream connection timeout detection done by the superior context...
}
