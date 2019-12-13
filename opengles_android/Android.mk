# Build the unit tests.
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(PLATFORM_VERSION), 4.4.4)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_4
endif

ifeq ($(PLATFORM_VERSION), 5.1)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_5
endif

ifeq ($(PLATFORM_VERSION), 5.1.1)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_5
endif

ifeq ($(PLATFORM_VERSION), 6.0.1)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_6
endif

ifeq ($(PLATFORM_VERSION), 6.0)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_6
endif

ifeq ($(PLATFORM_VERSION), 7.1.1)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_7
endif

ifeq ($(PLATFORM_VERSION), 7.1.2)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_7
endif

ifeq ($(PLATFORM_VERSION), 8.1.0)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_8
endif

ifeq ($(PLATFORM_VERSION), 9)
	LOCAL_CPPFLAGS += -DPLATFORM_VERSION_9
endif


LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := opengl_c

LOCAL_SRC_FILES := \
	GLUtil.cpp \
	main.cpp 
 
LOCAL_SHARED_LIBRARIES := \
	libEGL \
	libGLESv2 \
	libbinder \
	libcutils \
	libgui \
	libmedia \
	libstagefright \
	libstagefright_foundation \
	libstagefright_omx \
	libsync \
	libui \
	libutils \
	liblog
 
LOCAL_C_INCLUDES := \
	frameworks/av/media/libstagefright \
	frameworks/av/media/libstagefright/include \
	$(TOP)/frameworks/native/include/media/openmax \
 
#LOCAL_CFLAGS += -Werror -Wall
LOCAL_CLANG := true
 
#LOCAL_32_BIT_ONLY := true

 LOCAL_LDFLAGS := -llog -lEGL -lGLESv2 -ljnigraphics -lGLESv3 -fprofile-arcs

include $(BUILD_EXECUTABLE)  
