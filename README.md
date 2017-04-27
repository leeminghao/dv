# dv

dump verity informations

## Build

add -DFEC_NO_KLOG into `Android.mk`

path: system/extras/libfec
```
include $(CLEAR_VARS)
LOCAL_CFLAGS := $(common_cflags) -DFEC_NO_KLOG
LOCAL_C_INCLUDES := $(common_c_includes)
LOCAL_CLANG := true
LOCAL_SANITIZE := integer
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_MODULE := libfec
LOCAL_SRC_FILES := $(common_src_files)
LOCAL_STATIC_LIBRARIES := \
    libfec_rs \
    libext4_utils_static \
    libsquashfs_utils \
    libcutils \
    $(common_static_libraries)
include $(BUILD_STATIC_LIBRARY)
```