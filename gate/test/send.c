#include "Connect.h"

#ifdef __cplusplus
extern "C" {
#endif

char b[] = {0x24,0x24,0x24,0x24,0x00,0x0A,0xAA,0x11};

int main(int argc,char*argv[])
{
	char send[1] = {0};
	char recv[1024] = {0};
	Connect(0x11);
	int ret = 0;
    unsigned long count = 0;
	while(1)
	{
	    send[0] = count%0xFF;
		ret = SendTo(0xFF,send,1);
        printf("send return %d\n",ret);
        ret = GetMsg(recv,sizeof(recv));
        if(ret > 0)
		{
		    printf("-- %d --\n",send[0]-recv[8]);
        }
        count++;
        usleep(80*1000);
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

