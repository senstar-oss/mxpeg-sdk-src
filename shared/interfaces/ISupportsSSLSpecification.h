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
//  $LastChangedDate: 2007-06-27 16:52:38 +0200 (Wed, 27 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsSSLSpecification.h $
//
///////



#ifndef __MX_ISUPPORTSSSLSPECIFICATION_H__
#define __MX_ISUPPORTSSSLSPECIFICATION_H__



#define MX_ISUPPORTSSSLSPECIFICATION_ID "ISupportsSSLSpecification"



#include "IInterface.h"



class mxmIPAddress;



namespace mx {

  //! Allows for specifying a SSL connection .
  /*!
   *  \ingroup mxsdk_interfaces
   *
   *  \author <tt>[agu]</tt> Alexander Gugau
   */
  class ISupportsSSLSpecification : virtual public IInterface {
     
   public:
     //! Use a SSL connection.
     /*!
	   *  Use a SSL connection and set an optional proxy. If you don't want to set
		*  a proxy, you can pass mxmIPAddress::nullIP().
      *  Note that mxmIPAddress objects optionally include an <tt>IP</tt> port
      *  setting.
      */
		virtual void setSSL(const mxmIPAddress &proxy) = 0;
  };
};



#endif   // __MX_ISUPPORTSSSLSPECIFICATION_H__
