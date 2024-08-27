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
//  $LastChangedDate: 2007-06-20 18:55:14 +0200 (Wed, 20 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IHttpCameraRequest.h $
//
///////



#ifndef __MX_IHTTPCAMERAREQUEST_H__
#define __MX_IHTTPCAMERAREQUEST_H__



#define MX_IHTTPCAMERAREQUEST_ID "IHttpCameraRequest"

#include "IInterface.h"

#include <mxm/core/mxmString.h>
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmQtSlot.h>
#include <mxm/core/mxmQtSignal.h>

#include "../mx_types.h"
#include "../mx_error_codes.h"

class QHttp;

namespace mx {

  /*! Representation of a HTTP request which is sent to an IP camera.
	*  This HTTP request represents a "complex" request which can consist
	*  of a few atomic HTTP request. These atomic requests are handled as
	*  one request in a transactional sense, i.e. if one atomic request
	*  fails, the complex request fails and the complex request is finished
	*  when all atomic requests are finished.
	* 
	*  NOTE: This interface relates on Qt
	*
   *  \ingroup mxsdk_interfaces
   */
  class IHttpCameraRequest : IInterface {
  public:
	   /*! set a timeout. If the complex request is not finished within 
		 *  uTimeoutMs, the error state of the request is set to EHttpTimeOutError
		 */
		virtual void setTimeout(u32 uTimeoutMs) = 0;
		/*! call the slot set by the method <tt>setClientSlot<\tt>.
		 *  \param bDefered if true, the slot is connected via Qt::QueuedConnection,
		 *  Qt::AutoConnection otherwise. The slot is called only once, subsequent calls
		 *  to this method are ignored.
		 */
		virtual void callSlot(bool bDefered = false) = 0;
		/*! retrieve the current error status of the request.
		 */
		virtual EHttpError httpError() const = 0;
		/*! indicates that the client slot was already called.
		 */
		virtual bool signalled() const = 0;
		/*! indicates that the camera has responses from all all atomic requests.
		 */
		virtual bool completed() const = 0;
		virtual bool inSlot() const = 0;
		virtual bool sendDelayed() const = 0;
		virtual int group() const = 0;
		virtual bool send(QHttp* pHttp) = 0;
		virtual void setSignalled(bool bSignalled = true) = 0;
		virtual void setClientSlot(const mxmQtSlot& slot) = 0;
		/*! The signal requestFinished(IHttpCameraRequest*) is emitted after all atomic
		 *  requests are completed successfully or after the first unsuccessful request.
		 */
		virtual mxmQtSignal requestFinishedSignal() const = 0;
		/*! the client can set its own context, which is stored in the request itself.
		 *  The client must handle memory allocation and deallocation.
		 */
		virtual void setClientContext(void* pContext) = 0;
		/*! the driver can set its own context, which is stored in the request itself.
		 *  The caller must handle memory allocation and deallocation.
		 */
		virtual void setDriverContext(void* pContext) = 0;
		virtual void setRequestString(const mxmStringList& requestStrings) = 0;
		virtual mxm::smart<mxmStringList> getResponse() = 0;
		/*! add a user and a password to the request. Normally you should set this
		 *  only once, but it is perfectly legal to call this more than once.
		 */
		virtual void setUserAndPassword(const mxmString& user, const mxmString& pwd) = 0;
		/*! add a host and a port to the request. Normally you should set this
		 *  only once, but it is perfectly legal to call this more than once.
		 */
		virtual void setHost(const mxmString& host, u16 uPort) = 0;
		/*! add a proxy and a proxy port to the request. Normally you should set this
		 *  only once, but it is perfectly legal to call this more than once.
		 */
		virtual void setProxy(const mxmString& proxy, u16 uProxyPort) = 0;
		/*! add SSL socket, proxy need not to be set.
		 *  
		 */
		virtual void setSSL(const mxmString& proxy, u16 uProxyPort) = 0;

		virtual void* driverContext() = 0;
		/*! set the request state to aborted:
		 */
		virtual void abort() = 0;
		virtual void waitUntilRebooted(const mxmString& host, u16 uPort, int iRebootTimeOut) = 0;
  };
};



#endif   // __MX_IHTTPCAMERAREQUEST_H__
