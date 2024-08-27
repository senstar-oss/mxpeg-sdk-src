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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IASyncReboot.h $
//
///////



#ifndef __MX_IASYNCREBOOT_H__
#define __MX_IASYNCREBOOT_H__



#define MX_IASYNCREBOOT_ID "IASyncReboot"


#include "IInterface.h"

#include <mx_types.h>
// SDK includes
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxmQtSlot.h>


namespace mx {
   
   //! 
   /*!
    *  \ingroup mxsdk_interfaces
    */
class IAsyncReboot : public virtual IInterface {
 
public:
	static const u32 NoTimeout = (u32)(-1);



	/*! check if reboot is in progress
	 */
	virtual bool isRebooting() = 0;


	/*! send a reboot request to the camera
	*   The slot method has the following signature:
	*   <tt>void method(enum EHttpError error, void* context)</tt>.
	*/
	virtual void reboot(const mxmQtSlot& slot, 
		                 void* context,
							  u32 UTimeOutMs = 60000) = 0;

	/*! 
		\brief Returns the interal cam driver reboot status!
			
		possibly a handy information for the calling context. 
		INFO: should only be called after a commit! 

	*/
	virtual bool needReboot() = 0;


};	


};  // namespace mx


#endif   // __MX_IASYNCREBOOT_H__