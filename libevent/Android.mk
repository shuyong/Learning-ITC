LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#-UDEBUG
LOCAL_C_INCLUDES := include libevent/include
LOCAL_CFLAGS     := -Wall -Wextra -fvisibility=hidden -D_FORTIFY_SOURCE=2 -DDEBUG
#LOCAL_LDFLAGS    := -Wl,-O2 -Wl,--as-needed -Wl,-Bsymbolic

LOCAL_ARM_NEON   := true

LIBEVNET_SOURCES := \
	buffer.c \
	bufferevent.c \
	bufferevent_filter.c \
	bufferevent_pair.c \
	bufferevent_ratelim.c \
	bufferevent_sock.c\
	epoll.c \
	evdns.c \
	event.c \
	event_tagging.c \
	evmap.c \
	evrpc.c \
	evthread.c \
	evthread_pthread.c \
	evutil.c \
	evutil_rand.c \
	evutil_time.c \
	http.c \
	listener.c \
	log.c \
	poll.c \
	select.c \
	signal.c \
	strlcpy.c \
	$(empty)

LOCAL_MODULE := event
LOCAL_SRC_FILES := $(addprefix libevent/, $(LIBEVNET_SOURCES))

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

#-UDEBUG
LOCAL_C_INCLUDES:= include libevent/include
LOCAL_CFLAGS    := -Wall -Wextra -fvisibility=hidden -D_FORTIFY_SOURCE=2 -DDEBUG
LOCAL_LDFLAGS   := -Wl,-O2 -Wl,--as-needed -Wl,-Bsymbolic
#LOCAL_SHARED_LIBRARIES := event
LOCAL_STATIC_LIBRARIES := event
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/libevent/include

LOCAL_ARM_NEON  := true

LOCAL_MODULE    := event-read-fifo
LOCAL_SRC_FILES := \
	server/event-read-fifo.c \
	$(empty)

include $(BUILD_EXECUTABLE)

