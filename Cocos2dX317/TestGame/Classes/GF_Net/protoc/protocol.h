/********************************
*文件名：protocol.h
*功能：  协议封装
*作者：  Kyle.xu
*版本：  1.0

*最后更新时间：2017.10.11
**********************************/
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include "byyhead.pb.h"


using namespace std;
using namespace byyhead;
using namespace google::protobuf;

#define PROTOCOL_MAIN_VER 1      //协议主版本号
#define PROTOCOL_SUB_VER  0      //协议次版本号
#define PROTOCOL_ENC_TYPE 0      //协议加密方式


/**
 * @brief 报文格式控制宏
 * @info  报文格式 "<head_len body_len head body>"
   
 头部  headlen   bodylen 尾部
|0x3c|----4----|----4---|0x3e|      注意： 最少有10个字节长度


示例：

 头部  headlen   bodylen    msg head        msg body      尾部
|0x3c|----4----|----4----|*************|*****************|0x3e|  

 */
#define PROTOCOL_BYY_STX           0x3c   // "<"
#define PROTOCOL_BYY_ETX           0x3e   // ">"
#define PROTOCOL_HEAD_MIN_LEN      10
#define PROTOCOL_HEAD_OFFSET       (PROTOCOL_HEAD_MIN_LEN-1)
#define PROTOCOL_BODY_LEN_OFFSET   (1+4)
#define PROTOCOL_HEAD_LEN_OFFSET   (1)


/**
 * @brief  检查报文是否完整
 * @return <0  报文非法
 * @       =0  报文不完整
 * @       >0  报文有效长度
 */
int ByyCheckPkgLen(void *buff, int len);

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
int ByyPackPkg(char** pkg, int *len, const CByyHead *head, const Message *body);

/**
 * @brief  报文打包函数[没有包体]
 * @info   [注意] 打包后的报文为出参，函数内部分配内存，调用者需要free
 * @param  pkg  [出参]打包完后报文的buffer
 *         len  [出参]打包完后，报文的长度
 *         head [入参]报文头,PB格式
 * @return =0   打包成功
 * @       <0   打包失败
 */
int ByyPackPkgNoBody(char** pkg, int *len, const CByyHead *head);

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
int  ByySetPkgHead(char *buff, int  &len, CByyHead *newHead);


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
int ByyUnpackPkg(const char *buff, int len, CByyHead *head, Message *body);

/**
 * @brief  报文解包函数(解析包头)
 * @info   [注意] 必须为完整的一个包才能调用
 * @param  buff 报文buffer
 *         len  报文长度
 *         head 消息头
 * @return <0   解包失败
 * @       =0   解包成功
 */
int ByyUnpackPkgHead(const char *buff, int len, CByyHead *head);

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
int ByyUnpackPkgBody(const char *buff, int len, Message *body);

/**
 * @brief  获取序列号
 */
unsigned int GetSequence();

#endif /*__PROTOCOL_H__*/




