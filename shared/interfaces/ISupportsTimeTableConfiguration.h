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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsTimeTableConfiguration.h $
//
///////



#ifndef __MX_ISUPPORTSTIMETABLECONFIGURATION_H__
#define __MX_ISUPPORTSTIMETABLECONFIGURATION_H__



#define MX_ISUPPORTSTIMETABLECONFIGURATION_ID "ISupportsTimetableConfiguration"



#include "IInterface.h"
#include "ITimetableEntry.h"
#include "ICustomDay.h"


namespace mx {

  //! Allows for configuring <tt>MX</tt> time tables.
  /*!
   *  \ingroup mxsdk_interfaces
   */
  class ISupportsTimetableConfiguration : virtual IInterface {
  public:
	  /*! add a previously created time table entry
	   *  The unique identifier of the time table is its
	   *  name (see ITimetable documentation). Therefore
	   *  it is an error to add an entry without a name
	   *  or an existing name.
	   */
	  virtual EErrorCode addEntry(mxm::smart<ITimetableEntry> entry) = 0;
	  /*! remove a time table entry. If the entry was not found, 
	   *  mx::eObjectNotPresent is returned, mx::eOK otherwise.
	   */
	  virtual EErrorCode removeEntry(mxm::smart<ITimetableEntry> entry) = 0;
	  /*! remove a time table entry. If the entry was not found, 
	   *  mx::eObjectNotPresent is returned, mx::eOK otherwise.
	   *  /param name the name of the time table entry
	   */
	  virtual EErrorCode removeEntry(const mxmString& name) = 0;
	  /*! list all known time table entries
	   */
	  virtual mxm::smart<mxmStringList> listAllEntries() = 0;
	  /*! retrieve a time table entry. If the entry is not found a
	   *  null pointer is returned.
	   *  /param name the name of the time table entry
	   */	  
	  virtual mxm::smart<ITimetableEntry> entry(const mxmString& name) = 0;
	  /*! create a new time table entry
	   */
	  virtual mxm::smart<ITimetableEntry> createTimetableEntry() = 0;
	  /*! clear the list of time table entries
	   */
	  virtual void clearTimetableEntries() = 0;
	  /*! returns <tt>true</tt> if the driver understands the concept of custom days.
	   */
	  virtual bool isCustomDaySupported() = 0;
	  /*! create a new custom day entry
	   */
	  virtual mxm::smart<ICustomDay> createCustomDay() = 0;
	  /*! add a previously created custom day entry
	   *  The unique identifier of the custom day is its
	   *  name (see ICustomDay documentation). Therefore
	   *  it is an error to add an entry without a name
	   *  or an existing name.
	   */
	  virtual EErrorCode addCustomDay(mxm::smart<ICustomDay> customDay) = 0;
	  /*! remove a custom day entry. If the entry was not found, 
	   *  mx::eObjectNotPresent is returned, mx::eOK otherwise.
	   */
	  virtual EErrorCode removeCustomDay(mxm::smart<ICustomDay> customDay) = 0;
	  /*! remove a custom day entry. If the entry was not found, 
	   *  mx::eObjectNotPresent is returned, mx::eOK otherwise.
	   *  /param name the name of the ustom day entry
	   */
	  virtual EErrorCode removeCustomDay(const mxmString& name) = 0;
	  /*! list all known custom time entries
	   */
	  virtual mxm::smart<mxmStringList> customDays() = 0;
	  /*! retrieve a custom day entry. If the entry is not found a
	   *  null pointer is returned.
	   *  /param name the name of the time table entry
	   */	  
	  virtual mxm::smart<ICustomDay> customDay(const mxmString& name) = 0;
	  /*! clear the list of custom days
	   */
	  virtual void clearCustomDays() = 0;	

  
  };
};



#endif   // __MX_ISUPPORTSTIMETABLECONFIGURATION_H__
