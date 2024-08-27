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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/Framework/trunk/Framework.cpp $
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



#include <Framework/Framework.h>

#include <interfaces/IComponentInvoker.h>

#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxmApplication.h>

#include <cstring>

#ifdef MX_PLATFORM_WINDOWS
   #define PLUGIN_SEARCH_NAME "\\plugins\\*.dll"
   #define PLUGIN_PATH_NAME   "\\plugins\\"
#else
   #define PLUGIN_PATH        "./plugins"
   #include <dirent.h>
   #include <sys/stat.h>
   #include <dlfcn.h>
#endif



typedef mx::IComponent*(*ComponentSingletonCreatorFunction)(const char*);
typedef mxmStringList*(*ComponentSingletonEnumeratorFunction)();



mx::Framework* mx::Framework::s_pFramework = 0;

const char *mx::Framework::IdHintKey        = "_id";
const char *mx::Framework::ObsoletesHintKey = "_obsoletes";



/*!
 *  The framwork scans the <tt>SDK</tt>'s plugin folder for installed component
 *  packs and attempts to load what it finds. Unless you specify a list of
 *  statically linked components bootstrapped earlier - as for example via
 *  <tt>mxpeg_sdk::createStaticallyLinkedComponents()</tt> in simple
 *  <tt>MxPEG SDK</tt> scenarios (the <tt>MxPEG SDK</tt> is the subset of the
 *  <tt>MxSDK</tt> that is released to the general public under the <tt>BSD
 *  License</tt> in order to provide low level <tt>MxPEG</tt> decoding for
 *  integration purposes).
 *
 *  If <tt>dynamically_linked_components</tt> is given, the framework assumes
 *  ownership over the whole datastructure.
 */
mx::Framework::Framework(mxmList *statically_linked_components) {
   
   if(s_pFramework) mxm::terminal("framework instance already instantiated!",
                                  this);
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          "MOBOTIX SDK component framework initializing...",
                          this);
   
   s_pFramework = this;
   
   if(statically_linked_components) {
      
      mxm::sendStatusMessage(mxm::StatusMessage,
                             "using statically linked components provided by "
                              "the application context",
                             this);
      if(!useStaticallyLinkedComponents(statically_linked_components))
       mxm::terminal("failed to install any statically linked components!",
                     this);
      
      // the list _statically_linked_components_ has been deleted by
      // useStaticallyLinkedComponents() above
   }
   else {
      
      mxm::sendStatusMessage(mxm::StatusMessage,
                             "loading component pack plugins...",
                             this);
      if(!bindComponentPacks())
       mxm::terminal("failed to load any component packs!",
                     this);
   }
   
   mxm::smart<mxmStringList>
    component_ids = enumerateComponents();
   mxm::sendStatusMessage(mxm::StatusMessage,
                          mxmString("framework successfully initialized, ")
                           + "with " + ComponentList.size()
                           + " components installed : "
                           + component_ids->concatenate(", "),
                          this);
   int num_anonymous = ComponentList.size() - component_ids->size();
   if(num_anonymous)
    mxm::sendStatusMessage(mxm::WarningMessage,
                           mxmString("there are ")
                            + num_anonymous
                            + " anonymous components!",
                           this);
}


mx::Framework::~Framework() {
   
   // manually delete component singletons before unloading component packs...
   ComponentList.clear();
   
   // unload component packs (in reverse ordering...)
   while(ComponentPackEntry
          *entry = static_cast<ComponentPackEntry *>
                    (LoadedComponentPacks.removeItemFromHead()))
    delete entry;
         // TODO : maybe check whether or not there are still components around,
         //        and if so, inhibit unloading?
}


int mx::Framework::numComponents() {
   
   return(ComponentList.size());
}


/*!
 *  If less ids are returned than components are reported to be installed by
 *  \ref numComponents(), that means some of the installed components are
 *  anonymous - exactly as many as the observed difference.
 */
