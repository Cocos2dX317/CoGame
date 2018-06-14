#include <sys/types.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "protocol.h"

using namespace std;
using namespace byyhead;
using namespace google::protobuf;


/**
 * @brief  检查报文是否完整
 * @return <0  报文非法
 * @       =0  报文不完整
 * @       >0  报文有效长度
 */
int ByyCheckPkgLen(void *buff, int len)
{
    char *pkg = (char *)buff;

    // 入参检查
    if (NULL == pkg || len <= 0)
    {
        return -1;
    }

    // 报文还没有接收完成
    if (len <= PROTOCOL_HEAD_MIN_LEN)
    {
        return 0;
    }

    // 报文合法性检查
    if (*pkg != PROTOCOL_BYY_STX) 
    {
        return -1;
    }

    int  head_len = htonl(*(int *)(pkg + PROTOCOL_HEAD_LEN_OFFSET));
    int  body_len = htonl(*(int *)(pkg + PROTOCOL_BODY_LEN_OFFSET));
    int  msg_len  = head_len + body_len + PROTOCOL_HEAD_MIN_LEN;

    // 报文合法性检查
    if ((head_len < 0) || (body_len < 0) || (msg_len <= PROTOCOL_HEAD_MIN_LEN))
    {
        return -1;
    }

    // 报文还未收完整
    if (len < msg_len)
    {
        return 0;
    }

    // 报文合法性检查
    if (*(pkg + msg_len - 1) != PROTOCOL_BYY_ETX) 
    {
        return -1;
    }

    return msg_len;
}


/**
 * @brief  报文打包函数
 * @info   [注意] 打包后的报文为出参，函数内部分配内存，调用者需要free
 * @param  pkg  [出参]打包完后报文的buffer
 *         len  [出参]打包完后，报文的长度
 *         head [入参]报文头,PB格式
 *         body [入参]报文体,PB格式
 * @return =0   打包成功
 * @       <0   打包失败
 */
int  ByyPackPkg(char** pkg, int  *len, const CByyHead *head, const Message *body)
{
    char *buf;

    // 参数检查
    if (NULL == head || NULL == pkg || NULL == len || NULL == body)
    {
        return -1;
    }

    // 初始化报文长度
    int  head_len = head->ByteSize();
    int  body_len = body->ByteSize();
    int  msg_len  = PROTOCOL_HEAD_MIN_LEN + head_len + body_len;

    // 检查包头是否初始化
    if (!head->IsInitialized() || !head_len)
    {
        return -1;
    }

    // 检查包体是否初始化
    if (!body->IsInitialized() || !body_len)
    {
        return -1;
    }

    // 分配内存
    buf = (char *)malloc(msg_len);
    if (NULL == buf)
    {
        return -1;
    }

    // 组装报文
    // 格式 "(head_len body_len head body)"
    char *pos = buf;
    *pos++ = PROTOCOL_BYY_STX;
    *(int  *)pos = htonl(head_len);
    pos += sizeof(int );
    *(int  *)pos = htonl(body_len);
    pos += sizeof(int );

    // 序列化包头
    if (!head->SerializeToArray(pos, head_len))
    {
        free(buf);
        return -1;
    }
    pos += head_len;

    // 序列化包体
    if (body_len && !body->SerializeToArray(pos, body_len))
    {
        free(buf);
        return -1;
    }
    pos += body_len;
    *pos = PROTOCOL_BYY_ETX;

    // 设置出参
    *pkg = buf;
    *len = msg_len;

    return 0;
}


/**
 * @brief  报文打包函数[没有包体]
 * @info   [注意] 打包后的报文为出参，函数内部分配内存，调用者需要free
 * @param  pkg  [出参]打包完后报文的buffer
 *         len  [出参]打包完后，报文的长度
 *         head [入参]报文头,PB格式
 * @return =0   打包成功
 * @       <0   打包失败
 */
