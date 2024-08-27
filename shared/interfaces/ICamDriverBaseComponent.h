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
//  $Author: agu $
//  $LastChangedBy: agu $
//  $LastChangedDate: 2007-06-12 16:51:39 +0200 (Tue, 12 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ICamDriverBaseComponent.h $
//
///////



/*!
	\file Interface ICamDriverBaseComponent

	\author AGu
*/



#ifndef __MX_ICAMDRIVERBASECOMPONENT_H__
#define __MX_ICAMDRIVERBASECOMPONENT_H__



#define MX_ICAMDRIVERBASECOMPONENT_ID "ICamDriverBaseComponent"



// interface includes
#include "IComponent.h"
// SDK includes
#include <mxm/core/mxm_smart.h>



namespace mx {

   class IHttpCameraRequest;
	class ISimpleImageViewerHost;

   //! Common interface to camera driver base components.
   /*!
    *  \ingroup mxsdk_interfaces
    *
	 *  NOTE: This interface should be used from camera drivers only
	 *
    *  Via this interface camera drivers can access base functionality implemented
	 *  for a family of cameras (i.e. IP cameras)
    */
   class ICamDriverBaseComponent : public IComponent {
      
     public:
      //! Creates a request which can consist of a few atomic http-requests.
	   //! All requests which belong to the same group according to iGroup are
	   //! handled transactional, i.e. if one request fails, all requests fail
	   //! and if a request is signalled to the client only if all requests are
	   //! completed.
      //! to handle a configuration session
      virtual mxm::smart<IHttpCameraRequest> createHttpCameraRequest(int iGroup) = 0;
		//! creates a simple image viewer object, which is configured to retrieve an image
		//  from the device. 
		virtual mxm::smart<ISimpleImageViewerHost> createSimpleImageViewerHost() = 0;
   };
};



#endif  // __MX_ICAMDRIVERBASECOMPONENT_H__
