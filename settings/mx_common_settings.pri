#           ///          //                                                MxSDK
#          /////        ////
#         /// XXX     XXX ///
#        ///    XXX XXX    ///     $HeadURL:$
#       ///       XXX       ///     $LastChangedBy:$
#      ///      XXX XXX      ///     $LastChangedDate:$
#     ////    XXX     XXX    ////     $Author:$
#    ////                     ////
#   ////  M  O  B  O  T  I  X  ////////////////////////////////////////////////
#  //// Security Vision Systems //////////////////////////////////////////////


DA_DIR = $${MX_SDK_ROOTDIR}
count(DA_DIR, 0) {
  
  error(MX_SDK_ROOTDIR environment variable not defined!)
}


MX_DEBUG_SUFFIX    =  _debug

CONFIG             *= $$(MX_SDK_CONFIG)
CONFIG             *= exceptions rtti


mx_release_build {
  CONFIG           *= release
  !win32:CONFIG    -= debug debug_and_release
} else {
  CONFIG           *= debug
  !win32:CONFIG    -= release
}


CONFIG(debug, debug|release) {
  TARGET           =  $${MX_TARGET}$${MX_DEBUG_SUFFIX}
  DEFINES          *= __MX_DEBUG__ \
                      __MXM_DEBUG__
  OBJECTS_DIR      =  obj/debug
} else {
  TARGET           =  $${MX_TARGET}
  OBJECTS_DIR      =  obj/release
}


win32:DEFINES      *= MX_PLATFORM_WINDOWS \
                      _CRT_SECURE_NO_DEPRECATE


win32:DESTDIR      = $${MX_SDK_ROOTDIR}/bin_dist/bin/win32
macx:DESTDIR       = $${MX_SDK_ROOTDIR}/bin_dist/bin/macx
unix:!macx:DESTDIR = $${MX_SDK_ROOTDIR}/bin_dist/bin/linux


LIBS               *= -L$$DESTDIR

# to work around the problem with dlopen() and RTTI...
unix:!macx:LIBS    *= -Wl,-E


MOC_DIR         =  moc
RCC_DIR         =  rcc
