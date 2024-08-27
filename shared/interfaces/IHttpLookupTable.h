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
//  $LastChangedDate: 2007-07-02 12:33:34 +0200 (Mon, 02 Jul 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IHttpLookupTable.h $
//
///////



/*!
	\file Interface IHttpLookupTableComponent

	\author AGu
*/



#ifndef __MX_IHTTPLOOKUPTABLE_H__
#define __MX_IHTTPLOOKUPTABLE_H__



#define MX_IHTTPLOOKUPTABLE_ID "IHttpLookupTable"



// interface includes
#include "IInterface.h"

// global includes
#include <mx_error_codes.h>
#include <mx_enum_config.h>
// SDK includes
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmString.h>
#include <mxm/core/mxmStringList.h>

namespace mx {


/*!
 *  \ingroup mxsdk_interfaces
 *
 *  Via this interface calling contexts can get strings, it can send to the camera
 *  to request the actual settings or set them in the camera. The called context
 *  can parse the buffers returned by the camera and hold the settings in an internal
 *  state. This state is polled by the calling context afterwards.
 *  If the calling context wants to set the settings on the camera, it sets the 
 *  indiviual attributes via <tt>setIdValue</tt>. These attributes are stored in an 
 *  internal state in the called context until <tt>allSettingsUpdateString</tt>
 *  is called. After this call the internal state is reset.
 */
class IHttpLookupTable : public virtual IInterface {
   
public:

	/*!
		\enum eHTTPLookUpTableIds

		\brief Used as an ID for accessing the HTTPLookUpTable.

		Each ID corresponds with an http-table entry. A entry corresponds
		to an HTTP-Request string.
	*/
	enum EHttpLookupTableId {
		
		ID_NOID							= 0,	//!< No valid ID
		
		ID_PROP_LIST_ALL				= 101,
		ID_PROP_LIST_UPDATE			= 102,
		ID_PROP_MANUFAKTUR			= 103,	//!< Identifies HTTP-String used for requesting the camera manufacturer
		ID_PROP_MODEL					= 104,	//!< Identifies HTTP-String used for requesting the camera model
		ID_PROP_SOFT_VER				= 105,
		ID_PROP_RESO_VAL_RANGE		= 106,	//!< Identifies HTTP-String used for requesting the resolution value range
		ID_PROP_PTZ						= 107,	//!< Token for finding hardware PTZ property!
		ID_PROP_PTZ_SOFT				= 108,	//!< Token for finding software PTZ property!		
		ID_RINFO_BASIC					= 150,	//!< Mobo specific Http-String for fetching general information about a cam			

		ID_VIDEO_MJPEG					= 201,	//!< Identifies HTTP-String used for requesting a mjpeg stream

		ID_BRIGHTNESS					= 301,	//!< Connected to a atomic HTTP-String bonded to brightness
		ID_SHARPNESS					= 302,	//!< Connected to a atomic HTTP-String bonded to sharpness
		ID_RESOLUTION					= 303,	//!< Connected to a atomic HTTP-String bonded to resolution
		ID_SATURATION					= 304,	//!< Connected to a atomic HTTP-String bonded to saturation
		ID_BACKLIGHT					= 305,	//!< Connected to a atomic HTTP-String bonded to backlight
		ID_CONTRAST						= 306,	//!< Connected to a atomic HTTP-String bonded to contrast
		ID_FRAMES						= 307,	//!< Connected to a atomic HTTP-String bonded to frames
		ID_JPEGQ							= 308,	//!< Connected to a atomic HTTP-String bonded to jpeg quality
		ID_NOISE							= 309,	//!< Connected to a atomic HTTP-String bonded to noise reduction
		ID_AUTO_CONTRAST				= 310,	//!< Connected to a atomic HTTP-String bonded to auto contrast
		ID_WHITE_BALANCE				= 311,	//!< Connected to a atomic HTTP-String bonded to white balance
		ID_NIGHT_IMPROV				= 312,	//!< Connected to a atomic HTTP-String bonded to night improvement
		ID_F_STOP						= 313,	//!< Connected to a atomic HTTP-String bonded to exposure F-Stop
		ID_EXPOSURE_MIN				= 315,	//!< Connected to a atomic HTTP-String bonded to exposure time min
		ID_EXPOSURE_MAX				= 316,	//!< Connected to a atomic HTTP-String bonded to exposure time max
		ID_REDUCED_MODE				= 317,	//!< Connected to a atomic HTTP-String bonded to reduced mode
		ID_IMAGE_SETTINGS_ALL		= 318,	//!< Identifies a HTTP-String consisting of all available image settings
		ID_BESTIMAGERESET				= 320,	//!< Identifies a special section of control/control named bestimagereset 
		ID_RESOLUTION_VAL_RANGE		= 323,	//!< Identifies HTTP-String used for requesting the resolution value range
		ID_IMAGESETTINGS_LIST_ALL	= 324,
		ID_IMAGESETTINGS_UPDATE		= 325,
		ID_STREAMCODEX					= 326,
		ID_CAM_EYE_MODE				= 327,
		
