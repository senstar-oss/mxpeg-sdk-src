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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/shared/trunk/interfaces/ISimpleViewer.h $
//
///////



#ifndef __MX_ISIMPLEVIEWER_H__
#define __MX_ISIMPLEVIEWER_H__



#define MX_ISIMPLEVIEWER_ID "ISimpleViewer"



#include "IQtWidget.h"
#include "ICanUseDialogs.h"
#include "ICanUseKeyChain.h"

#include <mxm/core/mxmQtSignal.h>
#include <mxm/core/mxmQtSlot.h>



class QString;



namespace mx {
   
   //! Encodes the <i>concept</i> of a simple video stream viewer widget for
   //! demo purposes.
   /*!
    *  \ingroup mxsdk_interfaces
    *
    *  <tt>MxSDK</tt> camera drivers will provide viewer widgets implementing
    *  this interface in order to deliver <tt>[psc]</tt>'s "preview"
    *  functionality.
    *
    *  \author <tt>[khe]</tt> Kai Hergenroether
    */
   class ISimpleViewer : public virtual IQtWidget,
                         public virtual ICanUseDialogs,
                         public virtual ICanUseKeyChain {
      
    public:
      //! Changes the window title prefix.
      virtual void setWindowTitlePrefix(const QString& window_title_prefix) = 0;
      
      //! Toggles framerate measure mode.
      virtual void enableFramerateMeasureMode(bool enabled,
                                              int measure_interval_ms = 10000)
                    = 0;
      //! Toggles scale-to-fit feature.
      virtual void enableScaleToFit(bool enabled) = 0;
      
      //! Connects to the (driver-)preconfigured stream source and begins
      //! playback.
      /*!
       *  If the viewer was already streaming, it is stopped prior to
       *  reconnection.
       */
      virtual void play() = 0;
      //! For signal connection.
      virtual mxmQtSlot playSlot() = 0;
      //! Disconnects a potential stream source and switches to disconnected
      //! state.
      virtual void stop() = 0;
      //! For signal connection.
      virtual mxmQtSlot stopSlot() = 0;
      
      //! Returns the frame counter for the last stream played back.
      virtual int lastPlaybackFrameCounter() = 0;
      
      //! <tt>(float)</tt>-signature signal reporting a new framerate measure.
      virtual mxmQtSignal framerateMeasuredSignal() = 0;
      //! Signal emitted once when the first frame has come in after a call to
      //! playCameraLiveStream().
      virtual mxmQtSignal streamingStartedSignal() = 0;
      //! Signal emitted once when streaming has stopped.
      virtual mxmQtSignal streamingStoppedSignal() = 0;
      
      //! Adds an item to the viewer's context menu that is raised by
      //! right-clicking into the display.
      virtual void addMenuItem(const QString& item_text,
                               mxmQtSlot target_slot)   = 0;
      //! Adds a horizontal separator to the viewer's popup menu.
      virtual void addMenuSeparator() = 0;
   };
};



#endif   // __MX_ISIMPLEVIEWER_H__

      