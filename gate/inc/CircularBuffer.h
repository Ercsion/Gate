#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

class CCircularBuffer
{
public:

    CCircularBuffer();
    virtual ~CCircularBuffer();

    bool Create( int size );
    void Destroy();
    
    int GetMaxReadSize();
    int GetMaxWriteSize();
    virtual bool WriteBinary( char *buf, int len );
    virtual bool ReadBinary( char *buf, int len , bool flag );
    bool PeekChar( int pos, char &ch );
    bool FindChar( char lookFor, int &pos );
    bool SetChar(char ch, int pos, int len);
    void ClearBuf();

protected:

    char *m_Buf;
    int m_Size;   // 循环缓冲区长度
    int m_ReadPtr;   // 读指针
    int m_WritePtr;  // 写指针

private:


};

#ifdef __cplusplus  
}  
#endif

#endif //__CIRCULARBUFFER_H__

