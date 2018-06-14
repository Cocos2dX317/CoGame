#include "TcpPbClient.h"
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <Network.h>
#include "Utils.h"
#include "MajiangServer.h"

using namespace mahjong_msg;
using namespace cocos2d;

static pthread_t        s_requestThread;
static pthread_t        s_responseThread;
static pthread_mutex_t  s_mutex;
static bool				isConnected;
static sem_t *          s_pSem = NULL;
static int 				isClose = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define CC_ASYNC_TCPCLIENT_USE_NAMED_SEMAPHORE 1
#else
#define CC_ASYNC_TCPCLIENT_USE_NAMED_SEMAPHORE 0
#endif

#if CC_ASYNC_TCPCLIENT_USE_NAMED_SEMAPHORE
#define CC_ASYNC_TCPCLIENT_SEMAPHORE "ccTCPAsync"
#else
static sem_t s_sem;
#endif

static Queue<CCTcpRequest*> *s_requestQueue = NULL;
static Queue<CCTcpResponse*> *s_responseQueue = NULL;

static TcpPbClient *s_pTcpClient = NULL; // pointer to singleton
static TcpPbClient *self = NULL; // pointer to singleton

static void xorfunc(std::string &nString)
{
	const int KEY = 3752;
	int strLen = (nString.length());
	char *cString = (char*)(nString.c_str());
	
	for (int i = 0; i < strLen; i++)
	{
		*(cString + i) = (*(cString + i) ^ KEY);
	}
}
static void *requestThread(void *data)
{
	CCTcpRequest *request = NULL;
	
	while (1)
	{Utils::log("tcp connect---requestThread---2222222--");
		s_requestQueue->get_msg(request);
        if (request) {
            if (CONNECT_REQ == request->cmd)
            {Utils::log("tcp connect---requestThread-----");
                self->reset_response_queue();
                self->connect_req(request);
                Utils::log("tcp connect---requestThread-11111111----");
            }
            else if (DISCONNECT_REQ == request->cmd)
            {
                self->disconnect_req(request);
            }
            else if (DATA_REQ == request->cmd)
            {
                self->data_req(request);
            }
            delete request;
        }

    }
    pthread_exit(NULL);
}

static void *responseThread(void *data)
{
	while (1)
	{        
       
        
        if (isConnected == false)
        {
            sleep(1);
            continue;
        }
        Utils::log("lct:begin to read phase in responseThread");
                
        if (isConnected)
        {
            self->read_res();
        }
        else
        {
            Utils::log("lct:no connect");
            continue;
        }
	}
	
	pthread_exit(NULL);
}

TcpPbClient* TcpPbClient::getInstance()
{
    if (s_pTcpClient == NULL) {
        self = s_pTcpClient = new TcpPbClient();
       
    }
    
    return s_pTcpClient;
}

void TcpPbClient::destroyInstance()
{
    
}

TcpPbClient::TcpPbClient()
: fd(-1)
{
    m_RecvBuf = NULL;
}

TcpPbClient::~TcpPbClient()
{
    s_pTcpClient = NULL;
    
    if(m_RecvBuf != NULL)
    {
        delete m_RecvBuf;
    }
}

bool TcpPbClient::init()
{
	header_p = (struct Header*)header;
	isConnected = false;
	
	s_requestQueue = new Queue<CCTcpRequest*>();
	s_responseQueue = new Queue<CCTcpResponse*>();
    
    m_RecvBuf = (char *)malloc(MAX_RECV_LEN);
    if(m_RecvBuf == NULL)
    {
        Utils::log("malloc m_recvBuf failed");
        return false;
    }

	
#if CC_ASYNC_TCPCLIENT_USE_NAMED_SEMAPHORE
    s_pSem = sem_open(CC_ASYNC_TCPCLIENT_SEMAPHORE, O_CREAT, 0644, 0);
    if (s_pSem == SEM_FAILED) {
        Utils::log("Open TCPCLIENT Semaphore failed");
        s_pSem = NULL;
       // return false;
    }
#else
    int semRet = sem_init(&s_sem, 0, 0);
    if (semRet < 0) {
        Utils::log("Init TCPCLIENT Semaphore failed");
        return false;
    }
    
    s_pSem = &s_sem;
#endif
	
	pthread_mutex_init(&s_mutex, NULL);
	
	pthread_create(&s_requestThread, NULL, requestThread, NULL);
	pthread_detach(s_requestThread);
	
	pthread_create(&s_responseThread, NULL, responseThread, NULL);
	pthread_detach(s_responseThread);
    
    return true;
}

