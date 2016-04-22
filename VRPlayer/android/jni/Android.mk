SAMPLE_PATH := $(call my-dir)/../../src

# external-deps
GAMEPLAY_DEPS := $(call my-dir)/../../../GamePlay/external-deps/lib/android/$(TARGET_ARCH_ABI)

# ffmpeg
FFMPEG_DEPS := $(call my-dir)/../../ffmpeg/android/$(TARGET_ARCH_ABI)

# libgameplay
LOCAL_PATH := $(call my-dir)/../../../GamePlay/gameplay/android/libs/$(TARGET_ARCH_ABI)
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := libgameplay.so
include $(PREBUILT_SHARED_LIBRARY)

# libgameplay-deps
LOCAL_PATH := $(GAMEPLAY_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay-deps
LOCAL_SRC_FILES := libgameplay-deps.a
include $(PREBUILT_STATIC_LIBRARY)

# ffmpeg
LOCAL_PATH := $(FFMPEG_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg_codec
LOCAL_SRC_FILES := libavcodec.a
LOCAL_EXPORT_C_INCLUDES := ../ffmpeg/include
include $(PREBUILT_STATIC_LIBRARY)

# ffmpeg
LOCAL_PATH := $(FFMPEG_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg_filter
LOCAL_SRC_FILES := libavfilter.a
LOCAL_EXPORT_C_INCLUDES := ../ffmpeg/include
include $(PREBUILT_STATIC_LIBRARY)

# ffmpeg
LOCAL_PATH := $(FFMPEG_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg_util
LOCAL_SRC_FILES := libavutil.a
LOCAL_EXPORT_C_INCLUDES := ../ffmpeg/include
include $(PREBUILT_STATIC_LIBRARY)

# ffmpeg
LOCAL_PATH := $(FFMPEG_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg_sample
LOCAL_SRC_FILES := libswresample.a
LOCAL_EXPORT_C_INCLUDES := ../ffmpeg/include
include $(PREBUILT_STATIC_LIBRARY)

# ffmpeg
LOCAL_PATH := $(FFMPEG_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg_scale
LOCAL_SRC_FILES := libswscale.a
LOCAL_EXPORT_C_INCLUDES := ../ffmpeg/include
include $(PREBUILT_STATIC_LIBRARY)

# ffmpeg
LOCAL_PATH := $(FFMPEG_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg_format
LOCAL_SRC_FILES := libavformat.a
LOCAL_EXPORT_C_INCLUDES := ../ffmpeg/include
include $(PREBUILT_STATIC_LIBRARY)


# VRPlayer
LOCAL_PATH := $(SAMPLE_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := VRPlayer
LOCAL_SRC_FILES := ../../GamePlay/gameplay/src/gameplay-main-android.cpp VRGame.cpp ALMovieAudioFactory.cpp audiodecoder.cpp libavwrapper.cpp SphereVideoSurround.cpp StereoRender.cpp videoplayer.cpp videostate.cpp


LOCAL_CPPFLAGS += -std=c++11 -frtti -Wno-switch-enum -Wno-switch
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lz
LOCAL_CFLAGS    := -D__ANDROID__ -I"../../GamePlay/external-deps/include" -I"../../GamePlay/gameplay/src" -I"../ffmpeg/include"
LOCAL_STATIC_LIBRARIES := android_native_app_glue libgameplay-deps ffmpeg_format ffmpeg_codec ffmpeg_filter ffmpeg_util ffmpeg_sample ffmpeg_scale
LOCAL_SHARED_LIBRARIES := gameplay
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
