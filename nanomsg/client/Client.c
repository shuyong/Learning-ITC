#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#include "Message.h"
#include "Client.h"

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

    (void)MessageQueue_sendMessage(queue, handler, (Message* )message);

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

    (void)MessageQueue_sendMessage(queue, handler, (Message* )message);

    return message->result;
}

