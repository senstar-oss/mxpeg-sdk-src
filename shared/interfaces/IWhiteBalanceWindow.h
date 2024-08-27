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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IWhiteBalanceWindow.h $
//
//  ////



/*!
	\file Interface for video motion window definitions!

	\author dgr
*/



#ifndef __MX_IWHITEBALANCEWINDOW_H__
#define __MX_IWHITEBALANCEWINDOW_H__


// Basis Interfaces
#include "IDefWindow.h"


#define MX_IWHITEBALANCEWINDOW_ID "IWhiteBalanceWindow"



namespace mx {

   //! Common interface to all white balance windows typ definitions.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[dgr]</tt>
    */
   class IWhiteBalanceWindow : public virtual IDefWindow {

   };
};

 

#endif  // __MX_IWHITEBALANCEWINDOW_H__