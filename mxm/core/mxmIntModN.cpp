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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmIntModN.cpp $
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



#include <mxm/core/mxmIntModN.h>
#include <mxm/core/mxm_generic_stuff.h>



mxmIntModN::mxmIntModN(int n) {
  
  if(n < 2) mxm::terminal("N must be at least 2!",
                          this);
  
  N     = n;
  Value = 0;
}


mxmIntModN::mxmIntModN(const mxmIntModN &another_n) {
  
  *this = another_n;
}


mxmIntModN::mxmIntModN(volatile const mxmIntModN &another_n) {
  
  *this = another_n;
}


mxmIntModN &mxmIntModN::operator=(const mxmIntModN &another_n) {
  
  N     = another_n.N;
  Value = another_n.Value;
  
  return(*this);
}
mxmIntModN &mxmIntModN::operator=(volatile const mxmIntModN &another_n) {
  
  N     = another_n.N;
  Value = another_n.Value;
  
  return(*this);
}
volatile mxmIntModN &mxmIntModN::operator=(const mxmIntModN &another_n)
                                  volatile                              {
  
  N     = another_n.N;
  Value = another_n.Value;
  
  return(*this);
}



mxmIntModN &mxmIntModN::operator=(int value) {
  
  Value = value;
  
  while(Value < 0)  Value += N;
  while(Value >= N) Value -= N;
  
  return(*this);
}


volatile mxmIntModN &mxmIntModN::operator=(int value) volatile {
  
  Value = value;
  
  while(Value < 0)  Value += N;
  while(Value >= N) Value -= N;
  
  return(*this);
}


mxmIntModN &mxmIntModN::operator=(mxm::u64 value) {
  
  Value = value;
  
  while(Value < 0)  Value += N;
  while(Value >= N) Value -= N;
  
  return(*this);
}


mxmIntModN mxmIntModN::operator+(int other_n) const {
  
  mxmIntModN da_other((int)N);
  
  da_other = other_n;
  return(*this + da_other);
}


mxmIntModN &mxmIntModN::operator++(int some_shit) {
  
  Value++;
  if(Value >= N) Value -=N;
  
  return(*this);
}


mxmIntModN::operator int() const {
  
  return((int)Value);
}


mxmIntModN::operator int() volatile const {
  
  return((int)Value);
}


mxmIntModN operator+(const mxmIntModN &n1, const mxmIntModN &n2) {
  
  if(n1.N != n2.N) mxm::terminal("incompatible Ns for addition!");
  
  mxmIntModN sum = n1;
  sum.Value += n2.Value;
  while(sum.Value >= sum.N) sum.Value -= sum.N;
  
  return(sum);
}
