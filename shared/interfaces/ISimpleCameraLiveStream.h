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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleCameraLiveStream.h $
//
///////



#ifndef __MX_ISIMPLECAMERALIVESTREAM_H__
#define __MX_ISIMPLECAMERALIVESTREAM_H__



#define MX_ISIMPLECAMERALIVESTREAM_ID "ISimpleCameraLiveStream"



#include "IStream.h"
#include "ISupportsSimpleCameraSpecification.h"
#include "ISupportsSyncStreaming.h"



namespace mx {

   //! Provides simple, synchronized access to a camera live stream.
   /*!
    *  \ingroup mxsdk_interfaces
    */
   class ISimpleCameraLiveStream : public virtual IStream,
                                   public virtual 
                                    ISupportsSimpleCameraSpecification,
                                   public virtual ISupportsSyncStreaming             {
      
   };
};



#endif   // __MX_ISIMPLECAMERALIVESTREAM_H__
