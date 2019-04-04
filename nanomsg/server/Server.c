#include <assert.h>
#include <stdio.h>

#include "Server.h"
#include "Looper.h"

void *start_server (const char *url)
{
    Looper *looper = Looper_ctor(url);

    while (1) {
        int rc;
        rc = Looper_pollOnce (looper, 0);
	(void)rc;
    }

    Looper_dtor(looper);

    return 0;
}
