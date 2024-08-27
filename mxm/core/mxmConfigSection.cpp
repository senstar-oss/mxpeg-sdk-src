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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmConfigSection.cpp $
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



#include <mxm/core/mxmConfigSection.h>
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxm_generic_stuff.h>
#include <mxm/core/mxm_macros.h>

#ifdef MX_PLATFORM_CAMERA
  #include <mxm/basic/mxmPrivilegedSection.h>
  extern "C" {
    #include <mxsystem/mx_filenames.h>
    #include <mxsystem/systool.h>
  }
#endif

#include <cstdio>

#ifndef MX_PLATFORM_WINDOWS
  #include <unistd.h>
#endif



extern "C" const char *mx_libmxmodularMX_version(void);



const char *mxmConfigSection::LegacyProfileStringKey
             = "_legacy_profile_string_";
const char *mxmConfigSection::WhitespaceCharacters
             = mxm::WhitespaceCharacters;

const mxm::flags mxmConfigSection::HeaderTrailerNone     =  1;
const mxm::flags mxmConfigSection::HeaderLibBuildDate    =  2;
const mxm::flags mxmConfigSection::HeaderSectionName     =  4;
const mxm::flags mxmConfigSection::HeaderSectionOwner    =  8;
const mxm::flags mxmConfigSection::HeaderTimestamp       = 16;
const mxm::flags mxmConfigSection::TrailerProfileSummary = 32;
const mxm::flags mxmConfigSection::SingleLineProfiles    = 64;



mxmConfigSection::mxmConfigSection() {
  
  SectionName  = "_unspecified_";
  SectionOwner = "_unspecified_";
  
  CaseInsensitiveKeys = true;
  if(CaseInsensitiveKeys) OwnKeys.setCaseInsensitiveKeysMode(true);
  IsInternal = false;
}



/*!
 */
void mxmConfigSection::setName(const mxmString &section_name) {
  
  SectionName = section_name;
}


#ifdef MX_PLATFORM_CAMERA
/*!
 *  The owner of the configuration section. Can be written to the configuration
 *  file when appropriate \ref HeaderTrailer flags are passed into save().
 */
void mxmConfigSection::setOwner(const mxCameraService_t owner) {
  
  SectionOwner = mxCameraServiceNames[owner];
}
#endif


/*!
 *  The owner of the configuration section. Can be written to the configuration
 *  file when appropriate \ref HeaderTrailer flags are passed into save().
 */
void mxmConfigSection::setOwner(mxmString const &section_name) {
  
  SectionOwner = section_name;
}


/*!
 *  Internal mode defaults to <tt>off</tt>.
 *
 *  Only affects generation of correct path to configfile.
 */
void mxmConfigSection::setInternalMode(bool enable) {
  
  IsInternal = enable;
}


/*!
 *  If <tt>profile_type_ids</tt> is non-<tt>0</tt>, any string in this array
 *  is used as <i>profile type identifier</i>, enabling alternate parsing of
 *  input lines having a regular key matching one of those ids.
 *
 *  If <tt>allow_multi_line</tt> is set to <tt>true</tt>, the method will allow
 *  lines that span multiple text file lines separated by backslashes.
 *
 *  \return
 *  If anything goes wrong, the config section RAM object is cleared of all keys
 *  and profiles and <tt>false</tt> is returned. Otherwise, the method returns
 *  <tt>true</tt>.
 */
