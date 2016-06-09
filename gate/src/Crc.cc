#include "Crc.h"
#include "Common.h"
#include "stdio.h"
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************
crc16		X^16 + X^15 + X^2 + 1
****************************************************/
unsigned short crc16(unsigned char *buf,  unsigned short len)
{
	unsigned short i;
	unsigned short res;

	for (i = 0; i < len; i++)
	{
		if (buf[i] != 0)
		{
			res = 0;
			for (i = 0; i < len; i++)
			{
				res = (crc16_tab[buf[i] ^ (res & 0xFF)] ^ (res / 0x100));
			}
			//res = (res >> 8) | (res << 8);
			return (res);
		}
	}
	return (0xFFFF);
}

#ifdef __cplusplus
}
#endif