		ID_REC_SWITCH					= 401,	//!< Arm Recording": "Enable", "Off", "SI Open (high)", "SI Closed (low)"
		ID_REC_LIST_ALL				= 408,	//!< Http-Request string for listing the recording config section
		ID_REC_UPDATE					= 409,	//!< Contains the section for updating
		ID_REC_TIMETABLE				= 410,	//!< The current timetable (can be empty!) e.g. Weekdays_Mo-Fr
		ID_REC_FREEZE_ACTIVE			= 411,	//!< Terminate Recording (TR)
		ID_REC_FREEZE_ALERTMASK		= 412,	//!< Termination Condition -Select the events which will terminate recording
		ID_REC_FREEZE_DELAY			= 413,	//!< Time Before Terminate
		ID_REC_RECORDING_MODE		= 414,	//!< Recording Mode": "Snap Shot Recording", "Event Recording", "Continuous Recording"
		ID_REC_STORYPACKING			= 415,	//!< Pack Event Story
		ID_REC_STREAM_FRAMERATE		= 416,	//!< Continuous Recording Frame Rate 
		ID_REC_STREAM_AUDIO			= 417,	//!< Record Audio Data
		ID_REC_START_MASK			   = 418,	//!< Start Recording
		ID_REC_STORY_INTERVAL		= 419,	//!< Recording interval for story images in milliseconds [0..600000 ms]
		ID_REC_ANTE_COUNT				= 420,	//!< Pre-alarm images for event story
		ID_REC_POST_COUNT				= 421,	//!< Post-alarm images for event story.
		ID_REC_STREAM_EVENT_FPS_100= 422,	//!< Recording speed if an event is detected, in frames per second.
		ID_REC_STREAM_ANTE_TIME		= 423,	//!< Additional recording time before an event stream.
		ID_REC_STREAM_TIME			= 424,	//!< Recording Time": "2 s", "4 s", "6 s", ..., 10 min
		ID_REC_STRETCH_MASK			= 425,	//!< Retrigger Recording
		ID_REC_STOP_MASK				= 426,	//!< Stop Recording
		ID_REC_STORY_STOP_COUNT		= 427,	//!< Images to include in recorded snap shots after Stop Recording event has occurred.
		ID_REC_STREAM_STOP_TIME		= 428,	//!< Time to include in recorded stream after Stop Recording event has occurred. "2 s", "4 s", "6 s", ..., 10 min
		ID_REC_STOP_DEAD_TIME		= 429,	//!< Recording Dead Time
		ID_REC_HISTORY_SEC			= 430,	//!< History Recording (HR)
		ID_REC_HISTORY_IMG_PROF		= 431,	//!< Image Profile for History": "Live Image", "MEGA", "VGA", "CIF"

		ID_FSERVER_LIST_ALL			= 450	,	//!< List fileserversettings
		ID_FSERVER_UPDATE				= 451	,	//!< Requestring for setting FS valus on a cam.
		ID_FSERVER_TYPE				= 452 ,  //!< Recording Target: None, SMB, NFS, CIFS
		ID_FSERVER_REC_SERVER		= 453	,  //!< Server IP
		ID_FSERVER_REC_PATH			= 454	,	//!< Remote Directory
		ID_FSERVER_WIN_USERNAME		= 457	,  //!< The username!
		ID_FSERVER_WIN_PASSWD		= 458	,	//!< The passwd!
		ID_FSERVER_REC_SIZE			= 459,	//!< Max size in MB
		ID_FSERVER_REC_AGE			= 460,	//!< Max age of sequence in dayes
		ID_FSERVER_REC_COUNT			= 461,	//!< NbOf Sequences
		ID_FSEVER_UID					= 462,	//!< User ID for NFS-Server (default:0)
		ID_FSEVER_GID					= 463,	//!< Group ID for NFS-Server (default:65534)
		ID_FSERVER_PLEX_FS			= 464,	//!< Internal Storage Filesystem"
		ID_FSEVER_LOGMODE				= 465,	//!< Event Storage Logfile": "off", "on"
		ID_FSERVER_COPY_LOGFILES	= 466,	//!< Copy Camera Logfiles": "off", "on"

