#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <arpa/inet.h>
#include <map>
#include <algorithm>

#ifdef PC
#define GATE_SOCKET_PATH "gate_sock"
#else
#define GATE_SOCKET_PATH "/root/gate_sock"
#endif

#define HEAD_LEN  4

typedef struct DataType
{
    char head[HEAD_LEN];
    char len_h;
    char len_l;
    char dest_id;  //to
    char src_id;   //from
} __attribute__ ((__packed__))DataType_S;

#endif /* __COMMON_H__ */