void TcpPbClient::put(CCTcpRequest* request)
{
	s_requestQueue->put_msg(request);
}

CCTcpResponse *TcpPbClient::get()
{
	CCTcpResponse *response = NULL;
    if (s_requestQueue == NULL) {
        return NULL;
    }
	int ret = s_responseQueue->pop_msg(response);
	if (ret == 0)
	{
		return response;
	}
	else
	{
		return NULL;
	}	
}

void TcpPbClient::create_request(int cmd, string data)
{
    CCTcpRequest* request = new CCTcpRequest();
    request->cmd = cmd;
    request->data = data;
	s_requestQueue->put_msg(request);
}

void TcpPbClient::create_response(int cmd, string recv_data)
{
    CCTcpResponse* response = new CCTcpResponse();
    response->cmd = cmd;
    response->data = recv_data;
    s_responseQueue->put_msg(response);
}
void TcpPbClient::pop_response(){
    if (s_responseQueue) {
        s_responseQueue = new Queue<CCTcpResponse*>();
    }
}
void TcpPbClient::reset_connect()
{
    pthread_mutex_lock(&s_mutex);
	if (isConnected)
	{
        close(fd);
		fd = -1;
		isConnected = false;
        isClose = 0;
	}
    else
    {
		fd = -1;
		isConnected = false;
        isClose = 0;
    }
    pthread_mutex_unlock(&s_mutex);
}

void TcpPbClient::connect_req(CCTcpRequest *request)
{
   
    int timeout = 10;
    if (timeout <= 0) {
        timeout = 10;
    }
    
    if (sleepFlag) {
        fd = Network::tcp_connect(host.c_str(), port, timeout, 8,true);
    }else{
        fd = Network::tcp_connect(host.c_str(), port, timeout, 3);
    }
    
    Utils::log("address -------- %s   %d   %d", host.c_str(),  fd,  timeout);
    
	if (fd < 0)
	{
        create_response(CONNECT_ERROR_RES, "CONNECT_ERROR_RES");
	}
	else
	{
        create_response(CONNECT_OK_RES, "CONNECT_OK_RES");
		isConnected = true;
        Utils::log("connect is ok fd is [%d].", fd);
	}
}

void TcpPbClient::disconnect_req(CCTcpRequest *request)
{    
    reset_connect();
}

void TcpPbClient::data_req(CCTcpRequest *request)
{
	int ret = write_from_string(request->data);
    
     Utils::log("data_req    ------  %d", ret);
    
	if (ret < 0)
	{
        reset_connect();
        create_response(DISCONNECT_RES, "DISCONNECT_RES");
	}
	else
	{
        create_response(SEND_DATA_OK_RES, "SEND_DATA_OK_RES");
	}

}

bool TcpPbClient::isConnect()
{
    return isConnected;
}

ssize_t TcpPbClient::write_from_string(string &buffer)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    string result;
	//struct Header my_header;
    
    //Utils::log("buffer  -- %s", buffer.c_str());
	
    ptr = buffer.c_str();
    nleft = buffer.length();
    Utils::log("TcpPbClient write_from_string len[%d]", nleft);
    while (nleft > 0) {
        
        nwritten = write(fd, ptr, nleft);
        
        Utils::log("reponse  -- %s", ptr);
        
        if (nwritten <= 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EINPROGRESS)
                nwritten = 0;
            else
            {
                Utils::log("TcpPbClient write_from_string fd[%d] errno[%d]", fd, errno);
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }

    return result.length();
}

void TcpPbClient::set_host_port(std::string h, int p)
{
    Utils::log("set_host_port ip: %s ,  port: %d", h.c_str(), p);
    host = h;
    port = p;
    
}

