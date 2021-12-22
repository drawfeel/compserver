LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := nerccmd

LOCAL_CFLAGS := -Werror

LOCAL_LDFLAGS += -s -Wl

LOCAL_CPPFLAGS := -Werror -Wextra -Wdate-time -Weffc++ -fvisibility=hidden -fno-common -Wall
LOCAL_CPPFLAGS += -Wfloat-equal -Wformat=2 -Wshadow -fstack-protector-all

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES += nerccmd.cpp  \
                   utils.cpp  \
				   CmdSocket.cpp \
				   process_mpls_dump.cpp \
				   process_display.cpp \
				   process_ledpanel.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)


LOCAL_SHARED_LIBRARIES :=liblog libutils libcutils \
                         libmpls_dump \
						 libnerc_display \
						 libnerc_ledpanel


#LOCAL_STATIC_LIBRARIES := libnerc_ntp_diff

ifeq (1,$(filter 1,$(shell echo "$$(( $(PLATFORM_SDK_VERSION) >= 26 ))" )))
LOCAL_PROPRIETARY_MODULE := true
endif

include $(BUILD_EXECUTABLE)