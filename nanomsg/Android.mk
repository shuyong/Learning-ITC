LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)
#LOCAL_MODULE := nanomsg-prebuilt
#LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libnanomsg.so
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := nng-prebuilt
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libnng.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

#-UDEBUG
LOCAL_C_INCLUDES:= include
LOCAL_CFLAGS    := -Wall -Wextra -Werror -fvisibility=hidden -D_FORTIFY_SOURCE=2 -DDEBUG
LOCAL_LDFLAGS   := -Wl,-O2 -Wl,--as-needed -Wl,-Bsymbolic
#LOCAL_SHARED_LIBRARIES := nanomsg-prebuilt
LOCAL_STATIC_LIBRARIES := nng-prebuilt
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/nng/compat

LOCAL_ARM_NEON  := true

LOCAL_MODULE    := remote_call
LOCAL_SRC_FILES := \
	client/Client.c client/Dlhook_recv.c client/Message.c \
	server/Dlhook_send.c server/Looper.c server/Server.c \
	test/main.c \
	$(empty)

include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)