int  ByyPackPkgNoBody(char** pkg, int  *len, const CByyHead *head)
{
    char *buf;

    // 参数检查
    if (NULL == head || NULL == pkg || NULL == len)
    {
        return -1;
    }

    // 初始化报文长度
    int  head_len = head->ByteSize();
    int  msg_len  = PROTOCOL_HEAD_MIN_LEN + head_len;

    // 检查包头是否初始化
    if (!head->IsInitialized() || !head_len)
    {
        return -1;
    }

    // 分配内存
    buf = (char *)malloc(msg_len);
    if (NULL == buf)
    {
        return -1;
    }

    // 组装报文
    // 格式 "(head_len body_len head body)"
    char *pos = buf;
    *pos++ = PROTOCOL_BYY_STX;
    *(int  *)pos = htonl(head_len);
    pos += sizeof(int );
    *(int  *)pos = 0;
    pos += sizeof(int );

    // 序列化包头
    if (!head->SerializeToArray(pos, head_len))
    {
        free(buf);
        return -1;
    }
    pos += head_len;
    *pos = PROTOCOL_BYY_ETX;

    // 设置出参
    *pkg = buf;
    *len = msg_len;

    return 0;
}


/**
 * @brief  报文解包函数
 * @info   [注意] 必须为完整的一个包才能调用
 *         [注意] 报文可能没有包体
 * @param  buff 报文buffer
 *         len  报文长度
 *         head 消息头
 *         body 消息体
 * @return <0   解包失败
 * @       =0   解包成功
 */
int  ByyUnpackPkg(const char *buff, int  len, CByyHead *head, Message *body)
{
    // 入参检查
    if (NULL == buff || len <= 0 || NULL == head || NULL == body)
    {
        return -1;
    }

    // 报文合法性检查
    if ((len <= PROTOCOL_HEAD_MIN_LEN)
        || (*buff != PROTOCOL_BYY_STX)
        || (*(buff+len-1) != PROTOCOL_BYY_ETX))
    {
        return -1;
    }

    int  head_len = htonl(*(int *)(buff + PROTOCOL_HEAD_LEN_OFFSET));
    int  body_len = htonl(*(int *)(buff + PROTOCOL_BODY_LEN_OFFSET));
    int  msg_len  = head_len + body_len + PROTOCOL_HEAD_MIN_LEN;

    // 报文合法性检查
    if (body_len < 0 || head_len <= 0 || msg_len != len)
    {
        return -1;
    }

    // 解包头
    if (!head->ParseFromArray(buff + PROTOCOL_HEAD_OFFSET, head_len))
    {
        return -1;
    }

    // 解包体
    if (body_len && !body->ParseFromArray(buff + PROTOCOL_HEAD_OFFSET + head_len, body_len))
    {
        return -1;
    }

    return 0;
}

/**
 * @brief  设置新的包头
 * @info   [注意] 必须为完整的一个包才能调用
 *       
 * @param  buff    报文buffer
 *         len     报文长度
 *         newHead 新包头  
 * @return <0   设置失败
 * @       =0   设置成功
 */
int  ByySetPkgHead(char *buff, int  &len, CByyHead *newHead)
{
    // 入参检查
    if (NULL == buff || len <= 0)
    {
        return -1;
    }

    int  head_len = htonl(*(int *)(buff + PROTOCOL_HEAD_LEN_OFFSET));
    int  body_len = htonl(*(int *)(buff + PROTOCOL_BODY_LEN_OFFSET));
    int  msg_len  = head_len + body_len + PROTOCOL_HEAD_MIN_LEN;

    // 报文合法性检查
    if (body_len < 0 || head_len <= 0 || msg_len != len)
    {
        return -1;
    }

	//申请内存用于存储包体
	char *MsgBody = (char *)malloc(body_len);

    //保存包体
    memcpy(MsgBody, buff + PROTOCOL_HEAD_OFFSET + head_len, body_len);

	//////////////重新打包
	
	// 初始化报文长度
    int  newhead_len = newHead->ByteSize();
    int  newbody_len = body_len;
    int  newmsg_len  = PROTOCOL_HEAD_MIN_LEN + newhead_len + newbody_len;

    // 检查包头是否初始化
    if (!newHead->IsInitialized() || !newhead_len)
    {
		free(MsgBody);
        return -1;
    }

    // 组装报文
    // 格式 "(head_len body_len head body)"
    char *pos = buff;
    *pos++ = PROTOCOL_BYY_STX;
    *(int  *)pos = htonl(newhead_len);
    pos += sizeof(int );
    *(int  *)pos = htonl(newbody_len);
    pos += sizeof(int );

    // 序列化包头
    if (!newHead->SerializeToArray(pos, newhead_len))
    {
        free(MsgBody);
        return -1;
    }
    pos += newhead_len;

	//直接将包体数据拷贝进去
	memcpy(pos, MsgBody, newbody_len);
    
    pos += newbody_len;
    *pos = PROTOCOL_BYY_ETX;

    // 设置出参
    len = newmsg_len;

	free(MsgBody);
	
    return 0;
}


