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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IPTZSession.h $
//
///////



#ifndef __MX_IPTZSESSION_H__
#define __MX_IPTZSESSION_H__



#define MX_IPTZSESSION_ID "IPTZSession"



#include "IInterface.h"
#include "IEyeControl.h"
#include "IAsyncReboot.h"
#include "IAsyncRetrieveCommit.h"


// SDK includes
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmStringList.h>


namespace mx {
   
   //! .
   /*!
    *  \ingroup mxsdk_interfaces
    */
class IPTZSession :	 public virtual IAsyncRetrieveCommit	{
 
public:
	
   //! Informs about the interfaces provided by a particular config session
   //! object, so the calling context can learn what devices it can configure
   //! using the session, and what parameters.
   virtual mxm::smart<mxmStringList> enumerateInterfaces() = 0;


};	


};  // namespace mx


#endif   // __MX_IPTZSESSION_H__
