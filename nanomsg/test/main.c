
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "Server.h"
#include "Client.h"

#include "Dlhook.h"

extern pid_t gettid(void);

static void *start_dlhook(void *unused)
{
    (void)unused;
    set_dlhook();

    return 0;
}

static void print_dlhook(Dl_fps *dl_fps)
{
    printf ("dlopen  = %p\n", dl_fps->dlopen);
    printf ("dlclose = %p\n", dl_fps->dlclose);
    printf ("dlerror = %p\n", dl_fps->dlerror);
    printf ("dlsym   = %p\n", dl_fps->dlsym);
}

int main (int argc, const char *argv[])
{
    (void)argc; (void)argv;
    char* url_fmt = "inproc://function_%d";
    char url[64] = {0};

    /*  snprintf would be safer, but the above check protects us. */
    sprintf (url, url_fmt, (int)getpid());

    pthread_t dlhook_thread;
    if(pthread_create(&dlhook_thread, NULL, start_dlhook, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    printf ("1st call get_dlhook\n");
    print_dlhook(get_dlhook());
    printf ("2nd call get_dlhook\n");
    print_dlhook(get_dlhook());
    printf ("\n");

    /* this variable is our reference to the thread */
    pthread_t server_thread;

    /* create a thread which executes handler(message) */
    if(pthread_create(&server_thread, NULL, (void *(*) (void *))start_server, url)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    pid_t tid = gettid();

    MessageQueue *queue = MessageQueue_ctor(url);

    int x, y, result;

    x = 2;
    y = 3;

    result = Client1(queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    result = Client2(queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    x = 5;
    y = 7;

    result = Client1(queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    result = Client2(queue, x, y);
    printf ("%s %d: x = %d, y = %d, result = %d\n", __FUNCTION__, tid, x, y, result);

    MessageQueue_dtor(queue);

    return 0;
}
