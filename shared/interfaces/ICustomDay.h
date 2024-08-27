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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ICustomDay.h $
//
///////



#ifndef __MX_ICUSTOMDAY_H__
#define __MX_ICUSTOMDAY_H__



#define MX_ICUSTOMDAY_ID "ICustomDay"



#include "IInterface.h"

#include <mxm/core/mxmString.h>

#include "../mx_error_codes.h"


namespace mx {

  //! Representation <tt>MX</tt> custom days.
  /*!
   *  \ingroup mxsdk_interfaces
   */
  class ICustomDay : public virtual IInterface {
  public:
	  /* set the name of the custom day.
	   * This name identifies the time table, therefore it
	   * must be unique.
	   */
	  virtual void setName(const mxmString& name) = 0;
	  /* retrieve the cutom days's name
	   */
	  virtual mxmString name() = 0;
	  /*! add a day to the internal list.
	   *  NOTE: the strings will be replaced by a date type very soon!!!!
		*/
	  virtual EErrorCode addDay(const mxmString& day) = 0;
	  /*! add a day range to the internal list.
	   *  NOTE: the strings will be replaced by a date type very soon!!!!
		*/
	  virtual EErrorCode addRange(const mxmString& from, const mxmString& to) = 0;  
  };
};



#endif   // __MX_ICUSTOMDAY_H__
