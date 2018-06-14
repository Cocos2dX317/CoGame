#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>

#include "Network.h"
#include "GF_ChannelManager.h"
Network::Network()
{

}

Network::~Network()
{

}

/* below code copy from UNP v1 source code */
int Network::connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int flags, n, error;
    socklen_t   len;
    fd_set  rset, wset;
    struct timeval  tval;

    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    error = 0;
    if ((n = connect(sockfd, (struct sockaddr *) saptr, salen)) < 0) {
        if (errno != EINPROGRESS) {
            goto done;
        }
    }
    /* Do whatever we want while the connect is taking place. */
    if (n == 0)
        goto done;    /* connect completed immediately */

    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = nsec;
    tval.tv_usec = 0;

    if ( (n = select(sockfd+1, &rset, &wset, NULL,
                     nsec ? &tval : NULL)) == 0) {
        close(sockfd);        /* timeout */
        errno = ETIMEDOUT;
        goto done;
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            goto done;            /* Solaris pending error */
        }
    } else
        cout << "Unable to do select because of " << strerror(errno) << endl;

done:
    fcntl(sockfd, F_SETFL, flags);    /* restore file status flags */

    if (error) {
        close(sockfd);        /* just in case */
        errno = error;
        return(-1);
    }
    return(0);
}
int Network::tcp_connect(const char*ip, int port, int sec)
{
    int ret;
    int fd;
    unsigned long inaddr = 0;
    struct hostent *host;
    struct sockaddr_in addr;
        
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cout << "Unable to open socket because of " << strerror(errno) << endl;
        return -1;
    }
    
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inaddr = inet_addr(ip);
    if (inaddr == INADDR_NONE) {
        host = gethostbyname(ip);
        if (host == NULL) {
            perror("gethostbyname");
            close(fd);
            return -1;
        }
        memcpy((char*)&addr.sin_addr, host->h_addr, host->h_length);
    } else
        addr.sin_addr.s_addr = inaddr;
    
    ret = connect_nonb(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in), sec);
    if (ret < 0) {
        cout << "Unable to connect " << ip <<"("<<port<<")"<< " socket because of " << strerror(errno) << endl;
        close(fd);
        return -1;
    }
    //cout << "Connect to " << inet_ntoa(addr.sin_addr) << " OK." << endl;
    
    return fd;
}


int Network::tcp_connect_6(const char*ip, int port, int sec)
{
    
    int sockfd, len;
    struct sockaddr_in6 dest;      // IPv6
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0) {      // IPv6
        //cout<<errno<<" --1--  "<<sockfd<<endl;
        return -1;
    }
   // printf("socket created\n");
    
    /* 初始化服务器端（对方）的地址和端口信息 */
    bzero(&dest, sizeof(dest));
    dest.sin6_family = AF_INET6;     // IPv6
    dest.sin6_port = htons(port);     // IPv6
    if ( inet_pton(AF_INET6, ip, &dest.sin6_addr) < 0 ) {                 // IPv6
        //cout<<errno<<" --2--  "<<sockfd<<endl;
        return -1;
    }
   // printf("address created\n");
    
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
       // cout<<errno<<" --3--  "<<sockfd<<endl;
        return -1;
    }
   // printf("server connected\n");
    
    return sockfd;
    
    
//    
//    int ret;
//    int fd;
//    unsigned long inaddr = 0;
//    struct hostent *host;
//    struct sockaddr_in6 addr;
//    
//    fd = socket(AF_INET, SOCK_STREAM, 0);
//    if (fd < 0) {
//        cout << "Unable to open socket because of " << strerror(errno) << endl;
//        return -1;
//    }
//    
//    //memset(&addr, 0, sizeof(struct sockaddr_in6));
//    bzero(&addr, sizeof(addr));
//    
//    addr.sin6_len       = sizeof(addr);
//    addr.sin6_family = PF_INET6;
//    addr.sin6_port = htons(port);
//    inaddr = inet_addr(ip);
//    
//    
//    if ( inet_pton(AF_INET6, ip, &addr.sin6_addr) < 0 ) {                 // IPv6
//        cout<<errno<<" --2--  "<<fd<<endl;
//        return -1;
//    }
//    
//    ret = connect_nonb(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in6), sec);
//    if (ret < 0) {
//        cout << "Unable to connect " << ip <<"("<<port<<")"<< " socket because of " << strerror(errno) << endl;
//        close(fd);
//        return -1;
//    }
//    return fd;
}