mxm::smart<mxmStringList> mx::Framework::enumerateComponents() {
   
   mxm::smart<mxmStringList> component_ids = new mxmStringList();
   
   mxmList::Iterator iter = ComponentList.iterator();
   while(ComponentEntry
          *entry = static_cast<ComponentEntry *>(iter.nextItem())) {
      
      mxm::smart<mxmStringList>
       id_hint = entry->spComponent->enumerateCapabilities(IdHintKey);
      if(   !id_hint.null()
         && (id_hint->size() == 1))
       component_ids->add((*id_hint)[0]);
   }
   
   return(component_ids);
}


/*!
 *  \return
 *  The number of components installed from plugins. Note that this number can
 *  be greater than the number of component packs (plugins, shared objects,
 *  dynamic libraries) that have been bound, for component packs can contain
 *  multiple components / component invokers.
 */
int mx::Framework::bindComponentPacks() {
   
   int packs_loaded = 0;
   
#ifdef MX_PLATFORM_WINDOWS   // Windows implementation...
   {
      WIN32_FIND_DATA findFileData;
      HANDLE          hFind;
      
		mxmString pathName = mxmApplication::basePath();
		mxmString searchName = pathName + PLUGIN_SEARCH_NAME;
		pathName = pathName + PLUGIN_PATH_NAME;

      mxm::sendStatusMessage(mxm::StatusMessage,
                             mxmString("searching plugin folder : ") + pathName,
                             this);
      hFind = FindFirstFile(searchName.text(), &findFileData);
      while(hFind != INVALID_HANDLE_VALUE) {
         
			bool bLoad;
			char* pDbg = std::strstr(findFileData.cFileName, "_debug");
#if __MX_DEBUG__
			bLoad = pDbg ? true : false;
#else
			bLoad = pDbg ? false : true;
#endif
			mxmString fileName = pathName + findFileData.cFileName;

         if(bLoad && loadSharedLibrary(fileName))
				packs_loaded++;
         BOOL bFound = FindNextFile(hFind, &findFileData);
         if(!bFound)
				break;
      }
      FindClose(hFind);
   }
   
#else   // unified UNIX / OSX implementation...
   {
      DIR *dirp = opendir(PLUGIN_PATH);
      if(dirp) {
         
         struct dirent *entry;
         while(entry = readdir(dirp)) {
            
            mxmString plugin_filename = mxmString(PLUGIN_PATH)
                                         + "/"
                                         + entry->d_name;
            
            // we have to see to it that the we do not open plugins multiple
            // types due to the ubiquitous symbolic links encoding library
            // versioning information...
            struct stat plugin_file_info;
            if(lstat(plugin_filename.text(), &plugin_file_info)) {
               
               mxm::sendStatusMessage(mxm::WarningMessage,
                                      mxmString("could not stat plugin file ")
                                       + plugin_filename
                                       + ", ignoring...",
                                      this);
               continue;
            }
            if((plugin_file_info.st_mode & S_IFDIR) == S_IFDIR) {
               continue;                                     // ignore directory
            }
            if((plugin_file_info.st_mode & S_IFLNK) == S_IFLNK) {
               continue;                                       // ignore symlink
            }
            if(loadSharedLibrary(plugin_filename))
             packs_loaded++;
         }
         
         closedir(dirp);
      }
   }
#endif  // MX_PLATFORM...
   
   if(!packs_loaded)
    mxm::sendStatusMessage(mxm::WarningMessage,
                           "no plugins have been loaded!",
                           this);
   
   return(packs_loaded);
}


/*!
 *  Usable components get installed, the rest including the mxmList object is
 *  discarded.
 *
 *  \return
 *  The number of components installed from the specified list of
 *  application-context-provided components.
 */
int mx::Framework::useStaticallyLinkedComponents(
                      mxmList *statically_linked_components) {
   
   int installed = 0;
   
   while(mxmObject *obj = statically_linked_components
                           ->removeItemFromHead())      {
      
      IComponent *component = dynamic_cast<IComponent *>(obj);
      if(component) {
         
         ComponentEntry *entry = new ComponentEntry();
         entry->spComponent    = component;
         
         ComponentList.appendItemToTail(entry);
         
         installed++;
      }
      else {
         
         mxm::sendStatusMessage(mxm::WarningMessage,
                                mxmString("dropping non-component object of "
                                           "type \"")
                                 + mxm::rttiClassName(obj)
                                 + "\" from application-provided list!",
                                this);
         delete obj;
      }
   }
   
   delete statically_linked_components;
   
   return(installed);
}


