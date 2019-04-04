#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "half_duplex.h" /* For name of the named-pipe */

int main(int argc, char *argv[])
{
    int fd, ret_val, count, numread;
    char buf[MAX_BUF_SIZE];
    int i;

    /* Create the named - pipe */
    ret_val = mkfifo(HALF_DUPLEX, 0666);

    if ((ret_val == -1) && (errno != EEXIST)) {
        perror("Error creating the named pipe");
        exit (1);
    }

    for (i = 0; i < 5; i++) {
    /* Open the pipe for reading */
    fd = open(HALF_DUPLEX, O_RDONLY);

    do {
    /* Read from the pipe */
    numread = read(fd, buf, MAX_BUF_SIZE);
    if (numread == 0) {
	/* End of file */
	break;
    } else if (numread < 0) {
	perror("Error reading the named pipe");
	exit (1);
    }

    buf[numread] = '\0';

    printf("Half Duplex Server : Read From the pipe : %s\n", buf);

    /* Convert to the string to upper case */
    count = 0;
    while (count < numread) {
        buf[count] = toupper(buf[count]);
        count++;
    }
    
    printf("Half Duplex Server : Converted String   : %s\n", buf);
    } while (numread > 0);

    close(fd);
    }

    unlink(HALF_DUPLEX);

    return 0;
}

