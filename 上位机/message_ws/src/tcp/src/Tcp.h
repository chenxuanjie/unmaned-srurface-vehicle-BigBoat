#ifndef	__TCP_H
#define __TCP_H


#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include "track.h"


#define SERVER_PORT     8888     /* 服务器端口号 */
#define SERVER_IP       "10.23.22.218"
#define TCP_MAX_SIZE       100

class TCP {
public:
    TCP();

    int SetMode(string s);

    vector<string> SplitString(string s);

    char ConvertCommand(string s);

    int SendPoints(vector<Point> points);

    vector<Point> RecievePoints(string points_s, const string &pattern);

private:
    bool m_push_is_open;
    bool m_is_recieving;
    bool m_close_connection;
};

int Tcp_Reconnect(void);

/* -function about server ----------------------------*/
int Tcp_Server_Init(uint16_t port);
int Tcp_Server_Send(const char* buf);
int Tcp_Server_Read(char* buf);
void Tcp_Server_Close(void);
/* -function about client ---------------------------*/
int Tcp_Client_Init(uint16_t port, const char* IP);
int Tcp_Client_Send(const char* buf);
int Tcp_Client_Read(char* buf);
void Tcp_Client_Close(void);

#endif