bool mx::Framework::loadSharedLibrary(const mxmString &filename) {
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          mxmString("binding component pack ")
                           + filename + "...",
                          this);
   
   // ---------- load dynamic library... ----------
   
   ComponentSingletonEnumeratorFunction enumerator_func = 0;
   ComponentSingletonCreatorFunction    creator_func    = 0;
   
#ifdef MX_PLATFORM_WINDOWS
   {
      HINSTANCE hComponent = ::LoadLibrary(filename.text());
      if(hComponent == NULL) {
         
         DWORD err = ::GetLastError();
         char errtxt[300];
         std::sprintf(errtxt,
                      "failed to load component pack %s, error_code=%d\n",
                      filename.text(), err);
         ::MessageBox(NULL, errtxt, "Internal Error", MB_OK | MB_ICONSTOP);
      }
      else {
         
         enumerator_func = (ComponentSingletonEnumeratorFunction)
                            ::GetProcAddress(hComponent, "enumerateComponents");
         creator_func    = (ComponentSingletonCreatorFunction)
                            ::GetProcAddress(hComponent, "createComponent");
         
         if(   enumerator_func
            && creator_func)   {
            
            // register the loaded shared library...
            ComponentPackEntry *entry = new ComponentPackEntry();
            entry->Filename           = filename;
            entry->DLLHandle          = hComponent;
            
            LoadedComponentPacks.prependItemToHead(entry);
         }
         else {
            
            FreeLibrary(hComponent);
         }
      }
   }
   
#else
   {
      
      void *so_handle  = dlopen(filename.text(), RTLD_NOW
                                                  | RTLD_GLOBAL);
                 // RTDL_NOW :    if something's wrong, we rather want to know
                 //               now, instead of crashing later...
                 // RTDL_GLOBAL : needed on LINUX to get dlopen() and RTTI work
                 //               together properly
      if(so_handle) {
         
         enumerator_func = (ComponentSingletonEnumeratorFunction)
                            dlsym(so_handle, "enumerateComponents");
         creator_func    = (ComponentSingletonCreatorFunction)
                            dlsym(so_handle, "createComponent");
         
         if(   enumerator_func
            && creator_func)   {
            
            // register the loaded shared library...
            ComponentPackEntry *entry = new ComponentPackEntry();
            entry->Filename           = filename;
            entry->SharedObjectHandle = so_handle;
            
            LoadedComponentPacks.prependItemToHead(entry);
         }
         else {
            
            dlclose(so_handle);
         }
      }
   }
#endif
   
   if(   !enumerator_func
      || !creator_func)   {
      
      // we already cleaned up our mess!
      
      mxm::sendStatusMessage(mxm::WarningMessage,
                             mxmString("failed to load component pack ")
                              + filename + "!",
                             this);
      return(false);
   }
   
   // ok, dynamic library loaded and bound successfully.
   
   // ---------- instantiate and register component singletons... ----------
   
   mxmStringList* pComponents = enumerator_func();
   for(int i = 0; i < pComponents->size(); i++) {

      mx::IComponent* pComponentInterface
                       = creator_func((*pComponents)[i].text());
      
      ComponentEntry *pEntry = new ComponentEntry();
      pEntry->spComponent    = pComponentInterface;
      
      ComponentList.appendItemToTail(pEntry);
      
      mxmString component_type = "singleton";
      mxm::smart<IComponentInvoker> invoker = pEntry->spComponent;
      if(!!invoker)
       component_type = "multi-instance";
      mxmString component_id = "(anonymous)";
      mxm::smart<mxmStringList>
       id_hint = pComponentInterface->enumerateCapabilities(IdHintKey);
      if(   !id_hint.null()
         && (id_hint->size() == 1))
       component_id = mxmString("\"") + (*id_hint)[0] + "\"";
      mxm::smart<mxmStringList>
       interfaces = pComponentInterface->enumerateInterfaces();
      mxm::sendStatusMessage(mxm::StatusMessage,
                             mxmString("installed new ")
                              + component_type
                              + " component " + component_id
                              + " providing "
                              + interfaces->size()
                              + " interfaces: "
                              + interfaces->concatenate(", "),
                             this);
   }
   delete pComponents;
   
   return(true);
}


