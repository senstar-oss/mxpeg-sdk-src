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
HEADERS = ./include/mxcpcHuffmanTree.h \
          ./include/BufferedMxPEGDecoderBackEnd.h \
          ./include/DiagnosticsMxPEGTileReceiver.h \
          ./include/MxPEGParser.h \
          ./include/MxPEGRawFrameDumper.h \
          ./include/MxPEGScanDecoderSoftwareOnly.h \
          ./include/MxPEGScanDecoderSoftwareOnlyCore.h \
          ./include/MxPEGScanToJPEGConverterSoftwareOnly.h \
          ./include/SimpleMxPEGRenderer.h

#Source files
SOURCES = mxcpcHuffmanTree.cpp \
          BufferedMxPEGDecoderBackEnd.cpp \
          DiagnosticsMxPEGTileReceiver.cpp \
          MxPEGParser.cpp \
          MxPEGRawFrameDumper.cpp \
          MxPEGScanDecoderSoftwareOnly.cpp \
          MxPEGScanDecoderSoftwareOnlyCore.cpp \
          MxPEGScanToJPEGConverterSoftwareOnly.cpp \
          SimpleMxPEGRenderer.cpp \
          \
          MxPEGCoreComponent.cpp
