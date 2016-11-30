#include "Server.h"
#include "GLogHelper.h"

Server::Server():
m_GateFd(-1), m_ServerFd(-1), m_CrawlerFd(-1), m_OverTime(30), m_RunFlag(false)
{
    PrintVer();

    m_RecvBuf = new char[RECV_BUFFER_SIZE];
    InitUnixSocket();
    InitInetSocket();
}

Server::~Server()
{
    m_RunFlag = false;
    delete [] m_RecvBuf;
    m_RecvBuf = NULL;
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end(); ++it)
    {
        if(NULL != it->second->watchFlag)
            delete [] it->second->watchFlag;
        it->second->watchFlag = NULL;
    }
}

void Server::PrintVer(void)
{
    ILOG << PROJECT_NAME << " start.";
    ILOG << "     Programe Version V" << PROGRAM_VERSION;
    ILOG << "Attention : this version is new gate";
}

bool Server::InitUnixSocket(void)
{
    /// UNIX domain sockets
    struct sockaddr_un address;
    bzero(&address, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, GATE_SOCKET_PATH);

    /// create socket
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        PLOG(ERROR) << "socket error";
        return false;
    }
    ILOG << "Gate_sock_fd = " << fd;

    //reuse of local addresses
    int on = 1;
    if( -1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        PLOG(ERROR) << "setsockopet error";
        return false;
    }
    unlink(GATE_SOCKET_PATH);

    /// bind socket
    if(-1 == bind(fd, (struct sockaddr *)&address, sizeof(address)))
    {
        PLOG(ERROR) << "bind error";
        return false;
    }

    /// listen socket
    if(-1 == listen(fd, 5))
    {
        PLOG(ERROR) << "listen error";
        return false;
    }

    m_GateFd = fd;

    return true;
}

bool Server::InitInetSocket(void)
{

    /// INET domain sockets
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &address.sin_addr);
    address.sin_port = htons(PORT);

    /// create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        PLOG(ERROR) << "socket error";
        return false;
    }
    ILOG << "server_sock_fd = " << fd;

    //reuse of local addresses
    int on = 1;
    if( -1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        PLOG(ERROR) << "setsockopet error";
        return false;
    }

    /// bind socket
    if(-1 == bind(fd, (struct sockaddr*)&address, sizeof(address)))
    {
        PLOG(ERROR) << "bind error";
        return false;
    }

    /// listen socket
    if(-1 == listen(fd, 5))
    {
        PLOG(ERROR) << "listen error";
        return false;
    }

    m_ServerFd = fd;

    return true;
}

int Server::Run(void)
{
    m_RunFlag = true;

    struct timeval tv;
    int ret = 0;

    do
    {
        tv.tv_sec = 0;
        tv.tv_usec = 0;

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
            if (FD_ISSET(m_GateFd, &m_Set))
                SetupClientMap();

            /// is crawler connect
            if (FD_ISSET(m_ServerFd, &m_Set))
                AcceptCrawler();

            /// for each client and forward
            for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end();)
            {
                if (FD_ISSET(it->first, &m_Set))
                {
                    /// delete this client from map
                    if( -1 == RecvClient(it) )
                    {
                        FD_CLR(it->first, &m_Set);
                        close(it->first);
                        delete it->second;
                        m_ClientMap.erase(it++);
                        continue;
                    }
                }
                it++;
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
    /// add fd
    FD_SET(m_GateFd, &m_Set);
    FD_SET(m_ServerFd, &m_Set);
    m_MaxFd = (m_MaxFd < m_GateFd ? m_GateFd : m_MaxFd);
    m_MaxFd = (m_MaxFd < m_ServerFd ? m_ServerFd : m_MaxFd);

    /// add all client fd
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end();)
    {
        if(m_OverTime < (int)difftime(time(0), it->second->lastTime)
                && it->first != m_CrawlerFd )
        {
            WLOG << "Client fd " << it->first << " time out " << m_OverTime << "s and delete now";
            delete it->second;
            it->second = NULL;
            m_ClientMap.erase(it++);
            continue;
        }
        FD_SET(it->first, &m_Set);
        m_MaxFd = (m_MaxFd < it->first ? it->first : m_MaxFd);
        it++;
    }
}

void Server::SetupClientMap(void)
{
    /// new client
    int client_fd = accept(m_GateFd, NULL, NULL);
    if (client_fd > 0)
    {
        /// init struct Client_S and insert to map
        ILOG << "new client fd " << client_fd;
        Client_S *client = new Client_S;
        bzero(client, sizeof(Client_S));
        time(&(client->lastTime));
        m_ClientMap.insert(ClientPair(client_fd, client));
    }
}

