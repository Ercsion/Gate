#ifndef __RECBUF_H_
#define __RECBUF_H_

#include "CirBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_BUF_LEN  4*1024

//push error number
#define PUSH_NULL_DATA_PUSHED -1 //PUSH������
#define PUSH_DATA_BUFFER_FULL -2 //����������
#define PUSH_WRITE_BINARY_ERR -3 //д���ݳ���
//pop error number
#define POP_DATA_TO_NULL_BUF -4 //���治����
#define POP_NULL_BUFFER_DATA -0 //������Ϊ��
#define POP_NOT_FOUNDED_HEAD -5 //û�ҵ���ͷ
#define POP_NOT_FOUNDED_ENDD -6 //û�ҵ���β
#define POP_USER_BUF_IS_LESS -7 //������̫С
#define POP_READ_BINARY_ERRO -8 //�����ݳ���

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
