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
//  $LastChangedDate: 2007-06-22 15:48:41 +0200 (Fri, 22 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IHttpCameraConnection.h $
//
///////



#ifndef __MX_IHTTPCAMERACONNECTION_H__
#define __MX_IHTTPCAMERACONNECTION_H__



#define MX_IHTTPCAMERACONNECTION_ID "IHttpCameraConnection"

#include "ISupportsHostSpecification.h"
#include "ISupportsProxySpecification.h"
#include "ISupportsSSLSpecification.h"
#include "ICanUseKeyChain.h"
#include "IHttpError.h"

#include <mxm/core/mxmString.h>
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmQtSignal.h>

#include "../mx_types.h"
#include "../mx_error_codes.h"

namespace mx {

  /*! Representation of a HTTP connection to an IP camera, including
	*  the usual stuff like host, proxy, ... Additional to these attributes
	*  is the ability to query the camera for model information.
	*  
   *  \ingroup mxsdk_interfaces
   */
  class IHttpCameraConnection : virtual public ISupportsHostSpecification,
                                virtual public ISupportsProxySpecification,
										  virtual public ISupportsSSLSpecification,
										  virtual public ICanUseKeyChain,
                                virtual public IHttpError {
  public:
	  /*! Send a <tt>get</tt> request to the camera. The return value is an
	   *  identifier for this request and can be used to match the response.
	   */
	  virtual int get(const mxmString& request) = 0;
	  /*! This signal is emitted when a request is finished. The signature of
	   *  this signal is (int id, bool error, mxmString response). 
		*  The <tt>id</tt> is the identifier returned by the <tt>get</tt> method 
		*  and <tt>error</tt> indicates the success of the operation. In the case 
		*  of an error, more detailed information can be retrieved via 
		*  the <tt>IHttpError</tt> interface. In the error-free case the <tt>response</tt>
		*  is filled.
	   */
	  virtual mxmQtSignal requestFinishedSignal() = 0;
	  /*! Retrieve model information from the camera. 
	   */
	  virtual void retrieveCameraModel(const mxmString& request) = 0;
	  /*! This signal is emitted when the retrieval of the camera model is finished.
	   *  The signature of this signal is (bool error). 
		*  <tt>error</tt> indicates the success of the operation. In the case of 
		*  an error, more detailed information can be retrieved via the 
		*  <tt>IHttpError</tt> interface. In the error-free case the <tt>response</tt>
		*  is filled.
	   */
	  virtual mxmQtSignal cameraModelRetrievedSignal() = 0;
  };
};



#endif   // __MX_IHTTPCAMERACONNECTION_H__