		ID_EVENT_LIST_ALL				= 700,	//!<	List parameters in the even section
		ID_EVENT_UPDATE				= 701,	//!<  Update parameters request string.
		ID_VM_ON_OFF					= 702,	//!<  Video Motion Enable. Can be found in the event section.
		ID_VM_WINDOW_DEF				= 703,	//!<  Video Motion Definitions (VM)
		ID_VM_WINDOW_ID				= 704,	//!<  Show Video Motion Window ID
		ID_VM_WINDOW_TYPE				= 705,	//!<  Appearance of motion window border
		ID_VM_NIGHT_MODE				= 706,	//!<	Dark Video Motion Suppress
		ID_VM2_ON_OFF					= 711,	//!<  Video Motion Enable
		ID_VM2_WINDOW_DEF				= 712,	//!<  Video Motion Definitions (VM)
		ID_VM2_WINDOW_ID				= 713,	//!<  Show Video Motion Window ID
		ID_VM2_WINDOW_TYPE			= 714,	//!<  Appearance of motion window border
		ID_VM2_NIGHT_MODE				= 715,	//!<	Dark Video Motion Suppress
		ID_ES_MICRO_CHECK				= 716,	//!<  "Microphone (MI)"
		ID_ES_SIGNAL_IMPUT_CHECK	= 717,	//!<	Signal Input (SI)
		ID_ES_SIGNAL_IMPUT2_CHECK	= 718,	//!< 	Signal Input (SI2)
		ID_ES_SIGNAL_IMPUT3_CHECK	= 719,	//!< 	Signal Input (SI3)
		ID_ES_SIGNAL_IMPUT4_CHECK	= 720,	//!< 	Signal Input (SI4)
		ID_ES_PERIODIC_EVENT_CHECK	= 721,	//!<  "Periodic Event (PE)"
		ID_ES_TIME_TASK_CHECK		= 722,	//!<  "Time Task (TT)"
		ID_ES_USER_CLICK				= 723,	//!<  "User Click (UC)"
		ID_ES_REC_BEGIN_CHECK		= 724,	//!<  "Recording Begin (RB)"
		ID_ES_REC_END_CHECK			= 725,	//!<  "Recording End (RE)"
		ID_ES_REC_HISTORY_CHECK		= 726,	//!<	"Recording History (RH)"
		ID_ES_REC_TERMINATE_CHECK	= 727,	//!<	"Recording Terminate (RT)"
		ID_ES_BUTTONS_CHECK			= 728,	//!<	"Buttons (BT)"
		ID_ES_IP_RECEIVE_CHECK		= 729,	//!<	"IP Receive (RC)"
		ID_ES_TEMPERATURE_CHECK		= 730,	//!<  "Temperature (TP)"
		ID_ES_ILLUMINATION_CHECK	= 731,	//!<	"Illumination (IL)"
		ID_ES_RANDOM_EVENT_CHECK	= 732,	//!<	"Random Event (RD)"
		ID_ES_EVENT_COUNTER_CHECK	= 733,	//!<	"Event Counter (EC)"
		ID_ES_EVENT_LOGIC_CHECK		= 734,	//!<	"Event Logic (EL)"
		ID_ES_EVENT_LOGIC2_CHECK	= 735,	//!<	"Event Logic (EL2)"
		ID_ES_PASSIV_INFRARED_CHECK= 736,	//!<  "Passiv Infrared (PI)"
		ID_ES_COM_IN_CHECK			= 737,	//!<	"COM in (CI)"
		ID_ES_REMOTE_CONTROL_CHECK	= 738,	//!<	"IR remote control (IR)"

		ID_ARMING_MODE					= 801,	//!<	Activate arming mode. Can be found in the event section.

		ID_PTZ_EYE						= 903,	//!<  
		ID_JPG_SZ_X						= 904,	//!<
		ID_JPG_SZ_Y						= 905,	//!<
		ID_WIDTH_RATIO_X				= 906,	//!<
		ID_WIDTH_RATIO_Y				= 907,	//!<
		ID_CENTER_X						= 908,	//!<
		ID_CENTER_Y						= 909,	//!<

		ID_RC_REBOOT					= 1001,	//!<	Reboot the camera.
		ID_RC_STORE_CONFIG			= 1002,	//!<	Store config in camera.

		ID_ACTION_LIST_ALL			= 1100,	//!<  HTTP-Requeststring for control action sections
		ID_ACTION_UPDATE				= 1101,	//!<	Action update 
		ID_AC_VISUAL_ALARM			= 1102,	//!<	enable/disable visual alarm switch

		ID_EXPOSURECONTROL_LIST_ALL= 1200,	//!<	Fetch the camera section which contains exposure informations
		ID_EXPOSURECONTROL_UPDATE	= 1201,	//!<	Update the camera section which contains exposure informations

