#ifndef _CLIENT_H_
#define _CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Message.h"

int Client1(MessageQueue *queue, int x, int y);
int Client2(MessageQueue *queue, int x, int y);

#ifdef __cplusplus
}
#endif

#endif
