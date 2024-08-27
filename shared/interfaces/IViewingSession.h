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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IViewingSession.h $
//
///////



#ifndef __MX_IVIEWINGSESSION_H__
#define __MX_IVIEWINGSESSION_H__



#define MX_IVIEWINGSESSION_ID "IViewingSession"



#include "IInterface.h"
#include "ISimpleViewer.h"
#include "ISimpleImageViewer.h"

// SDK includes
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmStringList.h>


namespace mx {
   
   //! A session where you can specify a viewing session and obtain
	//! an interface to a viewing device.
   /*!
    *  \ingroup mxsdk_interfaces
    */
class IViewingSession : virtual public IInterface {
 
public:
   /*! Informs about the interfaces provided by a particular viewing session
   *   object, so the calling context can learn what devices it can configure
   *   using the session, and what parameters.
	*   Normally, the viewing session shoul support ISupportsSimpleCameraConfiguration.
	*/
   virtual mxm::smart<mxmStringList> enumerateInterfaces() = 0;
	/*! get an instance of a simple viewer object. This object is useful for a preview, for example.
	 *  The caller gets ownership of the object.
	 *  NOTE: you don't get a smart pointer, this is because you can retrieve a QWidget pointer from
	 *  the ISimpleViewer. If this QWidget gets a parent assigned, the parent will remove this QWidget
	 *  on destruction.
	 */
	virtual ISimpleViewer* getSimpleViewerInstance() = 0;
	/*! get an instance of a simple viewer object, which is configured to use a preview stream.
	 *  Maybe not every attribute is considered, i.e framerate, ...
	 *  If you need these attributes, you must use the method getSimpleViewerInstance,
	 */
	virtual ISimpleViewer* getPreviewInstance() = 0;
	/*! get an instance of a simple image viewer object, which is configured to retrieve an image
	 *  from the device. Since you don't get a smart pointer, you have to take care on the livetime
	 *  of the instance.
	 */
	virtual ISimpleImageViewer* getSimpleImageViewerInstance() = 0;
};	


};  // namespace mx


#endif   // __MX_IVIEWINGSESSION_H__