bool mxmConfigSection::load(const mxmStringList &config_section_file,
                            const char **profile_type_ids,
                            bool allow_multi_line) {
  
  mxmString line, left, tmp_txt,
            key, value;
  const char **type_ids;
  
  OwnKeys.clear();       // jettison all potentially present keys
  ProfileSets.clear();   // jetison all profiles
  
  mxmStringList::Iterator iter = config_section_file.iterator();
  while(mxmString *current_line = static_cast<mxmString *>(iter.nextItem())) {
    
    line = *current_line;
    
    if(allow_multi_line) {
      
      for(;;) {
        
        int len = line.length();
        if(!len) break;
        
        if(line.text()[len - 1] == '\\') {
          
          line.setLength(len - 1);   // _0_ ok
          
          mxmStringList::Iterator peek_iter = iter;
          mxmString *next_line = static_cast<mxmString *>(peek_iter.nextItem());
          if(!next_line) break;
          
          iter = peek_iter;
          line += *next_line;
        }
      }
    }
    
    // strip comment and do primary key/value split...
    line.split(left, tmp_txt, "#");
    left.split(key, value, "=");
    key.trim(WhitespaceCharacters);
    
    if(key.length()) {
      
      // check whether we have to enable alternate parsing for a profile
      // line...
      if(profile_type_ids) {
        
        type_ids = profile_type_ids;
        while(*type_ids) {
          
          if(CaseInsensitiveKeys) {
            if(mxmString::areEqualIgnoringCase(key, *type_ids)) break;
          }
          else {
            if(key == *type_ids) break;
          }
          
          type_ids++;
        }
        if(*type_ids) {
          
          // OK, we've got a profile line...
          
          line = value;
          
          // split off profile name...
          line.split(left, tmp_txt, ":");
          left.trim(WhitespaceCharacters);
          
          if(left.length()) {
            
            mxmPerlStyleHash &profile_hash = profile(key, left);
            
            // is it a legacy profile string?
            if(tmp_txt.characterPosition('=') == -1)
             profile_hash[LegacyProfileStringKey] = tmp_txt;
            else
             profile_hash.mergeKeysFromString(tmp_txt, ":", "=",
                                              WhitespaceCharacters);
          }
        }
        else {
          value.trim(WhitespaceCharacters);
          OwnKeys[key] = value;
        }
      }
      else {
        value.trim(WhitespaceCharacters);
        OwnKeys[key] = value;
      }
    }
  }
  
  return(true);
}


#ifdef __MX_CONFIGSECTION_LOADSAVE
/*!
 *  If <tt>profile_type_ids</tt> is non-<tt>0</tt>, any string in this array
 *  is used as <i>profile type identifier</i>, enabling alternate parsing of
 *  input lines having a regular key matching one of those ids.
 *
 *  If <tt>allow_multi_line</tt> is set to <tt>true</tt>, the method will allow
 *  lines that span multiple text file lines separated by backslashes.
 *
 *  If you specify a <tt>filename_override</tt>, it is this file that is
 *  accessed instead of the one implicitly composed from the section name.
 *
 *  \return
 *  If anything goes wrong, the config section RAM object is cleared of all keys
 *  and profiles and <tt>false</tt> is returned. Otherwise, the method returns
 *  <tt>true</tt>.
 */
