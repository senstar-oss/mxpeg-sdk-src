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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/IOKitComponent/trunk/include/FileWriter.h $
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



#ifndef __MX_FILEWRITER_H__
#define __MX_FILEWRITER_H__



#define MX_FILEWRITER_ID "FileWriter"



#include "StreamFile.h"

#include <interfaces/IFileWriter.h>
#include <interfaces/IComponent.h>
#include <interfaces/IComponentInvoker.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmObject.h>



namespace mx {
   
   //! File writer implementation.
   /*!
    *  \ingroup mxsdk_iokit
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class FileWriter : public mxmObject,
                      public virtual IComponent,
                      public virtual IFileWriter {
      
      //! These static methods provide interface and capability ids for
      //! registration with the framework (for both the component and its
      //! invoker).
    private:
      static mxm::smart<mxmStringList> _enumerateInterfaces() {
         return(new mxmStringList(
                       (mxmString(MX_FILEWRITER_ID),
                        MX_ICOMPONENT_ID,
                        MX_IFILEWRITER_ID,
                        MX_ISUPPORTSFILESPECIFICATION_ID,
                        MX_ISEGMENTEDSTREAMRECEIVER_ID,
                        MX_ISTREAMRECEIVER_ID,
                        MX_IERRORSTATE_ID)));
      }
      static mxm::smart<mxmStringList> _enumerateCapabilities(
                                          const mxmString &interface_id) {
         if(interface_id == mx::Framework::IdHintKey)
          return(new mxmStringList(MX_FILEWRITER_ID));
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
                                      const mxmString &interface_id) {
            return(_enumerateCapabilities(interface_id));
         }
         IComponent *newComponent() {
            return(new FileWriter());
         }
      };
    public:
      //! Generates an invoker singleton for the component.
      static IComponentInvoker *newInvoker() {
         return(new Invoker());
      }
      
    private:
      mxmString Filename;
      int       FileDescriptor;
      mxm::smart<mx::StreamFile> File;
      bool IndividualFileMode;
      int  IndividualFileNum;
      int  IndividualFileCounter;
      mxmString Separator;
      bool ErrorEncountered;
      bool Fresh,
           FreshSegment;
      bool HasBeenShutDown;
      
    public:
      FileWriter();
      ~FileWriter();
      
    public:
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateInterfaces();
      //! (Re)implemented.
      mxm::smart<mxmStringList> enumerateCapabilities(
                                   const mxmString &interface);
      
      //! (Re)implemented.
      void setFilename(const mxmString &filename);
      //! (Re)implemented.
      void setFileDescriptor(int fd);
      
      //! (Re)implemented.
      void receiveStreamBytes(const mxm::u8 *bytes, int byte_num);
      //! (Re)implemented.
      void newStreamSegment();
      
      //! (Re)implemented.
      void writeSingleConcatenatedFile();
      //! (Re)implemented.
      void setSegmentSeparator(const mxmString &separator);
      //! (Re)implemented.
      void writeIndividualFiles(int num);
      
      //! (Re)implemented.
      bool errorState();
      //! (Re)implemented.
      void setErrorState();
      //! (Re)implemented.
      void shutdown();
      
    private:
      void openFile();
      void closeFile();
      void reset(bool safe_settings = false);
   };
};



#endif   // __MX_FILEWRITER_H__