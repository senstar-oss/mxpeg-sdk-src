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
//  $Author: dgr $
//  $LastChangedBy: dgr $
//  $LastChangedDate: 2007-06-06 15:32:44 +0200 (Wed, 06 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsRecordingConfiguration.h $
//
///////



#ifndef __MX_ISUPPORTSRECORDINGCONFIGURATION_H__
#define __MX_ISUPPORTSRECORDINGCONFIGURATION_H__



#define MX_ISUPPORTSRECORDINGCONFIGURATION_ID "ISupportsRecordingConfiguration"



#include "IInterface.h"
#include "../mx_types.h"
#include "../mx_error_codes.h"
#include "../mx_enum_config.h"

#include <mxm/core/mxmString.h>


namespace mx {

  //! Allows to configure alarm recording settings.
  /*!
   *  \ingroup mxsdk_interfaces
   */
class ISupportsRecordingConfiguration : virtual IInterface {

public:

	/*! get the supported recording modes
	 *  possible recording modes are OR'ed
	 */
	virtual u32 supportedRecordingModes() = 0;
	/*! set record mode attribute
	 */
	virtual EErrorCode setRecordingMode(enum ERecordingMode eMode) = 0;
	/*! get record mode attribute
	 *  It is an error trying to get this attribute before
	 *  retrieve is called for this section
	 */
	virtual ERecordingMode recordingMode() = 0;

	virtual bool isRecordingFramerateSupported() = 0;

	virtual EErrorCode setRecordingFramerate(float framerate) = 0;

	virtual float recordingFramerate() = 0;

	virtual bool isPackedStorySupported() = 0;

	virtual EErrorCode setPackedStory(bool enable) = 0;

	virtual bool packedStory() = 0;

	virtual bool isAudioRecordingSupported() = 0;

	virtual EErrorCode setAudioRecording(bool enable) = 0;

	virtual bool audioRecording() = 0;

	virtual u32 supportedRecordingSwitches() = 0;
	/*! set recording attribute
	 */
	virtual EErrorCode setRecordingSwitch(ERecordingSwitch eSwitch) = 0;
	/*! get recording attribute
	 *  It is an error trying to get this attribute before
	 *  retrieve is called for this section
	 */
	virtual ERecordingSwitch recordingSwitch() = 0;
	virtual bool isRecordingSwitchSupported() = 0;

	virtual u32 supportedGeneralArmingSwitches() =0;
	/*! set general arming attribute
	 */
	virtual EErrorCode setGeneralArmingSwitch(ERecordingSwitch eSwitch) = 0;

	/*! get general arming attribute
	 *  It is an error trying to get this attribute before
	 *  retrieve is called for this section
	 */
	virtual ERecordingSwitch generalArmingSwitch() = 0;
	virtual bool isGeneralArmingSwitchSupported() = 0;
	
	/*! additional recording time before the event occurred.
	 *  The time span is measured in milliseconds
	 */
	virtual bool isPreRecordingTimeSupported() = 0;

	virtual EErrorCode setPreRecordingTime(u32 uTimeMs) = 0;

	virtual u32 preRecordingTime() = 0;

	/*! additional recording time after the event occurred.
	 *  The time span is measured in milliseconds
	 */
	virtual bool isPostRecordingTimeSupported() = 0;

	virtual EErrorCode setPostRecordingTime(u32 uTimeMs) = 0;

	virtual u32 postRecordingTime() = 0;

	/*! frame rate during the post recording time span
	 */
	virtual bool isEventFramerateSupported() = 0;

	virtual EErrorCode setEventFramerate(float framerate) = 0;

	virtual float eventFramerate() = 0;

	virtual u32 supportedRecordingEvents() = 0;

	virtual u32 activeRecordingEvents() = 0;

	virtual u32 recordingEvents() = 0;

	virtual EErrorCode setRecordingEvents(u32 recordingEvents) = 0;

  /*! returns <tt>true</tt> is the camera device supports storage 
   *  to external devices
	*/
	virtual bool isExternalStorageSupported() = 0;
	/* possible recording targets OR'ed. See enum ERecordingTargets
	 * for possible targets
	 */
	virtual u32 supportedRecordingTargets() = 0;

	virtual EErrorCode setRecordingTarget(ERecordingTarget target) = 0;
	virtual ERecordingTarget recordingTarget() = 0;

	virtual EErrorCode setStorageServerIP(const mxmString& ip) = 0;
	virtual mxmString storageServerIP() = 0;

	virtual EErrorCode setRemoteDirectory(const mxmString& dir) = 0;
	virtual mxmString remoteDirectory() = 0;

	virtual EErrorCode setRemoteUser(const mxmString& user) = 0;
	virtual mxmString remoteUser() = 0;

	virtual EErrorCode setRemotePassword(const mxmString& password) = 0;
	virtual mxmString remotePassword() = 0;
	/*! set the storage size in MB. If storage size is unlimited, this
	 *  setting will be ignored
	 */
	virtual EErrorCode setStorageSizeMB(u32 size) = 0;
	virtual u32 storageSizeMB() = 0;
	virtual EErrorCode setStorageSizeUnlimited(bool unlimited) = 0;
	virtual bool storageSizeUnlimited() = 0;

	virtual EErrorCode setTimeToKeep(u32 days) = 0;
	virtual u32 timeToKeep() = 0;
	virtual EErrorCode setTimeToKeepUnlimited(bool unlimited) = 0;
	virtual bool timeToKeepUnlimited() = 0;

	virtual EErrorCode setNumberOfEvents(u32 size) = 0;
	virtual u32 numberOfEvents() = 0;
	virtual EErrorCode setNumberOfEventsUnlimited(bool unlimited) = 0;
	virtual bool numberOfEventsUnlimited() = 0;

};


};



#endif   // __MX_ISUPPORTSRECORDINGCONFIGURATION_H__
