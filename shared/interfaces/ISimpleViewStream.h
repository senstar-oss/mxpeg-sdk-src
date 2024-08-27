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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleViewStream.h $
//
///////



#ifndef __MX_ISIMPLEVIEWSTREAM_H__
#define __MX_ISIMPLEVIEWSTREAM_H__



#define MX_ISIMPLEVIEWSTREAM_ID "ISimpleViewStream"



#include "IStream.h"
#include "ISupportsAsyncStreaming.h"



namespace mx {

   //! Interface to streams intended to be used with the SimpleViewComponent.
   /*!
    *  \ingroup mxsdk_interfaces
    */
   class ISimpleViewStream : public virtual IStream,
                             public virtual ISupportsAsyncStreaming {
      
      // add stuff needed to interface with simple view component
      
   };
};



#endif   // __MX_ISIMPLEVIEWSTREAM_H__
