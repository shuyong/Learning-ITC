#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

#include "Dlhook.h"

static Dl_fps dlhook = {0};

static Dl_fps *dlhook_recv();
static Dl_fps *dlhook_norm();

static Dl_fps *(*get_dlhook_)() = dlhook_recv;

static Dl_fps *dlhook_recv()
{
    char url[64] = {0};

    sprintf (url, DL_NOTE_FMT, (int)getpid());

    int fd;
    int rc;

    if ((fd = nn_socket(AF_SP, NN_PULL)) < 0) {
        fprintf(stderr, "nn_socket: %s\n", nn_strerror(nn_errno()));
        exit(1);
    }

    if ((rc = nn_bind(fd, url)) < 0) {
        nn_close (fd);
        fprintf(stderr, "nn_bind: %s\n", nn_strerror(nn_errno()));
        exit(1);
    }

   if ((rc = nn_recv(fd, &dlhook, sizeof(Dl_fps), 0)) < 0) {
        nn_close (fd);
        fprintf(stderr, "nn_recv: %s\n", nn_strerror(nn_errno()));
        exit(1);
   }

    nn_close (fd);

    get_dlhook_ = dlhook_norm;

    return &dlhook;
}

static Dl_fps *dlhook_norm()
{
    return &dlhook;
}

Dl_fps *get_dlhook()
{
    return get_dlhook_();
}

