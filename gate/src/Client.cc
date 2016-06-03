#include "Client.h"

#ifdef __cplusplus
extern "C" {
#endif

static char recv_msg[RECV_BUFFER_SIZE];
    
Client::Client(char ID)	:m_Flag(ID),m_DataBuf(NULL)
{
    Init();
}

Client::~Client()
{
    WLOG<<"Client 0x"<<hex<<(int)m_Flag<<" out";
    m_ConnectFlag = FLAG_CONNECTQUI;
    delete m_DataBuf;
    m_DataBuf = NULL;
}

bool Client::Init(void)
{
    /// UNIX domain sockets
    m_Addr.sun_family = AF_UNIX;
    strcpy(m_Addr.sun_path, GATE_SOCKET_PATH);

    m_ConnectFlag = FLAG_DISCONNECT;
    m_DataBuf = new DataBuf();
    
    /// pthread start
    if(0 != pthread_create(&m_Tid, NULL, DataHandleThread, this)) {
        PLOG(ERROR) << "Create pthread error";
        return false;
    }
    
    ILOG<<"Client 0x"<<hex<<(int)m_Flag<<" check in";
    return true;
}

void *Client::DataHandleThread(void *arg)
{
    Client& c = *(Client*)arg;
    int ret;
    struct timeval tv;
    
    while(1)
    {
        tv.tv_sec = 0;
        tv.tv_usec = 300;
        if(FLAG_CONNECTQUI == c.m_ConnectFlag)
        {
            return 0;
        }
        if(FLAG_DISCONNECT == c.m_ConnectFlag)
        {
            /// socket
            c.m_ServerFd = socket(AF_UNIX, SOCK_STREAM, 0);
            if(-1 == c.m_ServerFd)
            {
                PLOG(ERROR) << "Socket() error";
                return 0;
            }
            ILOG << "Server sock fd = " << c.m_ServerFd;
    
            ret = connect(c.m_ServerFd, (struct sockaddr *)&(c.m_Addr), sizeof(struct sockaddr_un));
            if ( -1 == ret )
            {
                PLOG(ERROR)<<"Connect to server failed";
                shutdown(c.m_ServerFd,SHUT_RDWR);
                sleep(1);
                continue;
            }
            else
            {
                ILOG<<"connect to server success";
                c.m_ConnectFlag = FLAG_CONNECTINT;
                continue;
            }
        }
        
        if(FLAG_CONNECTINT == c.m_ConnectFlag)
        {
            ret = c.OnWrite(c.m_Flag,NULL,0);
            if (-1 == ret)
            {
                PLOG(ERROR)<<"send to serve error";
                c.m_ConnectFlag = FLAG_DISCONNECT;
                continue;
            }else
            {
                ILOG<<"send flag "<<hex<<(int)c.m_Flag;
                c.m_ConnectFlag = FLAG_CONNECTING;
                time(&(c.m_LastTime));
                continue;
            }
        }
        
        FD_ZERO(&(c.m_Set));
        FD_SET(c.m_ServerFd, &(c.m_Set));

        if(TIME_OUT < difftime(time(0),c.m_LastTime))
        {
            ret = c.OnWrite(c.m_Flag,NULL,0);
            continue;
        }
        
        {
            ret = select(c.m_ServerFd + 1, &(c.m_Set), NULL, NULL, &tv);
            if (ret < 0 )
             {
                PLOG(ERROR)<<"select error!";
                continue;
            }
            else if(ret == 0)
            {
                continue;
            }else
            {
                if (FD_ISSET(c.m_ServerFd, &(c.m_Set)))
                {
                    bzero(recv_msg, RECV_BUFFER_SIZE);
                    long byte_num = recv(c.m_ServerFd, recv_msg, RECV_BUFFER_SIZE, 0);
                    if (byte_num > 0)
                    {
                        byte_num = byte_num < RECV_BUFFER_SIZE ? byte_num : RECV_BUFFER_SIZE;
                        c.DataPprocess(recv_msg, byte_num);
                    }
                    else if(byte_num < 0)
                    {
                        PLOG(ERROR) << "Receive error from gate";
                        c.m_ConnectFlag = FLAG_CONNECTINT;
                        continue;
                    }
                    else
                    {
                        WLOG<<"Gate quit and wait for gate";
                        c.m_ConnectFlag = FLAG_DISCONNECT;
                        continue;
                    }
                }
            }
        }
    }//end while(1)
    return NULL;
}


int Client::DataPprocess(char* buf,int len)
{
    DataType_S *data = (DataType_S *)buf;
    if(data->dest_id != m_Flag)
    {
        WLOG<<"Recv error flag 0x"<<hex<<(int)data->dest_id<<" from 0x"<<hex<<(int)data->dest_id;
        return -1;
    }
    if(data->dest_id == data->src_id)
    {
        ILOG<<"Server recognized";
        m_ConnectFlag = FLAG_CONNECTTED;
        return len;
    }
    if(NULL != m_DataBuf )
    {
        len = m_DataBuf->PushData(buf,len);
    }
    return len;
}

int Client::OnWrite(char dest_id, char* buf, int len)
{
    if(FLAG_DISCONNECT == m_ConnectFlag)
    {
        WLOG<<"Disconnect to server right now";
        return -1;
    }
    
    int dataLen;
    if(buf == NULL || len == 0)
        dataLen = sizeof(DataType_S);
    else
        dataLen = len + sizeof(DataType_S);
        
    char *buff = new char[dataLen];
    DataType_S* data = (DataType_S* )buff;
    memset(data->head, '$', HEAD_LEN);
    data->dest_id = dest_id;
    data->src_id = m_Flag;
    data->len_h = dataLen>>8;
    data->len_l = dataLen&0xFF;
    if(dataLen != sizeof(DataType_S) )
        memcpy(buff+sizeof(DataType_S), buf, len);
    
    int ret =  send(m_ServerFd, buff, dataLen, 0);
    time(&m_LastTime);
    delete [] buff;
    buff = NULL;
    return ret ;
}

int Client::OnRead(char* buf, int len)
{    
    if(FLAG_CONNECTTED != m_ConnectFlag)
        return -1;
    return m_DataBuf->PopData(buf,len);
}

#ifdef __cplusplus  
}  
#endif