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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/include/StreamFile.h $
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



#ifndef __MX_STREAMFILE_H__
#define __MX_STREAMFILE_H__



#define MX_STREAMFILE_ID "StreamFile"



#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>
#include <interfaces/IStreamSourceFile.h>
#include <interfaces/IStreamReceiver.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmObject.h>



namespace mx {
   
   //! Abstraction of a binary file, usable both for reading and writing
   //! (binary) streams.
   /*!
    *  \ingroup mxsdk_iokit
    *
    *  If invoked by specifying a filename to the constructor, the respective
    *  file is automatically opened. It is also automatically closed again when
    *  the stream file is deleted later.
    *
    *  If, on the other hand, the stream file is invoked by specifying a 
    *  <tt>POSIX</tt> file descriptor to the constructor, the file descriptor is
    *  used instead. Note that in this case the corresponing file is <b>not
    *  closed</b> upon stream file destruction.
    *
    *  The stream file sports a so-called <i>Timed Block Mode</i> that can be
    *  toggled at any time using the \ref setTimedBlockModeEnabled() method. If
    *  enabled, a UNIX <tt>select()</tt> call is issued prior to read attempts
    *  on the encapsuled file descriptor using the specified timeout. This
    *  causes the \ref fetchBytes() implementation to block in the kernel if no
    *  data is available, but at most for that many milliseconds as was
    *  specified to \ref setTimedBlockModeEnabled(). If such a blocking times
    *  out before any new data come in over the file descriptor,
    *  \ref fetchBytes() returns with a return value of <tt>0</tt>.
    *
    *  As usual, with <tt>Microsoft</tt> happily adding non-<tt>POSIX</tt> flags
    *  to <tt>POSIX</tt> interfaces, we have to provide a modified
    *  implementation for the <tt>WINDOWS</tt> platform.
    *
    *  Shut-down mode was retrofitted: potential file gets closed, then the
    *  object won't do anything any more except for indicating error state.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class StreamFile : public mxmObject,
                      public virtual IComponent,
                      public virtual IStreamSourceFile,
                      public virtual IStreamReceiver    {
      
      //! These static methods provide interface and capability ids for
      //! registration with the framework (for both the component and its
      //! invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList((mxmString(MX_STREAMFILE_ID),
                                   MX_ICOMPONENT_ID,
                                   MX_ISTREAMSOURCEFILE_ID,
                                   MX_ISUPPORTSFILESPECIFICATION_ID,
                                   MX_ISTREAMSOURCE_ID,
                                   MX_IREQUIRESACTIVATION_ID,
                                   MX_ISTREAMRECEIVER_ID,
                                   MX_ISUPPORTSSHUTTINGDOWN_ID,
                                   MX_IERRORSTATE_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface) {
         if(interface == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_STREAMFILE_ID));
         else
          return(new mxmStringList());
      }
      
    private:
      class Invoker : public mxmObject, public virtual IComponentInvoker {
       public:
         mxm::smart<mxmStringList> enumerateInterfaces() {
            return(_enumerateInterfaces());
         }
         mxm::smart<mxmStringList> enumerateCapabilities(
                                     const mxmString &interface) {
            return(_enumerateCapabilities(interface));
         }
         IComponent *newComponent() {
            return(new StreamFile());
         }
      };
    public:
      //! Generates an invoker singleton for the \ref StreamFile component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    private:
      mxmString Filename;
      int FileDescriptor;
      bool WriteMode;
      bool Online;
      
      int FD;
      bool StillUp;
      bool ErrorEncountered;
      bool HasBeenShutDown;
      bool CloseFileUponDestruction;
#ifdef MX_PLATFORM_WINDOWS
      int RestoreMode;
      long OSFHandle;
#endif
      bool TimedBlockMode;
      long BlockTimeout;
      
    public:
      StreamFile();
      ~StreamFile();

    public:
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateInterfaces() {
         return(_enumerateInterfaces());
      }
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateCapabilities(
                                   const mxmString &interface) {
         return(_enumerateCapabilities(interface));
      }
      
      //! <b>[Config Phase]</b> (Re)implemented.
      void setFilename(const mxmString &filename);
      //! <b>[Config Phase]</b> (Re)implemented.
      void setFileDescriptor(int fd);
      //! <b>[Config Phase]</b> Specifies whether the file should be opened for
      //! reading or for writing,
      void setWriteMode(bool enabled);
      
      //! Toggles/configures the timed block mode, see above.
      void setTimedBlockModeEnabled(bool enabled, long timeout = 1000);
      
      //! (Re)implemented.
      int fetchStreamBytes(mxm::u8 *buffer, int num);
      //! (Re)implemented.
      bool stillUp(void);
      
      //! (Re)implemented.
      void receiveStreamBytes(const mxm::u8 *bytes, int byte_num);
      
      //! (Re)implemented.
      bool errorState();
      //! (Re)implemented.
      void setErrorState();
      //! (Re)implemented.
      void shutdown();
      
      //! (Re)implemented.
      bool activate();
      
     private:
      //! Helper - just closes file, must be called only once!
      void closeFile();
   };
};



#endif   // __MX_STREAMFILE_H__
