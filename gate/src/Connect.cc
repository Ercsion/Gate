
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

void DisConnect(void)
{
    if(NULL != __client__)
    {
        delete __client__;
        __client__ = NULL;
    }
    return;
}

int SendTo(char dest_id, char* buf, int len)
{
    if(NULL == __client__)
        return -1;
    return __client__->OnWrite(dest_id,buf,len);
}
int GetMsg(char* buf, int len)
{    
    if(NULL == __client__ )
        return -1;
    return __client__->OnRead(buf, len);
}
#ifdef __cplusplus
}
#endif