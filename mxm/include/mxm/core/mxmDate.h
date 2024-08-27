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
//  $LastChangedDate: 2007-06-29 15:34:53 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/include/mxm/core/mxmDate.h $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licenced under the BSD licence,
     //  http://www.opensource.org/licenses/bsd-license.php
     //
     //  Copyright (c) 2005 - 2007, MOBOTIX AG
     //  All rights reserved.
     //
     //  Redistribution and use in source and binary forms, with or without
     //  modification, are permitted provided that the following conditions are
     //  met:
     //
     //  - Redistributions of source code must retain the above copyright
     //    notice, this list of conditions and the following disclaimer.
     //
     //  - Redistributions in binary form must reproduce the above copyright
     //    notice, this list of conditions and the following disclaimer in the
     //    documentation and/or other materials provided with the distribution.
     //
     //  - Neither the name of MOBOTIX AG nor the names of its contributors may
     //    be used to endorse or promote products derived from this software
     //    without specific prior written permission.
     //
     //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     //  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     //  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     //  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     //  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     //  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     //  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     //  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     //  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     //
///////



#ifndef __MXM_DATE_H__
#define __MXM_DATE_H__



#include <mxm/core/mxmObject.h>
#include <mxm/core/mxmString.h>


//! Date class, featuring methods greatly facilitating date handling.
/*!
 *  \ingroup modularMX_core
 *
 *  \author <tt>[agu]</tt> Alexander Gugau
 */
class MXM_CORE_API mxmDate : public mxmObject {
  
public:
	enum EDateFormat {
		eEuropean2,              //!<  DD.MM.YY
		eEuropean4,              //!<  DD.MM.YYYY
		eBritish2,               //!<  MM/DD/YY
		eBritish4,               //!<  MM/DD/YYYY
		eEuropean2Time,          //!<  DD.MM.YY HH:MM:SS
		eEuropean4Time,          //!<  DD.MM.YYYY HH:MM:SS
		eBritish2Time,           //!<  MM/DD/YY HH:MM:SS
		eBritish4Time            //!<  MM/DD/YYYY HH:MM:SS
	};

	/*! default constructor
	 *  initializes to 01.01.1900
	 */
	mxmDate();
	/*! constructor initializes to the given date
	 */
	mxmDate(int year, int month, int day);
	/*! copy constructor
	 */
	mxmDate(const mxmDate& date);
	/*! destructor
	 */
	~mxmDate();
	/*! returns the year of the date
	 */
	int year() const;
	/*! returns the month of the date
	 */
	int month() const;
	/*! returns the day of the date
	 */
	int day() const;
	/*! sets the date to the given year, month and day
	 *  returns <tt>false</tt> if one of the arguments
	 *  is in an invalid range
	 */
	bool setDate(int year, int month, int day);
	/*! returns <tt>true</tt> if the date is in a leap year
	 */
	bool isLeapYear() const;
	/*! convert the date to a string representation
	 */
	mxmString toString(EDateFormat format = eEuropean4) const;
	/*! assignment
	 */
	mxmDate& operator=(const mxmDate& date);
	/*! returns the current date
	 */
	static mxmDate currentDate();

	//@{
	/* compare dates.
	 *  NOTE: at this time, only the date part is compared,
	 *        not the time part.
	 */
	bool operator==(const mxmDate& date) const;
	bool operator!=(const mxmDate& date) const;
	bool operator<=(const mxmDate& date) const;
	bool operator>=(const mxmDate& date) const;
	bool operator<(const mxmDate& date) const;
	bool operator>(const mxmDate& date) const;
	//@}
	/*! add days to the date
	 */
	void addDays(int days);
	/*! add months to the date
	 */
	void addMonths(int months);
	/*! add years to the date
	 */
	void addYears(int years);

private:
	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;
	int m_second;
   void *StableABIDataExtension;
};



#endif   // __MXM_DATE_H__
