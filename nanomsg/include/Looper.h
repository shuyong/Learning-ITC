
#ifndef _LOOPER_H_
#define _LOOPER_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
    /**
     * Result from Looper_pollOnce() and Looper_pollAll():
     * The poll was awoken using wake() before the timeout expired
     * and no callbacks were executed and no other file descriptors were ready.
     */
    POLL_WAKE = -1,

    /**
     * Result from Looper_pollOnce() and Looper_pollAll():
     * One or more callbacks were executed.
     */
    POLL_CALLBACK = -2,

    /**
     * Result from Looper_pollOnce() and Looper_pollAll():
     * The timeout expired.
     */
    POLL_TIMEOUT = -3,

    /**
     * Result from Looper_pollOnce() and Looper_pollAll():
     * An error occurred.
     */
    POLL_ERROR = -4,
};

typedef struct Looper {
    int fd;
} Looper;

Looper *Looper_ctor(const char *url);

void Looper_dtor(Looper *looper);

/**
 * Waits for events to be available, with optional timeout in milliseconds.
 * Invokes callbacks for all file descriptors on which an event occurred.
 *
 * If the timeout is zero, returns immediately without blocking.
 * If the timeout is negative, waits indefinitely until an event appears.
 *
 * Returns POLL_WAKE if the poll was awoken using wake() before
 * the timeout expired and no callbacks were invoked and no other file
 * descriptors were ready.
 *
 * Returns POLL_CALLBACK if one or more callbacks were invoked.
 *
 * Returns POLL_TIMEOUT if there was no data before the given
 * timeout expired.
 *
 * Returns POLL_ERROR if an error occurred.
 *
 * Returns a value >= 0 containing an identifier if its file descriptor has data
 * and it has no callback function (requiring the caller here to handle it).
 * In this (and only this) case outFd, outEvents and outData will contain the poll
 * events and data associated with the fd, otherwise they will be set to NULL.
 *
 * This method does not return until it has finished invoking the appropriate callbacks
 * for all file descriptors that were signalled.
 */
int Looper_pollOnce (Looper *looper, int timeoutMillis);

/**
 * Like pollOnce(), but performs all pending callbacks until all
 * data has been consumed or a file descriptor is available with no callback.
 * This function will never return POLL_CALLBACK.
 */
int Looper_pollAll(Looper *looper, int timeoutMillis);

/**
 * Wakes the poll asynchronously.
 *
 * This method can be called on any thread.
 * This method returns immediately.
 */
void Looper_wake(Looper *looper);

#ifdef __cplusplus
}
#endif

#endif
