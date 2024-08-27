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
//  $LastChangedDate: $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IASyncRetriveCommit.h $
//
///////



#ifndef __MX_IASYNCRETRIVECOMMIT_H__
#define __MX_IASYNCRETRIVECOMMIT_H__



#define MX_IASYNCRETRIVECOMMIT_ID "IASyncRetriveCommit"



#include "IInterface.h"

#include <mx_types.h>
#include <mx_error_codes.h>
// SDK includes
#include <mxm/core/mxmStringList.h>
#include <mxm/core/mxmQtSlot.h>


namespace mx {
   
   //! Object implementing this interface describes a way for asynchronous client/camera communication.
   /*!
    *  \ingroup mxsdk_interfaces
    */
class IAsyncRetrieveCommit : public virtual IInterface {
 
public:
	static const u32 NoTimeout = (u32)(-1);

	/*! retrieve the settings from the camera for the supported interfaces
	*   i.e. ISupportsRecordingConfiguration, ...
	*   before you can get any property the retrieve method must be called
	*   succesfully, an exception is thrown otherwise
	*   NOTE: if there is a dependency of an attribute to an attribute of
	*   another interface section not listed in supportedInterfaces,
	*   this method adds the other interface to the list before retrieval.
	*   The slot method has the following signature:
	*   <tt>void method(enum EHttpError error, void* context)</tt>.
	*   
	*  \param supportedInterfaces
	*  \param slot
	*  \param context
	*  \param uTimeOutMs Maximum time to wait for retrieval. If <tt>0</tt>,
	*                    the method returns immediately, if
	*                    <tt>NoTimeout</tt>, the method potentially waits
	*                    forever.
	*  
	*/
	virtual void retrieve(mxmStringList& supportedInterfaces, 
		           const mxmQtSlot& slot,
					  void* context,
					  u32 uTimeOutMs = 5000) = 0;

	/*! check if a supported inteface is already retrieved
	 */
	virtual bool isRetrieved(const mxmString& supportedInterface) = 0;
	/*! check if retrieving the supported interface is currently executed
	 */
	virtual bool isRetrieving(const mxmString& supportedInterface) = 0;
	/*! check if the commit in progress
	 */
	virtual bool isCommitting() = 0;
	/*! cancel the last retrieve or commit request
	 */
	virtual mx::EErrorCode cancelAsyncRequest() = 0;

	/*! send all <tt>changed</tt> attributes to the camera
	*   The slot method has the following signature:
	*   <tt>void method(enum EHttpError error, void* context)</tt>.
	*   if <tt>bStorePermanent</tt> is true, the data will be saved
	*   permanently on the camera.
	 */
	virtual void commit(const mxmQtSlot& slot, 
		                 void* context,
							  u32 uTimeOutMs = 5000,
							  bool bStorePermanent = false) = 0;

};	


};  // namespace mx


#endif   // __MX_IASYNCRETRIVECOMMIT_H__