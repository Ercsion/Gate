#include "CirBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

CirBuffer::CirBuffer()
{
    m_Buf = NULL;
    m_Size = 0;
    m_ReadPtr = 0;
    m_WritePtr = 0;
}

CirBuffer::~CirBuffer()
{
    Destroy();
}

bool CirBuffer::Create( int size )
{
    m_Buf = new char[size];
    if( NULL ==  m_Buf )
        return false;

    m_Size = size;
    memset( m_Buf, 0, m_Size );

    return true;
}

void CirBuffer::Destroy()
{

    if( m_Buf )
        delete [] m_Buf ;

    m_Buf = NULL;
    m_Size = 0;
    m_ReadPtr = 0;
    m_WritePtr = 0;
}

int CirBuffer::GetMaxReadSize()
{
    if( m_Buf )
    {
        return ((m_ReadPtr > m_WritePtr) ? \
                (m_Size - (m_ReadPtr - m_WritePtr)) : (m_WritePtr - m_ReadPtr));
    }
    return false;
}

int CirBuffer::GetMaxWriteSize()
{
    if( m_Buf )
    {
        return ((m_WritePtr < m_ReadPtr) ? \
            (m_ReadPtr - m_WritePtr) : (m_Size - (m_WritePtr - m_ReadPtr)) );
    }
    return false;
}

bool CirBuffer::WriteBinary( char *buf, int len )
{
    if( len <= GetMaxWriteSize() )
    {
        if( m_WritePtr + len < m_Size )
        {
            memcpy(m_Buf + m_WritePtr, buf, len);
            m_WritePtr += len;
        }
        else
        {
            int FirstCopySize = m_Size - m_WritePtr;
            int SecondCopySize = len - FirstCopySize;

            memcpy( m_Buf + m_WritePtr, buf, FirstCopySize );
            memcpy( m_Buf, buf + FirstCopySize, SecondCopySize );
            m_WritePtr = SecondCopySize;
        }
        return true;
    }
    return false;
}

bool CirBuffer::ReadBinary( char *buf, int len , bool flag )
{
    if( len <= GetMaxReadSize() )
    {
        int newReadPtr = m_ReadPtr;
        if( m_ReadPtr + len < m_Size )
        {
            memcpy( buf, m_Buf + m_ReadPtr, len );
            newReadPtr += len;
        }
        else
        {
            int FirstCopySize = m_Size - m_ReadPtr;
            int SecondCopySize = len - FirstCopySize;

            memcpy( buf, m_Buf + m_ReadPtr, FirstCopySize );
            memcpy( buf + FirstCopySize, m_Buf, SecondCopySize );
            newReadPtr = SecondCopySize;
        }
        if(true == flag)
            m_ReadPtr = newReadPtr;
        return true;
    }
    return false;
}

bool CirBuffer::PeekChar( int pos, char &ch )
{
    if( pos >= 0 && pos <= GetMaxReadSize() )
    {
        ch = m_Buf[(m_ReadPtr + pos) % m_Size];
        return true;
    }
    return false;
}

bool CirBuffer::FindChar( char lookFor, int &pos )
{
    for( int i = 0; i < GetMaxReadSize(); i++ )
    {
        char ch;
        if( PeekChar( i, ch ) )
        {
            if( ch == lookFor )
            {
                pos = i + m_ReadPtr;
                return true;
            }
        }
    }
    return false;
}

bool CirBuffer::SetChar(char ch, int pos, int len)
{
    for(int i = 0; i < len; i++)
    {
        m_Buf[(pos + i) % m_Size] = ch;
    }
    return true;
}

void CirBuffer::ClearBuf()
{
    m_ReadPtr = 0;
    m_WritePtr = 0;
}

#ifdef __cplusplus
}
#endif
