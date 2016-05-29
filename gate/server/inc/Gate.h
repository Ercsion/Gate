#ifndef __GATE_H__
#define __GATE_H__

#include "Common.h"

#define BACKLOG 5 //完成三次握手但没有accept的队列的长度
#define CONCURRENT_MAX 8 //应用层同时可以处理的连接
#define BUFFER_SIZE 1024

typedef struct
{
    bool enable;  //是否可用
    char flag;    //唯一标识
    time_t acceptTime;
    time_t lastTime;
    int overTime_m; //接受超时时间
} __attribute__ ((__packed__))Client_S;

using namespace std;
class Gate
{
public:
    Gate(char* path,int len);
    ~Gate();
    int run(void);
    bool isRunning(void){return runFlag_m;}

protected:
    bool setUp(void);

    char *socketPath_m; /// server socket file path
    int serverFd_m;     /// server fd
    fd_set fdSet_m;     /// server and clients set
    bool runFlag_m;     /// flag of run
    int overTime_m;     /// over time of waiting for client flag
    int clientNum_m;    /// available client
    int maxFd_m;          /// max of all client

    std::map<int,Client_S> mClient_m;
};

#endif /* __GATE_H__ */
