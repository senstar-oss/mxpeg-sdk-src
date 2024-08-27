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
//  $LastChangedDate: 2007-06-21 09:56:37 +0200 (Thu, 21 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IConfigSession.h $
//
///////



#ifndef __MX_ICONFIGSESSION_H__
#define __MX_ICONFIGSESSION_H__



#define MX_ICONFIGSESSION_ID "IConfigSession"



#include "IInterface.h"
#include "IEyeControl.h"
#include "IAsyncReboot.h"
#include "IAsyncRetrieveCommit.h"


// SDK includes
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmStringList.h>


namespace mx {
   
   //! Object describing a session while configuring a particular device.
   /*!
    *  \ingroup mxsdk_interfaces
    */
class IConfigSession : public virtual IEyeControl,
							  public virtual IAsyncReboot,
							  public virtual IAsyncRetrieveCommit	{
 
public:
	
   //! Informs about the interfaces provided by a particular config session
   //! object, so the calling context can learn what devices it can configure
   //! using the session, and what parameters.
   virtual mxm::smart<mxmStringList> enumerateInterfaces() = 0;


};	


};  // namespace mx


#endif   // __MX_ICONFIGSESSION_H__
