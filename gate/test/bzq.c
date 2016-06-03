#include "Connect.h"

#define TM_GATE_Print()\
    {\
		printf("#[bzq][%24s ][%-3d]", basename(__FILE__), __LINE__);\
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

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc,char*argv[])
{
	char buff[] = {0x31,0x31,0x31,0x31};
	char msg[1024];
	Connect(0x31);
	int ret = 0;
	while(1)
	{
		ret = SendTo(0x42,buff,4);
		TM_GATE_Printf("SendTo() return %d",ret);
		sleep(1);
		ret = GetMsg(msg,sizeof(msg));
		TM_GATE_DPrintf(msg,ret,"GetMsg() return %d",ret);
		sleep(1);
	}
}

#ifdef __cplusplus
}
#endif
