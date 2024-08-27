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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IHttpError.h $
//
///////



#ifndef __MX_IHTTPERROR_H__
#define __MX_IHTTPERROR_H__

#define MX_IHTTPERROR_ID "IHttpError"

#include <mx_error_codes.h>

namespace mx {
   
   //! The error state of a HTTP(s) capable device.
   /*!
    *  \ingroup mxsdk_interfaces
    *
	 *  If a HTTP(s) capable device is in an erronous state, further
	 *  information can be retrieved.
	 *
    *  \author <tt>[agu]</tt> Alexander Gugau
    */
	class IHttpError : public virtual IInterface {
      
    public:
      //! Retrieve the HTTP error state from a device.
      virtual EHttpError httpError() = 0;      
   };
};



#endif   // __MX_IHTTPERROR_H__

      