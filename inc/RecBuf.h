#ifndef __RECBUF_H_
#define __RECBUF_H_

#include "CirBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_BUF_LEN  4*1024

//push error number
#define PUSH_NULL_DATA_PUSHED -1 //PUSH空数据
#define PUSH_DATA_BUFFER_FULL -2 //缓存区已满
#define PUSH_WRITE_BINARY_ERR -3 //写数据出错
//pop error number
#define POP_DATA_TO_NULL_BUF -4 //缓存不可用
#define POP_NULL_BUFFER_DATA -0 //缓存区为空
#define POP_NOT_FOUNDED_HEAD -5 //没找到包头
#define POP_NOT_FOUNDED_ENDD -6 //没找到包尾
#define POP_USER_BUF_IS_LESS -7 //缓冲区太小
#define POP_READ_BINARY_ERRO -8 //读数据出错

using namespace std;

class RecBuf : public CirBuffer
{

protected:
	char* m_BagHead;
	pthread_mutex_t m_Mutex;

public:
	RecBuf();
	virtual ~RecBuf();
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

#endif /* __RECBUF_H_ */
