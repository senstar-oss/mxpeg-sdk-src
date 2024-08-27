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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IMultiViewStream.h $
//
///////



#ifndef __MX_IMULTIVIEWSTREAM_H__
#define __MX_IMULTIVIEWSTREAM_H__



#define MX_IMULTIVIEWSTREAM_ID "IMultiViewStream"



#include "IStream.h"
#include "ISupportsAsyncStreaming.h"



namespace mx {

   //! Interface to streams intended to be used with the MultiViewComponent.
   /*!
    *  \ingroup mxsdk_interfaces
    */
   class IMultiViewStream : public virtual IStream,
                            public virtual ISupportsAsyncStreaming {
      
       // add stuff needed to interface with multi view component
      
   };
};



#endif   // __MX_IMULTIVIEWSTREAM_H__
