LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := LemonCrypt

LOCAL_SRC_FILES := \
    AES128Crypt.cpp \
    aes.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../base


include $(BUILD_SHARED_LIBRARY)

