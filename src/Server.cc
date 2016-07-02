#include "Server.h"
#include "GLogHelper.h"

Server::Server()
{
    PrintVer();
    Init();
}

Server::~Server()
{
    m_RunFlag = false;
    delete [] m_RecvBuf;
    m_RecvBuf = NULL;
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end();++it)
    {
		if(NULL != it->second->watchFlag)
		    delete [] it->second->watchFlag;
		it->second->watchFlag = NULL;
    }
}

bool Server::Init(void)
{
    m_OverTime = 5;
    m_RunFlag = false;
    m_RecvBuf = new char[RECV_BUFFER_SIZE];

    /// UNIX domain sockets
    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, GATE_SOCKET_PATH);

    /// create socket
    m_ServerFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == m_ServerFd)
    {
        PLOG(ERROR) << "socket error";
        return false;
    }
    ILOG << "server_sock_fd = " << m_ServerFd;

    //reuse of local addresses
    int on = 1;
    if( 0 > setsockopt(m_ServerFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        PLOG(ERROR) << "setsockopet error";
        return false;
    }
    unlink(GATE_SOCKET_PATH);

    /// bind socket
    if(-1 == bind(m_ServerFd, (struct sockaddr *)&address, sizeof(address)))
    {
        PLOG(ERROR) << "bind error";
        return false;
    }

    /// listen socket
    if(-1 == listen(m_ServerFd, 5))
    {
        PLOG(ERROR) << "listen error";
        return false;
    }
    return true;
}

void Server::PrintVer(void)
{
    ILOG << PROJECT_NAME << " start.";
    ILOG << "     Programe Version V" << PROGRAM_VERSION;
    ILOG << "Attention : this version is new gate";
}

int Server::Run(void)
{
    m_RunFlag = true;

    struct timeval tv;
    int ret = 0;

    do
    {
        tv.tv_sec = 0;
        tv.tv_usec = 300;

        /// setup server and clients fd
        SetupFDSet();

        ret = select(m_MaxFd + 1, &m_Set, NULL, NULL, &tv);
        if (ret < 0)
        {
            ELOG << "select error";
            continue;
        }
        else if(ret == 0)
        {
            continue;
        }
        else
        {
            /// is client connect
            if (FD_ISSET(m_ServerFd, &m_Set))
                SetupClientMap();

            /// for each client and forward
            for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end();)
            {
                if (FD_ISSET(it->first, &m_Set))
            	{
                	/// delete this client from map
            		if( -1 == RecvClient(it) )
		            {
		                WLOG << "delete client 0x" << hex << (it->second->flag&0xFF) <<dec;
		                FD_CLR(it->first, &m_Set);
		                delete it->second;
		                m_ClientMap.erase(it++);
		                continue;
		            }
            	}
                ++it;
            }
        }
    }
    while(1);

    m_RunFlag = false;
    return 0;
}

/// return 0 if flag is not found
int Server::GetClientFd(unsigned char flag)
{
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end(); ++it)
    {
        if(flag == it->second->flag)
        {
            return it->first;
        }
    }
    return 0;
}

void Server::SetupFDSet(void)
{
    /// clean set and max fd
    m_MaxFd = 0;
    FD_ZERO(&m_Set);
    /// add server fd
    FD_SET(m_ServerFd, &m_Set);
    m_MaxFd = (m_MaxFd < m_ServerFd ? m_ServerFd : m_MaxFd);

    /// add all client fd
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end();)
    {
        if(m_OverTime < (int)difftime(time(0), it->second->lastTime))
        {
            WLOG << "Client fd " << it->first << " time out " << m_OverTime << "s and delete now";
            delete it->second;
            it->second = NULL;
            m_ClientMap.erase(it++);
            continue;
        }
        //ILOG << "add " << it->first << " to SET";
        FD_SET(it->first, &m_Set);
        m_MaxFd = (m_MaxFd < it->first ? it->first : m_MaxFd);
        ++it;
    }
}

void Server::SetupClientMap(void)
{
    /// new client
    int client_fd = accept(m_ServerFd, NULL, NULL);
    if (client_fd > 0)
    {
        /// init struct Client_S and insert to map
        ILOG << "new client fd " << client_fd;
        Client_S *client = new Client_S;
        bzero(client,sizeof(Client_S));
        time(&(client->lastTime));
        m_ClientMap.insert(ClientPair(client_fd, client));
    }
}

