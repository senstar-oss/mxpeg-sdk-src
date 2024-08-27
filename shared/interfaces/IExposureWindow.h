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
//  $Author: dgr $
//  $LastChangedBy: dgr $
//  $LastChangedDate:  $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IExposureWindow.h $
//
//  ////



/*!
	\file Interface for exposure window definitions!

	\author dgr
*/



#ifndef __MX_IEXPOSUREWINDOW_H__
#define __MX_IEXPOSUREWINDOW_H__


// Basis Interfaces
#include "IDefWindow.h"


#define MX_IEXPOSUREWINDOW_ID "IExposureWindow"



namespace mx {

   //! Common interface to all exposure windows typ definitions.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[dgr]</tt>
    */
   class IExposureWindow : public virtual IDefWindow {

   };
};

 

#endif  // __MX_IEXPOSUREWINDOW_H__