void Server::AcceptCrawler(void)
{
    /// new client
    int crawler_fd = accept(m_ServerFd, NULL, NULL);
    if (crawler_fd > 0)
    {
        ILOG << "crawler[" << crawler_fd << "] is online";
        Client_S *client = new Client_S;
        bzero(client, sizeof(Client_S));
        time(&(client->lastTime));
        m_ClientMap.insert(ClientPair(crawler_fd, client));
        m_CrawlerFd = crawler_fd;
    }
}
int Server::RecvClient(ClientIter it)
{
    int ret = 0;
    bzero(m_RecvBuf, RECV_BUFFER_SIZE);
    long byte_num = recv(it->first, m_RecvBuf, RECV_BUFFER_SIZE, 0);

    if (byte_num >= (int)sizeof(DataType_S) + 2)
    {
        /// handle recv data
        byte_num = byte_num < RECV_BUFFER_SIZE ? byte_num : RECV_BUFFER_SIZE;
        //ILOG << "Recv " << byte_num << " bytes from fd " << it->first;
        time(&(it->second->lastTime));
        ret = DataPprocess(byte_num, it);
    }
    else if(0 < byte_num)
    {
        WLOG << "Recv from fd " << it->first << " lase then min bag length";
        ret = 0;
    }
    else if(0 == byte_num)
    {
        WLOG << "Client fd " << it->first << " out";
        m_CrawlerFd = -1;
        ret = -1;
    }
    else
    {
        PLOG(ERROR) << "Receive error from fd " << it->first;
        ret = 0;
    }
    return ret;
}

int Server::DataPprocess(const int DataLen, ClientIter it)
{

    char *buff = NULL;
    int len = m_ServerBuf.PushData(m_RecvBuf, DataLen);
    if(len <= 0 )
    {
        m_ServerBuf.PrintErrMsg(len);
        return len;
    }

    buff = new char[DataLen];
    if(buff == NULL)
    {
        WLOG << "new buff error";
        return -1;
    }

    while(len > 1)
    {
        bzero(buff, DataLen);
        len = m_ServerBuf.PopData(buff, DataLen);
        DataType_S *data = (DataType_S *)buff;
        if(0 >= len)
        {
            m_ServerBuf.PrintErrMsg(len);
            continue;
        }

        WriteToCrawler(buff, len);

        if( 0 == it->second->flag )
        {
            if(data->src_id == data->dest_id)
            {
                if(0 != GetClientFd(data->src_id))
                {
                    WLOG << "Get repeat client flag 0x" << hex << (data->src_id & 0xFF) << ",delete this client now." << dec;
                    continue;
                }
                else
                {
                    ILOG << "First recognition bag from 0x" << hex << (data->src_id & 0xFF) << dec;
                    it->second->flag = data->src_id;
                    if(sizeof(DataType_S) + 2 != DataLen)
                    {
                        it->second->watchLen  = (DataLen - sizeof(DataType_S) - 2) & 0xFF;
                        it->second->watchFlag = new unsigned char[it->second->watchLen];
                        ILOG << "with watch type len " << hex << (DataLen - sizeof(DataType_S) - 2) << dec;
                        memcpy(it->second->watchFlag, buff + sizeof(DataType_S), it->second->watchLen);
                    }
                    WriteTo(it->first, buff, len);
                    continue;
                }
            }
        }
        else if(it->second->flag == data->src_id)
        {
            if(data->src_id == data->dest_id)
            {
                ILOG << "Recognition bag from 0x" << hex << (data->src_id & 0xFF) << dec;
                continue;
            }
            else if(0xFF == data->dest_id)
            {
                ILOG << "Broadcast bag from 0x" << hex << (data->src_id & 0xFF);
                for(ClientIter iter = m_ClientMap.begin(); iter != m_ClientMap.end();)
                {
                    if(iter->first != it->first)
                    {
                        WriteTo(iter->first, buff, len);
                    }
                    ++iter;
                }
                continue;
            }
            else
            {
                ILOG << len << " bytes data from 0x" << hex << (data->src_id & 0xFF) << " to 0x" << hex << (data->dest_id & 0xFF) << dec;
                int dest_fd = GetClientFd(data->dest_id & 0xFF);
                if(dest_fd <= 2)
                {
                    WriteWatchFlag(buff, len);
                    continue;
                }
                WriteTo(dest_fd, buff, len);
                continue;
            }
        }
    }
    delete []buff;
    buff = NULL;
    return 0;
}

void Server::WriteWatchFlag(char *buff, int len)
{
    DataType_S *data = (DataType_S *)buff;
    for(ClientIter it = m_ClientMap.begin(); it != m_ClientMap.end(); ++it)
    {
        if(0 == it->second->watchLen || NULL == it->second->watchFlag)
            continue;

        if(it->second->flag == data->src_id)
            continue;

        for(unsigned char i = 0; i < it->second->watchLen; i++)
        {
            if(data->dest_id == it->second->watchFlag[i])
            {
                send(it->first, buff, len, 0);
                send(m_CrawlerFd, buff, len, 0);
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

int Server::WriteToCrawler(char *data, int len)
{
    if(0 >= m_CrawlerFd)
    {
        return 0;
    }

    int ret = send(m_CrawlerFd, data, len, 0);
    ILOG<<"send in WriteToCrawler return "<<ret;
    return ret;
}
void SignalHander(int signal)
{
    printf(" \n");
    WLOG << "Get signal " << signal;

    if ( signal != 6)  //28: SIGWINCH 6: SIGABRT
    {
        exit(-1);
    }
}
void CaptureAllSignal()
{
    for (int i = 0; i < 32; i ++)
    {
        if ((i == SIGINT) || (i == SIGKILL) || (i == SIGCHLD) || (i == SIGWINCH))
            {continue;}
        if (i == SIGPIPE)
    		{signal(i, SIG_IGN);}
        signal(i, SignalHander);
    }
}
int main(int argc, char *argv[])
{
    static GLogHelper glog((char *)PROJECT_NAME);
    CaptureAllSignal();

    Server gate;
    gate.Run();
    return 0;
}