// TODO Ok, this is where all the intelligence will have to go!
mx::EErrorCode 
 mx::Framework::provideComponent(const mx::Framework
                                        ::ComponentDescriptor &cd,
                                 mxm::smart<mx::IComponent> &spInterface,
                                 bool &instantiated_new)                  {
   
   // TODO : when we implement support for multithreaded scenarios, the
   //        returned smart handles will have to get installed a lock!
   
   spInterface      = 0;   // assume da worst...
   instantiated_new = false;
   
   // build list of candidates...
   mxmList candidates;
   mxmList::Iterator itComponents = ComponentList.iterator();
   while(ComponentEntry 
          *pEntry = static_cast<ComponentEntry*>(itComponents.nextItem())) {
      
      mxm::smart<IComponent> component = pEntry->spComponent;
      
      bool match = true;
      ComponentDescriptor::Iterator iter = cd.iterator();
      while(mxmPerlStyleHash::KeyValuePair
             *pair = static_cast<mxmPerlStyleHash::KeyValuePair *>
                      (iter.nextItem()))                           {
         
         mxmString                 required_interface    = *pair->key();
         mxm::smart<mxmStringList> required_capabilities = pair->value()
                                                            ->tokenize(",");
         if(   !component->enumerateInterfaces()
                 ->contains(required_interface)) {
            
            match = false;
            break;
         }
         if(!required_capabilities
              ->isSubsetOf(*component
                             ->enumerateCapabilities(required_interface))) {
            
            match = false;
            break;
         }
      }
      
      if(match) {
         
         ComponentEntry *new_entry = new ComponentEntry();
         
         new_entry->spComponent = component;
         
         candidates.appendItemToTail(new_entry);
      }
   }
   if(candidates.size() > 1)
    mxm::sendStatusMessage(mxm::StatusMessage,
                           mxmString(candidates.size())
                            + " candidate components for request <"
                            + cd.concatenate() + ">",
                           this);
   
   // apply "obsoletes" relation...
   itComponents = candidates.iterator();
   while(ComponentEntry 
          *pEntry = static_cast<ComponentEntry*>(itComponents.nextItem())) {
      
      mxm::smart<IComponent> component = pEntry->spComponent;
      
      mxm::smart<mxmStringList>
       obsoleted_components
        = component->enumerateCapabilities(ObsoletesHintKey);
      if(   !obsoleted_components.null()
         && obsoleted_components->size()) {
         
         mxmStringList::Iterator iter = candidates.iterator();
         while(ComponentEntry *entry = static_cast<ComponentEntry *>
                                        (iter.nextItem()))           {
            
            mxm::smart<mxmStringList>
             id = entry->spComponent->enumerateCapabilities(IdHintKey);
            if(   !id
               || (id->size() != 1)) continue;
            
            if(obsoleted_components->contains((*id)[0])) {
              
              mxmString superceding_component;
              mxm::smart<mxmStringList>
               superceding_id = component->enumerateCapabilities(IdHintKey);
              if(   !superceding_id.null()
                 || (superceding_id->size() == 1))
               superceding_component = (*superceding_id)[0];
              else
               superceding_component = "<anonymous>";
              
              mxm::sendStatusMessage(
                      mxm::StatusMessage,
                      mxmString("component \"")
                       + (*id)[0]
                       + "\" is obsoleted by component \""
                       + superceding_component
                       + "\"",
                      this);
              
              entry->RuledOut = true;
            }
         }
      }
   }
   
   // deliver component...
   itComponents = candidates.iterator();
   while(ComponentEntry 
          *pEntry = static_cast<ComponentEntry*>(itComponents.nextItem())) {
      
      if(pEntry->RuledOut) continue;
      
      mxm::smart<IComponent> component = pEntry->spComponent;
      
      mxm::smart<IComponentInvoker> invoker = component;
      if(!invoker) {
         
         spInterface = component;
      }
      else {
         
         spInterface = invoker->newComponent();
         instantiated_new = true;
      }
      
      mxm::sendStatusMessage(mxm::StatusMessage,
                             mxmString("delivered component for request <")
                              + cd.concatenate() + ">",
                             this);
      
      return(mx::eOK);
   }
   
   mxm::sendStatusMessage(mxm::WarningMessage,
                          mxmString("failed to satisfy component request for <")
                           + cd.concatenate() + ">",
                          this);
   
   return(mx::eInterfaceNotSupportedError);
}



