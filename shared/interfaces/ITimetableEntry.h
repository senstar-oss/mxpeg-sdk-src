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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ITimetableEntry.h $
//
///////



#ifndef __MX_ITIMETABLEENTRY_H__
#define __MX_ITIMETABLEENTRY_H__



#define MX_ITIMETABLE_ID "ITimetableEntry"



#include "IInterface.h"
#include "ICustomDay.h"

#include <mxm/core/mxmString.h>
#include <mxm/core/mxm_smart.h>

#include "../mx_types.h"
#include "../mx_error_codes.h"

namespace mx {

  //! Representation <tt>MX</tt> time tables.
  /*!
   *  \ingroup mxsdk_interfaces
   */
  class ITimetableEntry : virtual IInterface {
  public:
	  enum EWeekdays {
		  eMon     = 1,
		  eTue     = 2,
		  eWed     = 4,
		  eThu     = 8,
		  eFri     = 16,
		  eSat     = 32,
		  eSun     = 64
	  };

	  /* set the name of the time table.
	   * This name identifies the time table, therefore it
	   * must be unique.
	   */
	  virtual void setName(const mxmString& name) = 0;
	  /* retrieve the time table's name
	   */
	  virtual mxmString name() = 0;
	  /* time tables ranges supports the concept of being active or inactive. The <tt>index</tt> is
	   * the value that was returned by <tt>addRange</tt>.
	   */
	  virtual void setActive(int index, bool bActive) = 0;
	  virtual bool active(int index) = 0;

	  /*! add a range. The <tt>uWeekdays</tt> parameter is an OR'ed combination of
	   *  the enum EWeekdays, where the interval iHoursFrom:iMinutesFrom to iHoursTo:iMinutesTo
		*  is valid. The <tt>index</tt> is an unique identifier for this entry returned by the
		*  implementation. Use the <tt>index</tt> if you want to remove this entry.
		*/
	  virtual EErrorCode addRange(u32 uWeekdays, int iHoursFrom, int iMinutesFrom,
		                                          int iHoursTo, int iMinutesTo, 
																int& index) = 0;
	
	  /*! add a range. The <tt>customDays</tt> parameter references a custom day created earlier.
		*  The <tt>index</tt> is an unique identifier for this entry returned by the
		*  implementation. Use the <tt>index</tt> if you want to remove this entry.
		*/
	  virtual EErrorCode addRange(mxm::smart<ICustomDay> customDay, int iHoursFrom, int iMinutesFrom,
		                                                    int iHoursTo, int iMinutesTo, 
																			 int& index) = 0;

	  /*! <tt>true</tt> if the entry referenced by <tt>index</tt> uses a custom day, <tt>false</tt>
	   * otherwise. The result is undefined, if the index does not exist.
		*/
	  virtual bool usesCustomDay(int index) = 0;

	  /*! get the range <tt>index</tt>
	   *  returns false if the range uses a custom day or a range with index <tt>index</tt>
		*  is not defined.
		*/
	  virtual bool range(u32& uWeekdays, int& iHoursFrom, int& iMinutesFrom,
		                                          int& iHoursTo, int& iMinutesTo, 
																int index) = 0;

	  /*! get the range <tt>index</tt>
	   *  returns false if the range uses weekdays or a range with index <tt>index</tt>
		*  is not defined.
		*/
	  virtual bool range(mxm::smart<ICustomDay>& customDay, int& iHoursFrom, int& iMinutesFrom,
		                                                    int& iHoursTo, int& iMinutesTo, 
																			 int index) = 0;

	  /*! return the number of ranges set in this time table entry.
	   */
	  virtual int countRanges() = 0;
  };
};



#endif   // __MX_ITIMETABLEENTRY_H__
