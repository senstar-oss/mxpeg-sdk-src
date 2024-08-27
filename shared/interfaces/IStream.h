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
//  $Author: khe_admin $
//  $LastChangedBy: khe_admin $
//  $LastChangedDate: 2007-05-29 15:10:15 +0200 (Tue, 29 May 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IStream.h $
//
//  ////



/*!
	\file Interface IStream

	\author AGu
*/



#ifndef __MX_ISTREAM_H__
#define __MX_ISTREAM_H__



// globale Includes
#include <mx_types.h>
#include <mx_error_codes.h>
// Basis Interfaces
#include "IInterface.h"

// mxm includes
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxmStringList.h>

#define MX_ISTREAM_ID "IStream"



namespace mx {

   //! Common interface to all objects representing incoming media streams
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[agu]</tt>, <tt>[khe]</tt>
    */
   class IStream : public virtual IInterface {
   
     public:
      //! Informs about the interfaces implemented by the stream, so the calling
      //! context knows what there is to set up before calling connect().
      /*!
       *  Currently, it seems improbable that the interfaces involved need to
       *  support interface capabilities.
       */
      virtual mxm::smart<mxmStringList> enumerateInterfaces() = 0;
       
       
      //! Sets up the decoder configuration honoring the parameters set before,
      //! and opens the stream source.
      /*!
       *  Asynchronous streams - those implementing ISupportsAsyncStreaming -
       *  will begin to signal newly available frames. Synchronous streams on
       *  the other hand will have to be polled using
       *  ISupportsSyncStreaming::getFrame().
       */
      virtual mx::EErrorCode connect() = 0;
      //! Destroys the encapsuled decoder and thus disconnects from the stream
      //! source.
      /*!
       *  Asynchronous streams will cease to signal newly available frames.
       *
       *  Disconnected streams can be reconnected, maybe to different sources
       *  and with different parameter settings.
       */
      virtual void disconnect() = 0;
   };
};

 

#endif  // __MX_ISIMPLESTREAM_H__