int Server::RecvClient(ClientIter it)
{
    int ret = 0;
    bzero(m_RecvBuf, RECV_BUFFER_SIZE);
    long byte_num = recv(it->first, m_RecvBuf, RECV_BUFFER_SIZE, 0);

    if (byte_num >= (int)sizeof(DataType_S)+2)
    {
        /// handle recv data
        byte_num = byte_num < RECV_BUFFER_SIZE ? byte_num : RECV_BUFFER_SIZE;
        ILOG << "Recv " << byte_num << " bytes from fd " << it->first;
        time(&(it->second->lastTime));
        ret = DataPprocess(byte_num, it);
    }
    else if(byte_num > 0)
    {
        WLOG << "Recv from fd " << it->first << " lase then min bag length";
        ret = 0;
    }
    else if(0 == byte_num)
    {
        WLOG << "Client fd " << it->first << " out";
        ret = -1;
    }
    else
    {
        PLOG(ERROR) << "Receive error from fd " << it->first;
        ret = 0;
    }
    return ret;
}

int Server::DataPprocess(int DataLen, ClientIter it)
{
    DataType_S *data = (DataType_S *)m_RecvBuf;

    if( 0 == it->second->flag )
    {
        if(data->src_id == data->dest_id)
        {
            if(0 != GetClientFd(data->src_id))
            {
                WLOG<<"Get repeat client flag 0x"<<hex<<(data->src_id&0xFF)<<",delete this client now."<<dec;
                return -1;
            }
            else
            {
                ILOG << "First recognition bag from 0x" << hex << (data->src_id&0xFF) <<dec;
                it->second->flag = data->src_id;
				if(sizeof(DataType_S)+2 != DataLen)
				{
					it->second->watchLen  = (DataLen-sizeof(DataType_S)-2)&0xFF;
					it->second->watchFlag = new unsigned char[it->second->watchLen];
                	ILOG << "with watch type len " << hex << (DataLen-sizeof(DataType_S)-2) <<dec;
					memcpy(it->second->watchFlag,m_RecvBuf+sizeof(DataType_S),it->second->watchLen);
				}
                return WriteTo(it->first, m_RecvBuf, DataLen);
            }
        }
    }
    else if(it->second->flag == data->src_id)
    {
        if(data->src_id == data->dest_id)
        {
            ILOG << "Recognition bag from 0x" << hex << (data->src_id&0xFF) <<dec;
            return 0;
        }
		else if(0xFF == data->dest_id)
		{
			ILOG<<"Broadcast bag from 0x"<<hex<<(data->src_id&0xFF);
			for(ClientIter iter = m_ClientMap.begin(); iter!= m_ClientMap.end();)
			{
				if(iter->first != it->first)
				{
					WriteTo(iter->first,m_RecvBuf,DataLen);
				}
				++iter;
			}
			return DataLen;
		}
        else
        {
            ILOG <<"From 0x"<<hex<<(data->src_id&0xFF)<<" to 0x"<<hex<<(data->dest_id&0xFF)<<dec;
			int dest_fd = GetClientFd(data->dest_id&0xFF);
			if(dest_fd <= 2)
			{
				WriteWatchFlag(m_RecvBuf, DataLen);
				return 0;
			}
			return WriteTo(dest_fd, m_RecvBuf, DataLen);
        }
    }
    return 0;
}

void Server::WriteWatchFlag(char *m_RecvBuf, int len)
{
    DataType_S *data = (DataType_S *)m_RecvBuf;
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end();++it)
    {
		if(0 == it->second->watchLen || NULL == it->second->watchFlag)
		    continue;

		if(it->second->flag == data->src_id)
			continue;

		for(unsigned char i = 0;i<it->second->watchLen;i++)
		{
		    if(data->dest_id == it->second->watchFlag[i])
		    {
				send(it->first, m_RecvBuf, len, 0);
		    }
		}
    }
	return;
}
int Server::WriteTo(int fd, char *data, int len)
{
    int ret = send(fd, data, len, 0);
    if (-1 == ret )
    {
        PLOG(WARNING) << "send to fd(" << fd << ") error";
        return 0;
    }
    return ret;
}
void SignalHander(int signal)
{
	printf(" \n");
    WLOG<<"Get signal "<<signal;

    if ( signal != 6)  //28: SIGWINCH 6: SIGABRT
    {
        exit(-1);
    }
}
void CaptureAllSignal()
{
    for (int i = 0; i < 32; i ++)
    {
        if ((i == SIGINT)||(i == SIGKILL)||(i == SIGCHLD)||(i == SIGWINCH))
            {continue;}
		if (i == SIGPIPE)
    		{signal(i, SIG_IGN);}
    	signal(i, SignalHander);
    }
}

static GLogHelper glog((char *)PROJECT_NAME);

int main(int argc, char *argv[])
{
    CaptureAllSignal();

    Server gate;
    gate.Run();
	return 0;
}
