#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Message.h"
#include "Looper.h"
#include "Server.h"

void *start_server (void *args)
{
    int argc;
    char **argv;
    int *ip = args;
    char **cpp = args;

    argc = *ip;
    argv = cpp + 1;

    (void)server_main (argc, argv);

    return 0;
}

int server_main (int argc, char *argv[])
{
    if (argc != 5) {
        fprintf (stderr, "Usage: %s rfd1 wfd1 rfd2 wfd2\n", argv[0]);
	exit (-1);
    }

    int rfd1, rfd2;
    int wfd1, wfd2;

    rfd1 = atoi(argv[1]);
    wfd1 = atoi(argv[2]);

    rfd2 = atoi(argv[3]);
    wfd2 = atoi(argv[4]);

    Looper *looper = Looper_ctor();

    Looper_add_channel (looper, rfd1, wfd1, servant_handler);
    Looper_add_channel (looper, rfd2, wfd2, callee_handler);

    int rc;
    do {
        rc = Looper_poll_message (looper);
    } while (rc == 0);

    Looper_dtor(looper);

    return 0;
}
