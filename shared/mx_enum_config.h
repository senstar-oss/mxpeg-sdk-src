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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/mx_enum_config.h $
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



#ifndef __MX_ENUM_CONFIG_H__
#define __MX_ENUM_CONFIG_H__



namespace mx {
   
/*!
	\file				MxRecordingTypes.h
	\brief			Type definitions for mapping HTTPI API infos for represenation in GUI
	\author			D. Finkler
*/

// ---------------------------------------------------------------------------------
//! Recording On/Off or dependant from signals or master
// ---------------------------------------------------------------------------------

enum ERecordingSwitch {
	ersOff                    = 1,
	ersOn                     = 1 << 1,
	
	ersSignalInputHigh        = 1 << 2,
	ersSignalInputLow         = 1 << 3,
	ersCustomSignal1          = 1 << 4,
	ersCustomSignal2          = 1 << 5,
	ersCombinedSignals        = 1 << 6,
	ersMaster                 = 1 << 7,
	ersSlave                  = 1 << 8,
	
	ersMultipleValues         = 1 << 31
};

// ---------------------------------------------------------------------------------
//! Recording modes
// ---------------------------------------------------------------------------------

enum ERecordingMode {
	ermSnapshot               = 1,
	ermEvent                  = 2,
	ermContinuous             = 4,
	
	ermMultipleValues         = 1 << 31
};


// ---------------------------------------------------------------------------------
//! Recording events
// ---------------------------------------------------------------------------------

enum ERecordingEvent {
	ereEventCounter            = 1,			//!< EC
	ereEventLogic              = 1 << 1,	//!< EL
	ereEventLogic2             = 1 << 2,	//!< EL2
	ereVideoMotion             = 1 << 3,	//!< VM
	ereVideoMotion2            = 1 << 4,	//!< VM2
	ereMicrophone              = 1 << 5,	//!< MI
	ereSignalInput             = 1 << 6,	//!< SI
	ereSignalInput2            = 1 << 7,	//!< SI2
	ereSignalInput3            = 1 << 8,	//!< SI3
	ereSignalInput4            = 1 << 9,	//!< SI4
	erePeriodicEvent           = 1 << 10,	//!< PE
	ereTimerTask               = 1 << 11,	//!< TT
	ereUserClick               = 1 << 12,	//!< UC
	ereButton                  = 1 << 13,	//!< BT
	ereIPReceive               = 1 << 14,	//!< RC
	ereTemperature             = 1 << 15,	//!< TP
	ereIllumination            = 1 << 16,	//!< IL
	ereRandomEvent             = 1 << 17,	//!< RD
	erePassivInfrared				= 1 << 18,	//!< PI
	ereIRRemoteControl			= 1 << 19,	//!< IR
	ereCOMIn							= 1 << 20,	//!< CI
	
	ereMultipleValues          = 1 << 31,

	ereAllEvents               = 0xFFFFFFFF

};

/*! recording targets
 */
enum ERecordingTarget {
	eTargetNone                  = 0,
	eRemoteNFSFileServer         = 1,
	eRemoteCIFSFileServer        = 2,
	eRemoteSMBServer             = 4,
	eUSBStorageDevice            = 8,
	eCFFlashCard                 = 16
};


enum ECamEye {
	  eUndefEye							=	0,
	  eRightEye							=	1,
	  eLeftEye							=	2,
	  eBothEyes							=	3,	
	  eAutoEye							=	4
};

enum ENightImproveMode {
	
	eNightUndef							=	0,
	eNightAuto							=	1 << 1,
	eNightOff							=	1 << 2,
	eNightOn								=	1 << 3

};

enum EExposureWindowMode {
	
	eWindowUndef						=	0,
	eWindowAll							=	1 << 1,
	eWindowQuater						=	1 << 2,
	eWindowCenter						=	1 << 3,
	eWindowMoreToCome					=	1 << 4
};


enum EImageResolution{

	eResUndef							= 0,
	eResMega								= 1 << 1,
	eResVGA								= 1 << 2,
	eResCIF								= 1 << 3,
	eResD1PAL							= 1 << 4,
	eResPTZ								= 1 << 5,
	eResPDA								= 1 << 6,
	eResCustom							= 1 << 7,
	eResQCIF								= 1 << 8,
	eRes2CIFEXP							= 1 << 9,
	eRes4CIF								= 1 << 10,
	eRes1280x1024						= 1 << 11,
	eRes1280x720						= 1 << 12,
	eRes640x360							= 1 << 13
};

enum EStreamMode{

	eStreamUndef						= 0,
	eStreamQuality						= 1 << 1,
	eStreamFast							= 1 << 2
};

enum EStreamCodex{

	eCodexUndef							= 0,
	eMxPEG								= 1 << 1,
	eMJPEG								= 1 << 2
};

enum ENoiseSurp{

	eNoiseUndef							=0,
	eNoiseOff							=1 << 1,
	eNoiseLow							=1 << 2,
	eNoiseMedium						=1 << 3,
	eNoiseHight							=1 << 4
};


enum EWhiteBMode{

	eWhiteBUndef						=0,
	eWhiteBAuto							=1 << 1,
	eWhiteBCloud						=1 << 2,
	eWhiteBOutdoor						=1 << 3,
	eWhiteBSun							=1 << 4,
	eWhiteBNeon							=1 << 5,
	eWhiteBBulb							=1 << 6,
	eWhiteBGray							=1 << 7,
	eWhiteBNarrow						=1 << 8,
	eWhiteBIndoor						=1 << 9,
	eWhiteBIndoor2						=1 << 10,
	eWhiteBBulb2						=1 << 11,
	eWhiteBOutdoor2					=1 << 12,
	eWhiteBHold							=1 << 13
};

enum EContrastMode{

	eContrastUndef							=0,
	eContrastAuto							=1 << 1,
	eContrastSimple						=1 << 2,
	eContrastAdaptive						=1 << 3,
	eContrastOff							=1 << 4,

};


enum EPTZModes{

	ePTZUndef								= 0,
	eAbsolutPTZMode						= 1 << 1,
	eRelativeMode							= 1 << 2,

};

enum EPTZType{

	ePTZTypeUndef							= 0,
	eHardwarePTZ							= 1 << 1,
	eSoftwarePTZ							= 1 << 2,

};

enum EPTZMotionMode{

	ePTZMotionModeUndef					= 0,
	ePTZup									= 1 << 1,
	ePTZdown									= 1 << 2,
	ePTZleft									= 1 << 3,
	ePTZright								= 1 << 4,
	ePTZupleft								= 1 << 5,
	ePTZupright								= 1 << 6,
	ePTZdownleft							= 1 << 7,
	ePTZdownright							= 1 << 8,
	ePTZgotoPos								= 1 << 9,

};


};



#endif   // __MX_ENUM_CONFIG_H__