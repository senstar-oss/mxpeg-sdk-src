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
//  $LastChangedDate: 2007-06-29 16:03:10 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmConfigSection.h $
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



#ifndef __MXM_CONFIGSECTION_H__
#define __MXM_CONFIGSECTION_H__



#include <mxm/core/mxmPerlStyleHash.h>
#include <mxm/core/mxm_generic_stuff.h>

#ifdef MX_PLATFORM_CAMERA
  extern "C" {
    #include <mxsystem/mx_camera_services.h>
  }
#endif



/*! 
 *  \class mxmConfigSection mxmConfigSection.h mxmodularMX/mxmConfigSection.h
 *
 *  \brief In-RAM representation of a camera configuration file section
 *  featuring convenient access methods.
 *
 *  \ingroup modularMX_core
 *
 *  In order to be able to read in and process <tt>MX</tt> camera configuration
 *  files from a <tt>C++</tt> application, a convenient and sufficiently
 *  comfortable mechanism was needed. This class is <i>it</i>.
 *
 *  The class was also introduced to work towards a unification of configuration
 *  file format and access. Sadly, as of May 08, 2006, there are numerous
 *  variants of configuration file formats in use on <tt>MX</tt> cameras. As a
 *  result, the class' parser was designed to be configurable, so that it can
 *  actually deal with most of them. However, it is most desirable to have a
 *  unified file format for camera configuration, so maybe we will continually
 *  switch the older files to the new, unified format.
 *
 *  <b>Configuration File Structure</b>
 *
 *  The abstract conceptional structure of the camera configuration file is as
 *  follows.
 *  - The complete configuration is actually segmented into so-called <b>config
 *    sections</b> that are currently stored in individual files under
 *    <tt>/etc/config</tt>. There is an alternate path of
 *    <tt>/var/lib/config</tt>, where so-called <b>internal config sections</b>
 *    are stored that share the same file format (eclecticism) but serve dynamic
 *    communications purposes between camera services. To access the latter, the
 *    class can be switched to internal mode via setInternalMode().
 *  - Configuration information is stored in form of string-valued
 *    <b>key/value pairs</b>.
 *  - Key/value pairs are either <b>direct members</b> of the config section, or
 *    alternatively grouped into <b>profiles</b>.
 *  - Within one config section, there can be profiles of multiple <b>profile
 *    types</b>.
 *  - Within each key/value pair group, be it the config section's own set of
 *    direct keys, or any of the profiles, <b>keys are genuine</b>.
 *  - Keys are treated <b>case-insensitively</b>. Consequently, this affects
 *    <ul>
 *      <li>the config section's own keys
 *      <li>profile type identifiers
 *      <li>the keys in each individual profile
 *    </ul>
 *    However, upcoming standardization stipulates <b>keys to be lower-case</b>.
 *    To also correctly process legacy files, the class' current implementation
 *    can also be switched to case-sensitive keys mode.
 *
 *  \author <tt>[khe]</tt> Kai Hergenroether
 */
class MXM_CORE_API mxmConfigSection : public mxmObject {
 
 public:
  static const char *LegacyProfileStringKey;
  static const char *WhitespaceCharacters;
  
  /*!
   *  \name Format Style Flags
   *  When saving the configuration to a file using \ref save(), you can control
   *  the format and have various information written to the
   *  configuration file as comments.
   *  These flags describe how control the configuration file format.
   */
  //@{
    //! No comments.
    static const mxm::flags HeaderTrailerNone;
    //! Library build date.
    static const mxm::flags HeaderLibBuildDate;
    //! Section name, set with \ref setName() or with constructor
    //! \ref mxmConfigSection().
    static const mxm::flags HeaderSectionName;
    //! Section owner, set with \ref setOwner().
    static const mxm::flags HeaderSectionOwner;
    //! Time of writing of configuration file.
    static const mxm::flags HeaderTimestamp;
    //! Internal debugging data.
    static const mxm::flags TrailerProfileSummary;
    //! Write Profiles in single lines (instead of multiple, which is
    //! the default).
    static const mxm::flags SingleLineProfiles;

  //@}
  
 public:
  //! Represents a single profile within a config section.
  /*!
   *  \ingroup modularMX_core
   */
  class Profile : public mxmObject {
   private:
    mxmString Name;
    mxmPerlStyleHash Keys;
   public:
    Profile(const mxmString &profile_name, bool case_insensitive_keys);
   public:
    const mxmString &name(void);
    void setName(mxmString const & new_name);
    mxmPerlStyleHash &keys(void);
    int size(void);
  };
 private:
  class ProfileSet : public mxmObject {
   private:
    mxmString ProfileTypeId;
    mxmList Profiles;
    bool CaseInsensitiveKeys;
   public:
    ProfileSet(const mxmString &type_id, bool case_insensitive_keys);
   public:
    const mxmString &profileTypeId(void);
    //! This method adds the profile, if not already present.
    Profile &addorgetProfile(const mxmString &profile_name);
    //! Does not add missing profile.
    Profile &getProfile(int const profile_index);
    //! Returns <tt>0</tt> if specified profile is not present.
    Profile *lookupProfile(const mxmString &profile_name);
    //! Returns the list of profiles currently stored in the profile set.
    mxmStringList *enumerateProfiles(void);
    //! Moves the specified profile to the beginning of the set.
    void moveProfileToTop(const mxmString &profile_name);
    //! Returns an interator for the profiles stored in the set.
    mxmList::Iterator iterator(void);
    //! Tells how many profiles are currently stored in the set.
    int size(void);
  };
  
