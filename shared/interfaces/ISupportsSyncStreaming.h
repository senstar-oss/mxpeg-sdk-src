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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsSyncStreaming.h $
//
///////



#ifndef __MX_ISUPPORTSSYNCSTREAMING_H__
#define __MX_ISUPPORTSSYNCSTREAMING_H__



#define MX_ISUPPORTSSYNCSTREAMING_ID "ISupportsSyncStreaming"



#include "IInterface.h"

#include <mxm/core/mxmFrameInfo.h>

namespace mx {

   //! Provides the means to stream synchrounously.
   /*!
    *  \ingroup mxsdk_interfaces
    */
   class ISupportsSyncStreaming : virtual IInterface {
     
     public:
      static const u32 NoTimeout = (u32)(-1);
        
      //! Synchronously polls the next frame.
      /*!
       *  The current frame is copied to the buffer indicated by
       *  <tt>pFrame</tt>. For one, <tt>uSize</tt> serves as input specifying
       *  the size of the user-provided buffer, and for another it gets
       *  overwritten  with the frame size actually written, thus also serving
       *  as output. In case of too small a buffer (eInvalidValueError), 
		 *  nothing gets written - only the actual frame size is reported as described above. 
       *  The calling context then has to restart the operation providing a sufficiently
       *  large buffer.
       *
       *  \param pFrame Buffer
       *  \param uSize[in] Buffer size
       *  \param uSize[out] Actual frame size
       *  \param sInfo  defined in mx_types.h
       *  \param uTimeOutMs Maximum time to wait for a new frame. If <tt>0</tt>,
       *                    the method returns immediately, if
       *                    <tt>NoTimeout</tt>, the method potentially waits
       *                    forever.
       */
		virtual mx::EErrorCode getFrame(u8* pFrame, u32& uSize, u32 uTimeOutMs = NoTimeout, mx::flags flags = 0) = 0;
      //! getFrame() version returning some additional frame information. 
      virtual mx::EErrorCode getFrame(u8* pFrame, u32& uSize, mxmFrameInfo& sInfo, u32 uTimeOutMs = NoTimeout, mx::flags flags = 0) = 0;
   };
};



#endif   // __MX_ISUPPORTSSYNCSTREAMING_H__
