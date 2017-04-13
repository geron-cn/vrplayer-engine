SRC_PATH := $(call my-dir)



# libpng
include $(CLEAR_VARS)
LOCAL_MODULE := png
LOCAL_MODULE_FILENAME := png
LOCAL_PATH := $(SRC_PATH)/3d/libs/$(TARGET_ARCH_ABI)
LOCAL_EXPORT_C_INCLUDES := $(SRC_PATH)/3d/png
LOCAL_SRC_FILES := libpng.a
include $(PREBUILT_STATIC_LIBRARY)

# zlib
include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_MODULE_FILENAME := zlib
LOCAL_PATH := $(SRC_PATH)/3d/libs/$(TARGET_ARCH_ABI)
LOCAL_SRC_FILES := libz.a
LOCAL_EXPORT_C_INCLUDES := $(SRC_PATH)/3d/zlib
include $(PREBUILT_STATIC_LIBRARY)

#ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
#	LOCAL_CFLAGS += -mfloat -abi=soft
#endif

LOCAL_PATH := $(SRC_PATH)
include $(CLEAR_VARS)     
LOCAL_CFLAGS += -D__ANDROID__ 
LOCAL_CXXFLAGS += -D__ANDROID__ -DUSE_FILE32API -std=c++11 -frtti -Wno-switch-enum -Wno-switch -fexceptions
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lGLESv1_CM -lOpenSLES -lz -latomic
LOCAL_C_INCLUDES := $(SRC_PATH) $(SRC_PATH)/3d $(SRC_PATH)/3d/png  $(SRC_PATH)/3d/zlib $(SRC_PATH)/FileUtils
LOCAL_SRC_FILES := $(wildcard $(SRC_PATH)/3d/*.cpp) \
             $(SRC_PATH)/jnihelper.cpp \
             $(wildcard $(SRC_PATH)/FileUtils/*.cpp)


LOCAL_STATIC_LIBRARIES :=  android_native_app_glue zlib png
LOCAL_WHOLE_LIBRARIES += zlib png
LOCAL_MODULE := scenerenderer
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)



