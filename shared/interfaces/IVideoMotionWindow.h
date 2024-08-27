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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IVideoMotionWindow.h $
//
//  ////



/*!
	\file Interface for video motion window definitions!

	\author dgr
*/



#ifndef __MX_IVIDEOMOTIONWINDOW_H__
#define __MX_IVIDEOMOTIONWINDOW_H__


// Basis Interfaces
#include "IDefWindow.h"

 
#define MX_IVIDEOMOTIONWINDOW_ID "IVideoMotionWindow"



namespace mx {

   //! Common interface to all video motion windows typ definitions.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[dgr]</tt>
    */
   class IVideoMotionWindow : public virtual IDefWindow {

   };
};

 

#endif  // __MX_IVIDEOMOTIONWINDOW_H__