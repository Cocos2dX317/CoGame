#ifndef _TCP_PB_CLIENT_H_
#define _TCP_PB_CLIENT_H_

#include <string>
#include <semaphore.h>
#include <Queue.h>

#define MAX_RECV_LEN 64*1024
#define SELECT_READ_TIMEOUT 300*1000   //读取超时时间 微秒

enum request_cmd
{
    CONNECT_REQ = 0,
    DISCONNECT_REQ,
    DATA_REQ,
};

enum response_cmd
{
    CONNECT_OK_RES = 0,
    CONNECT_ERROR_RES,
    SEND_DATA_OK_RES,
    SEND_DATA_ERROR_RES,
    RECV_DATA_OK_RES,
    RECV_DATA_ERROR_RES,
    DISCONNECT_RES,
};

struct Header {
    //unsigned char   sig[2];
    unsigned int    length;
    //unsigned int    info;
};

class CCTcpRequest
{
public:
    int cmd;
    std::string data;
};

class CCTcpResponse
{
public:
    int cmd;
    std::string data;
};


class TcpPbClient
{
public:
    /** Return the shared instance **/
    static TcpPbClient *getInstance();
    
    /** Relase the shared instance **/
    static void destroyInstance();
	
    TcpPbClient();
    virtual ~TcpPbClient();
    bool init(void);
	void put(CCTcpRequest* request);
	CCTcpResponse *get();
    void create_request(int cmd, std::string data);
    void create_response(int cmd, std::string recv_data);
    void pop_response();
    void reset_connect();
	void connect_req(CCTcpRequest *request);
    void disconnect_req(CCTcpRequest *request);
	void data_req(CCTcpRequest *request);
    void local_process(CCTcpRequest *request);
	ssize_t write_from_string(std::string &buffer);
	void set_host_port(std::string h, int p);
	void connect();
    void connect(bool sleep);
    void reset_response_queue();
    void disconnect(int type = 0);
	void send_data(std::string &data);
    void read_res();
    bool isConnect();
public:
    int 				fd;
	std::string 		host;
	int 				port;
    char                header[sizeof(struct Header)];
    struct Header       *header_p;
    bool                sleepFlag;
    char              * m_RecvBuf;  //接收数据的缓冲区
};

#endif //_TCP_PB_CLIENT_H_