void TcpPbClient::connect()
{ Utils::log("tcp connect--------");
    sleepFlag = false;
    reset_response_queue();
    reset_connect();
    create_request(CONNECT_REQ, "CONNECT_REQ");
}
void TcpPbClient::connect(bool sleep){
    Utils::log("tcp connect-----111111---");
    sleepFlag = true;
    reset_response_queue();
    reset_connect();
    create_request(CONNECT_REQ, "CONNECT_REQ");
}


void TcpPbClient::reset_response_queue()
{
    CCTcpResponse *response = NULL;
    while (true) {
        response = get();
        if (!response)
            break;
        delete response;
    }
}

void TcpPbClient::disconnect(int type)
{
    if (type == 0)
    {
        create_request(DISCONNECT_REQ, "DISCONNECT_REQ");
    }
    else
    {
        reset_connect();
    }
}

void TcpPbClient::send_data(std::string &data)
{
    //Utils::log("send --  %s", data.c_str());
    create_request(DATA_REQ, data);
}


void TcpPbClient::read_res()
{
    int res = 0;
    fd_set fds;
    struct timeval tv;
    
    tv.tv_sec = SELECT_READ_TIMEOUT / (1000 * 1000);
    tv.tv_usec = SELECT_READ_TIMEOUT % (1000 * 1000);
    
    isClose = 1;   //是否断开网络的标志位  0 代表断开  1 开启网络接收
    
    int recvlen = 0;
    int PkgLen = 0;
    int BufPos = 0;  ////剩余部分无法处理的消息长度
    
    char* lastBuf = (char *)malloc(MAX_RECV_LEN);//剩余部分无法处理的消息
    if(lastBuf == NULL)
    {
        Utils::log("lastBuf malloc failure !!");
        return ;
    }
    
    while (1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        
        res = select(fd + 1, &fds, NULL, NULL, &tv);
        if(res == 0)     //超时
        {
            continue;
        }
        else if(res < 0) //select出错则退出循环
        {
            Utils::log("select error !!");
            
            reset_connect();
            create_response(DISCONNECT_RES, "DISCONNECT_RES");
            break;
        }
        else  //有可读事件
        {
            if(FD_ISSET(fd, &fds) > 0)   //判断目标fd是否在读取集合中
            {
                //Utils::log("recv_buf : len =  %s  msg =  %d", host.c_str(),  ret);
                recvlen = read(fd, m_RecvBuf + BufPos, MAX_RECV_LEN);
                if(recvlen <= 0)
                {
                    if (errno == EAGAIN || errno == EINPROGRESS || errno == EINTR)
                    {
                        continue;
                    }
                    
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    Utils::log("DISCONNECT_RES  lct:parse header recvlen[%d] fd[%d] errno[%d]", recvlen, fd, errno);
                    break;
                }
                
                //记录下当前解m_RecvBuf（包）的位置
                int CurPos = 0;
                //循环处理m_RecvBuf（包）中的消息
                while(1)
                {
                    //验包
                    PkgLen = ByyCheckPkgLen(m_RecvBuf + CurPos, recvlen - CurPos);
                    Utils::log("address= %s , port=%d ,PkgLen=  %d", host.c_str(),port,  PkgLen);
                    if(PkgLen > 0)
                    {
                        //处理消息
                        string msg;
                        msg.assign(m_RecvBuf + CurPos, PkgLen);
                        create_response(RECV_DATA_OK_RES, msg);
                        //protoc_inst->revc_loginResponse(msg);
                        //准备下一个
                        //刚好够解包长度，解包完成
                        if(recvlen == PkgLen)
                        {
                            BufPos = 0;
                            break;
                        }
                        
                        CurPos += PkgLen;
                    }
                    else
                    {
                        BufPos = recvlen - CurPos;
                        
                        //剩余部分消息留到下一个外部循环处理
                        memcpy(lastBuf, m_RecvBuf + CurPos, BufPos);
                        memset(m_RecvBuf, 0, MAX_RECV_LEN);
                        memcpy(m_RecvBuf, lastBuf, BufPos);
                        memset(lastBuf, 0, MAX_RECV_LEN);
                        
                        break;
                    }
                }
            }
        }
        
        if (isClose == 0)
        {
            Utils::log("lct:close\n");
            break;
        }
    }
    
    free(lastBuf);
    
    Utils::log("lct:read end\n");
}
