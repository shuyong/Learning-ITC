
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A message that can be posted to a Looper.
 */
typedef struct Message {
    /* The message type. (interpretation is left up to the handler) */
    int what;
} Message;

/*
 * Attribute for softfp-calling-style functions
 */
#ifdef __ARM_PCS_VFP
#    define SOFTFP __attribute__((pcs("aapcs")))
#else
#    define SOFTFP
#endif

/**
 * Interface for a Looper message handler.
 */
typedef struct MessageHandler {
    /**
     * Handles a message.
     */
    void *(*handleMessage) (Message* message) SOFTFP;
} MessageHandler;

typedef struct MessageQueue {
    int fd;
} MessageQueue;

MessageQueue *MessageQueue_ctor(const char *url);

void MessageQueue_dtor(MessageQueue *queue);

/**
 * Enqueues a message to be processed by the specified handler.
 *
 * The handler must not be null.
 * This method can be called on any thread.
 */
void *MessageQueue_sendMessage(MessageQueue *queue, const MessageHandler* handler, Message* message);

#ifdef __cplusplus
}
#endif

#endif
