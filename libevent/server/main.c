
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define __USE_GNU	// for strdup
#include <string.h>

#include "Server.h"
#include "Client.h"

static struct Dl_fps dlhook[1] = {0};

extern pid_t gettid(void);

static void print_dlhook(Dl_fps *dlhook)
{
    printf ("dlopen  = %p\n", dlhook->dlopen );
    printf ("dlclose = %p\n", dlhook->dlclose);
    printf ("dlerror = %p\n", dlhook->dlerror);
    printf ("dlsym   = %p\n", dlhook->dlsym  );
}

extern int usleep(__useconds_t usec);
extern void pipe_init(struct io_port* cport, struct io_port* sport);

int main (int argc, const char *argv[])
{
    (void)argc; (void)argv;

    struct io_port cport[2];
    struct io_port sport[2];

    pipe_init(&cport[0], &sport[0]);
    pipe_init(&cport[1], &sport[1]);

    char *args[8] = {0};
    char str[32];
    args[0] = (char*)5;
    args[1] = strdup("dlhook");
    /*  snprintf would be safer, but the above check protects us. */
    memset (str, 0, sizeof(str));
    sprintf (str, "%d", sport[0].rfd);
    args[2] = strdup(str);
    memset (str, 0, sizeof(str));
    sprintf (str, "%d", sport[0].wfd);
    args[3] = strdup(str);
    memset (str, 0, sizeof(str));
    sprintf (str, "%d", sport[1].rfd);
    args[4] = strdup(str);
    memset (str, 0, sizeof(str));
    sprintf (str, "%d", sport[1].wfd);
    args[5] = strdup(str);

    pthread_t server_thread;
    if(pthread_create(&server_thread, NULL, start_server, (void*)args)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    usleep (20 * 1000);

    MessageQueue *dlhook_queue;
    MessageQueue *call_queue;

    Client_init(&cport[0], &dlhook_queue, &cport[1], &call_queue);

    /* test case 1 */
    printf ("1st call get dlhook\n");
    Client0(dlhook_queue, dlhook);
    print_dlhook(dlhook);
    printf ("2nd call get dlhook\n");
    Client0(dlhook_queue, dlhook);
    print_dlhook(dlhook);
    printf ("\n");

    /* test case 2 */
    pid_t tid = gettid();

    int x, y, result;

    x = 2;
    y = 3;

    result = Client1(call_queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    result = Client2(call_queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    /* test case 3 */
    x = 5;
    y = 7;

    result = Client1(call_queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    result = Client2(call_queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    MessageQueue_dtor(dlhook_queue);
    MessageQueue_dtor(call_queue);

    return 0;
}
