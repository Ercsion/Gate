
#include "Connect.h"

#ifdef __cplusplus
extern "C" {
#endif
static Client* __client__ = NULL;

void Connect(char ID)
{
    if(NULL == __client__)
    {
        static Client c(ID);
        __client__  = &c;
    }
    return;
}

void ConnectWatchFlag(char ID,unsigned char* watchFlag, int watchLen)
{
    if(NULL == __client__)
    {
        static Client c(ID,watchFlag,watchLen);
        __client__  = &c;
    }
    return;
}

void DisConnect(void)
{
    if(NULL != __client__)
    {
        delete __client__;
        __client__ = NULL;
    }
    return;
}

int SendToID(char dest_id, char* buf, int len)
{
    if(NULL == __client__)
        return -1;
    return __client__->OnWrite(dest_id,buf,len);
}

int SendTo(char* buf, int len)
{
    if(NULL == __client__)
        return -1;
    return __client__->OnWrite(buf,len);
}

int GetMsg(char* buf, int len)
{
    if(NULL == __client__ )
        return -1;
    return __client__->OnRead(buf, len);
}

int GetFlag(void)
{
	if(NULL == __client__ )
        return -1;
	return __client__->GetConnectFlag();
}

#ifdef __cplusplus
}
#endif

