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


#Header files
HEADERS = ./include/MxPEGScanDecoderIPP.h \
          ./include/MxPEGScanDecoderIPPCore.h \
          ./include/MxPEGScanToJPEGConverterIPP.h
          

#Source files
SOURCES = MxPEGScanDecoderIPP.cpp \
          MxPEGScanDecoderIPPCore.cpp \
          MxPEGScanToJPEGConverterIPP.cpp \
          \
          IPPAcceleratedMxPEGComponent.cpp
