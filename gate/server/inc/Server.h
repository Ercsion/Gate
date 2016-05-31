#ifndef __GATE_H__
#define __GATE_H__

#include "Common.h"
#define PROJECT_NAME "Gate"
#define PROGRAM_VERSION "0.1"
#define RECV_BUFFER_SIZE 1024

typedef struct
{
    char flag;
    time_t lastTime;
} __attribute__ ((__packed__))Client_S;

typedef std::map<int,Client_S*> ClientMap;
typedef std::pair<int,Client_S*> ClientPair;
typedef map<int, Client_S *>::iterator ClientIter;

using namespace std;
class Server
{
public:
    Server();
    ~Server();
    int Run(void);
    
protected:
    bool Init(void);
    void SetupFDSet(void);
    void SetupClientMap(void);
    int GetClientFd(char flag);
    int RecvClient(ClientIter it);
    int DataPprocess(int DataLen,ClientIter it);
    int WriteTo(int fd, char* data, int len);
    
    int    m_ServerFd;     /// server fd
    int    m_OverTime;     /// over time of waiting for client
    int    m_MaxFd;        /// max of all client
    char*  m_RecvBuf;      /// message from client buffer
    bool   m_RunFlag;      /// server run flag
    fd_set m_Set;          /// server and clients set
    ClientMap m_ClientMap;
};

#endif /* __GATE_H__ */
