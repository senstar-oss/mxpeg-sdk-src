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


MX_TARGET       = MxPEGCoreComponent
MX_SDK_ROOTDIR  = ..


TEMPLATE        = lib
QT              *= opengl
CONFIG          *= opengl
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


CONFIG(debug, debug|release) {
   win32:LIBS   *= mxFramework$${MX_DEBUG_SUFFIX}1.lib \
                   mxm_core$${MX_DEBUG_SUFFIX}1.lib
   !win32:LIBS  *= -lmxFramework$${MX_DEBUG_SUFFIX} \
                   -lmxm_core$${MX_DEBUG_SUFFIX}
} else {
   win32:LIBS   *= mxFramework1.lib \
                   mxm_core1.lib
   !win32:LIBS  *= -lmxFramework \
                   -lmxm_core
}


win32:DEFINES   *= MX_MXPEGCORE_API_WINDOWS_EXPORTS


DEPENDPATH       =  ./include \
                    ../shared \
                    \
                    ../mxm/include \
                    ../Framework/include \
                    \
                    ../../bin_dist/inc
INCLUDEPATH      =  $${DEPENDPATH}


DESTDIR          =  $${DESTDIR}/plugins


include(version.pri)
include(MxPEGCoreComponent.pri)

