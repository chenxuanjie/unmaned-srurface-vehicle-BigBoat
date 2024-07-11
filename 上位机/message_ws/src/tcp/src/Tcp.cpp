#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>
#include "Tcp.h"


static int connfd_server;
static int sockfd_client;

struct sockaddr_in ServerAddr;
struct sockaddr_in ClientAddr;
socklen_t ClientAddr_len = sizeof(ClientAddr);
int sockfd1 = 0;

TCP::TCP()
{
    m_push_is_open = false;
    m_is_recieving = false;
    m_close_connection = false;
}

/*
* S: 模式设置
* P: 客户端发点至服务器
* R: 服务器发点至客户端
* Q: 
*/
int TCP::SetMode(string s)
{
    /* 错误判断 */
    if (s[0] != 'S')
    {
        cout << "Tcp SetMode error: the input string is wrong." << endl;
        return -1;
    }

    /* 设定标志位 */
    m_push_is_open = s[2] == '1' ? true : false;	/* 1为打开，0为关闭 */
    m_is_recieving = s[4] == '3' ? true : false;	/* 3为打开，2为关闭 */
    m_close_connection = s[6] == 'q' ? true : false;	/* q为关闭 */

    /* 处理对应的标志 */
    if (!m_close_connection)
    {
        cout << "Tcp SetMode: close server." << endl;
        Tcp_Server_Close();
    }
    return 0;
}

/* 只查找一次，不允许存在相同指令 */
vector<string> TCP::SplitString(string s)
{
    vector<char> signs = { 'S', 'P', 'Q'};
    vector<string> sub_strings;
    size_t pos;
    map<int, char> signs_in_strings;

    /* 查找所有对应的符号 */
    for (vector<char>::iterator sign = signs.begin(); sign != signs.end(); sign++)
    {
        if ((pos = s.find(*sign)) != string::npos)
            /* 记录符号及对应的位置 */
            signs_in_strings.insert(pair<int, char>(pos, *sign));
    }
    /* 如果没有找到对应的符号 */
    if (signs_in_strings.empty())
    {
        cout << "SplitString: find string error! String does not fit for transmit protocol." << endl;
        return vector<string>();;
    }
    /* 根据key值，分割字符串*/
    else
    {
        int start = 0;

        for (map<int, char>::iterator it = signs_in_strings.begin(); it != signs_in_strings.end(); it++)
        {
            sub_strings.push_back(s.substr(start, it->first - start));
            start = it->first;
        }
        sub_strings.push_back(s.substr(start));
    }

    return sub_strings;
}

char TCP::ConvertCommand(string s)
{
    switch(s[0])
    {
        case 'S':
            return 'S';
        case 'P':
            return 'P';
        case 'Q':
            return 'Q';
        default:
            return '\0';
    }
}

int TCP::SendPoints(vector<Point> points)
{
    string points_s = "R";
    string pattern = ",";
    string temp;

    /* 检查传入点集是否为空 */
    if (points.empty())
    {
        cout << "Send points error: The points is empty." << endl;
        return -1;
    }

    /* 将点集转换为字符串 */
    for (vector<Point>::iterator it = points.begin(); it != points.end(); it++)
    {
        /* 经度 */
        temp = to_string(it->m_longitude);
        points_s += temp;
        points_s += pattern;
        /* 纬度 */
        temp = to_string(it->m_latitude);
        points_s += temp;
        points_s += pattern;
    }
    points_s += "E";

    /* 发送点集 */
    if (-1 == send(connfd_server, points_s.c_str(), points_s.size(), 0))
    {
        perror("server send failed");
        printf("This message is failed to send:\r\n\t%s\r\n", points_s.c_str());
        return -1;
    }
    else
    {
        cout << "server send message: " << points_s << endl;
    }

    return 0;
}

/* 解析传输协议，返回点集 */
vector<Point> TCP::RecievePoints(string points_s, const string &pattern)
{
    int pos(0);
    vector<Point> points;

    /* 检查传入字符串是否有误 */
    if (points_s.find('R') == string::npos)
    {
        cout << "Convert string error: The string does not meet 'R' at first. " << endl;
        return points;
    }
    else
        points_s = points_s.substr(1, points_s.size());	/* 去掉R，便于处理字符串 */

    /* 从字符串首字母开始找逗号 */
    while ((pos = points_s.find(pattern)) != string::npos)
    {
        Point point(0, 0);
        string temp;

        /* 得到经度 */
        temp = points_s.substr(0, pos);
        point.m_longitude = stof(temp);
        /* 得到纬度 */
        points_s = points_s.substr(pos + 1, points_s.size());	/* 从逗号的下一位开始为新的字符串 */
        if ((pos = points_s.find(pattern)) != string::npos)
        {
            temp = points_s.substr(0, pos);
            point.m_latitude = stof(temp);
            points_s = points_s.substr(pos + 1, points_s.size());	/* 从逗号的下一位开始为新的字符串 */
        }

        points.push_back(point);
    }
    return points;
}

