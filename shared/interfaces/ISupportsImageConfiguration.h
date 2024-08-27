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
//  $LastChangedDate:  $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsImageConfiguration.h $
//
///////



#ifndef __MX_ISUPPORTSIMAGECONFIGURATION_H__
#define __MX_ISUPPORTSIMAGECONFIGURATION_H__



#define MX_ISUPPORTSIMAGECONFIGURATION_ID "ISupportsImageConfiguration"
        


#include "IInterface.h"
#include "IExposureWindow.h"
#include "../mx_types.h"
#include "../mx_error_codes.h"
#include "../mx_enum_config.h"

#include <mxm/core/mxmString.h>
#include <mxm/core/mxmList.h>

#include <vector>

namespace mx {


  //! Allows to configure image control settings.
  /*!
   *  \ingroup mxsdk_interfaces
   */
class ISupportsImageConfiguration : virtual IInterface {

public:
	//@{
	/** Fetch/set/add exposure image user-defined window definitions.*/
	/*virtual bool						isExposureWindowSupported( )								= 0;
	virtual IExposureWindow			createExposureWindow(	float x, 
																			float y, 
																			float width, 
																			float height )					= 0;

	virtual bool						addExposureWindow(IExposureWindow* window)			= 0;
	virtual void						removeExposureWindow(IExposureWindow* window)		= 0;

	virtual mxm::smart<mxmList>	exposureWindows( )											= 0;
	virtual void						clearExposureWindows()										= 0;
	virtual void						resetExposureWindows()										= 0;
	//@}
	*/
	//@{
	/** Set exposure mode.																					*/
	virtual EExposureWindowMode	exposureWindowMode()											= 0;
	virtual EErrorCode				setExposureWindowMode( EExposureWindowMode mode )	= 0; 
	//@}

	//@{
	/** Everthing you need in order to define the exposure window weight in reference to the rest of the image. 
	    Caution: Values != [0,1] map to whole image for exposure. 
	*/
	virtual bool						isExposureWindowWeightSupported()						= 0;
	virtual float						exposureWindowWeight()										= 0;
	virtual EErrorCode				setExposureWindowWeight( float percentage )			= 0; 
	//@}

	//@{
	/** Everthing you need in order to define the white balance window weight in reference to the rest of the image. 
	    Caution: Values != [0,1] map to whole image for exposure. 
	*/
	virtual bool						isWhiteBalanceWindowWeightSupported()								= 0;
	virtual float						whiteBalanceWindowWeight()									= 0;
	virtual EErrorCode				setWhiteBalanceWindowWeight( float percentage )		= 0; 
	//@}

	//@{
	/**  Average Brightness Window value																	*/
	virtual bool						isAverageBrightnessSupported()							= 0;
	virtual float						averageBrightness()											= 0;
	virtual EErrorCode				setAverageBrightness( float percentage )				= 0;
	virtual std::vector<float>		averageBrightnessValRange()								= 0;
	//@}

	//@{
	/**  Exposure value max/min																	*/
	virtual bool						isExposureRangeSupported()									= 0;
	virtual float						exposureRangeMin()											= 0; //!< Return value in msec
	virtual float						exposureRangeMax()											= 0; //!< Return value in msec
	virtual EErrorCode				setExposureRangeMin( float miliSec )					= 0;
	virtual EErrorCode				setExposureRangeMax( float miliSec )					= 0;
	virtual std::vector<float>		exposureRangeValRange()										= 0;
	//@}

	//@{
	/**  If support by a given model set/fetch night improvement mode.							*/
	virtual bool						isNightImprovementSupported()								= 0;
	virtual EErrorCode				setNightImprovement(ENightImproveMode mode)			= 0; 
	virtual ENightImproveMode		nightImprovement()											= 0; //!< See ENightImproveMode
	virtual u32							nightImprovementValRange()									= 0;
	//@}

