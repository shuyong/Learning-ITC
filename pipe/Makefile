LOCAL_C_INCLUDES = -I include
CFLAGS = -Wall -Wextra -g -O2 -std=c99 $(LOCAL_C_INCLUDES)
CFLAGS += -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -fvisibility=hidden -D_FORTIFY_SOURCE=2 -DDEBUG -Wa,--noexecstack -Wformat -Werror=format-security -fPIE
LDFLAGS = -Wl,--gc-sections -Wl,--build-id -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings -fPIE -pie  -Wl,-O2 -Wl,--as-needed -Wl,-Bsymbolic
LDLIBS = -levent -lpthread -ldl

TARGETS = remote_call

OBJS = \
       client/Client.o client/Message.o \
       pipe/bridge.o pipe/io.o \
       server/Looper.o server/Servant.o server/Server.o \
       test/gettid.o test/main.o \
       $(empty)

all: $(TARGETS)

$(TARGETS) : $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGETS) $(OBJS) $(LDLIBS)
	@echo "Link $(TARGETS) done."

clean :
	@echo "Clean objects ..."
	@rm -f $(TARGETS) $(OBJS)

%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@

%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $< -o $@

%.o : %.S
	@$(CC) -c $(CFLAGS) $< -o $@


.PHONY: all clean