int Tcp_Server_Init(uint16_t port)
{
    char* ip_str = new char(20);
    
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    memset(&ClientAddr, 0, sizeof(ClientAddr));
    //填充变量
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddr.sin_port = htons(SERVER_PORT);

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);    /* get socket descriptor */
    if (-1 == sockfd1)
    {
        perror("server create socket failed");
        return -1;
    }
    /* 绑定地址 */
    if (-1 == bind(sockfd1, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)))
    {
        perror("server bind address failed");
        return -1;
    }
    cout << "listening..." << endl;
      /* 监听客户端的端口信息 */
    if (-1 == listen(sockfd1, 50)) // 待修改
    {
        perror("server listen failed");
        return -1;
    }
    cout << "accepting..." << endl;
    /* 等待客户端接收 */
    connfd_server = accept(sockfd1, (struct sockaddr* )&ClientAddr, &ClientAddr_len);
    if (-1 == connfd_server)
    {
        perror("server accept failed");
        return -1;
    }
    cout << "server aceept success!" << endl;
    /* show client information */
    inet_ntop(AF_INET, &ClientAddr, ip_str, sizeof(ip_str));
    cout << "The client port: " << ClientAddr.sin_port << endl;
    cout << "The client IP: " << ip_str << endl;

    return connfd_server;
}

int Tcp_Server_Send(const char* buf)
{
    static int n = 0;
    /* check socket */
    if (connfd_server <= 0)
    {
        cout << "Server send failed! Please check your network, maybe your socket is no run as expected." << endl;
        return -1;
    }
    /* send buf */
    if (-1 == send(connfd_server, buf, TCP_MAX_SIZE, 0))
    {
        perror("server send failed");
        printf("This message is failed to send:\r\n\t%s\r\n", buf);
        return -1;
    }
    else
    {
        ;
        // cout << "server send message: " << ++n << endl;  
    } 

    return 0;
}

int Tcp_Reconnect(void)
{
    if (-1 == listen(sockfd1, 50)) // 待修改
    {
        perror("server listen failed");
        return -1;
    }
    cout << "accepting..." << endl;
    /* 等待客户端接收 */
    connfd_server = accept(sockfd1, (struct sockaddr* )&ClientAddr, &ClientAddr_len);
    if (-1 == connfd_server)
    {
        perror("server accept failed");
        return -1;
    }

    return 0;
}

int Tcp_Server_Read(char* buf)
{
    /* check socket */
    if (connfd_server <= 0)
    {
        cout << "Server read failed! Please check your network, maybe your socket is no run as expected." << endl;
        return -1;
    }
    /* read buf */
    /* check socket */
    if (connfd_server <= 0)
    {
        cout << "Server read failed! Please check your network, maybe your socket is no run as expected." << endl;
        return -1;
    }
        /* 阻塞等待 */
    if (recv(connfd_server, buf, TCP_MAX_SIZE, 0) <= 0)
    {
        perror("server read failed");
        return -1;
    }
    cout << "server read: " << buf << endl;

    return 0;
}

void Tcp_Server_Close(void)
{
    close(connfd_server);
}

/**
* 线程初始化函数（客户端）。
* 阻塞等待通信。建议利用多线程。
**/
int Tcp_Client_Init(uint16_t port, const char* IP)
{
    struct sockaddr_in ServerAddr;    
    socklen_t ServerAddr_len = sizeof(ServerAddr);

    memset(&ServerAddr, 0, sizeof(ServerAddr));
    
    //填充变量
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(SERVER_PORT);
    if (-1 == inet_pton(AF_INET, IP, &ServerAddr.sin_addr.s_addr))
    {
        perror("client get IP failed");
        return -1;
    }
    /* socket init */
    sockfd_client = socket(AF_INET, SOCK_STREAM, 0);    /* get socket descriptor */
    if (-1 == sockfd_client)
    {
        perror("create socket failed");
        return -1;
    }
    /* connect with server */
    cout << "connecting..." << endl;
    while (-1 == connect(sockfd_client, (struct sockaddr *)&ServerAddr, ServerAddr_len))
    {
        perror("client connect failed");
    }
    cout << "client: connect succeess!" << endl;

    return sockfd_client;
}

int Tcp_Client_Send(const char* buf)
{
    static int n = 0;

    /* check socket */
    if (sockfd_client <= 0)
    {
        cout << "Client send failed! Please check your network, maybe your socket is no run as expected." << endl;
        return -1;
    }

    if (-1 == send(sockfd_client, buf, TCP_MAX_SIZE, 0))
    {
        perror("client send failed");
        printf("This message is failed to send:\r\n\t%s\r\n", buf);
        return -1;
    }
    else
    {
        cout << "client send message: " << ++n << endl;  
    } 

    return 0;
}

int Tcp_Client_Read(char* buf)
{
    /* check socket */
    if (sockfd_client <= 0)
    {
        cout << "Client read failed! Please check your network, maybe your socket is no run as expected." << endl;
        return -1;
    }
        /* 阻塞等待 */
    if (-1 == recv(sockfd_client, buf, TCP_MAX_SIZE, 0))
    {
        perror("client read failed");
        return -1;
    }
    cout << "client read: " << buf << endl;

    return 0;
}

void Tcp_Client_Close(void)
{
    close(sockfd_client);
}
