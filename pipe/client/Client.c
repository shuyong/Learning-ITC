#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#include "Message.h"
#include "Client.h"

static void *handleDummy (Message* message) SOFTFP
{
    return message;
}

int Client0(MessageQueue *queue, struct Dl_fps* dlhook)
{
    MessageHandler handler[1];
    MessageDlhook MessageDlhook[1];
    Message* message = (Message* )MessageDlhook;

    handler->handleMessage = (void *(*) (Message* ) SOFTFP)handleDummy;
    message->what = MSG_DLHOOK;
    MessageDlhook->dlopen  = NULL;
    MessageDlhook->dlclose = NULL;
    MessageDlhook->dlerror = NULL;
    MessageDlhook->dlsym   = NULL;

    (void)MessageQueue_push_message(queue, (Message* )message, handler);

    dlhook->dlopen  = MessageDlhook->dlopen ;
    dlhook->dlclose = MessageDlhook->dlclose;
    dlhook->dlerror = MessageDlhook->dlerror;
    dlhook->dlsym   = MessageDlhook->dlsym  ;

    return dlhook->dlopen != NULL;
}

extern pid_t gettid(void);

    typedef struct MessageAdd {
        Message common;
	int x;
	int y;
	int result;
    } MessageAdd;

    static void *handleMessageAdd (MessageAdd* message) SOFTFP
    {
	int x = message->x;
	int y = message->y;
	int result;
        
	result = x + y;

	message->result = result;

        pid_t tid = gettid();

        printf ("%s %d: result = %d\n", __FUNCTION__, tid, message->result);

	return 0;
    };

int Client1(MessageQueue *queue, int x, int y)
{
    MessageHandler handler[1];
    MessageAdd message[1];

    handler->handleMessage = (void *(*) (Message* ) SOFTFP)handleMessageAdd;
    message->x = x;
    message->y = y;

    (void)MessageQueue_push_message(queue, (Message* )message, handler);

    return message->result;
}

    typedef struct MessageMul {
        Message common;
	int x;
	int y;
	int result;
    } MessageMul;

    static void *handleMessageMul (MessageMul* message) SOFTFP
    {
	int x = message->x;
	int y = message->y;
	int result;
        
	result = x * y;

	message->result = result;

        pid_t tid = gettid();

        printf ("%s %d: result = %d\n", __FUNCTION__, tid, message->result);

	return 0;
    };

int Client2(MessageQueue *queue, int x, int y)
{
    MessageHandler handler[1];
    MessageMul message[1];

    handler->handleMessage = (void *(*) (Message* ) SOFTFP)handleMessageMul;
    message->x = x;
    message->y = y;

    (void)MessageQueue_push_message(queue, (Message* )message, handler);

    return message->result;
}

int Client_init(struct io_port* port1, MessageQueue **dlhook_queue, struct io_port* port2, MessageQueue **call_queue)
{
    *dlhook_queue = MessageQueue_ctor(port1->rfd, port1->wfd);
    *call_queue = MessageQueue_ctor(port2->rfd, port2->wfd);

    return 0;
}