/*!
 *  If the desired interface is implemented by a <i>singleton
 *  component</i>, a handle to the singleton is returned. If, on the other
 *  hand, there is a <i>component generator</i> for the desired interface,
 *  a new component object is created and a handle to it is returned.
 *
 *  \note <tt>[khe]</tt> I consider the name \ref getComponentInterface()
 *        misleading, for this is the method an application context
 *        uses to obtain a component, or even to create a new component
 *        instance. The fact that the return value is typed as interface
 *        is an implementational detail.
 *
 *  \param cd
 *  Component description, allowing to specify multiple required interfaces
 *  together with their required capability modifiers.
 *
 *  \param spInterface
 *  smart pointer to the interface if successful, <tt>null</tt>-handle
 *  otherwise
 *
 *  \return
 *  If successful, <tt>eOK</tt> is returned, see file <tt>mx_error_codes.h</tt>
 *  for other possible values. Otherwise, <tt>spInterface</tt> is set to a
 *  <tt>null</tt> handle and an appropriate error code is returned.
 */
mx::EErrorCode 
 mx::Framework::getComponentInterface(const mx::Framework
                                             ::ComponentDescriptor &cd,
                                      mxm::smart<mx::IComponent> &spInterface) {
   
   bool instantiated_new;
   
   return(framework()->provideComponent(cd,
                                        spInterface,
                                        instantiated_new));
}


mx::EErrorCode 
 mx::Framework::getComponentInterface(const char* InterfaceId,
                                      mxm::smart<mx::IComponent> &spInterface,
                                      const mxmStringList &Capabilities) {
   
   ComponentDescriptor cd;
   cd.addRequiredInterface(InterfaceId,
                           Capabilities);
   
   return(getComponentInterface(cd,
                                spInterface));
}


mx::EErrorCode 
 mx::Framework::getComponentInterface(const char* InterfaceId,
                                      mxm::smart<mx::IComponent> &spInterface,
                                      const mxmString &Capability) {
   
   mxmStringList capabilities;
   if(Capability.length()) capabilities.add(Capability);
   
   return(getComponentInterface(InterfaceId,
                                spInterface,
                                capabilities));
}


mxm::smart<mx::IComponent>  mx::Framework
                             ::getComponent(const ComponentDescriptor &cd) {
   
   mxm::smart<IComponent> component;
   if(getComponentInterface(cd, component) != mx::eOK)
    mxm::terminal(mxmString("failed to satisfy component request for <")
                   + cd.concatenate() + ">",
                  framework());
   
   return(component);
}


mxm::smart<mx::IComponent> mx::Framework
                            ::getComponent(const mxmString &interface_id,
                                           const mxmStringList
                                                  &interface_capabilities) {
   
   ComponentDescriptor cd;
   cd.addRequiredInterface(interface_id,
                           interface_capabilities);
   
   return(getComponent(cd));
}


mxm::smart<mx::IComponent>  mx::Framework
                             ::getComponent(const mxmString &interface_id,
                                            const mxmString
                                                   &interface_capability) {
   
   mxmStringList capabilities;
   if(interface_capability.length()) capabilities.add(interface_capability);
   
   return(getComponent(interface_id,
                       capabilities));
}


