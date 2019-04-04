LOCAL_C_INCLUDES = 
CFLAGS = -Wall -Wextra -g -O2 -std=c99 $(LOCAL_C_INCLUDES)
CFLAGS += -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wno-invalid-command-line-argument -Wno-unused-command-line-argument  -DANDROID -Wall -Wextra -fvisibility=hidden -D_FORTIFY_SOURCE=2 -DDEBUG -D__ANDROID_API__=21 -Wa,--noexecstack -Wformat -Werror=format-security -fPIE
LDFLAGS = -Wl,--gc-sections -Wl,--build-id -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings -fPIE -pie  -Wl,-O2 -Wl,--as-needed -Wl,-Bsymbolic
LDLIBS = -levent -lpthread -ldl

TARGETS = remote_call

OBJS = \
       Client.o Message.o \
       bridge.o io.o \
       Looper.o Servant.o Server.o \
       gettid.o main.o \
       $(empty)

all: $(TARGETS)

$(TARGETS) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGETS) $(OBJS) $(LDLIBS)

clean :
	rm -f $(TARGETS) $(OBJS)

%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@

%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $< -o $@

%.o : %.S
	@$(CC) -c $(CFLAGS) $< -o $@


.PHONY: all clean