 private:
  mxmString SectionName;
  mxmString SectionOwner;
  mxmPerlStyleHash OwnKeys;
  mxmList ProfileSets;
  bool CaseInsensitiveKeys;
  bool IsInternal;
  void *StableABIDataExtension;
  
 public:
  mxmConfigSection();
  
 public:
  //! Renames the config section.
  void setName(const mxmString &section_name);
#ifdef MX_PLATFORM_CAMERA
  //! Sets owner of the config section.
  void setOwner(const mxCameraService_t owner);
#endif
  //! Sets owner of the config section.
  void setOwner(const mxmString &section_owner);
  //! Specifies whether or not the config section is an internal one (alternate
  //! directory in filesystem).
  void setInternalMode(bool enable);
  //! Loads the config section from a string list holding a config section file
  //! in memory.
  bool load(const mxmStringList &config_section_file,
            const char **profile_type_ids = 0,
            bool allow_multi_line = false);
/*
  //! Loads the specified config section from the file system.
  bool load(const mxmString &section_name,
            const char **profile_type_ids = 0,
            bool allow_multi_line = false,
            const mxmString &filename_override = "");
  //! Saves the config section RAM image to the file system under the section
  //! name last used for loading or set via setName().
  bool save(mxm::flags header_style = HeaderLibBuildDate
                                       | HeaderSectionName
                                       | TrailerProfileSummary,
            const mxmString &filename_override = "",
            bool dont_use_tempfile = false);
*/
  //! Deletes all key/value pairs, those in profiles as well as the direct ones.
  void clear();
  //! In all profiles, and in the config section's set of own direct keys, this
  //! method visits all key/value pairs mathing the key specified, and removes
  //! from their value strings the specified characters.
  void purgeValueCharacters(const mxmString &key,
                            const mxmString &characters_to_purge);
  //! For testing purposes.
  void dump(std::FILE *outfile = stdout);
  
  //! Returns a reference to the value for one of the config section's own keys.
  mxmString &operator[](const mxmString &key);
  //! Returns a reference to the list of key / value pairs (w/o profile data).
  mxmPerlStyleHash & getKeys(void);
  //! The profile and respective profile set are transparently created if not
  //! already present.
  mxmPerlStyleHash &profile(const mxmString &profile_type_id,
                            const mxmString &profile_name);
  //! Returns the profile referenced through profile_type_id and
  //! profile_index (starting with 0).
  mxmPerlStyleHash &profile(const mxmString &profile_type_id,
                            int const profile_index,
                            mxmString &profile_name) const;
  //! Returns the specified profile stored in the config section, or <tt>0</tt>
  //! in case such is not present.
  mxmPerlStyleHash *lookupProfile(const mxmString &profile_type_id,
                                  const mxmString &profile_name) const;
  //! Returns the list of currently present profiles for the given profile type. 
  mxm::smart<mxmStringList> enumerateProfiles(const mxmString
                                                     &profile_type_id);
  //! Removes the specified profile if present.
  void deleteProfile(const mxmString &profile_type_id,
                     const mxmString &profile_name);
  //! Rename the specified profile if present and return true on success.
  bool renameProfile(mxmString const & profile_type_id,
                     mxmString const & profile_name_old,
                     mxmString const & profile_name_new);
  //! Returns the number of profiles in the specified profile set, or
  //! <tt>-1</tt> if the set is not present.
  int profileSetSize(const mxmString &profile_type_id,
                     mxmList::Iterator *profile_iterator = 0) const;
  //! Removes the profile set for the specified profile type (if present).
  void deleteProfileSet(const mxmString &profile_type_id);
  //! Move the specified profile to the beginning of its profile set.
  void moveProfileToTop(const mxmString &profile_type_id,
                        const mxmString &profile_name);
  
 private:
  //! This method adds the profile set if not already present.
  ProfileSet &profileSet(const mxmString &type_id);
  //! Private helper method, returns <tt>0</tt> if the profile set is not
  //! present.
  ProfileSet *lookupProfileSet(const mxmString &type_id,
                               mxmList::Iterator *profile_set_iterator = 0)
              const;
  //! Announces change to the config section in a system-wide fashion.
  void announceChange(void);
};



#endif   // __MXM_CONFIGSECTION_H__

