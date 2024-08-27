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
//  $Author: agu_admin $
//  $LastChangedBy: agu_admin $
//  $LastChangedDate: 2007-06-05 16:51:39 +0200 (Tue, 05 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsSimpleCameraSpecification.h $
//
///////



#ifndef __MX_ISUPPORTSSIMPLECAMERASPECIFICATION_H__
#define __MX_ISUPPORTSSIMPLECAMERASPECIFICATION_H__



#define MX_ISUPPORTSSIMPLECAMERASPECIFICATION_ID "ISupportsSimpleCameraSpecification"



#include "IInterface.h"
#include "mx_types.h"


namespace mx {

   //! Allows for specifying a camera in a sortof old-fashioned way, as needed
   //! to please <tt>[gdm]</tt>.
   /*!
    *  \ingroup mxsdk_interfaces
    */
   class ISupportsSimpleCameraSpecification : virtual IInterface {
      
     public: 
      virtual void setHost(const char* sHost) = 0;           //!< Bsp.: 192.168.2.143, cam01.mobotix.net, ...
      virtual void setPort(u16 uPort) = 0;
      virtual void setProxy(const char* sProxy) = 0;         //!< Proxyname oder IP
      virtual void setProxyPort(u16 uPort) = 0;
		virtual void setSSL(bool bOn) = 0;
      virtual void setUsername(const char* sUser) = 0;
      virtual void setPassword(const char* sPasswd) = 0;
      virtual void setFPS(float fFps) = 0;                   //!< Frames per second
      virtual void setGuestAccess(bool bGuestAccess) = 0;    
      virtual void setAudio(bool bOn) = 0;

      virtual bool isSupportedUsername() = 0;
      virtual bool isSupportedPassword() = 0;
      virtual bool isSupportedFPS() = 0;
      virtual bool isSupportedGuestAccess() = 0;
      virtual bool isSupportedAudio() = 0;
		virtual bool isSupportedSSL() = 0;
   };
};



#endif   // __MX_ISUPPORTSSIMPLECAMERASPECIFICATION_H__
