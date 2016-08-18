LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := LemonNet

LOCAL_SRC_FILES := \
    Socket.cpp \
    UdpSocket.cpp \
    TcpSocket.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../base


include $(BUILD_SHARED_LIBRARY)