bool Network::isIPV6Net(const std::string domainStr)
{
    bool isIPV6Net = false;
    
    struct addrinfo *result = NULL, *curr;
    
    struct sockaddr_in6 dest;
    bzero(&dest, sizeof(dest));
    
    dest.sin6_family  = AF_INET6;
    
    int ret = getaddrinfo(domainStr.c_str(),NULL,NULL,&result);
    if (ret == 0)
    {
        for (curr = result; curr != NULL; curr = curr->ai_next)
        {
            switch (curr->ai_family)
            {
                case AF_INET6:
                {
                    isIPV6Net = true;
                    break;
                }
                case AF_INET:
                    
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    freeaddrinfo(result);
    
    return isIPV6Net;
}

std::string Network::domainToIP(const char* pDomain)
{
    if (isIPV6Net(pDomain))
    {
        struct addrinfo hint;
        memset(&hint, 0x0, sizeof(hint));
        hint.ai_family = AF_INET6;
        hint.ai_flags = AI_V4MAPPED;
        
        addrinfo* answer = NULL;
        getaddrinfo(pDomain, NULL, &hint, &answer);
        
        if (answer != NULL)
        {
            char hostname[1025] = "";
            
            getnameinfo(answer->ai_addr,answer->ai_addrlen,hostname,1025,NULL,0,0);
            
            char ipv6[128] = "";
            memcpy(ipv6,hostname,128);
            
            cout<<"domainToIP addrStr:%s" <<ipv6<<endl;
            
            return ipv6;
        }
        
        freeaddrinfo(answer);
    }
    else
    {
        struct hostent* h = gethostbyname(pDomain);
        if( h != NULL )
        {
            unsigned char* p = (unsigned char *)(h->h_addr_list)[0];
            if( p != NULL )
            {
                char ip[16] = {0};
                sprintf(ip, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
                return ip;
            }
        }
    }
    return "";
}

int Network::tcp_fenfa_connect(const char* ip, int port, int sec)
{         //cout <<"is ipv6 ---  "<<isIPV6Net(ip)<<endl;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return tcp_connect(ip, port, sec);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (isEqualOS(GF_ChannelManager::ktOS_IOS)) {
        if (false/*GF_MajiangDataManager::getInstance()->gamePayMode == 0*/) {
            if (isIPV6Net(ip)) {
                string str = domainToIP(ip);
                return tcp_connect_6(str.c_str(), port, sec);
            }
            return tcp_connect(ip, port, sec);
        }else{
            return tcp_connect(ip, port, sec);
        }
    }else{
        return tcp_connect(ip, port, sec);
    }
   
#endif

 
}

int Network::tcp_connect(const char* ip, int port)
{
    int ret;
    int fd;
    unsigned long inaddr = 0;
    struct hostent *host;
    struct sockaddr_in addr;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cout << "Unable to open socket because of " << strerror(errno) << endl;
        return -1;
    }
    
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inaddr = inet_addr(ip);
    if (inaddr == INADDR_NONE) {
        host = gethostbyname(ip);
        if (host == NULL) {
            perror("gethostbyname");
            close(fd);
            return -1;
        }
        memcpy((char*)&addr.sin_addr, host->h_addr, host->h_length);
    } else
        addr.sin_addr.s_addr = inaddr;
    
    ret = connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        cout << "Unable to connect " << ip << " socket because of " << strerror(errno) << endl;
        close(fd);
        return -1;
    }
    //cout << "Connect to " << inet_ntoa(addr.sin_addr) << " OK." << endl;
    
    return fd;
}
int Network::tcp_connect(const char*ip, int port, int sec, int cnt,bool sleepFlag){
    int ret;
    int orginCnt = cnt;
    while (1)
    {
        ret = Network::tcp_fenfa_connect(ip, port, sec);
        if (ret < 0)
        {
            cnt--;
            cout<<"ret----cnt"<<cnt<<endl;
            if (cnt < 0)
            {
                return -1;
            }
            if (sleepFlag) {
                if (orginCnt>=8) {
                    if (cnt == 0) {
                        sleep(5);
                    }else if(cnt == 1){
                        sleep(4);
                    }else if(cnt == 2){
                        sleep(3);
                    }else if(cnt == 3){
                        sleep(2);
                    }else if(cnt == 4){
                        sleep(1);
                    }
                }
            }
            
        }
        else
        {
            return ret;
        }
    }
}
int Network::tcp_connect_delay(const char*ip, int port, int sec, int cnt){
    int ret;
    
    while (1)
    {
        ret = Network::tcp_fenfa_connect(ip, port, sec);
        if (ret < 0)
        {
            cnt--;
            if (cnt < 0)
            {
                return -1;
            }
                 sleep(2);
        }
        else
        {
            return ret;
        }
    }
}
int Network::tcp_connect(const char*ip, int port, int sec, int cnt)
{
    int ret;
    
    while (1)
    {
        ret = Network::tcp_fenfa_connect(ip, port, sec);
    
        if (ret < 0)
        {
            cnt--;
            if (cnt < 0)
            {
                return -1;
            }
         //   sleep(2);
        }
        else
        {
            return ret;
        }
    }
}
