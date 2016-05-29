#include "Gate.h"
using namespace std;
Gate::Gate(char *path, int len)
{
    LOG_IF(FATAL, NULL == path) << "path is NULL!";
    socketPath_m = (char *)malloc(sizeof(char) * len);
    LOG_IF(FATAL, NULL == socketPath_m) << "malloc fail!";
    strcpy(socketPath_m, path);
    ILOG << "socketPath_m = " << socketPath_m;
    setUp();
}
Gate::~Gate()
{
    free(socketPath_m);
    socketPath_m = NULL;
}
bool Gate::setUp(void)
{
    /// 本地地址
    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, socketPath_m);

    /// 创建socket
    serverFd_m = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == serverFd_m)
    {
        PLOG(ERROR) << "socket error";
        return false;
    }
    ILOG << "server_sock_fd = " << serverFd_m;

    //地址允许重复绑定
    int on = 1;
    if( 0 > setsockopt(serverFd_m, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        PLOG(ERROR) << "setsockopet error";
        return false;
    }
    unlink(socketPath_m);

    /// 绑定socket
    if(-1 == bind(serverFd_m, (struct sockaddr *)&address, sizeof(address)))
    {
        PLOG(ERROR) << "bind error";
        return false;
    }

    /// listen
    if(-1 == listen(serverFd_m, 5))
    {
        PLOG(ERROR) << "listen error";
        return false;
    }
    return true;
}

int Gate::run(void)
{
    char recv_msg[BUFFER_SIZE];
    struct timeval tv;
    map<int, Client_S>::iterator it;

    do
    {
        /// 设置超时时间
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        FD_ZERO(&fdSet_m);
        /// 服务器端socket
        FD_SET(serverFd_m, &fdSet_m);
        if (maxFd_m < serverFd_m)
        {
            maxFd_m = serverFd_m;
        }

        /// 客户端socket
        for(it = mClient_m.begin(); it != mClient_m.end(); ++it)
        {
            ILOG << "add " << it->first << " to SET";
            FD_SET(it->first, &fdSet_m);
            maxFd_m = (maxFd_m < it->first ? it->first : maxFd_m);
        }

        /// ret为未状态发生变化的文件描述符的个数
        int ret = select(maxFd_m + 1, &fdSet_m, NULL, NULL, &tv);
        if (ret < 0)
        {
            ELOG << "select 出错";
            continue;
        }
        else if(ret == 0)
        {
            WLOG << "select 超时";
            continue;
        }
        else
        {
            /// 检测是否有客户端连接
            if (FD_ISSET(serverFd_m, &fdSet_m))
            {
                //有新的连接请求
                struct sockaddr_un client_address;
                socklen_t address_len;
                int client_fd = accept(serverFd_m, (struct sockaddr *)&client_address, &address_len);
                if (client_fd > 0)
                {
                    /// 将client_fd加入mClient_m中
                    map<int, Client_S>::iterator iter;
                    iter = mClient_m.find(client_fd);
                    if(iter != mClient_m.end())
                    {
                        WLOG << "client alreadly exist" << iter->first;
                    }
                    else
                    {
                        ILOG << "new client fd" << iter->first;
                    }
                }
            }
            /// 遍历map，处理每一个client
            for(it = mClient_m.begin(); it != mClient_m.end(); ++it)
            {
                if (FD_ISSET(it->first, &fdSet_m))
                {
                    //处理某个客户端过来的消息
                    bzero(recv_msg, BUFFER_SIZE);
                    long byte_num = recv(it->first, recv_msg, BUFFER_SIZE, 0);
                    if (byte_num > 0)
                    {
                        if (byte_num > BUFFER_SIZE)
                        {
                            byte_num = BUFFER_SIZE;
                        }
                        recv_msg[byte_num] = '\0';
                        ILOG << "receive " << byte_num << "bytes from " << it->first;
                    }
                    else if(byte_num < 0)
                    {
                        WLOG << "receive error from " << it->first;
                    }
                    else
                    {
                        FD_CLR(it->first, &fdSet_m);
                        mClient_m.erase(it->first);
                        WLOG << it->first << " out";
                    }
                }
            }

        }
    }
    while(1);
    return 0;
}
