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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmSmartHandle.h $
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



#ifndef __MXM_SMARTHANDLE_H__
#define __MXM_SMARTHANDLE_H__



#include <mxm/core/mxmLockInterface.h>
#include <mxm/core/mxmSmartHandleCore.h>



namespace mxm {
  
  //! <tt>modularMX</tt> smart pointer implementation - <b>please read <i>Thread
  //! Safety</i> note below</b>.
  /*!
   *  \ingroup modularMX_core
   *
   *  \author <tt>[khe]</tt> Kai Hergenroether
   *
   *  <h2>Semantics</h2>
   *
   *  The used smart pointer semantics is <i>shared</i>: By passing an object
   *  allocated on the heap to a smart pointer, you thereby create the first
   *  smart <i>handle</i> for that <i>handlee</i> object. This handle can
   *  then be copied by various methods, for example by assigning it to other,
   *  type-compatible smart handles or by passing it as parameter into methods.
   *  All the resulting copies, that all refer to the same one handlee object on
   *  the heap, are said to form a <i>handle group</i>. The moment the handlee
   *  was passed to the first of the handles, <i>ownership</i> over it was
   *  transfered from the current context to this handle group. At the time the
   *  last member of the group dies, the handlee object is automatically
   *  deleted.
   *
   *  Smart handles have the following benefits:
   *  - Because they implement pretty many of the ubiquitous operators, they can
   *    be used quite analogously to regular, so-called <i>dumb</i> pointers.
   *  - Since they provide a means for <i>automatic garbage collection</i>,
   *    they help in preventing memory leaks.
   *  - They relieve the programmer from burning his brains with the omnipresent
   *    object ownership question.
   *  - When used for binding dynamically allocated resources to <tt>C++</tt>
   *    scopes (blocks delimited by <tt>{}</tt> brackets) by putting them as
   *    automatic (local) variables on the stack, they support the <tt>RAII</tt>
   *    programming technique (resource allocation is initialization), and
   *    thus
   *    <ul>
   *    <li> considerably simplify programming (especially when it comes to
   *    cleaning up when prematurely exiting from a method)
   *    <li> render the resulting code less error-prone
   *    </ul>
   *
   *  Note that the terms <i>smart pointer</i> and <i>smart handle</i> will
   *  in the following be used synonymously.
   *
   *  <h2>Ownership</h2>
   *
   *  - If you pass a handlee object to a smart pointer, the ownership over that
   *    object is transferred to the smart pointer and the respective emerging
   *    smart pointer group.
   *  - Never delete a smartly handled object yourself.
   *  - Never pass a given handlee object to multiple smart pointer groups.
   *  - Never pass a smart-pointer-handled object to another object that is
   *    designed to assume ownership itself.
   *  - Only if you hold the only  smart pointer for a given object can you
   *    use mxm::smart<T>::releaseSmartlyHandledObject() to transfer the object
   *    ownership from the smart handle back to yourself. The object is then no
   *    longer smartly handled.
   *
   *  <h2><tt>Null</tt>-ness</h2>
   *
   *  In analogy to <tt>null</tt>-pointers, <tt>mxm</tt> smart pointers can also
   *  be <tt>null</tt>-handles. <tt>null</tt>-handles result from
   *  - using the default constructor
   *  - passing dumb <tt>0</tt> to the copy constructor
   *  - assigning dumb <tt>0</tt> via the assignment operator. If in this case the
   *    smart pointer was the only/last handle for a given handlee object, the
   *    object is deleted. A potentially assigned synchronization lock in
   *    <tt>owned</tt> mode is likewise discarded.
   *
   *  <h3><tt>null</tt>-handle Properties</h3>
   *
   *  - A <tt>null</tt>-handle is always "alone", which means it is sole member
   *    of the handle group spawned by itself.
   *  - It can have a lock assigned, though.
   *  - Dereferencing a <tt>null</tt>-handle is generally a bad idea, because
   *    the lower bytes in the virtual address space usually are under the
   *    jurisdiction of the operating system and as a result access protected.
   *    In  most cases you don't have any business of poking around there
   *    anyways.
   *
   *  <h3>Testing for <tt>null</tt>-ness</h3>
   *  To test whether or not a given
   *  smart handle is a <tt>null</tt> handle, either use null() or the
   *  overloaded <tt>!</tt> operator as in
   *  <pre>
   *    if(!handle) {
   *
   *      // _handle_ is _null_ handle
   *    }
   *  </pre>
   *  An implicit conversion to <tt>bool</tt>
   *  was not included in the class to avoid the risk of comparing smart handles
   *  with different template arguments and thus incomatible types. So don't use
   *  <pre>
   *    if(handle) 
   *     handle->doSomething();
   *  </pre>
   *
   *  <h2>Multithreaded Scenarios</h2>
   *
   *  As of <tt>20070328</tt>, <tt>mxm</tt> smart handles provide
   *  synchronization mechanisms to allow safe operation in multithreaded
   *  scenarios. Such scenarios can for example be found
   *  - in the <tt>FUSE</tt>-based <tt>unirecfs</tt> filesystem daemon for
   *    retrieving recorded events from CF/SD flashcards. <tt>FUSE</tt>
   *    filesystem daemons usually are massively multithreaded to deliver good
   *    performance for concurrent file accesses. Since we wish to exploit that,
   *    and since <tt>mxm::smart</tt> handles are used bigtime to tie together
   *    the various components of a <tt>unirec</tt> storage facility, they
   *    better be threadsafe.
   *  - in <tt>MxSDK</tt> client applications as the <tt>MxControlCenter</tt>
   *    and the <tt>MxPEG Server</tt>. The SDK's component framework uses
   *    <tt>mxm::smart</tt> handles to facilitate the handling of loaded
   *    components and their resources. Since SDK clients might very well be
   *    multithreaded, as is the <tt>MxPEG Server</tt> in quite a relaxed
   *    fashion, these handles, again, better be threadsafe.
   *
   *  <h3>How to use Thread Protection?</h3>
   *
   *  - Check whether or not you really need thread protection, see section
   *    <tt>Usage Practices</tt> below.
   *  - Create the handlee object on the stack.
   *  - Pass the handlee object to the first smart handle.
   *  - Invoke mxmSmartHandleCore::setLock() on that handle to attach an
   *    appropriate lock. If you specify the respective flag, you can transfer
   *    ownership over the lock object to the emerging smart handle group.
   *    If you don't, you can have multiple handle groups share the same lock
   *    object for protection - but you are responsible for deleting the lock
   *    when it is no longer used. We will add a shared lock pool facility into
   *    the upcoming mxmApplication class.
   *  - When the smart handle subsequently gets copied, thereby populating its
   *    smart handle group, the locking information is copied to each new
   *    member. As a result, the whole handle group's collective structure can
   *    enjoy the protection of that lock.
   *  - If the lock ownership was transferred to the handle group when the lock
   *    was initially attached, the lock object will automatically be deleted at
   *    the time the last member of the handle group dies.
   *
   *  <b>Note that the handlee object itself will not be protected by the smart
   *  handle group</b>. If you spread the members of a given handle group out
   *  over multiple threads that then use the respective handlee object as a
   *  shared resource, you yourself are responsible of providing adequate
   *  synchronization when accessing it.
   *
   *  <h3>Thread Safety (!!! IMPORTANT !!!)</h3>
   *
   *  This section is only relevant for the special case in which you intend to
   *  use a given smart handle instance as a shared resource itself, meaning: in
   *  case you wish to access it simultaneously from multiple threads (the
   *  handle, not the handlee object!). This might for example be the case if
   *  you have used smart pointers to tie together a complex object structure
   *  that is then accessed by multiple threads without having access locks
   *  preventing the threads from simultaneously following (dereferencing) the
   *  object references.
   *
   *  The thread safety semantics for a given smart handle object are the same
   *  as for the <tt>boost::shared_ptr</tt> implementation, and defined as
   *  follows:
   *  - <tt>mxm::smart</tt> objects offer the same level of thread safety as
   *    built-in types.
   *  - An <tt>mxm::smart</tt> instance can be "read" (accessed using only
   *    <tt>const</tt> operations) simultaneously by multiple threads.
   *    Specifically, a given smart handle can be simultaneously dereferenced,
   *    potentially together with invocations of other <tt>const</tt>
   *    operations.  Note that for example copying a given handle by
   *    construcing another one with the copy constructor
   *    <tt>mxm::smart&lt;T&gt;(const mxm::smart&lt;T&gt; &other)</tt>
   *    is a <tt>const</tt> ("read") operation on the handle <tt>other</tt>.
   *  - Different <tt>mxm::smart</tt> instances can be "written to" (accessed
   *    using mutable operations such as assignment <tt>operator=()</tt>)
   *    simultaneosly by multiple threads, even when these instances are copies
   *    and refer to the same handlee object (= are in the same handle group).
   *  - <b>Any other simultaneous accesses result in undefined behavior</b>.
   *
   *  <h3>Usage Practices</h3>
   *
   *  - Protection for a smart handle group is only necessary if the group
   *    members will be spread out across multiple threads. This should be the
   *    exception more than the rule. Only attach locks if it is really
   *    necessary!
   *  - In general, he who creates the handlee object and places it under smart
   *    handle control also decides whether or not protection is needed and
   *    attaches an appropriate lock to the first handle if necessary. An
   *    attached lock can not be removed from the smart handle group for a
   *    given handlee.
   *  - There are exceptions to the rule:
   *    throughout the <tt>mxm</tt> library it is quite common to find so-called
   *    <i>generator methods</i> that dynamically create a datastructure on the
   *    heap and conveniently return a smart handle to it, as for example
   *    mxmString::tokenize(). In most cases these handles won't feature a lock,
   *    because the called method is a library functionality that might very
   *    well be used in a monothreaded setting. There might be other methods
   *    however that simply return smart handles that were passed into their
   *    objects from the outside earlier. So there is a certain uncertainty
   *    involved when accepting a smart handle as a return value regarding
   *    whether they are protected or not. This uncertainty might hurt you if
   *    you need to ensure protection before using the handle further, because
   *    mxmSmartHandleCore::setLock() will fail badly if its preconditions are
   *    not met. So do some documentation:
   *  - If a method returns a smart handle, if it is possible help client
   *    programmers by <b>documenting</b> clearly
   *    <ul>
   *    <li> whether or not a lock is attached to the handle
   *    <li> whether or not the handle is currently the single handle for the
   *         handlee structure
   *    </ul>
   *    It is absolutely clear then whether or not the client code can for
   *    itself retrofit lock protection for the handle. Do not mess with the
   *    locking settings of a smart handle that you did not create yourself
   *    and/or the protection and group status of which is unclear (because of
   *    missing documentation)!
   *
   *  <h3>Implementation (Advanced)</h3>
   *
   *  To deliver the above straightforward,
   *  <tt>boost::shared_ptr</tt>-compatible thread safety semantics, the
   *  following conventions were used for the implementation:
   *  - All handles in a given handle group form a collective doublelinked
   *    ring structure that in its entirety is fully protected by the shared
   *    lock attached to all its member handles, except for the small bunch of
   *    exceptions listed below.
   *  - The exceptions to this general rule are as follows:
   *    <ul>
   *    <li>The two flavors of the assignment operator operator=()
   *        constitute a safety breach when activated simultaneously with the
   *        same or other handle operations. This is because potentially two
   *        locks are involved in such an assignment (or a sequence of
   *        assignments), that can not be acquired simultaneously (for deadlock
   *        and other reasons). However, the assignment operators are not
   *        <tt>const</tt>, so everytning is OK because simultaneous activation
   *        is forbidden by the above thread safety semantics.
   *    <li>The <tt>const</tt> operations
   *        <ul>
   *        <li> <tt>operator!()</tt> (test for <tt>null</tt>-ness)
   *        <li> <tt>null()</tt> (test for <tt>null</tt>-ness)
   *        <li> <tt>operator->()</tt> (dereferencing)
   *        <li> <tt>operator*()</tt> (dereferencing)
   *        <li> <tt>operator==()</tt> (comparison)
   *        <li><tt>template&lt;class NewT&gt; operator smart<NewT>()</tt>
   *             (<tt>dynamic_cast</tt>-style type conversion)
   *        </ul>
   *        read out the handle's (handles') encapsuled dumb handlee pointer(s)
   *        without acquiring the lock, for performance reasons. However, the
   *        only operations modifying this dumb handlee pointer are, again, the
   *        assignment operators. Since simultaneous activation of these with
   *        themselves or other handle operations is forbidden by the above
   *        thread safety semantics, this also does not pose a problem.
   *    <li>mxmSmartHandleCore::setLock() is not threadsafe. But it is not
   *        <tt>const</tt>, so that's ok.
   *    </ul>
   *
   *    To summarize, since all potentially malicious operations are
   *    non-<tt>const</tt>, it is safe to simultaneously activate <tt>const</tt>
   *    operations on the same given smart handle object.
   *
   *  <h2>Notes</h2>
   *
   *  <h3>Templates and code bloat</h3>
   *
   *  Although this class is a <tt>C++</tt> template class, the main
   *  functionality is inherited from the non-template mxmSmartHandleCore. Only
   *  the code needed to implement type-safety remains here, hopefully minimzing
   *  code expansion.
   *
   *  <h3>Administrative Datastructure</h3>
   *
   *  This implementation currently uses a doublelinked list to link together
   *  all handles for a given "handlee" object. Since we do not intend to
   *  instantiate vast numbers of handles per handled object, this makes sense.
   *
   */
  template<class T> class smart : public mxmSmartHandleCore {
    
   public:
    //! <i>[Not synchronized]</i> Constructs a default-state-handle:
    //!  <tt>null</tt>-handle (=> alone), no lock.
    smart();
    //! <b>[Synchronized via target group's lock]</b> Copy constructor.
    smart(const smart<T> &other);
    //! <i>[Not synchronized]</i> Constructor immediately assigning a dumb
    //! handlee pointer.
    smart(T *handlee);
    //! <b>[Synchronized]</b>
    ~smart();
    
   public:
    //! <b>[*** Synchronized, but SAFETY BREACH]</b> Assignment operator.
    smart<T> &operator=(const smart<T> &other);
    //! <b>[*** Synchronized, but SAFETY BREACH]</b> Assigns handlee object.
    smart<T> &operator=(T *handlee);
    //! <i>[Not synchronized]</i> Dereference operator.
    T *operator->(void) const;
    //! <i>[Not synchronized]</i> Dereference operator.
    T &operator*(void) const;
    //! <i>[Not synchronized]</i> Comparison.
    bool operator==(const smart<T> &other) const;
    //! <b>[Synchronized]</b> 
    //! Dynamic type cast: This is advanced magic stuff for inheritance-based
    //! type conversion between smart handles for different, but related handle
    //! types.
    template<class NewT>
    operator smart<NewT>(void) const;
    
   private:
    //! <i>[Not synchronized]</i> (Re)implemented.
    void deleteHandlee(void);
  };

};   // namespace _mxm_



