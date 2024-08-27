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


MX_TARGET        = mxFramework
MX_SDK_ROOTDIR   = ..

TEMPLATE         = lib
CONFIG          -= qt
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


CONFIG(debug, debug|release) {
  win32:LIBS    *= mxm_core$${MX_DEBUG_SUFFIX}1.lib USER32.LIB
  !win32:LIBS   *= -lmxm_core$${MX_DEBUG_SUFFIX}
} else {
  win32:LIBS    *= mxm_core1.lib USER32.LIB
  !win32:LIBS   *= -lmxm_core
}


win32:DEFINES   *= MX_FRAMEWORK_API_WINDOWS_EXPORTS


DEPENDPATH       = ../shared \
                   ../mxm/include \
                   ./include \
                   \
                   ../bin_dist/inc
INCLUDEPATH      = $${DEPENDPATH}


UI_DIR           = ui


include(version.pri)
include(Framework.pri)