		ID_PTZ_CONTINUOUS_MOVE		= 1301,	//!<	HTTP-Request for continuous pan/tilt/move actions. 

	};


	/*! get a string which request the camera model when sent to the camera
	 */
	virtual mxmString cameraModelRequestString() = 0;
	/*! parse the buffer returned by the camera and extract the model.
	 *  The model is stored in the calling context. Other methods may depend
	 *  on the model, so it is a good idea to retrieve the model in an early stage.
	 *  If a method depends on the model and it is not stored yet, the method throws
	 *  an <tt>cameraModelUnknownException</tt> exception.
	 */
	virtual mxmString parseBufferForModel(const mxmString& buffer) = 0;
	/*! find out if a certain attribute is supported by this camera model
	 */ 
	virtual bool isSupported(enum EHttpLookupTableId id) = 0;
	/*! get one or more strings which request image settings section of the camera.
	 *  If more than one string is returned, they all must be sent to the camera
	 */
	virtual mxm::smart<mxmStringList> imageSettingsRequestString() = 0;
	/*! get one or more strings which request a ptz command for  settings section of the camera.
	 *  If more than one string is returned, they all must be sent to the camera
	 *
	 */
	virtual mxm::smart<mxmStringList> continuousPTZPanTiltRequestString( mxmString& pan, mxmString& tilt )=0;
	/*! get one or more strings which request recording settings section of the camera.
	 *  If more than one string is returned, they all must be sent to the camera
	 */
	virtual mxm::smart<mxmStringList> recordingSettingsRequestString() = 0;
	/*! get one or more strings which time table image settings section of the camera.
	 *  If more than one string is returned, they all must be sent to the camera
	 */
	virtual mxm::smart<mxmStringList> timetableSettingsRequestString() = 0;
	/*! get one or more strings which request video motion settings section of the camera.
	 *  If more than one string is returned, they all must be sent to the camera
	 */
	virtual mxm::smart<mxmStringList> vmSettingsRequestString() = 0;
	/*! parse the buffer returned by the <tt>imageSettingsRequestString</tt>.
	 *  The result is hold in an internal state.
	 */
	virtual EErrorCode parseImageSettings(const mxmString& buffer) = 0;
	/*! parse the buffer returned by the <tt>recordingSettingsRequestString</tt>.
	 *  The result is hold in an internal state.
	 */
	virtual EErrorCode parseRecordingSettings(const mxmString& buffer) = 0;
	/*! parse the buffer returned by the <tt>timetableSettingsRequestString</tt>.
	 *  The result is hold in an internal state.
	 */
	virtual EErrorCode parseTimetableSettings(const mxmString& buffer) = 0;
	/*! parse the buffer returned by the <tt>vmSettingsRequestString</tt>.
	 *  The result is hold in an internal state.
	 */
	virtual EErrorCode parseVmSettings(const mxmString& buffer) = 0;
	/*! retrieve a value from the internal state. The return value
	 *  must be converted from string to the right data type by the calling context.
	 */
	virtual mxmString idValue(enum EHttpLookupTableId id) = 0;
	/*! set a value to the internal state. 
	 */
	virtual EErrorCode setIdValue(enum EHttpLookupTableId id, const mxmString& value) = 0;
	/*! get one or more strings which sets all settings of all sections, which are set in the
	 *  internal state.
	 *  If more than one string is returned, they all must be sent to the camera
	 */
	virtual mxm::smart<mxmStringList> allSettingsUpdateString(bool bStorePermanent = false) = 0;
	/*! get the range of an attribute
	 */
	virtual mxm::smart<mxmStringList> valueRange(enum EHttpLookupTableId id) = 0;
	/*! get the interval step size of an attribute
	 */
	virtual float valueIntervalStepSize(enum EHttpLookupTableId id) = 0;
	/*! returns the request string, which is used to reboot the camera
	 */
	virtual mxmString rebootRequestString() = 0;
	/*! returns the request string, which is used to store the data permanently in the camera
	 */
	virtual mxmString storeRequestString() = 0;

	//@{
	/**  Set lookup table eye state! */ 
	virtual void			setEye( mx::ECamEye eye ) = 0;
	virtual mx::ECamEye	eye()							  = 0;
	//@}

	//! Fetch camera model name
	virtual mxmString	getModelName()					  = 0;

	/*! returns the reboot status
	 */
	virtual bool needReboot() = 0;

	/*! resets table
	*/
	virtual void reset() = 0;


};


};  // namespace mx



#endif  // __MX_IHTTPLOOKUPTABLE_H__
