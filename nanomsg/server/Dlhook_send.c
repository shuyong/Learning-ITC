#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

#include "Dlhook.h"

static const Dl_fps dlhook = {
    .dlopen  = dlopen,
    .dlclose = dlclose,
    .dlerror = dlerror,
    .dlsym   = dlsym,
};

void set_dlhook()
{
    char url[64] = {0};

    sprintf (url, DL_NOTE_FMT, (int)getpid());

    int fd;
    int rc;

    if ((fd = nn_socket(AF_SP, NN_PUSH)) < 0) {
        fprintf(stderr, "nn_socket: %s\n", nn_strerror(nn_errno()));
        exit(1);
    }

    if ((rc = nn_connect(fd, url)) < 0) {
        nn_close (fd);
        fprintf(stderr, "nn_connect: %s\n", nn_strerror(nn_errno()));
        exit(1);
    }

    if ((rc = nn_send(fd, &dlhook, sizeof(Dl_fps), 0)) < 0) {
        nn_close (fd);
        fprintf(stderr, "nn_send: %s\n", nn_strerror(nn_errno()));
        exit(1);
    }

    usleep(20 * 1000); // wait for messages to flush before shutting down

    nn_close (fd);
}