template<class T>
mxm::smart<T>::smart() {
  
}


template<class T>
mxm::smart<T>::smart(const smart<T> &other) {
  
  joinGroup(other);
}


template<class T> 
mxm::smart<T>::smart(T *handlee) {
  
  Handlee = static_cast<void *>(handlee);   // OK, if _0_ (= no effect)
}


template<class T> 
mxm::smart<T>::~smart() {
  
  // kills potential handlee and lock stuff...
  reset();
                     // this should have been called by the mxmSmartHandleCore
                     // destructor - however, since it calls the virtual method
                     // deleteHandlee(), the latter would then not be correctly
                     // activated due to virtual function invocation working
                     // differently when triggered directly or indirectly from a
                     // destructor
}



/*!
 *  Before joining the new handle group, the handle is reset to default state:
 *  <tt>null</tt>-handle (=> alone), no lock.
 */
template<class T>
mxm::smart<T> &mxm::smart<T>::operator=(const smart<T> &other) {
  
  reset();
  
  // NOTE : synchronization breach: problem here is
  // - there are potentially two locks involved
  // - the handle is temporarily unprotected
  // Scenario of pain: Another thread can now read Lock==0 as part of some
  // handle operation and decide that the handle (group) is unprotected.
  // <preempt> We join another protected handle group by calling joinGroup()
  // below and link into the group's doublelinked ring structure. <preempt> The
  // other thread resumes its operation and modifies the same ring structure
  // without having acquired the lock. Very unfortunate situation.
  
  joinGroup(other);
  
  return(*this);
}


