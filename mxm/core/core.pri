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
HEADERS = \
    ../include/mxm/core/mxmAudioBlockInfo.h \
    ../include/mxm/core/mxmApplication.h \
    ../include/mxm/core/mxmBidirectionalIteratorInterface.h \
    ../include/mxm/core/mxmBuffer.h \
    ../include/mxm/core/mxmConfigSection.h \
    ../include/mxm/core/mxmContainer.h \
    ../include/mxm/core/mxmContainerInterface.h \
    ../include/mxm/core/mxmDate.h \
    ../include/mxm/core/mxmDecider.h \
    ../include/mxm/core/mxmDefaultSynchronizationFactory.h \
    ../include/mxm/core/mxmException.h \
    ../include/mxm/core/mxmFrameInfo.h \
    ../include/mxm/core/mxmHuffmanTableId.h \
    ../include/mxm/core/mxmInterface.h \
    ../include/mxm/core/mxmIntModN.h \
    ../include/mxm/core/mxmIPAddress.h \
    ../include/mxm/core/mxmIteratorInterface.h \
    ../include/mxm/core/mxmList.h \
    ../include/mxm/core/mxmLocalEndian.h \
    ../include/mxm/core/mxmLockInterface.h \
    ../include/mxm/core/mxmMutableBidirectionalIteratorInterface.h \
    ../include/mxm/core/mxmMutableIteratorInterface.h \
    ../include/mxm/core/mxmNonCopyable.h \
    ../include/mxm/core/mxmNopStatusMessageHandler.h \
    ../include/mxm/core/mxmNumberSet.h \
    ../include/mxm/core/mxmObject.h \
    ../include/mxm/core/mxmPerlStyleHash.h \
    ../include/mxm/core/mxmQtSignal.h \
    ../include/mxm/core/mxmQtSlot.h \
    ../include/mxm/core/mxmRelationDecider.h \
    ../include/mxm/core/mxmSmartHandle.h \
    ../include/mxm/core/mxmSmartHandleCore.h \
    ../include/mxm/core/mxmStatusMessageHandlerInterface.h \
    ../include/mxm/core/mxmString.h \
    ../include/mxm/core/mxmStringDecider.h \
    ../include/mxm/core/mxmStringList.h \
    ../include/mxm/core/mxmThreadConditionVariable.h \
    ../include/mxm/core/mxmThreadLock.h \
    ../include/mxm/core/mxmTime.h \
    ../include/mxm/core/mxmTypeDecider.h \
    ../include/mxm/core/mxmUndecodedMxPEGFrameDescriptor.h \
    ../include/mxm/core/mxm_generic_stuff.h \
    ../include/mxm/core/mxm_macros.h \
    ../include/mxm/core/mxm_core_platform_macros.h \
    ../include/mxm/core/mxm_smart.h \
    ../include/mxm/core/mxm_types.h 


#Source files
SOURCES = \
    ./mxmAudioBlockInfo.cpp \
    ./mxmApplication.cpp \
    ./mxmBuffer.cpp \
    ./mxmContainer.cpp \
    ./mxmConfigSection.cpp \
    ./mxmDate.cpp \
    ./mxmDefaultSynchronizationFactory.cpp \
    ./mxmException.cpp \
    ./mxmFrameInfo.cpp \
    ./mxmHuffmanTableId.cpp \
    ./mxmInterface.cpp \
    ./mxmIntModN.cpp \
    ./mxmIPAddress.cpp \
    ./mxmList.cpp \
    ./mxmLocalEndian.cpp \
    ./mxmNonCopyable.cpp \
    ./mxmNopStatusMessageHandler.cpp \
    ./mxmNumberSet.cpp \
    ./mxmObject.cpp \
    ./mxmPerlStyleHash.cpp \
    ./mxmQtSignal.cpp \
    ./mxmQtSlot.cpp \
    ./mxmSmartHandleCore.cpp \
    ./mxmString.cpp \
    ./mxmStringDecider.cpp \
    ./mxmStringList.cpp \
    ./mxmThreadConditionVariable.cpp \
    ./mxmThreadLock.cpp \
    ./mxmTime.cpp \
    ./mxmTypeDecider.cpp \
    ./mxmUndecodedMxPEGFrameDescriptor.cpp \
    ./mxm_generic_stuff.cpp
