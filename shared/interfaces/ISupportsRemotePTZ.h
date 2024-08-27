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
//  $LastChangedDate: 2007-05-29 15:10:15 +0200 (Tue, 29 May 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsRemotePTZ.h $
//
///////



#ifndef __MX_ISUPPORTSREMOTEPTZ_H__
#define __MX_ISUPPORTSREMOTEPTZ_H__



#define MX_ISUPPORTSREMOTEPTZ_ID "ISupportsRemotePTZ"



#include "IInterface.h"


namespace mx {

  //! Allows for setting <tt>MX</tt> PTZ parameters in a live stream.
  /*!
   *  \ingroup mxsdk_interfaces
   */
  class ISupportsRemotePTZ : virtual IInterface {
  public:
	  enum EEye {
		  eRightEye,
		  eLeftEye,
		  eBothEyes,
		  eAutoEye
	  };

	  /*! select the eye, for which the settings are - if not specified the active eye is used
	   */
	  virtual EErrorCode setEye(EEye eEye) = 0;
	  /*! set image size of stream in pixels
	   */
	  virtual EErrorCode setStreamImageSize(u16 uSizeX, u16 uSizeY) = 0;
	  /*! set area of sensor image that has to be streamed, width and height are normalized to FocalWidth
	   */
	  virtual EErrorCode setWidthRatio(double dWidthRatioX, double dWidthRatioY) = 0;
	  /*! set center postion of streamed image relating to optical center of the sensor image
	   */
	  virtual EErrorCode setCenter(double dCenterX, double dCenterY) = 0;  
  	  /*! send all changes to the camera
	   */
	  virtual void updateRemotePTZ() = 0;
	
  };
};



#endif   // __MX_ISUPPORTSREMOTEPTZ_H__
