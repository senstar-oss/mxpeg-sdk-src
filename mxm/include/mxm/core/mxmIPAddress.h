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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmIPAddress.h $
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



#ifndef __MXM_IPADDRESS_H__
#define __MXM_IPADDRESS_H__



#include <mxm/core/mxmIPAddress.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_core_platform_macros.h>



//! IP address, including port specification if desired.
/*!
 *  \ingroup modularMX_core
 *
 *  This implementation is a join of the obsoleted <tt>mxcpcIPAddress</tt> and
 *  the former <tt>mxmIPAddress</tt> that was developed on the camera.
 *
 *  The class does lazy DNS lookup automatically, i.e. the address
 *  is resolved exactly once, when the data is used. To force the DNS lookup
 *  the <tt>resolve()</tt> method can be used.
 *
 *  There is a special value named <tt>null</tt>-IP, being nonsensical
 *  <tt>0.0.0.0</tt>, no port. It can be conveniently used in APIs to denote
 *  "no IP set".
 *
 *  \note The <tt>WSAStartup()</tt> / <tt>WSACleanup()</tt> hazzle on the
 *        <tt>Windows</tt> platform is currently transparently taken care of
 *        by <tt>mxmIPAddress</tt>.
 */
class MXM_CORE_API mxmIPAddress : public mxmObject {
   
 public:
   static const int NoPort  = -1;
   static const int PortMax = 65535;
   
 private:
   int IP1, IP2, IP3, IP4;
   int Port;
   mxmString HostName;
   bool Resolved;                 // indicates that hostname is already resolved
   void *StableABIDataExtension;
   
 public:
   //! Returns a <tt>nully</tt>-IP.
   static mxmIPAddress nullIP();
   
 public:
   //! Initializes as <tt>null</tt>-IP, which is <tt>0.0.0.0</tt>, no port.
   mxmIPAddress();
   //! Initializes as specified <tt>IP4</tt> address, including port if given.
   mxmIPAddress(int ip1, int ip2, int ip3, int ip4, int port = NoPort);
   //! Initializes from the specified host name.
   mxmIPAddress(const mxmString& hostName);
   //! Copy constructor.
   mxmIPAddress(const mxmIPAddress& ip);
 private:
   void initCommonStuff();
 public:
   ~mxmIPAddress();
   
 public:
   //! Assignment.
   mxmIPAddress& operator=(const mxmIPAddress& ip);
   //! Assignment.
   mxmIPAddress& operator=(const mxmString& hostName);
   //! Comparison.
   bool operator==(const mxmIPAddress& ip) const;
   //! Negated Comparison.
   bool operator!=(const mxmIPAddress& ip) const;
   //! Comparison like above, but ignoring the port settings.
   bool sameHost(const mxmIPAddress& ip) const;
   
   //! Reconfigures port information, <tt>mxmIPAddress::NoPort</tt> unsets port.
   void setPort(int port);
   
   int ip1()  const { resolve(); return(IP1);  }
   int ip2()  const { resolve(); return(IP2);  }
   int ip3()  const { resolve(); return(IP3);  }
   int ip4()  const { resolve(); return(IP4);  }
   int port() const { resolve(); return(Port); }
   bool resolved() const { return Resolved; }
  
   //! Returns string representation in form of <tt>ip1.ip2.ip3.ip4[:port]</tt>
   //! - in case of an unresolvable address this would be "0.0.0.0".
   mxmString toString() const;
   //! Explicitly triggers <tt>DNS</tt> resolving.
   bool resolve() const;

 private:
   //! Uses <tt>ntohl()</tt> to properly convert the byte order - the port
   //! setting is cleared.
   void setFromNetwork(mxm::u32 networkOrderIp);
};



#endif   // __MXM_IPADDRESS_H__
