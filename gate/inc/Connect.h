#include "Client.h"
#ifdef __cplusplus  
extern "C"{  
#endif  
  
void Connect(char ID);
void DisConnect(void);
int SendTo(char dest_id, char* buf, int len);
int GetMsg(char* buf, int len);

#ifdef __cplusplus  
}  
#endif