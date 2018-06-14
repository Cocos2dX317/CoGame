#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <fstream>
#include <string>
#include "cocos2d.h"

#include <sys/socket.h>
//#define MAXBUF 1024
using namespace std;

class Network {
public:
    Network();
    ~Network();
    
    static int connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);
    static int tcp_connect(const char*ip, int port, int sec);
    static int tcp_connect_6(const char*ip, int port, int sec);
    static int tcp_connect(const char*ip, int port);
    static int tcp_connect(const char*ip, int port, int sec, int cnt);
    static int tcp_connect_delay(const char*ip, int port, int sec, int cnt);
    static int tcp_connect(const char*ip, int port, int sec, int cnt,bool sleepFlag);

    static int tcp_fenfa_connect(const char*ip, int port, int sec);
    
    static std::string domainToIP(const char* pDomain);
    static bool isIPV6Net(const std::string domainStr);
};

#endif
