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
    cout<<"Client 0x"<<hex<<(int)m_Flag<<" out"<<endl;
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
    m_DataBuf = new RecBuf();

    /// pthread start
    if(0 != pthread_create(&m_Tid, NULL, DataHandleThread, this)) {
        cout << "Create pthread error" << endl;
        return false;
    }

    cout<<"Client 0x"<<hex<<(unsigned char)m_Flag+0<<" check in" << endl;
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
                cout << "Socket() error" << endl;
                return 0;
            }

            ret = connect(c.m_ServerFd, (struct sockaddr *)&(c.m_Addr), sizeof(struct sockaddr_un));
            if ( -1 == ret )
            {
                cout <<"Connect to server failed" << endl;
                shutdown(c.m_ServerFd,SHUT_RDWR);
                sleep(1);
                continue;
            }
            else
            {
                cout <<"connect to server success" << endl;
                c.m_ConnectFlag = FLAG_CONNECTINT;
                continue;
            }
        }

        if(FLAG_CONNECTINT == c.m_ConnectFlag)
        {
            ret = c.OnWrite(c.m_Flag,NULL,0);
            if (-1 == ret)
            {
                cout <<"send to serve error" << endl;
                c.m_ConnectFlag = FLAG_DISCONNECT;
                continue;
            }else
            {
                cout <<"send flag "<<hex<<(unsigned char)c.m_Flag+0 << endl;
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
            //continue;
        }

        {
            ret = select(c.m_ServerFd + 1, &(c.m_Set), NULL, NULL, &tv);
            if (ret < 0 )
             {
                cout <<"select error!" << endl;
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
                        cout << "Receive error from gate" << endl;
                        c.m_ConnectFlag = FLAG_CONNECTINT;
                        continue;
                    }
                    else
                    {
                        cout <<"Gate quit and wait for gate" << endl;
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
        cout <<"Recv error flag 0x"<<hex<<(int)data->dest_id<<" from 0x"<<hex<<(int)data->dest_id << endl;
        return -1;
    }
    if(data->dest_id == data->src_id)
    {
        cout <<"Server recognized" << endl;
        m_ConnectFlag = FLAG_CONNECTTED;
        return len;
    }
    if(NULL != m_DataBuf )
    {
        len = m_DataBuf->PushData(buf,len);
    }
    return len;
}

int Client::OnWrite(char dest, char* buf, int len)
{
    if(FLAG_DISCONNECT == m_ConnectFlag)
    {
        cout <<"Disconnect to server right now" << endl;
        return -1;
    }

    int dataLen;
    if(buf == NULL || len == 0)
        dataLen = sizeof(DataType_S) + 2;
    else
        dataLen = sizeof(DataType_S) + len + 2;

    char *buff = new char[dataLen];
    bzero(buff, dataLen);
    DataType_S* data = (DataType_S* )buff;
    memset(data->head, '$', HEAD_LEN);
    data->len_h = dataLen>>8;
    data->len_l = dataLen&0xFF;
    data->dest_id = dest;
    data->src_id = m_Flag;
    if(dataLen != sizeof(DataType_S) )
        memcpy(buff+sizeof(DataType_S), buf, len);

    unsigned short crc = crc16((unsigned char*)buff,dataLen-2);
    buff[dataLen-2] = crc >> 8;
    buff[dataLen-1] = crc & 0xFF;

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
	bzero(buf,len);
    int ret =  m_DataBuf->PopData(buf,len);
	if(0 < ret && ret > (int)sizeof(DataType_S))
	{
		unsigned short crc =  ((buf[ret-2] & 0xFF) << 8) | (buf[ret-1] & 0xFF);
		if( crc != crc16((unsigned char *)buf, ret-2))
			return 0;
	}
	return ret;
}

#ifdef __cplusplus
}
#endif

