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

#include "Crc.h"

#ifdef PC
#define GATE_SOCKET_PATH "/home/icu/gate_sock"
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

#define TM_GATE_Print()\
    {\
		printf("#[schedlu][%24s ][%-3d]", basename(__FILE__), __LINE__);\
    }
#define TM_GATE_Printf(format, ...)\
    {\
        TM_GATE_Print();\
        printf(format,##__VA_ARGS__);printf("\n");\
    }

#define TM_GATE_DPrintf(buf,len,format,...)\
{\
    TM_GATE_Print();\
    printf("   /---------------------------------[ ");\
    printf(format,##__VA_ARGS__);printf("\n");\
    TM_GATE_Print();printf("   |");\
    for (int i = 0; i < len; i++)\
    {\
        printf("\033[32m %02X\033[0m", (buf)[i] & 0xff);\
        if (!((i + 1) % 20))\
        {\
            if(i!=len-1){\
                printf("\n");\
                TM_GATE_Print();printf("   |");}\
        }\
    }\
    printf("\n");\
    TM_GATE_Printf("   \\---------------------------------[ length %3d ]--------------",len);\
}


#endif /* __COMMON_H__ */

