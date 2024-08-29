#           ///          //                                  C++ Cross Platform
#          /////        ////
#         /// XXX     XXX ///            ///////////   /////////     ///   ///
#        ///    XXX XXX    ///         ///             ///    ///   ///  ///
#       ///       XXX       ///         /////////     ///      //  //////
#      ///      XXX XXX      ///               ///   ///    ///   ///  ///
#     ////    XXX     XXX    ////    ////////////  //////////    ///   ///
#    ////                     ////
#   ////  M  O  B  O  T  I  X  ////////////////////////////////////////////////
#  //// Security Vision Systems //////////////////////////////////////////////
#
#  $Author: khe_admin $
#  $LastChangedBy: khe_admin $
#  $LastChangedDate: 2007-06-28 11:26:00 +0200 (Thu, 28 Jun 2007) $
#  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxgconv/trunk/mxgconv.cpp $
#
# ####
#    #
#    #  MOBOTIX MxPEG SDK
#    #
#    #  This file belongs to the C++ library released as part of the MxPEG SDK.
#    #
#    #  This software is licenced under the BSD licence,
#    #  http:#www.opensource.org/licenses/bsd-license.php
#    #
#    #  Copyright (c) 2005 - 2007, MOBOTIX AG
#    #  All rights reserved.
#    #
#    #  Redistribution and use in source and binary forms, with or without
#    #  modification, are permitted provided that the following conditions are
#    #  met:
#    #
#    #  - Redistributions of source code must retain the above copyright
#    #    notice, this list of conditions and the following disclaimer.
#    #
#    #  - Redistributions in binary form must reproduce the above copyright
#    #    notice, this list of conditions and the following disclaimer in the
#    #    documentation and/or other materials provided with the distribution.
#    #
#    #  - Neither the name of MOBOTIX AG nor the names of its contributors may
#    #    be used to endorse or promote products derived from this software
#    #    without specific prior written permission.
#    #
#    #  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#    #  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#    #  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#    #  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#    #  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#    #  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#    #  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#    #  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#    #  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#    #  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#    #  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#    #
# ####



MX_TARGET       = mxgconv
MX_SDK_ROOTDIR  = ..


TEMPLATE        = app
CONFIG          -= qt
#QT              *= network opengl
CONFIG          *= console
include($${MX_SDK_ROOTDIR}/settings/mx_common_settings.pri)


win32:DEFINES   *= MX_STATIC_LIB
win32:LIBS      *= ws2_32.lib user32.lib

unix:!macx:LIBS *= -ldl


DEPENDPATH       =  ../shared \
                    ../Framework/include \
                    ../mxmQt/include \
                    ../mxm/include \
                    ../mxpeg_sdk/include \
                    \
                    ../../bin_dist/inc
INCLUDEPATH      =  $${DEPENDPATH}


# IPP stuff... If you wish to use IPP
# (1) uncomment the lines below and set the MX_IPP_DIR environment variable to
#     point to your IPP installation
# (2) include the *IPP.cpp files in the compilation below
# (3) go to the file "<SDK_ROOT>/mxpeg_sdk/static_component_bootstrap.cpp" and
#     uncomment the lines for the *IPP components

#unix {
#  INCLUDEPATH   *= $(MX_IPP_DIR)/include
#}
#win32 {
#}
#
#unix {
#  macx:LIBS     *= -L$(MX_IPP_DIR)/lib
#  !macx:LIBS    *= -L$(MX_IPP_DIR)/sharedlib
#  LIBS          *= -lippcc -lippj -lippcore
#}
#win32 {
#  CONFIG(debug, debug|release) {
#    LIBS   *= MiniIPPdlld.lib
#  } else {
#    LIBS   *= MiniIPPdll.lib
#  }
#}


include(version.pri)


