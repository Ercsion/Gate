#include "Client.h"


int main(int argc,char*argv[])
{
	char send[1] = {0};
	char recv[1024] = {0};
	int ret = 0;
	Client c(0xFF);
	int f = 1;
	char last = 0;
	while(1)
	{
		ret = c.OnRead(recv,sizeof(recv));
        if(ret > 0)
		{
			if(1 == f)
			{
				last = recv[8];
				f = 2;
			}
			if(last != recv[8]){
				exit(-1);
			}
			last++;
            send[0] = recv[8];
			ret = c.OnWrite(0x11,send,1);
        }
		usleep(100*1000);
	}
	return 0;
}
