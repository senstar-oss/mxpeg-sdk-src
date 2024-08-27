#           ///          //                                   MX Camera Software
#          /////        ////                                           modularMX
#         /// XXX     XXX ///
#        ///    XXX XXX    ///     $HeadURL:$
#       ///       XXX       ///     $LastChangedBy:$
#      ///      XXX XXX      ///     $LastChangedDate:$
#     ////    XXX     XXX    ////     $Author:$
#    ////                     ////
#   ////  M  O  B  O  T  I  X  /////////////////////////////////////////////////
#  //// Security Vision Systems ///////////////////////////////////////////////


MX_TARGET       = IPPAcceleratedMxPEGComponent
MX_SDK_ROOTDIR  = ..


TEMPLATE        = lib
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


mx_no_ipp {
  
  error(IPP support is disabled in MX_SDK_CONFIG!)
}
DA_DIR = $$(MX_IPP_DIR)
count(DA_DIR, 0) {
  
  error(MX_IPP_DIR environment variable not defined!)
}


CONFIG(debug, debug|release) {
   win32:LIBS   *= mxFramework$${MX_DEBUG_SUFFIX}1.lib \
                   mxm_core$${MX_DEBUG_SUFFIX}1.lib
   !win32:LIBS  *= -lmxFramework$${MX_DEBUG_SUFFIX} \
                   -lmxm_core$${MX_DEBUG_SUFFIX}
} else {
   win32:LIBS   *= mxFramework.lib \
                   mxm_core1.lib
   !win32:LIBS  *= -lmxFramework \
                   -lmxm_core
}


win32:DEFINES   *= MX_IPPACCELERATEDMXPEG_API_WINDOWS_EXPORTS


DEPENDPATH       =  ./include \
                    ../shared \
                    \
                    ../mxm/include \
                    ../Framework/include \
                    \
                    ../../bin_dist/inc
INCLUDEPATH      =  $${DEPENDPATH}


unix {
  INCLUDEPATH   *= $(MX_IPP_DIR)/include
}
win32 {
}

unix {
  macx:LIBS     *= -L$(MX_IPP_DIR)/lib
  !macx:LIBS    *= -L$(MX_IPP_DIR)/sharedlib
  LIBS          *= -lippcc -lippj -lippcore
}
win32 {
  CONFIG(debug, debug|release) {
    LIBS   *= MiniIPPdlld.lib
  } else {
    LIBS   *= MiniIPPdll.lib
  }
}


DESTDIR          =  $${DESTDIR}/plugins


include(version.pri)
include(IPPAcceleratedMxPEGComponent.pri)

