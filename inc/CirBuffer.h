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
    bool FindChar( char lookFor, int &pos );
    bool SetChar(char ch, int pos, int len);
    void ClearBuf();

protected:

    char *m_Buf;
    int m_Size;   // ѭ������������
    int m_ReadPtr;   // ��ָ��
    int m_WritePtr;  // дָ��

private:


};

#ifdef __cplusplus
}
#endif

#endif //__CIRBUFFER_H__

