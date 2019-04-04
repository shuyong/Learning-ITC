#ifndef _LOOPER_H_
#define _LOOPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DL_NOTE_FMT	"inproc://dlhook_%d"

typedef struct Dl_fps {
    void *(*dlopen) (const char *filename, int flags);
    int   (*dlclose)(void *handle);
    char *(*dlerror)(void);
    void *(*dlsym)  (void *handle, const char *symbol);
} Dl_fps;

Dl_fps *get_dlhook();
void    set_dlhook();

#ifdef __cplusplus
}
#endif

#endif
