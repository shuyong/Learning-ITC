#!/bin/sh

cc -Wall -O2 -g -o hd_server hd_server.c
cc -Wall -O2 -g -o hd_client hd_client.c

cc -Wall -O2 -g -o fd_server fd_server.c
cc -Wall -O2 -g -o fd_client fd_client.c

