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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsCameraSpecification.h $
//
///////



#ifndef __MX_ISUPPORTSCAMERASPECIFICATION_H__
#define __MX_ISUPPORTSCAMERASPECIFICATION_H__



#define MX_ISUPPORTSCAMERASPECIFICATION_ID "ISupportsCameraSpecification"



#include "IInterface.h"



namespace mx {

  //! Allows for specifying a camera.
  /*!
   *  \ingroup mxsdk_interfaces
   */
  class ISupportsCameraSpecification : virtual IInterface {
     
    public:
     
     // insert more comfortable camera/stream setup methods here
  };
};



#endif   // __MX_ISUPPORTSCAMERASPECIFICATION_H__
