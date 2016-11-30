#include "Connect.h"

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc,char*argv[])
{
	unsigned char send[100] = {0};
	Connect(0x11);
	int ret = 0;
	unsigned long count = 0;
	unsigned char last = 0;
	while(1)
	{
	    send[0] = count%0xFF;
		ret = SendToID(0XAA,(char*)send,10);
        if(ret <8)printf(">>>>>>>>>> ret = %d\n",ret);
        printf("last = 0x%02x, this = 0x%02x, 0x%02x\n",last,send[0],send[0]-last);
        last = send[0];
        count++;
        usleep(1000*1000);
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

