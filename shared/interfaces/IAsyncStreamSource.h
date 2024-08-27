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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IAsyncStreamSource.h $
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



#ifndef __MX_IASYNCSTREAMSOURCE_H__
#define __MX_IASYNCSTREAMSOURCE_H__



#define MX_IASYNCSTREAMSOURCE_ID "IAsyncStreamSource"



#include "IStreamSource.h"

#include <mxm/core/mxmQtSignal.h>



namespace mx {
  
  //! Interface to byte stream sources that support asynchronous incoming data
  //! notification.
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class IAsyncStreamSource : public virtual IStreamSource {
    
   public:
    //! Returns the <tt>Qt</tt> signal that is emitted when new stream bytes
    //! become available at the stream source.
    /*!
     *  <b>Requirement:</b> When a context reads the source "empty" (until
     *  IStreamSource::fetchStreamBytes() reports <tt>0</tt>) for each such
     *  signal, and always tests IStreamSource::stillUp() afterwards, it must be
     *  guaranteed that it does not miss stream disconnections.
     *
     *  \note   In rare special conditions it is allowed that
     *          IStreamSource::fetchStreamBytes() returns <tt>0</tt> bytes even
     *          though available stream bytes have been signalled. Therefore,
     *          the return value of IStreamSource::fetchStreamBytes() must
     *          always be tested against <tt>0</tt>.
     */
    virtual mxmQtSignal streamBytesAvailableSignal() const = 0;
    //! Blocks until new stream bytes become available.
    /*!
     *  \param timeout_ms
     *  <tt>0</tt> means no timeout
     *
     *  \return <tt>true</tt> in case bytes have become available for reading,
     *          and <tt>false</tt> otherwise.
     *
     *  \note   In rare special conditions it is allowed that
     *          IStreamSource::fetchStreamBytes() returns <tt>0</tt> bytes even
     *          though the wait function has returned and reported available
     *          data. Therefore, the return value of
     *          IStreamSource::fetchStreamBytes() must always be tested against
     *          <tt>0</tt>.
     */
    virtual bool waitForStreamBytes(int timeout_ms = 0) = 0;
  };
};


#endif   // __MX_IASYNCSTREAMSOURCE_H__