bool mxmConfigSection::load(const mxmString &section_name,
                            const char **profile_type_ids,
                            bool allow_multi_line,
                            const mxmString &filename_override) {
  
  std::FILE *config_file = 0;
  bool eof_encountered;
  mxmString line, left, tmp_txt,
            key, value;
  const char **type_ids;
  mxmString dir;
  mxmString filename;
  
  OwnKeys.clear();       // jettison all potentially present keys
  ProfileSets.clear();   // jetison all profiles
  
  SectionName = section_name;
  
  // attempt to open config section file...
#ifdef MX_PLATFORM_CAMERA
  if(IsInternal)
   dir = MX_CONFIGFILE_INTERNAL_DIR;
  else
   dir = MX_CONFIGFILE_DIR;
  filename = dir + "/" + SectionName;
#else
   dir = ".";
   filename = SectionName + ".mx_config_section";
#endif
  if(filename_override.length())
   filename = filename_override;
  config_file = std::fopen(filename.text(), "r");
  if(!config_file) return(false);
  
  do {
    
    eof_encountered = line.readLine(config_file, 0, allow_multi_line);
    
    // strip comment and do primary key/value split...
    line.split(left, tmp_txt, "#");
    left.split(key, value, "=");
    key.trim(WhitespaceCharacters);
    
    if(key.length()) {
      
      // check whether we have to enable alternate parsing for a profile
      // line...
      if(profile_type_ids) {
        
        type_ids = profile_type_ids;
        while(*type_ids) {
          
          if(CaseInsensitiveKeys) {
            if(mxmString::areEqualIgnoringCase(key, *type_ids)) break;
          }
          else {
            if(key == *type_ids) break;
          }
          
          type_ids++;
        }
        if(*type_ids) {
          
          // OK, we've got a profile line...
          
          line = value;
          
          // split off profile name...
          line.split(left, tmp_txt, ":");
          left.trim(WhitespaceCharacters);
          
          if(left.length()) {
            
            mxmPerlStyleHash &profile_hash = profile(key, left);
            
            // is it a legacy profile string?
            if(tmp_txt.characterPosition('=') == -1)
             profile_hash[LegacyProfileStringKey] = tmp_txt;
            else
             profile_hash.mergeKeysFromString(tmp_txt, ":", "=",
                                              WhitespaceCharacters);
          }
        }
        else {
          value.trim(WhitespaceCharacters);
          OwnKeys[key] = value;
        }
      }
      else {
        value.trim(WhitespaceCharacters);
        OwnKeys[key] = value;
      }
    }
  } while(!eof_encountered);
  
  std::fclose(config_file);
  return(true);
}



/*!
 *  \warning The method is currently not fully implemented!!!
 *
 *  <tt>format_style</tt> controls the configuration file format and
 *  which meta information is included as comments.
 *
 *  The config section is first written to a temporary file, then renamed -
 *  unless suppressed by passing <tt>true</tt> for <tt>dont_use_tempfile</tt>.
 *  If the config section was a non-internal one, the change to it is signalled
 *  in a system-wide fashion.
 *
 *  You can also ask the config section to be saved to an arbitrary location by
 *  specifying a <tt>filename_override</tt>. In this case, no change will be
 *  signalled. The settings concerning the temporary file still apply, however.
 *
 *  \return
 *  <tt>false</tt> in case of a write error. The config section RAM image
 *  remains unaffected, the temporary file is removed. If the method was
 *  instructed not to use any temporary file, it is the target file that is
 *  deleted. In case of success, the method returns <tt>true</tt>.
 */
