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
#define GATE_SOCKET_PATH "/opt/gate_sock"
#else
#define GATE_SOCKET_PATH "/root/gate_sock"
#endif

#define HEAD_LEN  4

typedef struct DataType
{
    unsigned char head[HEAD_LEN];
    unsigned char len_h;
    unsigned char len_l;
    unsigned char dest_id;  //to
    unsigned char src_id;   //from
} __attribute__ ((__packed__))DataType_S;

#define TM_GATE_Print()\
    {\
        time_t now = time((time_t*)NULL);\
        struct tm* p = localtime(&now);\
        struct timeval tv;\
        gettimeofday(&tv, NULL);\
        printf("DAT %02d/%02d/%02d %02d:%02d:%02d.%06d [%lu %24s %3d] ",\
        p->tm_year%100,p->tm_mon+1,p->tm_mday,\
        p->tm_hour,p->tm_min,p->tm_sec,\
        (int)tv.tv_usec,\
        syscall(SYS_gettid),basename(__FILE__), __LINE__);\
    }

#define TM_GATE_Printf(format, ...)\
    {\
        TM_GATE_Print();\
        printf(format,##__VA_ARGS__);printf("\n");\
    }

#define TM_GATE_DPrintf(buf,len,format,...)\
{\
    TM_GATE_Print();\
    printf("   /------------------[ ");\
    printf(format,##__VA_ARGS__);printf("\n");\
    TM_GATE_Print();printf("   |");\
    for (int i = 0; i < len; i++)\
    {\
        printf("\033[32m %02X\033[0m", (buf)[i] & 0xff);\
        if (!((i + 1) % 10))\
        {\
            if(i!=len-1){\
                printf("\n");\
                TM_GATE_Print();printf("   |");}\
        }\
    }\
    printf("\n");\
    TM_GATE_Printf("   \\------------------[ length %3d",len);\
}


#endif /* __COMMON_H__ */

