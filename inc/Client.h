#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Clicom.h"
#include "RecBuf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLIENT_VERSION "0.3"

using namespace std;

#define FLAG_CONNECTQUI 0x00 //make thread quit
#define FLAG_DISCONNECT 0x01 //disconnect
#define FLAG_CONNECTINT 0x02 //send flag to server
#define FLAG_CONNECTING 0X03 //wait for server answer
#define FLAG_CONNECTTED 0X04 //connected

#define TIME_OUT 3
#define RECV_BUFFER_SIZE 1024

class Client
{

public:
    Client(char ID, unsigned char* watchFlag=NULL, int watchLen=0);
    ~Client();
    int OnWrite(char* buf, int len);  //makeup head yourself
    int OnWrite(char dest, char* buf, int len);
    int OnRead(char* buf, int len);
    int GetConnectFlag(void){return m_ConnectFlag;}

protected:
    bool Init(void);
    static void *DataHandleThread(void *arg);
    int DataPprocess(char* buf,int len);

    unsigned char  m_Flag;         /// this client flag
    unsigned char* m_WatchFlag;
    int            m_WatchLen;
    int            m_ServerFd;     /// server fd
    int            m_ConnectFlag;  /// connect flag
    RecBuf*        m_DataBuf;
    pthread_t      m_Tid;          /// pthrea id
    fd_set         m_Set;          /// server and client set
    time_t         m_LastTime;     /// time of recognition
	struct sockaddr_un m_Addr;

};

#ifdef __cplusplus
}
#endif

#endif /* __CLIENT_H__ */