bool mxmConfigSection::save(mxm::flags format_style,
                            const mxmString &filename_override,
                            bool dont_use_tempfile)             {
  
  std::FILE *config_file = 0;
  mxmPerlStyleHash::KeyValuePair *key_value_pair;
  ProfileSet *profile_set;
  int i,
      num, profile_num, profile_type_num;
  mxmString dir;
  mxmString filename,
            filename_to_open;
  
  if(IsInternal)
   dir = MX_CONFIGFILE_INTERNAL_DIR;
  else
   dir = MX_CONFIGFILE_DIR;
  if(filename_override.length())
   filename = filename_override;
  else
   filename = dir + "/" + SectionName;
  if(dont_use_tempfile)
   filename_to_open = filename;
  else
   filename_to_open = filename + ".tmp";
  
  // init counters...
  num              = 0;
  profile_num      = 0;
  profile_type_num = 0;
  
  mxmList::Iterator          iter;
  mxmPerlStyleHash::Iterator key_iter; 
  
  config_file = std::fopen(filename_to_open.text(), "w");
  if(!config_file) return(false);

  if(format_style & HeaderSectionName)
   fputs((mxmString("## MOBOTIX Camera Software, "
                     "configuration file, section \"")
           + SectionName + "\"\n").text(),
         config_file);
  
  if(   (format_style & HeaderSectionOwner)
     || (format_style & HeaderTimestamp))   {
    
    mxmString txt = "# written";
    if(format_style & HeaderSectionOwner)
     txt += " by " + SectionOwner;
    if(format_style & HeaderTimestamp)
     txt += " at " + mxm::timeISO();
    txt += "\n"; 
    fputs(txt.text(), config_file);
  }
  
  if(format_style & HeaderLibBuildDate)
   fputs((mxmString("## generated by modularMX, build ")
           + mx_libmxmodularMX_version()
           + "\n").text(), config_file);
  
  // write out profiles...
  iter = ProfileSets.iterator();
  while((profile_set = static_cast<ProfileSet *>(iter.nextItem())) != 0) {
    
    for(i = 0; i < profile_set->size(); i++) {
      
      Profile       &profile = profile_set->getProfile(i);
      mxmPerlStyleHash &keys = profile.keys();
      mxmPerlStyleHash::Iterator another_key_iter;
      mxmPerlStyleHash::KeyValuePair *pair;
      bool first_key;
      
      std::fprintf(config_file,
                   "%s=%s:",
                   profile_set->profileTypeId().text(),
                   profile.name().text());
      
      first_key = true;
      another_key_iter = keys.iterator();
      while((pair = static_cast<mxmPerlStyleHash::KeyValuePair *>
                     (another_key_iter.nextItem())) != 0)              {
        
        if(!first_key) {
          if(format_style & SingleLineProfiles)
           std::fprintf(config_file, ":");
          else
           std::fprintf(config_file, "\\\n : ");
        }
        else if(!(format_style & SingleLineProfiles)) {
          std::fprintf(config_file, "\\\n   ");
        }
        
        std::fprintf(config_file,
                     "%s=%s",
                     pair->key()->text(),
                     pair->value()->text());
        
        first_key = false;
        
        num++;
      }
      
      std::fprintf(config_file, "\n");
      
      profile_num++;
    }
    
    profile_type_num++;
  }
  
  // write out own keys...
  key_iter = OwnKeys.iterator();
  while((key_value_pair = static_cast<mxmPerlStyleHash::KeyValuePair *>
                           (key_iter.nextItem())) != 0)                 {
    
    std::fprintf(config_file,
                 "%s=%s\n",
                 key_value_pair->key()->text(),
                 key_value_pair->value()->text());
    
    num++;
  }
  if(format_style & TrailerProfileSummary) {
    std::fprintf(config_file,
                 "## %d keys, %d profiles, %d profile types.\n",
                 num, profile_num, profile_type_num);
  }
  
  bool gotta_problem = false;
  if(std::ferror(config_file)) gotta_problem = true;
  if(std::fclose(config_file)) gotta_problem = true;
  if(   (filename_to_open != filename)
     && rename(filename_to_open.text(),
               filename.text()))
   gotta_problem = true;
  if(gotta_problem) {
    unlink(filename_to_open.text());
    return(false);
  }
  
  if(   !IsInternal
     && !filename_override.length()) announceChange();
  
  return(true);
}
#endif


void mxmConfigSection::clear() {
  
  OwnKeys.clear();
  ProfileSets.clear();
}


/*!
 *  If returned string is <tt>null</tt>-string, the queried key is not present.
 *
 *  Simply relays the call to the repective operator of the encapsuled
 *  mxmPerlStyleHash holding the config section's own keys. Consult that
 *  operator's documetnation for details.
 */
mxmString &mxmConfigSection::operator[](const mxmString &key) {
  
  return(OwnKeys[key]);
}

mxmPerlStyleHash & mxmConfigSection::getKeys(void) {
  return(OwnKeys);
}

/*!
 */
mxmPerlStyleHash &mxmConfigSection::profile(const mxmString &profile_type_id,
                                            const mxmString &profile_name)    {
  
  return(profileSet(profile_type_id).addorgetProfile(profile_name).keys());
}


/*!
 *  The name of the profile found is stored in profile_name.
 */
mxmPerlStyleHash &mxmConfigSection::profile(const mxmString &profile_type_id,
                                            int const profile_index,
                                            mxmString &profile_name) const    {
  
  ProfileSet *da_profile_set = lookupProfileSet(profile_type_id);

  if(da_profile_set) {

    Profile &p = da_profile_set->getProfile(profile_index);
    
    profile_name = p.name();
    
    return(p.keys());
  }
  
  mxm::terminal("invalid profile type.",
                this);
}


