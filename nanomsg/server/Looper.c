
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#include "Looper.h"
#include "Message.h"
#include "Message_private.h"

#define MAXCOUNT	16

Looper *Looper_ctor(const char *url)
{
    Looper *looper;

    looper = (Looper *)malloc(sizeof(Looper));

    int fd;

    fd = nn_socket (AF_SP, NN_REP);
    if (fd < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        exit (-1);
    }

    if (nn_bind (fd, url) < 0) {
        fprintf (stderr, "nn_bind: %s\n", nn_strerror (nn_errno ()));
        nn_close (fd);
        exit (-1);
    }

    looper->fd = fd;

    return looper;
}

void Looper_dtor(Looper *looper)
{
    int fd;

    fd = looper->fd;
    nn_close (fd);

    free (looper);
}

int Looper_pollOnce (Looper *looper, int timeoutMillis)
{
    int fd;
    (void)timeoutMillis;

    fd = looper->fd;

    MessagePair queue[1];
    int rc;

    rc = nn_recv (fd, queue, sizeof (MessagePair), 0);
    if (rc < 0) {
        /*  Any error here is unexpected. */
        fprintf (stderr, "nn_recv: %s\n", nn_strerror (nn_errno ()));
        exit (-1);
    }
    assert (rc == sizeof (MessagePair));

    /* execute the handler */
    void *ret = queue->handler->handleMessage (queue->message);

    rc = nn_send (fd, &ret, sizeof (void *), 0);
    if (rc < 0) {
        /*  There are several legitimate reasons this can fail.
            We note them for debugging purposes, but then ignore
            otherwise.  If the socket is closed or failing, we will
            notice in recv above, and exit then. */
        fprintf (stderr, "nn_send: %s (ignoring)\n",
            nn_strerror (nn_errno ()));
        exit (-1);
    }
    assert (rc == sizeof (void *));

    return 0;
}

int Looper_pollAll(Looper *looper, int timeoutMillis)
{
    int fd;
    (void)timeoutMillis;

    fd = looper->fd;

    MessagePair queue[MAXCOUNT];
    int rc;
    int count;

    rc = nn_recv (fd, queue, sizeof (queue), 0);
    if (rc < 0) {
        /*  Any error here is unexpected. */
        fprintf (stderr, "nn_recv: %s\n", nn_strerror (nn_errno ()));
        exit (-1);
    }
    assert (rc % sizeof (MessagePair) == 0);
    count = rc / sizeof (MessagePair);

    for (int i = 0; i < count; i++) {
        /* execute the handler */
        void *ret = queue[i].handler->handleMessage (queue->message);

        rc = nn_send (fd, ret, sizeof (void *), 0);
        if (rc < 0) {
            /*  There are several legitimate reasons this can fail.
                We note them for debugging purposes, but then ignore
                otherwise.  If the socket is closed or failing, we will
                notice in recv above, and exit then. */
            fprintf (stderr, "nn_send: %s (ignoring)\n",
                nn_strerror (nn_errno ()));
            exit (-1);
        }
        assert (rc == sizeof (void *));
    }

    return 0;
}