HEADERS          =  ../mxm/include/mxm/core/mxmApplication.h \
                    ../mxm/include/mxm/core/mxmAudioBlockInfo.h \
                    ../mxm/include/mxm/core/mxmBidirectionalIteratorInterface.h \
                    ../mxm/include/mxm/core/mxmBuffer.h \
                    ../mxm/include/mxm/core/mxmConditionVariableInterface.h \
                    ../mxm/include/mxm/core/mxmConfigSection.h \
                    ../mxm/include/mxm/core/mxmContainer.h \
                    ../mxm/include/mxm/core/mxmContainerInterface.h \
                    ../mxm/include/mxm/core/mxmDate.h \
                    ../mxm/include/mxm/core/mxmDecider.h \
                    ../mxm/include/mxm/core/mxmDefaultSynchronizationFactory.h \
                    ../mxm/include/mxm/core/mxmException.h \
                    ../mxm/include/mxm/core/mxmFrameInfo.h \
                    ../mxm/include/mxm/core/mxmHuffmanTableId.h \
                    ../mxm/include/mxm/core/mxmIPAddress.h \
                    ../mxm/include/mxm/core/mxmIntModN.h \
                    ../mxm/include/mxm/core/mxmInterface.h \
                    ../mxm/include/mxm/core/mxmIteratorInterface.h \
                    ../mxm/include/mxm/core/mxmList.h \
                    ../mxm/include/mxm/core/mxmLocalEndian.h \
                    ../mxm/include/mxm/core/mxmLockInterface.h \
                    ../mxm/include/mxm/core/mxmMutableBidirectionalIteratorInterface.h \
                    ../mxm/include/mxm/core/mxmMutableIteratorInterface.h \
                    ../mxm/include/mxm/core/mxmNonCopyable.h \
                    ../mxm/include/mxm/core/mxmNopStatusMessageHandler.h \
                    ../mxm/include/mxm/core/mxmNumberSet.h \
                    ../mxm/include/mxm/core/mxmObject.h \
                    ../mxm/include/mxm/core/mxmPerlStyleHash.h \
                    ../mxm/include/mxm/core/mxmQtSignal.h \
                    ../mxm/include/mxm/core/mxmQtSlot.h \
                    ../mxm/include/mxm/core/mxmRelationDecider.h \
                    ../mxm/include/mxm/core/mxmSmartHandle.h \
                    ../mxm/include/mxm/core/mxmSmartHandleCore.h \
                    ../mxm/include/mxm/core/mxmStatusMessageHandlerInterface.h \
                    ../mxm/include/mxm/core/mxmString.h \
                    ../mxm/include/mxm/core/mxmStringDecider.h \
                    ../mxm/include/mxm/core/mxmStringList.h \
                    ../mxm/include/mxm/core/mxmSynchronizationFactoryInterface.h \
                    ../mxm/include/mxm/core/mxmThreadConditionVariable.h \
                    ../mxm/include/mxm/core/mxmThreadLock.h \
                    ../mxm/include/mxm/core/mxmTime.h \
                    ../mxm/include/mxm/core/mxmTypeDecider.h \
                    ../mxm/include/mxm/core/mxmUndecodedMxPEGFrameDescriptor.h \
                    ../mxm/include/mxm/core/mxm_colorspaces.h \
                    ../mxm/include/mxm/core/mxm_core_platform_macros.h \
                    ../mxm/include/mxm/core/mxm_generic_stuff.h \
                    ../mxm/include/mxm/core/mxm_jpeg.h \
                    ../mxm/include/mxm/core/mxm_macros.h \
                    ../mxm/include/mxm/core/mxm_smart.h \
                    ../mxm/include/mxm/core/mxm_types.h \
                    \
                    ../mxmQt/include/mxmQt/mxm_qt.h \
                    ../mxmQt/include/mxmQt/mxm_qt_platform_macros.h \
                    \
                    ../Framework/include/Framework/Framework.h \
                    \
                    ../IOKitComponent/include/FileWriter.h \
                    ../IOKitComponent/include/GetConfigSection.h \
                    ../IOKitComponent/include/GetWebPage.h \
                    ../IOKitComponent/include/HTTPGetStreamSource.h \
                    ../IOKitComponent/include/HTTPGetStreamSourceCore.h \
                    ../IOKitComponent/include/KeyChain.h \
                    ../IOKitComponent/include/StreamFile.h \
                    \
                    ../MxPEGCoreComponent/include/BufferedMxPEGDecoderBackEnd.h \
                    ../MxPEGCoreComponent/include/DiagnosticsMxPEGTileReceiver.h \
                    ../MxPEGCoreComponent/include/MxPEGParser.h \
                    ../MxPEGCoreComponent/include/MxPEGRawFrameDumper.h \
                    ../MxPEGCoreComponent/include/MxPEGScanDecoderSoftwareOnly.h \
                    ../MxPEGCoreComponent/include/MxPEGScanDecoderSoftwareOnlyCore.h \
                    ../MxPEGCoreComponent/include/MxPEGScanToJPEGConverterSoftwareOnly.h \
                    ../MxPEGCoreComponent/include/SimpleMxPEGRenderer.h \
                    ../MxPEGCoreComponent/include/mxcpcHuffmanTree.h \
                    \
                    ../IPPAcceleratedMxPEGComponent/include/MxPEGScanDecoderIPP.h \
                    ../IPPAcceleratedMxPEGComponent/include/MxPEGScanDecoderIPPCore.h \
                    ../IPPAcceleratedMxPEGComponent/include/MxPEGScanToJPEGConverterIPP.h \
                    \
                    ../mxpeg_sdk/include/mxpeg_sdk/static_component_bootstrap.h


