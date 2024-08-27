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
//  $LastChangedDate: 2007-03-06 16:42:19 +0100 (Di, 06 Mrz 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsEventConfiguration.h $
//
///////



#ifndef __MX_ISUPPORTSEVENTCONFIGURATION_H__
#define __MX_ISUPPORTSEVENTCONFIGURATION_H__



#define MX_ISUPPORTSEVENTCONFIGURATION_ID "ISupportsEventConfiguration"
        


#include "IInterface.h"
#include "IVideoMotionWindow.h"
#include "../mx_types.h"
#include "../mx_error_codes.h"
#include "../mx_enum_config.h"

#include <mxm/core/mxmString.h>
#include <mxm/core/mxmList.h>


namespace mx {

	//! Window origin lower left corner
	class VideoMotionWindow : public mxmObject {
	
	public:
		VideoMotionWindow(){
			x=y=width=height=area=max_area=0;
			eye=eRightEye;
		}
	
	public:
		int x;
		int y;
		int width;
		int height;
		int area;
		int max_area;

		ECamEye eye;
	};

  //! Allows to configure event control settings.
  /*!
   *  \ingroup mxsdk_interfaces
   */
class ISupportsEventConfiguration : IInterface {

public:
	//@{	
	/** All you need to enable/disable VM1								 */
	virtual bool				isVideoMotion1Supported()		= 0;
	virtual EErrorCode		setVideoMotion1(bool enable)	= 0;
	virtual bool				videoMotion1()						= 0;
	//@}

	//@{	
	/** All you need to enable/disable the visual alarm								 */
	virtual bool				isVisualAlarmSupported() = 0;
	virtual EErrorCode		setVisualAlarm(bool enable) = 0;
	virtual bool				visualAlarm() = 0;
	//@}

	//@{
	/** Fetch and set video motion windows								*/
	virtual EErrorCode				setVideoMotion1Window( mxmList *windows ) = 0;
	virtual mxm::smart<mxmList>	videoMotion1Windows( ) = 0;
	//@}

};


};



#endif   // __MX_ISUPPORTSEVENTCONFIGURATION_H__
