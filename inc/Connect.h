#include "Client.h"
#ifdef __cplusplus
extern "C"{
#endif

void Connect(char ID);
void ConnectWatchFlag(char ID,unsigned char* watchFlag, int watchLen);
void DisConnect(void);
int SendToID(char dest_id, char* buf, int len);
int SendTo(char* buf, int len);
int GetMsg(char* buf, int len);
int GetFlag(void);

#ifdef __cplusplus
}
#endif