SOURCES          =  ../mxm/core/mxmApplication.cpp \
                    ../mxm/core/mxmAudioBlockInfo.cpp \
                    ../mxm/core/mxmBuffer.cpp \
                    ../mxm/core/mxmConfigSection.cpp \
                    ../mxm/core/mxmContainer.cpp \
                    ../mxm/core/mxmDate.cpp \
                    ../mxm/core/mxmDefaultSynchronizationFactory.cpp \
                    ../mxm/core/mxmException.cpp \
                    ../mxm/core/mxmFrameInfo.cpp \
                    ../mxm/core/mxmHuffmanTableId.cpp \
                    ../mxm/core/mxmIPAddress.cpp \
                    ../mxm/core/mxmIntModN.cpp \
                    ../mxm/core/mxmInterface.cpp \
                    ../mxm/core/mxmList.cpp \
                    ../mxm/core/mxmLocalEndian.cpp \
                    ../mxm/core/mxmNonCopyable.cpp \
                    ../mxm/core/mxmNopStatusMessageHandler.cpp \
                    ../mxm/core/mxmNumberSet.cpp \
                    ../mxm/core/mxmObject.cpp \
                    ../mxm/core/mxmPerlStyleHash.cpp \
                    ../mxm/core/mxmQtSignal.cpp \
                    ../mxm/core/mxmQtSlot.cpp \
                    ../mxm/core/mxmSmartHandleCore.cpp \
                    ../mxm/core/mxmString.cpp \
                    ../mxm/core/mxmStringDecider.cpp \
                    ../mxm/core/mxmStringList.cpp \
                    ../mxm/core/mxmThreadConditionVariable.cpp \
                    ../mxm/core/mxmThreadLock.cpp \
                    ../mxm/core/mxmTime.cpp \
                    ../mxm/core/mxmTypeDecider.cpp \
                    ../mxm/core/mxmUndecodedMxPEGFrameDescriptor.cpp \
                    ../mxm/core/mxm_generic_stuff.cpp \
                    \
                    #../mxmQt/qt/mxm_qt.cpp \
                    \
                    ../Framework/Framework.cpp \
                    \
                    ../IOKitComponent/FileWriter.cpp \
                    #../IOKitComponent/GetConfigSection.cpp \
                    #../IOKitComponent/GetWebPage.cpp \
                    #../IOKitComponent/HTTPGetStreamSource.cpp \
                    #../IOKitComponent/HTTPGetStreamSourceCore.cpp \
                    #../IOKitComponent/KeyChain.cpp \
                    ../IOKitComponent/StreamFile.cpp \
                    \
                    ../MxPEGCoreComponent/BufferedMxPEGDecoderBackEnd.cpp \
                    ../MxPEGCoreComponent/DiagnosticsMxPEGTileReceiver.cpp \
                    ../MxPEGCoreComponent/MxPEGParser.cpp \
                    ../MxPEGCoreComponent/MxPEGRawFrameDumper.cpp \
                    ../MxPEGCoreComponent/MxPEGScanDecoderSoftwareOnly.cpp \
                    ../MxPEGCoreComponent/MxPEGScanDecoderSoftwareOnlyCore.cpp \
                    ../MxPEGCoreComponent/MxPEGScanToJPEGConverterSoftwareOnly.cpp \
                    #../MxPEGCoreComponent/SimpleMxPEGRenderer.cpp \
                    ../MxPEGCoreComponent/mxcpcHuffmanTree.cpp \
                    \
                    #../IPPAcceleratedMxPEGComponent/MxPEGScanDecoderIPP.cpp \
                    #../IPPAcceleratedMxPEGComponent/MxPEGScanDecoderIPPCore.cpp \
                    #../IPPAcceleratedMxPEGComponent/MxPEGScanToJPEGConverterIPP.cpp \
                    \
                    ../mxpeg_sdk/static_component_bootstrap.cpp \
                    \
                    mxgconv.cpp