/**
 * @brief  报文解包函数(解析包头)
 * @info   [注意] 必须为完整的一个包才能调用
 * @param  buff 报文buffer
 *         len  报文长度
 *         head 消息头
 * @return <0   解包失败
 * @       =0   解包成功
 */
int  ByyUnpackPkgHead(const char *buff, int  len, CByyHead *head)
{
    // 入参检查
    if (NULL == buff || NULL == head || len <= 0)
    {
        return -1;
    }

    // 报文合法性检查
    if ((len <= PROTOCOL_HEAD_MIN_LEN)
        || (*buff != PROTOCOL_BYY_STX)
        || (*(buff+len-1) != PROTOCOL_BYY_ETX))
    {
        return -1;
    }

    int  head_len = htonl(*(int *)(buff + PROTOCOL_HEAD_LEN_OFFSET));
    int  body_len = htonl(*(int *)(buff + PROTOCOL_BODY_LEN_OFFSET));
    int  msg_len  = head_len + body_len + PROTOCOL_HEAD_MIN_LEN;

    // 报文合法性检查
    if (body_len < 0 || head_len < 0 || msg_len != len)
    {
        return -1;
    }

    // 解析包头
    if (!head->ParseFromArray(buff + PROTOCOL_HEAD_OFFSET, head_len))
    {
        return -1;
    }

    return 0;
}


/**
 * @brief  报文解包函数(解析包体)
 * @info   [注意] 必须为完整的一个包才能调用
 *         [注意] 可能没有包体
 * @param  buff 报文buffer
 *         len  报文长度
 *         body 消息体
 * @return <0   解包失败
 * @       =0   解包成功
 */
int  ByyUnpackPkgBody(const char *buff, int  len, Message *body)
{
    // 参数检查
    if (NULL == buff || NULL == body || len <= 0)
    {
        return -1;
    }

    // 报文合法性检查
    if ((len <= PROTOCOL_HEAD_MIN_LEN)
        || (*buff != PROTOCOL_BYY_STX)
        || (*(buff+len-1) != PROTOCOL_BYY_ETX))
    {
        return -1;
    }

    int  head_len = htonl(*(int *)(buff + PROTOCOL_HEAD_LEN_OFFSET));
    int  body_len = htonl(*(int *)(buff + PROTOCOL_BODY_LEN_OFFSET));
    int  msg_len  = head_len + body_len + PROTOCOL_HEAD_MIN_LEN;

    // 报文合法性检查
    if (body_len < 0 || head_len < 0 || msg_len != len)
    {
        return -1;
    }

    // 解析包体
    if (body_len && !body->ParseFromArray(buff + PROTOCOL_HEAD_OFFSET + head_len, body_len))
    {
        return -1;
    }

    return 0;
}

/**
 * @brief  获取序列号,序列号从1起始
 */
unsigned int GetSequence()
{
	static unsigned int s_Sequence = 1;
	
	if(s_Sequence >= 0xFFFFFFFF)  //32位无符号整形
	{
		s_Sequence = 1;
	}
	
	s_Sequence++;
	
	return s_Sequence;
}



