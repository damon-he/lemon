LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := LemonBase

LOCAL_SRC_FILES := \
    File.cpp \
    Timer.cpp \
    Log.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../thread


LOCAL_CPPFLAGS += -DANDROID  -Wno-error=format-security

LOCAL_SHARED_LIBRARIES := libLemonThread

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

