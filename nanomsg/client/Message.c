#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#include "Message.h"
#include "Message_private.h"

MessageQueue *MessageQueue_ctor(const char *url)
{
    MessageQueue *queue;
    
    queue = (MessageQueue *)malloc(sizeof(MessageQueue));

    int fd;

    fd = nn_socket (AF_SP, NN_REQ);
    if (fd < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        exit (-1);
    }

    if (nn_connect (fd, url) < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        nn_close (fd);
        exit (-1);        
    }

    queue->fd = fd;

    return queue;
}

void MessageQueue_dtor(MessageQueue *queue)
{
    int fd;

    fd = queue->fd;
    nn_close (fd);

    free (queue);
}

void *MessageQueue_sendMessage(MessageQueue *queue, const MessageHandler* handler, Message* message)
{
    MessagePair sendMessage[1];

    sendMessage->message = (Message        *)message;
    sendMessage->handler = (MessageHandler *)handler;

    int fd;
    int rc;

    fd = queue->fd;

    rc = nn_send (fd, sendMessage, sizeof (MessagePair), 0);
    if (rc < 0) {
        fprintf (stderr, "nn_send: %s\n", nn_strerror (nn_errno ()));
        exit (-1);
    }
    assert (rc == sizeof (MessagePair));

    void *ret;
    rc = nn_recv (fd, &ret, sizeof (void *), 0);
    if (rc < 0) {
        fprintf (stderr, "nn_recv: %s\n", nn_strerror (nn_errno ()));
        exit (-1);
    }
    assert (rc == sizeof (void *));

    return ret;
}


