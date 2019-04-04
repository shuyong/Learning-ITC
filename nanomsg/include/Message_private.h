#ifndef _MESSAGE_PRIVATE_H_
#define _MESSAGE_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MessagePair {
    Message        *message;
    MessageHandler *handler;
} MessagePair;

#ifdef __cplusplus
}
#endif

#endif
