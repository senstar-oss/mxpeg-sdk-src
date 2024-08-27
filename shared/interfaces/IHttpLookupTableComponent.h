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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IHttpLookupTableComponent.h $
//
///////



/*!
	\file Interface IHttpLookupTable

	\author AGu
*/



#ifndef __MX_IHTTPLOOKUPTABLECOMPONENT_H__
#define __MX_IHTTPLOOKUPTABLECOMPONENT_H__



#define MX_IHTTPLOOKUPTABLECOMPONENT_ID "IHttpLookupTableComponent"



// interface includes
#include "IComponent.h"
// SDK includes
#include <mxm/core/mxm_smart.h>


namespace mx {

	class IHttpLookupTable;

   //! Common interface to camera driver components.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  Via this interface calling contexts can instantiate a HttpLookupTable
    *  for a certain camera type.
    */
   class IHttpLookupTableComponent : public virtual IComponent {
      
     public:
		  //! Creates a lookup table allowing the calling context to retrieve
		  //! strings which can be sent to the camera
		  virtual mxm::smart<IHttpLookupTable> createHttpLookupTable() = 0; 
   };
};



#endif  // __MX_IHTTPLOOKUPTABLECOMPONENT_H__
