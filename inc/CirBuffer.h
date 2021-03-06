#ifndef __CIRBUFFER_H__
#define __CIRBUFFER_H__

#include "Clicom.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

class CirBuffer
{
public:

    CirBuffer();
    virtual ~CirBuffer();

    bool Create( int size );
    void Destroy();

    int GetMaxReadSize();
    int GetMaxWriteSize();
    virtual bool WriteBinary( char *buf, int len );
    virtual bool ReadBinary( char *buf, int len , bool flag );
    bool PeekChar( int pos, char &ch );
    bool FindFlag( int &pos );
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

#endif //__CIRBUFFER_H__