	//@{
	/** If support by a given model set/fetch f -stop number									*/
	virtual bool						isFStopNumberSupported()									= 0;
	virtual float						fStopNumber()													= 0; //!< aperture fraction
	virtual EErrorCode				setFStopNumber( float fStop )								= 0;
	virtual std::vector<float>		fStopNumberValRange( )										= 0;
	//@}

	//@{
	/** Fetch/set the camera image resolution														*/
	virtual bool						isResolutionSupported()										= 0;
	virtual EImageResolution		resolution()													= 0; //!< See EImageResolution
	virtual EErrorCode				setResolution( EImageResolution res )					= 0; 
	virtual u32							resolutionValRange( )										= 0; 
	//@}
	
	//@{
	/** Fetch/set ...															*/
	virtual bool						isStreamFPSSupported()										= 0;
	virtual float						streamFPS()														= 0; //!< frames per sec.
	virtual EErrorCode				setStreamFPS( float fps )									= 0;
	virtual std::vector<float>		streamFPSValRange( )											= 0;
	//@}

	//@{
	/** Fetch/set ...															*/
	virtual bool						isStreamQualitySupported()									= 0;
	virtual float						streamQuality()												= 0; //! Return value [0,1]
	virtual EErrorCode				setStreamQuality( float qa )								= 0; 
	virtual float						streamQualityInterval()										= 0;
	//@}

	//@{
	/** Fetch/set ...															*/
	virtual bool						isStreamCodexSupported()									= 0;
	virtual EStreamCodex				streamCodex()													= 0; //!< See EStreamCodex
	virtual EErrorCode				setStreamCodex( EStreamCodex codex )					= 0; 
	virtual u32							streamCodexValRange( )										= 0;
	//@}

	//@{
	/** Fetch/set ...												*/
	virtual bool						isStreamModeSupported()										= 0;
	virtual EStreamMode				streamMode()													= 0; //!< See EStreamMode
	virtual EErrorCode				setStreamMode( EStreamMode mode )						= 0; 
	virtual u32							streamModeValRange( )										= 0; 
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isNoiseSurppressSupported()								= 0;
	virtual ENoiseSurp				noiseSurppress()												= 0; //!< See ENoiseSurp
	virtual EErrorCode				setNoiseSurppress( ENoiseSurp mode )					= 0;
	virtual u32							noiseSurppressValRange( )									= 0;
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isAutoContrastSupported()									= 0;
	virtual EContrastMode			autoContrast()													= 0; //!< See EContrastMode
	virtual u32							autoContrastValRange()										= 0;
	virtual EErrorCode				setAutoContrast( EContrastMode mode )					= 0; 
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isWhiteBalanceSupported()									= 0;
	virtual EWhiteBMode				whiteBalance()													= 0; //!< See EWhiteBMode
	virtual EErrorCode				setWhiteBalance( EWhiteBMode mode )						= 0;
	virtual u32							whiteBalanceValRange( )										= 0;
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isSharpenSupported()											= 0;
	virtual float						sharpen()														= 0; //!< Return value range [0,1]
	virtual EErrorCode				setSharpen( float val )										= 0;
	virtual float						sharpenInterval( )											= 0;
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isBrightnessSupported()										= 0;
	virtual float						brightness()													= 0;//!< Return value range [-1,1]
	virtual EErrorCode				setBrightness( float val )									= 0;
	virtual float						brightnessInterval( )										= 0;
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isBacklightSupported()										= 0;
	virtual float						backlight()														= 0;//!< Return value range [-1,1]
	virtual EErrorCode				setBacklight( float val )									= 0;
	virtual float						backlightInterval( )											= 0;
	//@}

	//@{
	/** Fetch/set 	...														*/
	virtual bool						isSaturationSupported()										= 0;
	virtual float						saturation()													= 0;//!< Return value range [-1,1]
	virtual EErrorCode				setSaturation( float val )									= 0;
	virtual float						saturationInterval( )										= 0;
	//@}
	





};


};



#endif   // __MX_ISUPPORTSIMAGECONFIGURATION_H__
