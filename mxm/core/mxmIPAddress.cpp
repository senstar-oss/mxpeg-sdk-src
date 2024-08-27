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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmIPAddress.cpp $
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



#include <mxm/core/mxmIPAddress.h>



#ifdef MX_PLATFORM_WINDOWS
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
#endif




mxmIPAddress::mxmIPAddress() {
   
   initCommonStuff();
   
   IP1  = 0;
   IP2  = 0;
   IP3  = 0;
   IP4  = 0;
   
   Port = NoPort;
   
   Resolved = true;
}


mxmIPAddress::mxmIPAddress(int ip1, int ip2, int ip3, int ip4,
                           int port)                           {
   
   initCommonStuff();
   
   IP1  = ip1;
   mxm::clamp_int(&IP1, 0, 255);
   
   IP2  = ip2;
   mxm::clamp_int(&IP2, 0, 255);
   
   IP3  = ip3;
   mxm::clamp_int(&IP3, 0, 255);
   
   IP4  = ip4;
   mxm::clamp_int(&IP4, 0, 255);
   
   Port = port;
   if(Port != NoPort) mxm::clamp_int(&Port, 0, PortMax);
   
   Resolved = true;
}



mxmIPAddress::mxmIPAddress(const mxmString& hostName) {
   
   initCommonStuff();
   
   int ip1, ip2, ip3, ip4,
       port;
   
   // check if sth like "10.0.0.52:80" or "10.0.0.52"
   // in the first case convertedFields is 5, in the
   // second case convertedFields is 4
   int convertedFields = std::sscanf(hostName.text(), "%d.%d.%d.%d:%d", 
                                    &ip1, &ip2, &ip3, &ip4, &port);

   if(convertedFields >= 4) {
      
      IP1 = ip1;
      mxm::clamp_int(&IP1, 0, 255);
      IP2 = ip2;
      mxm::clamp_int(&IP2, 0, 255);
      IP3 = ip3;
      mxm::clamp_int(&IP3, 0, 255);
      IP4 = ip4;
      mxm::clamp_int(&IP4, 0, 255);
      
      Port = NoPort;
      if(convertedFields == 5) {
        
        Port = port;
        mxm::clamp_int(&Port, 0, PortMax);
      }
      
      Resolved = true;
      
   } else {   // it's sth like "cam01.mobotix.net"
      
      HostName              = hostName;
      IP1 = IP2 = IP3 = IP4 = 0;
      Port                  = NoPort;
      
      Resolved = false;
   }
}

// copy ctor
mxmIPAddress::mxmIPAddress(const mxmIPAddress& ip) {
   
   initCommonStuff();
   
   *this = ip;
}


void mxmIPAddress::initCommonStuff() {
   
#ifdef MX_PLATFORM_WINDOWS
   WSADATA wsaData;
   mxm::u16 versionRequested;
   versionRequested = MAKEWORD(2,2);
   WSAStartup(versionRequested, &wsaData);
#endif
}


mxmIPAddress::~mxmIPAddress() {
   
#ifdef MX_PLATFORM_WINDOWS
   WSACleanup();
#endif
}



// assignment
mxmIPAddress& mxmIPAddress::operator=(const mxmIPAddress& ip) {

   IP1      = ip.IP1;
   IP2      = ip.IP2;
   IP3      = ip.IP3;
   IP4      = ip.IP4;
   Port     = ip.Port;
   
   HostName = ip.HostName;
   
   Resolved = ip.Resolved;

   return(*this);
}


mxmIPAddress& mxmIPAddress::operator=(const mxmString& hostName) {

   *this = mxmIPAddress(hostName);
   
   return(*this);
}


/*!
 *  A comparison with unresolvable addresses involved always returns
 *  <tt>false</tt>.
 *
 *  \note If you intend to only compare the host part, ignoring the port
 *        setting, use \ref sameHost().
 */
bool mxmIPAddress::operator==(const mxmIPAddress& ip) const {

   // see that both are recolved...
   resolve();
   ip.resolve();
   
   // comparison with unresolvable addresses always returns false
   if(!Resolved || !ip.Resolved)
    return false;
   
   return(   (IP1  == ip.IP1)
          && (IP2  == ip.IP2)
          && (IP3  == ip.IP3)
          && (IP4  == ip.IP4)
          && (Port == ip.Port));
}


bool mxmIPAddress::operator!=(const mxmIPAddress& ip) const {
   
   return(!(*this == ip));
}


bool mxmIPAddress::sameHost(const mxmIPAddress& ip) const {
  
  mxmIPAddress ip1 = *this,
               ip2 = ip;
  ip1.setPort(NoPort);
  ip2.setPort(NoPort);
  
  return(ip1 == ip2);
}


/*!
 *  Only works for resolvable addresses.
 */
void mxmIPAddress::setPort(int port) {
   
   resolve();
   if(resolved()) {
      
      Port = port;
      if(Port != NoPort) mxm::clamp_int(&Port, 0, PortMax);
   }
}


mxmString mxmIPAddress::toString() const {

   resolve();
   
   mxmString txt = mxmString(IP1) + "." + IP2 + "." + IP3 + "." + IP4;
   if(Port != NoPort) txt += mxmString(":") + Port;
   
   return(txt);
}


/*!
 *  If the address is already resolved, the method does nothing.
 *
 *  \return <tt>true</tt> in case the address is properly resolved upon return,
 *          and <tt>false</tt> otherwise
 */
bool mxmIPAddress::resolve() const {

   if(Resolved) return(true);                                   // nothing to do

   struct addrinfo* result = 0;
   struct addrinfo  hints  = { 0, PF_INET, 0, 0 };
   mxm::u32 ip;

   getaddrinfo(HostName.text(), NULL, &hints, &result);
   if(result) {
      
      struct addrinfo* current = result;
      while(current) {
         
         if(current->ai_family == PF_INET) {
            ip = ((struct sockaddr_in *)(current->ai_addr))->sin_addr.s_addr;
            break;
         }
         current = current->ai_next;
      }
      
      freeaddrinfo(result);
      
      if(current) {
         
         const_cast<mxmIPAddress *>(this)->setFromNetwork(ip);
         
         // get port setting...
         mxmString hostname = HostName,
                   left, right;
         hostname.split(left, right, ":");
         if(right.length()) {
           
           int port;
           right.toInt(port);
           mxm::clamp_int(&port, 0, PortMax);
           const_cast<mxmIPAddress *>(this)->Port = port;
         }
         
         const_cast<mxmIPAddress *>(this)->Resolved = true;
         
         return(true);
      }
   }
   
   // defaults are properly set
   
   return(false);
}


void mxmIPAddress::setFromNetwork(mxm::u32 networkOrderIp) {
   
   mxm::u32 hostOrderIp = ntohl(networkOrderIp);
   IP1 = (hostOrderIp & 0xff000000) >> 24;
   IP2 = (hostOrderIp & 0x00ff0000) >> 16;
   IP3 = (hostOrderIp & 0x0000ff00) >> 8;
   IP4 = (hostOrderIp & 0x000000ff);
}


mxmIPAddress mxmIPAddress::nullIP() {
   
   return(mxmIPAddress());
}
