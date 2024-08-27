#           ///          //                                  MX Camera Software
#          /////        ////                                          modularMX
#         /// XXX     XXX ///
#        ///    XXX XXX    ///     $HeadURL:$
#       ///       XXX       ///     $LastChangedBy:$
#      ///      XXX XXX      ///     $LastChangedDate:$
#     ////    XXX     XXX    ////     $Author:$
#    ////                     ////
#   ////  M  O  B  O  T  I  X  ////////////////////////////////////////////////
#  //// Security Vision Systems //////////////////////////////////////////////


MX_TARGET       = mxmQt
MX_SDK_ROOTDIR  = ../..

TEMPLATE        = lib
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


CONFIG(debug, debug|release) {
  win32:LIBS    *= mxm_core$${MX_DEBUG_SUFFIX}1.lib
  !win32:LIBS   *= -lmxm_core$${MX_DEBUG_SUFFIX}
} else {
  win32:LIBS    *= mxm_core1.lib
  !win32:LIBS   *= -lmxm_core
}


win32:DEFINES *= MXM_QT_API_WINDOWS_EXPORTS


DEPENDPATH  =  ../include \
               ../../mxm/include
INCLUDEPATH =  $${DEPENDPATH}


include(qt.pri)
include(version.pri)
