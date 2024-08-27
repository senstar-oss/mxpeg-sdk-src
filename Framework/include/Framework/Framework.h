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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/Framework/trunk/include/Framework/Framework.h $
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



#ifndef __MX_FRAMEWORK_H__
#define __MX_FRAMEWORK_H__



// MS Visual C++ stuff...
#if defined(_MSC_VER) && !defined(MX_STATIC_LIB)
	#ifdef MX_FRAMEWORK_API_WINDOWS_EXPORTS
		#define MX_FRAMEWORK_API __declspec(dllexport)
	#else
		#define MX_FRAMEWORK_API __declspec(dllimport)
	#endif
#else 
	#define MX_FRAMEWORK_API
#endif    // _MSC_VER



#include <mx_error_codes.h>
#include <interfaces/IComponent.h>
#include <mxm/core/mxmObject.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxmPerlStyleHash.h>
#include <mxm/core/mxmApplication.h>

#ifdef MX_PLATFORM_WINDOWS
   #define WIN32_LEAN_AND_MEAN
	#undef UNICODE
	#include <windows.h>
	#include <direct.h>
#endif


namespace mx {
   
   //! Central component manager for the <tt>MxSDK</tt>.
   /*!
    *  \ingroup mxsdk_framework
    *
    *  Supports plugin and self-registration of components.
    *
    *  \warning Does not yet support multi-threading.
    *
    *  \author <tt>[agu]<tt> Alexander Gugau
    *  \author <tt>[khe]<tt> Kai Hergenroether
    */
   class MX_FRAMEWORK_API Framework : public mxmObject {
      
    public:
      //! Selection flag.
      static const mxm::flags HarmlessSelectionFailures = 1;
      static const char *IdHintKey;
      static const char *ObsoletesHintKey;
      
    public:
      //! Holds meta-information describing a component type.
      class MX_FRAMEWORK_API ComponentDescriptor : public mxmPerlStyleHash {
       private:
         void *StableABIDataExtension;
       public:
         //! Adds an interface requirement to the component description.
         void addRequiredInterface(const mxmString &interface_id,
                                   const mxmStringList &interface_capabilities);
         //! Convenience version.
         void addRequiredInterface(const mxmString &interface_id,
                                   const mxmString &interface_capability = "");
      };
      
    private:
      class ComponentEntry : public mxmObject {
       public:
         mxm::smart<mx::IComponent> spComponent;
         bool RuledOut;      // when used during the component selection process
       public:
         ComponentEntry();
      };
      class ComponentPackEntry : public mxmObject {
       public:
        mxmString Filename;
#ifdef MX_PLATFORM_WINDOWS
       public:
         HINSTANCE DLLHandle;
#else
       public:
         void *SharedObjectHandle;
#endif
       public:
         ~ComponentPackEntry();   // unloads the dynamic library
      };
      
    private:
      static Framework* s_pFramework;
      
    private:
      mxmList LoadedComponentPacks;
      mxmList ComponentList;
      void *StableABIDataExtension;
      
    public:
      //! Retrieves the framework instance.
      static Framework* framework();
        
      //! Returns a smart handle for a component satisfying the requested
      //! constraints.
      static mx::EErrorCode getComponentInterface(const ComponentDescriptor &cd,
                                                  mxm::smart<mx::IComponent>
                                                  &spInterface);
      //! Convenience version.
      static mx::EErrorCode getComponentInterface(const char* InterfaceId,
                                                  mxm::smart<mx::IComponent>
                                                  &spInterface,
                                                  const mxmStringList&
                                                   Capabilities);
      //! Convenience version.
      static mx::EErrorCode getComponentInterface(const char* InterfaceId,
                                                  mxm::smart<mx::IComponent>
                                                   &spInterface,
                                                  const mxmString& Capability
                                                                    = "");
      
      //! Returns smart handle for arbitrary type of component, but throws
      //! terminal exception in case of failure.
      static mxm::smart<IComponent> getComponent(const ComponentDescriptor &cd);
      //! Convenience version.
      static mxm::smart<IComponent> getComponent(const mxmString &interface_id,
                                                 const mxmStringList
                                                       &interface_capabilities);
      //! Convenience version.
      static mxm::smart<IComponent> getComponent(const mxmString &interface_id,
                                                 const mxmString
                                                        &interface_capability
                                                          = "");
      
      //! Instantiates a multiple-instance type of component with semantics
      //! comparable to <tt>operator new()</tt>: returns dump pointer and throws
      //! terminal exception in case of failure.
      static IComponent *newComponentInstance(const ComponentDescriptor &cd,
                                              mxm::flags selection_flags = 0);
      //! Convenience version.
      static IComponent *newComponentInstance(const mxmString &interface_id,
                                              const mxmStringList
                                               &interface_capabilities,
                                              mxm::flags selection_flags = 0);
      //! Convenience version.
      static IComponent *newComponentInstance(const mxmString &interface_id,
                                              const mxmString
                                                     &interface_capability = "",
                                              mxm::flags selection_flags = 0);
      
    public:
      //! Plugin manager constructor.
      Framework(mxmList *statically_linked_components = 0);
      ~Framework();
      
    public:
      //! Returns the number of installed components.
      int numComponents();
      //! Returns the ids of the installed components.
      mxm::smart<mxmStringList> enumerateComponents();
      
    private:
      //! Dynamically loads all present component packs into the address space.
      int bindComponentPacks();
      //! Installs components bootstrapped by the application context.
      int useStaticallyLinkedComponents(mxmList *statically_linked_components);
      //! Helper method doing the actual dynamic loading of component pack
      //! libraries.
      bool loadSharedLibrary(const mxmString &filename);
      //! Component selection (and possibly instantiation) algorithm.
      mx::EErrorCode provideComponent(const ComponentDescriptor &cd,
                                      mxm::smart<mx::IComponent> &spInterface,
                                      bool &instantiated_new);
   };

};   // mx namespace



#endif  // __MX_FRAMEWORK_H__
