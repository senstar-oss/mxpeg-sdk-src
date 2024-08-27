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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IEyeControl.h $
//
///////



#ifndef __MX_IEYECONTROL_H__
#define __MX_IEYECONTROL_H__



#define MX_IEYECONTROL_ID "IEyeControl"



#include "IInterface.h"

#include <mx_enum_config.h>
// SDK includes


namespace mx {
   
   //! Control currently selected eye!
   /*!
    *  \ingroup mxsdk_interfaces
    */
class IEyeControl : public virtual IInterface {
 
public:
	
	//@{
	/**  A session can be valid for a defined eye state e.g. "left eye" or "both eyes".  */
	virtual void		setEye( ECamEye eye )=0;
	virtual ECamEye	eye()=0;
	//@}

};	


};  // namespace mx


#endif   // __MX_IEYECONTROL_H__