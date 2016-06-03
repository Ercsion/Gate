#include "Client.h"

#define TM_GATE_Print()\
    {\
		printf("#[schedlu][%24s ][%-3d]", basename(__FILE__), __LINE__);\
    }
#define TM_GATE_Printf(format, ...)\
    {\
        TM_GATE_Print();\
        printf(format,##__VA_ARGS__);printf("\n");\
    }

#define TM_GATE_DPrintf(buf,len,format,...)\
{\
    TM_GATE_Print();\
    printf("   /---------------------------------[ ");\
    printf(format,##__VA_ARGS__);printf("\n");\
    TM_GATE_Print();printf("   |");\
    for (int i = 0; i < len; i++)\
    {\
        printf("\033[32m %02X\033[0m", (buf)[i] & 0xff);\
        if (!((i + 1) % 20))\
        {\
            if(i!=len-1){\
                printf("\n");\
                TM_GATE_Print();printf("   |");}\
        }\
    }\
    printf("\n");\
    TM_GATE_Printf("   \\---------------------------------[ length %3d ]--------------",len);\
}

#if 1
int main(int argc,char*argv[])
{
	char buff[] = {0x42,0x42,0x42,0x42};
	char msg[1024];
	int ret = 0;
	Client c(0x42);
	while(1)
	{
		ret = c.OnWrite(0x31,buff,4);
		TM_GATE_Printf("OnWrite() return %d",ret);
		sleep(1);
		ret = c.OnRead(msg,sizeof(msg));
		TM_GATE_DPrintf(msg,ret,"OnRead() return %d",ret);
		sleep(1);
	}
}
#else
int main(int argc,char*argv[])
{
	
}
#endif