/*!
 *  The config section retains ownership over the profile.
 */
mxmPerlStyleHash
 *mxmConfigSection::lookupProfile(const mxmString &profile_type_id,
                                  const mxmString &profile_name) const {
  
  ProfileSet *da_profile_set = lookupProfileSet(profile_type_id);
  if(da_profile_set) {
    Profile *da_profile = da_profile_set->lookupProfile(profile_name);
    if(da_profile)
     return(&(da_profile->keys()));
    else
     return(0);
  }
  else {
    return(0);
  }
}


/*!
 */
mxm::smart<mxmStringList>
      mxmConfigSection::enumerateProfiles(const mxmString &profile_type_id) {
  
  mxm::smart<mxmStringList> profile_list;
  ProfileSet *profile_set = lookupProfileSet(profile_type_id);
  
  if(profile_set) {
    profile_list = profile_set->enumerateProfiles();
  }
  else {
    profile_list = new mxmStringList();
    MXM_CHECK_NULL(profile_list);
  }
  
  return(profile_list);
}


void mxmConfigSection::deleteProfile(const mxmString &profile_type_id,
                                     const mxmString &profile_name)    {
  
  ProfileSet *profile_set = lookupProfileSet(profile_type_id);
  if(profile_set) {
    
    mxmList::Iterator iter = profile_set->iterator();
    while(Profile *profile = static_cast<Profile *>(iter.nextItem())) {
      
      if(profile->name() == profile_name) {
        
        delete iter.removeCurrentItem();
        return;
      }
    }
  }
}

void mxmConfigSection::moveProfileToTop(const mxmString &profile_type_id,
                                        const mxmString &profile_name)    {
  
  ProfileSet *profile_set = lookupProfileSet(profile_type_id);
  if(profile_set)
   profile_set->moveProfileToTop(profile_name);
}


bool mxmConfigSection::renameProfile(mxmString const & profile_type_id,
                                     mxmString const & profile_name_old,
                                     mxmString const & profile_name_new) {
  
  if (profile_name_old.length() == 0 || profile_name_new.length() ==0)
    return false;

  ProfileSet *profile_set = lookupProfileSet(profile_type_id);

  if(profile_set) {
    
    mxmList::Iterator iter = profile_set->iterator();
    while(Profile *profile = static_cast<Profile *>(iter.nextItem())) {
      
      if(profile->name() == profile_name_old) {
        profile->setName(profile_name_new);
        return true;
      }
    }
  }
  return false;
}


/*!
 *  In case the profile set is present, and <tt>profile_iterator</tt> is given,
 *  an iterator for the respective profiles is copied to this location. This
 *  is an mxmList::Iterator for a list holding mxmConfigSection::Profile
 *  objects.
 */
int mxmConfigSection
     ::profileSetSize(const mxmString &profile_type_id,
                      mxmList::Iterator *profile_iterator) const {
  
  ProfileSet *da_profile_set;
  
  da_profile_set = lookupProfileSet(profile_type_id);
  
  if(da_profile_set) {
    if(profile_iterator)
     *profile_iterator = da_profile_set->iterator();
    return(da_profile_set->size());
  }
  else {
    return(-1);
  }
}


mxmConfigSection::ProfileSet &mxmConfigSection::profileSet(
                                const mxmString &type_id
                              )                             {
  
  ProfileSet *da_profile_set = lookupProfileSet(type_id);
  
  if(da_profile_set) {
    return(*da_profile_set);
  }
  else {
    da_profile_set = new ProfileSet(type_id, CaseInsensitiveKeys);
    MXM_CHECK_NULL(da_profile_set);
    ProfileSets.appendItemToTail(da_profile_set);
    return(*da_profile_set);
  }
}


