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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmContainer.h $
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



#ifndef __MXM_CONTAINER_H__
#define __MXM_CONTAINER_H__



#include <mxm/core/mxmObject.h>
#include <mxm/core/mxmContainerInterface.h>



class mxmTypeDecider;



//! Base class to all <tt>modularMX</tt> containers.
/*!
 *  \ingroup modularMX_core
 *
 *  When we purged the camera filesystem of the prohibitively large 
 *  <tt>libstdc++</tt> and installed our <tt>MX</tt> C++ mini-runtime instead,
 *  we somehow had to come up with a replacement for the widely used
 *  <tt>STL</tt> container templates, such as <tt>vector</tt>, <tt>list</tt> and
 *  <tt>map</tt>. These replacements are the subclasses to mxmContainer.
 *
 *  In contrast to the <tt>STL</tt> containers, <tt>modularMX</tt> containers
 *  manage items that are subclassed from mxmObject, allocated dynamically on
 *  the heap via <tt>new</tt>, and referenced by pointers. When added to a
 *  container, the ownership over an item is transferred to that container that
 *  will delete it porperly the moment it is destroyed itself.
 *
 *  The set of object types that can be added to a container can be, but does
 *  not have to be, restricted by installing an mxmTypeDecider with the
 *  container, that will then most likely use <tt>RTTI</tt> (runtime type
 *  information) to reject invalid types.
 *
 *  The items stored in a container can be investigated and possibly modified
 *  via <i>iterators</i>. To obtain an iterator for a given specific
 *  mxmContainer implementation, call its <tt>iterator()</tt> method, such as
 *  for example mxmList::iterator().
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmContainer : public mxmObject,
                                  public mxmContainerInterface {
  
 protected:
  mxmTypeDecider *ItemTypeDecider;
  
 public:
  //! Default constructor.
  mxmContainer();
  //! Pass to this constructor an appropriate type decider if you wish to
  //! restrict the set of possible mxmObject types that can be added to this
  //! container.
  mxmContainer(mxmTypeDecider *item_type_decider);
  ~mxmContainer();
  
 public:
  //! (Re)implementation for interface mxmContainerInterface. 
  void clear(void);
  //! Item type decider to be used from now on.
  void setItemTypeDecider(mxmTypeDecider *item_type_decider);
};



#endif   // __MXM_CONTAINER_H__

