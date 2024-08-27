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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IStreamSource.h $
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



#ifndef __MX_ISTREAMSOURCE_H__
#define __MX_ISTREAMSOURCE_H__



#define MX_ISTREAMSOURCE_ID "IStreamSource"



#include "IErrorState.h"
#include "IRequiresActivation.h"

#include <mxm/core/mxm_types.h>



namespace mx {
  
  //! Interface to byte stream sources, as for example sources of (media)
  //! streams.
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   */
  class IStreamSource : public virtual IErrorState,
                        public virtual IRequiresActivation {
    
   public:
    //! Fetches at most <tt>num</tt> raw stream bytes from the stream source,
    //! writes them to the specified buffer, and returns the number of bytes
    //! actually fetched.
    /*!
     *  A return value of <tt>0</tt> is not necessarily an error - maybe there
     *  simply were no more bytes available when the method was called.
     *
     *  However, in case of error, it is also <tt>0</tt> that is returned. 
     *
     *  In case of a return value of <tt>0</tt>,
     *  use the stillUp() method to test whether or not the stream source is
     *  still operational. If it is not, all subsequent calls to fetchBytes()
     *  will return <tt>0</tt>.
     */
    virtual int fetchStreamBytes(mxm::u8 *buffer, int num) = 0;
    //! Returns whether or not the stream source is still operational.
    /*!
     *  If a stream source has become "not-up" once, it will never come up
     *  again. Instead, its fetchBytes() method will return <tt>0</tt> on all
     *  subsequent calls. Possible examples for a stream source that has become
     *  no longer up are an encountered <tt>EOF</tt> or read error.
     */
    virtual bool stillUp(void) = 0;
  };
};


#endif   // __MX_ISTREAMSOURCE_H__

