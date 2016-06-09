#include "RecBuf.h"

#ifdef __cplusplus
extern "C" {
#endif

RecBuf::RecBuf()
{
	pthread_mutex_init (&m_Mutex, NULL);
	m_BagHead = new char[HEAD_LEN];
	memset(m_BagHead,'$',HEAD_LEN);
	Create(DEFAULT_BUF_LEN);
}

RecBuf::~RecBuf()
{
	pthread_mutex_destroy(&m_Mutex);
}

/// push in
int RecBuf::PushData( char *buf,int len )
{
	pthread_mutex_lock(&m_Mutex);

	if( (buf == NULL) || (len <= 0) )
	{
		pthread_mutex_unlock(&m_Mutex);
		return PUSH_NULL_DATA_PUSHED;
	}

	if( 0 == GetMaxWriteSize() )
	{
		pthread_mutex_unlock(&m_Mutex);
		return PUSH_DATA_BUFFER_FULL;
	}

	if(len > GetMaxWriteSize())
	{
		len = GetMaxWriteSize();
	}

	if ( false == WriteBinary( buf, len ) )
	{
		pthread_mutex_unlock(&m_Mutex);
		return PUSH_WRITE_BINARY_ERR;
	}

	pthread_mutex_unlock(&m_Mutex);
	return len;
}

/// pop out
int RecBuf::PopData(char *buf, int len)
{
	pthread_mutex_lock(&m_Mutex);

	if( buf == 0 || len<=0 )
	{
		pthread_mutex_unlock(&m_Mutex);
		return POP_DATA_TO_NULL_BUF;
	}

	if( 0 == GetMaxReadSize())
	{
		pthread_mutex_unlock(&m_Mutex);
		return POP_NULL_BUFFER_DATA;
	}

/****************************************************************************/
	int bagLen = 0;
	char tmp[sizeof(DataType_S)] = {'0'};
	DataType_S *data = NULL;

	while( 0 < GetMaxReadSize() )
	{
		if( false == FindChar(m_BagHead[0], m_ReadPtr) )
		{
			if(!GetMaxWriteSize())
				CirBuffer::ClearBuf();

			pthread_mutex_unlock(&m_Mutex);
			return POP_NOT_FOUNDED_HEAD;
		}

		if ( false == ReadBinary(tmp, sizeof(DataType_S), false) )
		{
			pthread_mutex_unlock(&m_Mutex);
			return POP_READ_BINARY_ERRO;
		}

		if( true == CompareHead(tmp) )
		{
			data = (DataType_S*)tmp;
			bagLen = (data->len_h<<8) | (data->len_l);
			if(bagLen <= 0)
			{
				pthread_mutex_unlock(&m_Mutex);
				return POP_NOT_FOUNDED_ENDD;
			}
			else if(bagLen <= len)
			{
				if ( false == ReadBinary(buf, bagLen, true) )
				{
					pthread_mutex_unlock(&m_Mutex);
					return POP_READ_BINARY_ERRO;
				}
				pthread_mutex_unlock(&m_Mutex);

				return bagLen;
			}
			else
			{
				pthread_mutex_unlock(&m_Mutex);
				return POP_USER_BUF_IS_LESS;
			}
		}
		else
		{
			m_ReadPtr++;
			continue;
		}
	}

	pthread_mutex_unlock(&m_Mutex);
	return POP_NOT_FOUNDED_HEAD;
}

bool RecBuf::CompareHead( char *buf )
{
	bool find = true;
	for(int i = 0; i < HEAD_LEN; i++)
	{
		if(buf[i] != m_BagHead[i])
		{
			find = false;
			break;
		}
	}
	return find;
}

bool RecBuf::PrintErrMsg(int errNu)
{
	return PrintErrMsg("", errNu);
}

bool RecBuf::PrintErrMsg(const char *userMsg, int errNu)
{
	switch(errNu)
	{
		case PUSH_NULL_DATA_PUSHED: //PUSH空数据
			cout << userMsg << "in push null data pushed." << endl;
			break;
		case PUSH_DATA_BUFFER_FULL: //缓存区已满
			cout << userMsg << "in push, data buf is full, abandon" << endl;
			break;
		case PUSH_WRITE_BINARY_ERR: //写数据出错
			cout << userMsg << "in push, write binary error." << endl;
			break;
		case POP_DATA_TO_NULL_BUF: //缓存不可用
			cout << userMsg << "in pop, receive buf is null." << endl;
			break;
		case POP_NULL_BUFFER_DATA: //缓存区为空
			cout << userMsg << "in pop, buffer have no data." << endl;
			break;
		case POP_NOT_FOUNDED_HEAD: //没找到包头
			cout << userMsg << "in pop, no bag head, abandon." << endl;
			break;
		case POP_NOT_FOUNDED_ENDD: //没找到包尾
			cout << userMsg << "in pop, no bag end, continue..." << endl;
			break;
		case POP_USER_BUF_IS_LESS: //缓冲区太小
			cout << userMsg << "in pop, user buf is less." << endl;
			break;
		case POP_READ_BINARY_ERRO: //读数据出错
			cout << userMsg << "in pop, read binary error." << endl;
			break;
		default:
			cout << userMsg <<"Unknow error number "<<errNu<<endl;
	}
	return true;
}

#ifdef __cplusplus
}
#endif
