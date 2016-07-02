#include "Client.h"

unsigned char tmp[] = {0x11,0x12};
int main(int argc,char*argv[])
{
	unsigned char recv[1024] = {0};
	int ret = 0;
	Client c(0x33,tmp,2);
	while(1)
	{
		ret = c.OnRead((char*)recv,sizeof(recv));
        if(ret > 0)
		{
			TM_GATE_DPrintf(recv,ret,"OnRead return %d",ret);
        }
		usleep(100*1000);
	}
	return 0;
}
