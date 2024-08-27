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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISupportsPTZControl.h $
//
///////



#ifndef __MX_ISUPPORTSPTZCONTROL_H__
#define __MX_ISUPPORTSPTZCONTROL_H__



#define MX_ISUPPORTSPTZCONTROL_ID "ISupportsPTZControl"
        
#include "IInterface.h"
#include "../mx_types.h"
#include "../mx_error_codes.h"
#include "../mx_enum_config.h"

#include <mxm/core/mxmString.h>
#include <mxm/core/mxmList.h>

#include <vector>

namespace mx {


  //! Allows to control PTZ devices!.
  /*!
   *  \ingroup mxsdk_interfaces
   */
class ISupportsPTZControl : virtual IInterface {

public:

	//@{
	/** Gerneral info and settigs "*/
	virtual bool						isPTZSupported()									= 0;	
	//@}

	//@{
	/** Set PTZ interface modes "*/
	virtual EPTZModes					mode()												= 0; //!< Current PTZ mode
	virtual EErrorCode				setMode(EPTZModes mode)							= 0;
	virtual u32							supportedModes()									= 0;
	//@}

	//@{
	/** Elementary PTZ  controls "relative to current position"*/
	virtual void						stop()													= 0; 
	virtual void						up( float velocity )									= 0; //!< velocity interval [0,1]
	virtual void						down( float velocity )								= 0; //!< velocity interval [0,1]
	virtual void						right( float velocity )								= 0; //!< velocity interval [0,1]
	virtual void						left( float velocity )								= 0; //!< velocity interval [0,1]
	virtual void						upleft( float velo_up, float velo_left )		= 0; //!< velocity interval [0,1]
	virtual void						upright( float velo_up, float velo_right)		= 0; //!< velocity interval [0,1]
	virtual void						downleft( float velo_down, float velo_left)	= 0; //!< velocity interval [0,1]
	virtual void						downright( float velo_down, float velo_right)= 0; //!< velocity interval [0,1]
	virtual void						zoom( float velocity )								= 0; //!< velocity interval [-1,1]
	//@}

	//@{
	/** Elementary PTZ controls "absolut to current position"*/
	virtual void						pos(int &phi, int &theta  )				= 0;
	virtual EErrorCode				gotoPos(int phi, int theta )				= 0;
	virtual void						setReferencePoint()							= 0;
	//@}





};


};



#endif   // __MX_ISUPPORTSPTZCONTROL_H__
