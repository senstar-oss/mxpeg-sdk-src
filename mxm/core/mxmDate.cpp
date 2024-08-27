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
//  $LastChangedDate: 2007-06-29 12:31:37 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxm/trunk/core/mxmDate.cpp $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licensed under the BSD licence,
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



#include <mxm/core/mxmDate.h>

#ifdef MX_PLATFORM_WINDOWS
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <time.h>


mxmDate::mxmDate() {
	m_year = 1900;
	m_month = 1;
	m_day = 1;
	m_hour = 0;
	m_minute = 0;
	m_second = 0;
}

mxmDate::mxmDate(int year, int month, int day) {
	m_year = year;
	m_month = month;
	m_day = day;
	m_hour = 0;
	m_minute = 0;
	m_second = 0;
}

mxmDate::mxmDate(const mxmDate& date) {
	m_year = date.m_year;
	m_month = date.m_month;
	m_day = date.m_day;
	m_hour = date.m_hour;
	m_minute = date.m_minute;
	m_second = date.m_second;
}

mxmDate::~mxmDate() {
}

mxmDate& mxmDate::operator =(const mxmDate& date) {
	m_year = date.m_year;
	m_month = date.m_month;
	m_day = date.m_day;
	m_hour = date.m_hour;
	m_minute = date.m_minute;
	m_second = date.m_second;
	return *this;
}

int mxmDate::year() const {
	return m_year;
}

int mxmDate::month() const {
	return m_month;
}

int mxmDate::day() const {
	return m_day;
}

bool mxmDate::setDate(int year, int month, int day) {
	if(year < 0 || year > 9999)
		return false;
	if(month <= 0 || month > 12)
		return false;
	if(day <= 0)
		return false;
	switch(month) {
		case 2: {   // Feb, check for leap year
				mxmDate tmp(year, 1, 1);
				if((tmp.isLeapYear() && day > 29) || (!tmp.isLeapYear() && day > 28))
					return false;
			}
			break;
		case 1:
		case 3:
		case 5: 
		case 7:
		case 8:
		case 10:
		case 12:
			if(day > 31)
				return false;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if(day > 30)
				return false;
			break;
	}
	m_year = year;
	m_month = month;
	m_day = day;
	m_hour = 0;
	m_minute = 0;
	m_second = 0;
	return true;
}

bool mxmDate::operator ==(const mxmDate& date) const {
	if(m_year == date.m_year && m_month == date.m_month && m_day == date.m_day)
		return true;
	else
		return false;
}

bool mxmDate::operator !=(const mxmDate& date) const {
	return !operator==(date);
}

bool mxmDate::operator <(const mxmDate& date) const {
	if((m_year < date.m_year) ||
		(m_year == date.m_year && m_month < date.m_month) ||
		(m_year == date.m_year && m_month == date.m_month && m_day < date.m_day))
		return true;
	else
		return false;
}

bool mxmDate::operator >=(const mxmDate &date) const {
	return !operator<(date);
}

bool mxmDate::operator <=(const mxmDate &date) const {
	return operator<(date) || operator==(date);
}

bool mxmDate::operator >(const mxmDate &date) const {
	return !operator<=(date);
}

bool mxmDate::isLeapYear() const {
	if(m_year % 400 == 0)
		return true;
	else if(m_year % 25 == 0)
		return false;
	else if(m_year % 4 == 0)
		return true;
	else
		return false;
}

// TODO: british time format should be am/pm

mxmString mxmDate::toString(mxmDate::EDateFormat format) const {
	char tmp[20];
	
	switch(format) {
		case eEuropean4:
			std::sprintf(tmp, "%02d.%02d.%04d", m_day, m_month, m_year);
			break;
		case eEuropean2:
			std::sprintf(tmp, "%02d.%02d.%02d", m_day, m_month, m_year % 100);
			break;
		case eBritish2:
			std::sprintf(tmp, "%02d/%02d/%02d", m_month, m_day, m_year % 100);
			break;
		case eBritish4:
			std::sprintf(tmp, "%02d/%02d/%04d", m_month, m_day, m_year);
			break;
		case eEuropean4Time:
			std::sprintf(tmp, "%02d.%02d.%04d %02d:%02d:%02d", m_day, m_month, m_year, m_hour, m_minute, m_second);
			break;
		case eEuropean2Time:
			std::sprintf(tmp, "%02d.%02d.%02d %02d:%02d:%02d", m_day, m_month, m_year % 100, m_hour, m_minute, m_second);
			break;
		case eBritish2Time:
			std::sprintf(tmp, "%02d/%02d/%02d %02d:%02d:%02d", m_month, m_day, m_year % 100, m_hour, m_minute, m_second);
			break;
		case eBritish4Time:
			std::sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d", m_month, m_day, m_year, m_hour, m_minute, m_second);
			break;
	}
	return tmp;
}

void mxmDate::addDays(int days) {

	int tmpDays = m_day + days;

	// check if it is in same month, call recursive if not
	switch(m_month) {
		case 2: {   // Feb, check for leap year
				if(isLeapYear()) {
					if(tmpDays > 29) {
						tmpDays -= 29;
						m_month++;
					} else {
						m_day = tmpDays;
						tmpDays = 0;
					}
				} else {
					if(tmpDays > 28) {
						tmpDays -= 28;
						m_month++;
					} else {
						m_day = tmpDays;
						tmpDays = 0;
					}
				}
			}
			break;
		case 1:
		case 3:
		case 5: 
		case 7:
		case 8:
		case 10:
			if(tmpDays > 31) {
				tmpDays -= 31;
				m_month++;
			} else {
				m_day = tmpDays;
				tmpDays = 0;
			}
			break;
		case 12:
			if(tmpDays > 31) {
				tmpDays -= 31;
				m_month = 1;
				m_year++;
			} else {
				m_day = tmpDays;
				tmpDays = 0;
			}
			break;		
		case 4:
		case 6:
		case 9:
		case 11:
			if(tmpDays > 30) {
				tmpDays -= 30;
				m_month++;
			} else {
				m_day = tmpDays;
				tmpDays = 0;
			}
			break;
	}
	if(tmpDays > 0)
		addDays(tmpDays);	
}

void mxmDate::addMonths(int months) {
	
	int tmpMonth = m_month + months;
	if(tmpMonth > 12) {
		tmpMonth -= 12;
		m_year++;
		addMonths(tmpMonth);
	} else
		m_month = tmpMonth;
}

void mxmDate::addYears(int years) {
	m_year += years;
}

mxmDate mxmDate::currentDate() {
	
   mxmDate current;

#ifdef MX_PLATFORM_WINDOWS
	
   SYSTEMTIME st;
	
   memset(&st, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&st);
	
   current.m_year   = st.wYear;
	current.m_month  = st.wMonth;
	current.m_day    = st.wDay;
	current.m_hour   = st.wHour;
	current.m_minute = st.wMinute;
	current.m_second = st.wSecond;

#else
	
   time_t ltime;
   tm t;
	
   time(&ltime);
   localtime_r(&ltime, &t);
	
   current.m_year   = t.tm_year + 1900;
	current.m_month  = t.tm_mon  + 1;
	current.m_day    = t.tm_mday;
	current.m_hour   = t.tm_hour;
	current.m_minute = t.tm_min;
	current.m_second = t.tm_sec;
#endif

	return current;
}
