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
//  $Author: agu_admin $
//  $LastChangedBy: agu_admin $
//  $LastChangedDate: 2007-06-05 16:51:39 +0200 (Tue, 05 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ICamDriverComponent.h $
//
///////



#ifndef __MX_ICAMDRIVERCOMPONENT_H__
#define __MX_ICAMDRIVERCOMPONENT_H__



#define MX_ICAMDRIVERCOMPONENT_ID "ICamDriverComponent"



// interface includes
#include "IComponent.h"
// SDK includes
#include <mxm/core/mxm_smart.h>



namespace mx {

   class ISimpleCameraLiveStream;
   class ISimpleViewStream;
   class IConfigSession;
	class IViewingSession;

   //! Common interface to camera driver components.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  Via this interface calling contexts can instantiate streams and config
    *  sessions from/for the respective camera type.
    *
    *  \author AGu
    */
   class ICamDriverComponent : public virtual IComponent {
      
     public:
      //! Creates a simple stream that can be polled for new frames in a
      //! synchronous fashion.
      virtual mxm::smart<ISimpleCameraLiveStream> createSimpleCameraLiveStream() = 0;
      //! Creates a stream that can be used with the SimpleView component.
      virtual mxm::smart<ISimpleViewStream> createSimpleViewStream() = 0;
      //! Creates a session allowing the calling context to configure a given
      //! camera.
      virtual mxm::smart<IConfigSession> createConfigSession() = 0;
		//! Creates a session allowing the calling context to view a stream from a camera.
		virtual mxm::smart<IViewingSession> createViewingSession() = 0;
   };
};



#endif  // __MX_ICAMDRIVERCOMPONENT_H__
