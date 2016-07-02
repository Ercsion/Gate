#include "Connect.h"

#ifdef __cplusplus
extern "C" {
#endif
unsigned char tmp[] = {0x11,0x12};
int main(int argc,char*argv[])
{
	unsigned char send[] = {0x00,0x51,0x09,0x00,0x01,0x10,0x83,0x80,0xe1,0x35};
    //unsigned char recv[1024] = {0};
	ConnectWatchFlag(0x3a,tmp,2);
	int ret = 0;
    int timeout = 0;
	time_t tm;

    while( 4 != GetFlag())
    {
        time(&tm);
    }
	while(1)
	{
        timeout = difftime(time(0),tm);
		if( 0 == timeout%2 )
        {
			send[7] ++;
			ret = SendTo(0x11,(char*)send,sizeof(send));
			if(ret > 10)
			{
				TM_GATE_DPrintf(send,(int)sizeof(send),"<<<<<<<<<< SendTo return %d",ret);
			}
		}
        #if 0
        ret = GetMsg((char*)recv,sizeof(recv));
        if(ret > 0)
		{
			TM_GATE_DPrintf(recv,ret,"<<<<<<<<<< GetMsg return %d",ret);
        }
        #endif
		usleep(1000*1000);
    }
    return 0;
}

int main1(int argc,char*argv[])
{
    printf("sizeof(char)  = %d\n",sizeof(char));
    printf("sizeof(short) = %d\n",sizeof(short));
    printf("sizeof(int)   = %d\n",sizeof(int));
    printf("sizeof(long)  = %d\n",sizeof(long));
    printf("sizeof(double)= %d\n",sizeof(double));
    printf("---------------------\n");
    printf("sizeof(unsigned char)  = %d\n",sizeof(unsigned char));
    printf("sizeof(unsigned short) = %d\n",sizeof(unsigned short));
    printf("sizeof(unsigned int)   = %d\n",sizeof(unsigned int));
    printf("sizeof(unsigned long)  = %d\n",sizeof(unsigned long));
    printf("sizeof(long long)      = %d\n",sizeof(long long));
    return 0;
}
#ifdef __cplusplus
}
#endif

