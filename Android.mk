LOCAL_PATH := $(call my-dir)

LEMON_SRC_PATH := $(LOCAL_PATH)

include $(LEMON_SRC_PATH)/lemon/thread/Android.mk
include $(LEMON_SRC_PATH)/lemon/base/Android.mk
include $(LEMON_SRC_PATH)/lemon/net/Android.mk
include $(LEMON_SRC_PATH)/lemon/crypt/Android.mk

