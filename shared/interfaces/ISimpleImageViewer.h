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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleImageViewer.h $
//
///////



#ifndef __MX_ISIMPLEIMAGEVIEWER_H__
#define __MX_ISIMPLEIMAGEVIEWER_H__



#define MX_ISIMPLEIMAGEVIEWER_ID "ISimpleImageViewer"



#include "IQtWidget.h"
#include "IHttpError.h"

#include <mxm/core/mxmQtSignal.h>


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
	class ISimpleImageViewer : public virtual IQtWidget,
	                           public virtual IHttpError {
      
    public:
      //! Request an image.
      virtual void requestImage() = 0;      
      //! <tt>(bool)</tt>-signature signal reporting an image was retrieved.
      virtual mxmQtSignal imageReadySignal() = 0;
   };
};



#endif   // __MX_ISIMPLEIMAGEVIEWER_H__

      