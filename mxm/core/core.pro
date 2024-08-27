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


MX_TARGET       = mxm_core
MX_SDK_ROOTDIR  = ../..

TEMPLATE        = lib
CONFIG          -= qt
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


win32:DEFINES *= MXM_CORE_API_WINDOWS_EXPORTS

win32:LIBS    *= WS2_32.lib user32.lib

INCLUDEPATH =  ../include 
DEPENDPATH  =  ../include


include(core.pri)
include(version.pri)
