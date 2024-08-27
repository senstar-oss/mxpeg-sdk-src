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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/IDefWindow.h $
//
//  ////



/*!
	\file Interface for Window definitions!

	\author dgr
*/



#ifndef __MX_IDEFWINDOW_H__
#define __MX_IDEFWINDOW_H__



// globale Includes
#include <mx_types.h>
#include <mx_error_codes.h>
// Basis Interfaces
#include "IInterface.h"


#define MX_IDEFWINDOW_ID "IDefWindow"



namespace mx {

   //! Common interface to all windows typ definitions.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  \author <tt>[dgr]</tt>
    */
   class IDefWindow : public virtual IInterface {
   
     public:
      
		//@{ 
	   /** Fkt. for read/write window values  */
		//! Fetch size of current window
      /*!
       *  Relative values i.e. valid values lie in the interval [0,1]. 
       */
      virtual void size(float& x, float& y, float& width, float& height) = 0;

	   //! Set size of current window
      /*!
       *  Relative values i.e. valid values are must lie in a interval [0,1]. 
       */
      virtual mx::EErrorCode setSize(float& x, float& y, float& width, float& height) = 0;
		//@}
 
		//@{
		/** Fetch/set windows attach eye values i.e. a bit mask. Example: "...001" ~ right, "...011" ~ right&&left, etc.	*/
		virtual u32					eye()							= 0;
      virtual mx::EErrorCode	setEye(u32 validEyes)	= 0;
      //@}

		//! Is window a valid definition?
		virtual bool isValid()									= 0;


		//@{
		/** Exclude property. Meaning? In the context of the windows character it is somehow equivalent	"invert". 
		*   Example: A exposure window set to exclude is exempted from from the exposure control i.e. is considered
		*   as rest.
		*/
		virtual bool				isExclude()										=0;
		virtual mx::EErrorCode	exclude()										=0;
		//@}



   };
};

 

#endif  // __MX_IDEFWINDOW_H__