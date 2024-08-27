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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxmQt/trunk/include/mxmQt/mxm_qt.h $
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



#ifndef __MXM_QT_H__
#define __MXM_QT_H__



#include <mxmQt/mxm_qt_platform_macros.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxmQtSignal.h>
#include <mxm/core/mxmQtSlot.h>

#include <QString>



namespace mxm {
   
   //! Conversion.
   MXM_QT_API mxmString mxmStr(const QString   &q_str);
   //! Conversion.
   MXM_QT_API QString     qStr(const mxmString &mxm_str);
   
   //! For conveniently wiring up <tt>Qt</tt> signals.
   MXM_QT_API bool connect(const mxmQtSignal &source_signal,
                           const mxmQtSlot   &target_slot);
   //! For conveniently wiring up <tt>Qt</tt> signals.
   MXM_QT_API bool connect(const mxmQtSignal &source_signal,
                           const mxmQtSignal &target_signal);
   //! For conveniently wiring up <tt>Qt</tt> signals.
   MXM_QT_API bool connect(const mxmQtSignal &source_signal,
                           const QObject *receiver, const char *method);
   //! For conveniently wiring up <tt>Qt</tt> signals.
   MXM_QT_API bool connect(const QObject *sender, const char *signal,
                           const mxmQtSlot &target_slot);
   //! For conveniently wiring up <tt>Qt</tt> signals.
   MXM_QT_API bool connect(const QObject *sender, const char *signal,
                           const mxmQtSignal &target_signal);
};



#endif   // __MXM_QT_H__