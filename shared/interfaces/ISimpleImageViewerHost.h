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
//  $LastChangedDate: 2007-06-11 10:18:50 +0200 (Mon, 11 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleImageViewerHost.h $
//
///////



#ifndef __MX_ISIMPLEIMAGEVIEWERHOST_H__
#define __MX_ISIMPLEIMAGEVIEWERHOST_H__



#define MX_ISIMPLEIMAGEVIEWERHOST_ID "ISimpleImageViewerHost"



#include "ISimpleImageViewer.h"
#include "ICanUseKeyChain.h"
#include "ISupportsHostSpecification.h"
#include "ISupportsProxySpecification.h"
#include "ISupportsWebPageSpecification.h"



namespace mx {
   
   //! Encodes the <i>concept</i> of a simple image viewer widget.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  <tt>MxSDK</tt> camera drivers will provide viewer widgets implementing
    *  this interface in order to deliver a fixed-image functionality.
    *
    *  \author <tt>[agu]</tt> Alexander Gugau
    */
	class ISimpleImageViewerHost : public virtual ISimpleImageViewer,
											 public virtual ICanUseKeyChain,
											 public virtual ISupportsHostSpecification,
											 public virtual ISupportsProxySpecification,
											 public virtual ISupportsWebPageSpecification {
      
   };
};



#endif   // __MX_ISIMPLEIMAGEVIEWERHOST_H__

      