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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IVMWindow.h $
//
//  ////



/*!
	\file Interface for video motion window definitions!

	\author dgr
*/



#ifndef __MX_IVMWINDOW_H__
#define __MX_IVMWINDOW_H__


// Basis Interfaces
#include "IDefWindow.h"


#define MX_IVMWINDOW_ID "IVMWindow"



namespace mx {

   //! Common interface to all video motion windows typ definitions.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[dgr]</tt>
    */
   class IVMWindow : public virtual IDefWindow {

		
		//@{
		/**	Fkt. used for holding VM-window sensitivity													*/
		virtual bool				sensitivity()									=0;
		virtual mx::EErrorCode	setSensitivity(float percentage)			=0;

		//@}
   };
};

 

#endif  // __MX_IVMWINDOW_H__