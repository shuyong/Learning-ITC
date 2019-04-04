#ifndef _CLIENT_H_
#define _CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Message.h"

typedef struct Dl_fps {
    void *(*dlopen) (const char *filename, int flags);
    int   (*dlclose)(void *handle);
    char *(*dlerror)(void);
    void *(*dlsym)  (void *handle, const char *symbol);
} Dl_fps;

extern int Client0(MessageQueue *queue, struct Dl_fps* dlhook);
extern int Client1(MessageQueue *queue, int x, int y);
extern int Client2(MessageQueue *queue, int x, int y);
extern int Client_init(struct io_port* port1, MessageQueue **dlhook_queue, struct io_port* port2, MessageQueue **call_queue);

#ifdef __cplusplus
}
#endif

#endif
