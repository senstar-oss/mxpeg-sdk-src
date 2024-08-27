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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IInterface.h $
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



#ifndef __MX_IINTERFACE_H__
#define __MX_IINTERFACE_H__

#include "../mx_capabilities.h"

#define MX_IINTERFACE_ID "IInterface"



namespace mx {

   class IInterface;
};



//! The "mother" of all interfaces used for inter-component communications.
/*!
    \ingroup mxsdk_interfaces
    
    These interfaces contain nothing but pure virtual methods and are designed
    to minimize compilation dependencies between <tt>MxSDK</tt> components. To
    that end they limit themselves to use in their signatures only trivial data
    types or those provided by the underlying SDK's system libraries
    (<tt>mxm</tt>, <tt>mxcpc</tt>, ...). Interfaces to GUI components may
    obviously also use <tt>Qt</tt> datatypes.
    
    <h2>Interface classes</h2>
    
    The <tt>MxSDK</tt>'s <tt>I*</tt> interfaces come in two flavors:
    <ol>
      <li><b>Feature Interfaces</b> describe individual features that a
      component may or may not have, depending on whether or not it implements
      the respective interfaces. Among others, we have:
       - <b><tt>ISupports*Specification</tt></b>  Interfaces : denote that the
         component allows the client context to specify a specific set of
         parameters. Example: <tt>ISupportsHostSpecification</tt>
       - <b><tt>IRequires*</tt></b>               Interfaces : indicate that
         the component requires some external entity implementing a particular
         concept (interface) in order to function properly. Example:
         <tt>IRequiresExternalAuthenticator</tt>
       - <b><tt>ICanUse*</tt></b>                 Interfaces : indicate that the
         component allows client contexts to toggle whether or not it should
         use (activate) a specific concept. To that end the component provides
         appropriate <tt>use*()</tt> methods. Example:
         <tt>ICanUseAuthenticationDialog</tt> with its method
         <tt>ICanUseAuthenticationDialog::useAuthenticationDialog()</tt>.
       - <b><tt>I*Receiver</tt></b>               Interfaces : for dataflow
         modelling - indicate that the component receives data of a specific
         kind for further processing. Such interfaces typically contain
         methods named <tt>receive*()</tt>, where <tt>*</tt> stands for the
         respective type of data, that client contexts may call to actively
         perform the transfer. These interfaces can therefore be seen as
         <i>"passive"</i> interfaces. Example: <tt>IStreamReceiver</tt> with
         the method <tt>IStreamReceiver::receiveStreamBytes()</tt>
       - <b><tt>ISends*</tt></b>                  Interfaces : for dataflow
         modelling - form the counterpart to the aforementioned
         <tt>I*Receiver</tt> interfaces and indicate that the components
         actively send data of the respective type by calling the
         <tt>receive*()</tt> methods. In order to do so, they obviously need to
         know where to transfer the data, so each <tt>ISends*</tt> interface
         contains a respective <tt>set*Receiver()</tt> method. Example:
         <tt>ISendsMxPEGTiles</tt> with its method
         <tt>ISendsMxPEGTiles::setMxPEGTileReceiver()</tt>
       - <b><tt>IProcesses*</tt></b>              Interfaces : for dataflow
         modelling - are similar to <tt>I*Receiver</tt> interfaces in that they
         also describe components that accept data of a specific kind for
         further processing, but differ in that the respective components are
         not "passive" in doing so, but perform the active role instead. As such
         they rather actively fetch their input data from an appropriate data
         source (guess what, implemented by <tt>I*Source</tt> interfaces) by
         calling respective <tt>fetch*()</tt> methods on it. These fetches are
         implemented in a matching <tt>process*()</tt> method. Example:
         <tt>IProcessesStream</tt> with its method
         <tt>IProcessesStream::processStreamBytes()</tt>
       - <b><tt>I*Source</tt></b>                 Interfaces : for dataflow
         modelling - see <tt>IProcesses*</tt> interfaces above. Example:
         <tt>IStreamSource</tt> with its method
         <tt>IStreamSource::fetchStreamBytes()</tt>.
      <li><b>Conceptual Interfaces</b> encode concepts by accumulating feature
      interfaces and possibly other conceptual interfaces. Example:
      <tt>IHTTPGetStreamSource</tt>. Click it to learn which fearture interfaces
      and other concept iterfaces it inherits (The dashed interface boxes
      indicate that the interfaces are inherited virtually).
    </ol>
    
    As <tt>I*</tt> interfaces are by general rule always inherited virtually,
    that is: using the <tt>virtual</tt> keyword, concept interfaces are not
    required to be mutually disjoint.

    \author <tt>[agu]</tt> Alexander Gugau
    \author <tt>[khe]</tt> Kai Hergenroether
*/
class mx::IInterface {

  public:
   virtual ~IInterface() {}
};



#endif  // __MX_IINTERFACE_H__
