MX_SDK_ROOTDIR  = .


TEMPLATE = subdirs
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


SUBDIRS      = mxm \
               mxmQt \
               Framework \
               IOKitComponent \
               MxPEGCoreComponent \
               mxpeg-decoder


!mx_no_ipp {
  SUBDIRS   *= IPPAcceleratedMxPEGComponent
}