void mxmConfigSection::deleteProfileSet(const mxmString &profile_type_id) {
  
  mxmList::Iterator profile_set_iterator;
  ProfileSet *da_profile_set;
  
  da_profile_set = lookupProfileSet(profile_type_id,
                                    &profile_set_iterator);
  
  if(da_profile_set) {
    delete profile_set_iterator.removeCurrentItem();
  }
}


/*!
 */
void mxmConfigSection
      ::purgeValueCharacters(const mxmString &key,
                             const mxmString &characters_to_purge) {
  
  mxmList::Iterator iter;
  ProfileSet *set;
  int i;
  
  set = 0;
  iter = ProfileSets.iterator();
  while((set = static_cast<ProfileSet *>(iter.nextItem())) != 0) {
    
    for(i = 0; i < set->size(); i++) {
      
      Profile &profile               = set->getProfile(i);
      mxmPerlStyleHash &profile_hash = profile.keys();
      
      profile_hash.purgeValueCharacters(key, characters_to_purge);
    }
  }
  
  OwnKeys.purgeValueCharacters(key, characters_to_purge);
}


/*!
 *  Prints out the current config section RAM image (to <tt>stdout</tt>).
 */
void mxmConfigSection::dump(std::FILE *outfile) {
  
  mxmList::Iterator iter;
  ProfileSet *set;
  int i;
  mxmPerlStyleHash::Iterator key_iter;
  mxmPerlStyleHash::KeyValuePair *pair;
  
  std::fprintf(outfile, "mxmConfigSection@%x dumping contents...\n",
                        (unsigned int)this);
  
  std::fprintf(outfile, "%d profile sets\n", ProfileSets.size());
  
  set = 0;
  iter = ProfileSets.iterator();
  while((set = static_cast<ProfileSet *>(iter.nextItem())) != 0) {
    
    std::fprintf(outfile, "profile set for type \"%s\", # of profiles=%d\n",
                          set->profileTypeId().text(), set->size());
    
    for(i = 0; i < set->size(); i++) {
      
      Profile &profile               = set->getProfile(i);
      mxmPerlStyleHash &profile_hash = profile.keys();
      mxmPerlStyleHash::Iterator iter;
      
      std::fprintf(outfile, "  profile name \"%s\", # of key_value_pairs=%d\n",
                            profile.name().text(), profile.size());
      
      iter = profile_hash.iterator();
      while((pair = static_cast<mxmPerlStyleHash::KeyValuePair *>
                                 (iter.nextItem())) != 0)              {
        std::fprintf(outfile, "    %s=%s\n", pair->key()->text(),
                                             pair->value()->text());
      }
    }
  }
  
  std::fprintf(outfile, "------ %d own keys ------\n", OwnKeys.size());
  
  key_iter = OwnKeys.iterator();
  while((pair = static_cast<mxmPerlStyleHash::KeyValuePair *>
                 (key_iter.nextItem())) != 0)                      {
    std::fprintf(outfile, "  %s=%s\n", pair->key()->text(),
                                       pair->value()->text());
  }
}


mxmConfigSection::ProfileSet
 *mxmConfigSection::lookupProfileSet(const mxmString &type_id,
                                     mxmList::Iterator *profile_set_iterator)
                     const                                                    {
  
  mxmList::Iterator iter;
  ProfileSet *da_profile_set;
  
  iter = ProfileSets.iterator();
  while((da_profile_set = static_cast<ProfileSet *>(iter.nextItem())) != 0) {
    
    if(CaseInsensitiveKeys) {
      if(mxmString::areEqualIgnoringCase(da_profile_set->profileTypeId(),
                                         type_id)) break;
    }
    else {
      if(da_profile_set->profileTypeId() == type_id) break;
    }
  }
  
  if(   da_profile_set
     && profile_set_iterator)
   *profile_set_iterator = iter;
  
  return(da_profile_set);
}


mxmConfigSection::Profile::Profile(const mxmString &profile_name,
                                   bool case_insensitive_keys) {
  
  Name = profile_name;
  
  if(case_insensitive_keys) Keys.setCaseInsensitiveKeysMode(true);
}


