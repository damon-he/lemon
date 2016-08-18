LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := LemonThread

LOCAL_SRC_FILES := \
    Thread.cpp \
    ThreadPool.cpp 

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../base


LOCAL_CPPFLAGS += -DANDROID


include $(BUILD_SHARED_LIBRARY)