/*!
 *  You need to use this dumb pointer method version in case you handle
 *  component ownership yourself. For example this applies to components that
 *  are <tt>Qt</tt> widgets as these will most likely be linked in as children
 *  of other widgets that then assume ownership. <b>Get this right or you will
 *  crash!</b>
 *
 *  \param selection_flags
 *  Specify \ref HarmlessSelectionFailures in case you don't want failures to
 *  look up the requested component to be terminal, but to return <tt>0</tt>
 *  instead.
 */
mx::IComponent *mx::Framework
                 ::newComponentInstance(const ComponentDescriptor &cd,
                                        mxm::flags selection_flags)    {
   
   mxm::smart<IComponent> component;
   bool mostly_harmless = selection_flags & HarmlessSelectionFailures;
   bool instantiated_new;
   
   if(framework()
       ->provideComponent(cd, component, instantiated_new) != mx::eOK) {
      
      if(mostly_harmless)
       return(0);
      else
       mxm::terminal(mxmString("failed to satisfy component request for <")
                      + cd.concatenate() + ">",
                     framework());
   }
   if(!instantiated_new) {
      
      mxmString txt = "can't create another instance of a singleton-type "
                       "component!";
      if(mostly_harmless) {
         mxm::sendStatusMessage(mxm::WarningMessage, txt, framework());
         return(0);
      }
      else {
         mxm::terminal(txt, framework());
      }
   }
   
   // release new component instance from smart handle control...
   IComponent *dumb_component_ptr = &*component;
   component.releaseSmartlyHandledObject();
   
   return(dumb_component_ptr);
}


mx::IComponent *mx::Framework
                 ::newComponentInstance(const mxmString &interface_id,
                                        const mxmStringList
                                               &interface_capabilities,
                                        mxm::flags selection_flags)     {
   ComponentDescriptor cd;
   cd.addRequiredInterface(interface_id,
                           interface_capabilities);
   
   return(newComponentInstance(cd,
                               selection_flags));
}


mx::IComponent *mx::Framework
                 ::newComponentInstance(const mxmString &interface_id,
                                        const mxmString
                                               &interface_capability,
                                        mxm::flags selection_flags) {
   
   mxmStringList capabilities;
   if(interface_capability.length()) capabilities.add(interface_capability);
   
   return(newComponentInstance(interface_id,
                               capabilities,
                               selection_flags));
}


mx::Framework *mx::Framework::framework() {
   
   if(!s_pFramework)
    mxm::terminal("no mx::Framework instance present!");
   
   return(s_pFramework);
}


mx::Framework
 ::ComponentPackEntry::~ComponentPackEntry() {
   
   mxm::sendStatusMessage(mxm::StatusMessage,
                          mxmString("unloading component pack ")
                           + Filename,
                          this);
   
#ifdef MX_PLATFORM_WINDOWS
   FreeLibrary(DLLHandle);
#else
   {
      char txt[80];
      std::sprintf(txt, "%x", (unsigned int)SharedObjectHandle);
      mxm::sendStatusMessage(mxm::DebugMessage,
                             mxmString("calling dlclose(0x")
                              + txt + ")...",
                             this);
      dlclose(SharedObjectHandle);
   }
#endif
}


void mx::Framework::ComponentDescriptor
      ::addRequiredInterface(const mxmString &interface_id,
                             const mxmStringList &interface_capabilities) {
  
  mxmStringList caps;
  mxmStringList::Iterator iter = interface_capabilities.iterator();
  while(mxmString *cap = static_cast<mxmString *>(iter.nextItem())) {
    
    if(*cap == "")
     continue;
    if(cap->subString(","))
     mxm::terminal("capability identifiers must not contain comma characters!",
                   this);
    
    caps.add(*cap);
  }
  caps.removeDuplicates();
  
  (*this)[interface_id] = caps.concatenate(",");
}


void mx::Framework::ComponentDescriptor
      ::addRequiredInterface(const mxmString &interface_id,
                             const mxmString &interface_capability) {
   
   mxmStringList capabilities;
   
   if(interface_capability.length()) capabilities.add(interface_capability);
   
   addRequiredInterface(interface_id, capabilities);
}


mx::Framework::ComponentEntry::ComponentEntry() {
   
   RuledOut = false;
}