const mxmString &mxmConfigSection::Profile::name(void) { 
  
  return(Name);
}

void mxmConfigSection::Profile::setName(mxmString const & new_name) {
  Name = new_name;
}



mxmPerlStyleHash &mxmConfigSection::Profile::keys(void) {
  
  return(Keys);
}


int mxmConfigSection::Profile::size(void) {
  
  return(Keys.size());
}


mxmConfigSection::ProfileSet::ProfileSet(const mxmString &type_id,
                                         bool case_insensitive_keys) {
  
  ProfileTypeId = type_id;
  CaseInsensitiveKeys = case_insensitive_keys;
}


const mxmString &mxmConfigSection::ProfileSet::profileTypeId(void) {
  
  return(ProfileTypeId);
}


/*!
 */
mxmConfigSection::Profile &mxmConfigSection::ProfileSet::addorgetProfile(
                             const mxmString &profile_name
                           )                                      {
  
  Profile *da_profile = lookupProfile(profile_name);
  
  if(da_profile) {
    return(*da_profile);
  }
  else {
    da_profile = new Profile(profile_name, CaseInsensitiveKeys);
    MXM_CHECK_NULL(da_profile);
    Profiles.appendItemToTail(da_profile);
    return(*da_profile);
  }
}


mxmConfigSection::Profile &mxmConfigSection::ProfileSet::getProfile(
                             int const profile_index
                           )                                      {
  
  mxmList::Iterator iter;
  Profile *da_profile;
  int i;
  
  da_profile = 0;
  iter = Profiles.iterator();
  for(i = 0; i <= profile_index; i++) {
    da_profile = static_cast<Profile *>(iter.nextItem());
    if(!da_profile) break;
  }
  if(da_profile)
   return(*da_profile);
  else
   mxm::terminal("profile index out of range.",
                 this);
}


mxmConfigSection::Profile
 *mxmConfigSection::ProfileSet::lookupProfile(const mxmString &profile_name) {

  mxmList::Iterator iter;
  Profile *da_profile;
  
  iter = Profiles.iterator();
  while(da_profile = static_cast<Profile *>(iter.nextItem()))
   if(da_profile->name() == profile_name) return(da_profile);
  
  return(0);
}


mxmStringList *mxmConfigSection::ProfileSet::enumerateProfiles(void) {
  
  mxmStringList *profile_list;
  mxmList::Iterator iter;
  Profile *profile;
  
  profile_list = new mxmStringList();
  MXM_CHECK_NULL(profile_list);
  
  iter = Profiles.iterator();
  while(profile = static_cast<Profile *>(iter.nextItem())) {
    mxmString *new_name = new mxmString(profile->name());
    MXM_CHECK_NULL(new_name);
    profile_list->appendItemToTail(new_name);
  }
  
  return(profile_list);
}


void mxmConfigSection::ProfileSet
      ::moveProfileToTop(const mxmString &profile_name) {
  
  mxmList::Iterator iter = Profiles.iterator();
  
  while(Profile *da_profile = static_cast<Profile *>(iter.nextItem()))
   if(da_profile->name() == profile_name) {
     
     iter.removeCurrentItem();
     Profiles.prependItemToHead(da_profile);
     return;
   }
}


mxmList::Iterator mxmConfigSection::ProfileSet::iterator(void) {
  
  return(Profiles.iterator());
}

int mxmConfigSection::ProfileSet::size(void) {
  
  return(Profiles.size());
}


/*!
 *  If the config section is an internal one, the method does nothing.
 */
void mxmConfigSection::announceChange(void){
  
#ifdef MX_PLATFORM_CAMERA
  if(!IsInternal) {
        
    char *args[] = { MXM_CONFIGFILE_CONFIGFLASHER,
                     "changed",
                     (char *)SectionName.text(),   // stays valid long enough
                     0 };
    
    mx_system(args);
  }
#endif
}

