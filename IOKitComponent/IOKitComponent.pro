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


MX_TARGET       = IOKitComponent
MX_SDK_ROOTDIR  = ..


TEMPLATE        = lib
QT              *= network
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


CONFIG(debug, debug|release) {
   win32:LIBS   *= mxFramework$${MX_DEBUG_SUFFIX}1.lib \
                   mxmQt$${MX_DEBUG_SUFFIX}1.lib \
                   mxm_core$${MX_DEBUG_SUFFIX}1.lib
   !win32:LIBS  *= -lmxFramework$${MX_DEBUG_SUFFIX} \
                   -lmxmQt$${MX_DEBUG_SUFFIX} \
                   -lmxm_core$${MX_DEBUG_SUFFIX}
} else {
   win32:LIBS   *= mxFramework1.lib \
                   mxmQt1.lib \
                   mxm_core1.lib
   !win32:LIBS  *= -lmxFramework \
                   -lmxmQt \
                   -lmxm_core
}


win32:DEFINES  *= MX_IOKIT_API_WINDOWS_EXPORTS


DEPENDPATH       =  ./include \
                    ../shared \
                    ../Framework/include \
                    ../mxmQt/include \
                    ../mxm/include \
                    \
                    ../../bin_dist/inc
INCLUDEPATH      =  $${DEPENDPATH}


DESTDIR          =  $${DESTDIR}/plugins


include(version.pri)
include(IOKitComponent.pri)

