
.phony: all install

NDK_DIR = $(HOME)/android/android-ndk-r19c

all:
	@$(NDK_DIR)/ndk-build \
		NDK_PROJECT_PATH=. \
		APP_BUILD_SCRIPT=./Android.mk \
		NDK_APPLICATION_MK=./Application.mk \
		$(empty)

clean:
	@mkdir -p bak && cp -a --parents include/{evconfig-private.h,event2/event-config.h} bak
	@rm -rf obj libs include
	@mv bak include

install:
	@test -d libs             || mkdir libs
	@test -d libs/arm64-v8a   || mkdir libs/arm64-v8a
	@test -d libs/armeabi-v7a || mkdir libs/armeabi-v7a
	@test -d libs/x86         || mkdir libs/x86
	@test -d libs/x86_64      || mkdir libs/x86_64
	@test -d include          || mkdir include 
	@test -d include/event2   || mkdir include/event2
	@cp -a obj/local/arm64-v8a/libevent.a   libs/arm64-v8a/
	@cp -a obj/local/armeabi-v7a/libevent.a libs/armeabi-v7a/
	@cp -a obj/local/x86/libevent.a         libs/x86/
	@cp -a obj/local/x86_64/libevent.a      libs/x86_64/
	@cp -a libevent/event.h                 include/
	@cp -a libevent/include/event2/*.h      include/event2/

