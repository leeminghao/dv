LOCAL_PATH := $(call my-dir)

common_static_libraries := \
    liblogwrap \
    libfec \
    libfec_rs \
    libbase \
    libmincrypt \
    libcrypto_static \
    libext4_utils_static \
    libsquashfs_utils

include $(CLEAR_VARS)

LOCAL_CLANG := true
LOCAL_SANITIZE := integer
LOCAL_SRC_FILES := main.cpp
LOCAL_MODULE := dv
LOCAL_MODULE_TAGS := optional
LOCAL_STATIC_LIBRARIES := $(common_static_libraries)
LOCAL_C_INCLUDES := \
  system/extras/libfec/include
LOCAL_FORCE_STATIC_EXECUTABLE := true

include $(BUILD_EXECUTABLE)
