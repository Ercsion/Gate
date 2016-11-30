#include "Client.h"


int main(int argc,char*argv[])
{
	unsigned char recv[1024] = {0};
	int ret = 0;
	Client c(0xAA);
	unsigned char last = 0;
	while(1)
	{
		ret = c.OnRead((char*)recv,sizeof(recv));
        if(ret > 0)
		{
        	printf("last = 0x%02x, this = 0x%02x, 0x%02x\n",last,recv[8],recv[8]-last);
			last = recv[8];
        }
		usleep(100*1000);
	}
	return 0;
}
