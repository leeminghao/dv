LOCAL_PATH := $(call my-dir)

common_static_libraries := \
    liblogwrap \
    libbase \
    libfec_rs \
    libmincrypt \
    libcrypto_static \
    libext4_utils_static \
    libsquashfs_utils

common_c_includes := \
    $(LOCAL_PATH)/include \
    external/fec \
    system/extras/ext4_utils \
    system/extras/squashfs_utils

include $(CLEAR_VARS)

LOCAL_CLANG := true
LOCAL_SANITIZE := integer
LOCAL_SRC_FILES := \
    dump.cpp \
    main.cpp \
    fec_open.cpp \
    fec_verity.cpp \
    fec_read.cpp \
    fec_process.cpp
LOCAL_MODULE := dv
LOCAL_MODULE_TAGS := optional
LOCAL_STATIC_LIBRARIES := $(common_static_libraries)
LOCAL_C_INCLUDES := $(common_c_includes)
LOCAL_CFLAGS := -DFEC_NO_KLOG
LOCAL_FORCE_STATIC_EXECUTABLE := true

include $(BUILD_EXECUTABLE)