/*!
 *  Before assigning the new handlee object, the handle is reset to default
 *  state: <tt>null</tt>-handle (=> alone), no lock.
 */
template<class T>
mxm::smart<T> &mxm::smart<T>::operator=(T *handlee) {
  
  reset();
  
  Handlee = static_cast<void *>(handlee);   // OK, if _0_ (= no effect)
  
  // NOTE : synchronization breach possible: a potential previous lock
  //        protection has just been removed by reset(). If later on the handle
  //        joins another protected handle group, the process as a whole is a
  //        transition from one lock protection space to another, with a
  //        temporary lack of protection in between, just like the
  //        operator=(const smart<T> &other) assignment above.
  
  return(*this);
}


template<class T>
T *mxm::smart<T>::operator->(void) const {
  
  return(static_cast<T *>(Handlee));
}


template<class T>
T &mxm::smart<T>::operator*(void) const {
  
  return(*static_cast<T *>(Handlee));
}


template<class T>
bool mxm::smart<T>::operator==(const mxm::smart<T> &other) const {
  
  return(Handlee == other.Handlee);
}


/*!
 *  Internally uses a <tt>dynamic_cast</tt> to do the conversion.
 *
 *  \return default-state (=> <tt>null</tt>-) handle in case <tt>RTTI</t> says
 *          the requested conversion is not possible.
 */
template<class T>
template<class NewT>
mxm::smart<T>::operator mxm::smart<NewT>(void) const {
  
  smart<NewT> new_handle;   // creates deafult state handle
  
  if(Lock) Lock->acquire();   // -----------------------------------------------
  
  NewT *new_type = dynamic_cast<NewT *>(static_cast<T *>(Handlee));
  
  if(new_type) new_handle.joinGroup(*this,
                                    static_cast<void *>(new_type),
                                    true);
                                // _true_ => don't synchronize, already did that
  
  if(Lock) Lock->release();   // -----------------------------------------------
  
  return(new_handle);
}


template<class T>
void mxm::smart<T>::deleteHandlee(void) {
  
  delete static_cast<T *>(Handlee);
}



#endif   // __MXM_SMARTHANDLE_H__
