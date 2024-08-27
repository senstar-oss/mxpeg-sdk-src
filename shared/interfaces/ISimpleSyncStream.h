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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleSyncStream.h $
//
///////



#ifndef __MX_ISIMPLESYNCSTREAM_H__
#define __MX_ISIMPLESYNCSTREAM_H__



#define MX_ISIMPLESYNCSTREAM_ID "ISimpleSyncStream"



#include "IStream.h"
#include "ISupportsSimpleCameraSpecification.h"
#include "ISupportsSyncStreaming.h"



namespace mx {

   //! Interface to pull the stream from a camera
   /*!
    *  \ingroup mxsdk_interfaces
    */
   class ISimpleSyncStream : public virtual IStream,
                             public virtual ISupportsSimpleCameraSpecification,
                             public virtual ISupportsSyncStreaming            {
      
   };
};



#endif   // __MX_ISIMPLESYNCSTREAM_H__
