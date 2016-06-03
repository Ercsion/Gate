#ifndef __DATABUF_H_
#define __DATABUF_H_

#include "CircularBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_BUF_LEN  4*1024

//push error number
#define PUSH_NULL_DATA_PUSHED -0xA1 //PUSH空数据
#define PUSH_DATA_BUFFER_FULL -0xA3 //缓存区已满
#define PUSH_WRITE_BINARY_ERR -0xA4 //写数据出错
//pop error number
#define POP_DATA_TO_NULL_BUF -0xB1 //缓存不可用
#define POP_NULL_BUFFER_DATA -0xB2 //缓存区为空
#define POP_NOT_FOUNDED_HEAD -0xB4 //没找到包头
#define POP_NOT_FOUNDED_ENDD -0xB5 //没找到包尾
#define POP_USER_BUF_IS_LESS -0xB6 //缓冲区太小
#define POP_READ_BINARY_ERRO -0xB7 //读数据出错

using namespace std;

class DataBuf : public CCircularBuffer
{
	
protected:
	char* m_BagHead;
	pthread_mutex_t m_Mutex;

public:
	DataBuf();
	virtual ~DataBuf();
	virtual int PushData( char *buf,int len );
	virtual int PopData ( char *buf,int len );
	bool CompareHead( char *buf );

	//for debug
    bool PrintErrMsg( int errNu );
    bool PrintErrMsg( const char *msg, int errNu );
};

#ifdef __cplusplus  
}  
#endif

#endif /* __DATABUF_H_ */
