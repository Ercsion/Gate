#ifndef __GLOBAL_H__
#define __GLOBAL_H__

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

#include "glog/logging.h"
#include "glog/log_severity.h"
#include "glog/raw_logging.h"
#include "glog/stl_logging.h"
#include "glog/vlog_is_on.h"
#include "GLogHelper.h"
#include "ColorHelper.h"

#define GATE_SOCKET_PATH "gate_sock"

typedef struct dataHeadType
{
    char dest_id;  //目标地址
    char src_id;   //源地址
} __attribute__ ((__packed__))dataHeadType_S;

#endif /* __GLOBAL_H__ */
