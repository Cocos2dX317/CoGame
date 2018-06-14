//
//  protocManager.cpp
//  YSZ
//
//  Created by boyiu123 on 2017/4/18.
//
//
#include "MajiangServer.h"
#include <stdio.h>
#include "Utils.h"
#include <fstream>
#include <iostream>
#include "TcpPbClient.h"


using namespace mahjong_msg;

MajiangServer::MajiangServer()
{
    m_uid = 0;
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
}
MajiangServer::~MajiangServer()
{
    google::protobuf::ShutdownProtobufLibrary();
}

bool MajiangServer::init()
{

    return true;
}

void MajiangServer::set_uid(int uid){
    m_uid = uid;
}

//
int MajiangServer::send_data(int main_cmd,int cmd,const google::protobuf::Message* msg)
{
    CByyHead ByyHead;
    char *Pkg = NULL;
    int PkgLen = 0;
    
    //消息头封包
    ByyHead.set_main_ver(PROTOCOL_MAIN_VER);
    ByyHead.set_sub_ver(PROTOCOL_SUB_VER);
    ByyHead.set_main_cmd(main_cmd);
    ByyHead.set_sub_cmd(cmd);
    ByyHead.set_sequence(GetSequence());
    ByyHead.set_enc_type(PROTOCOL_ENC_TYPE);
    ByyHead.set_pro_type(0);
    ByyHead.set_from_uid(m_uid);
    
    //消息体封包
    
    ByyPackPkg(&Pkg, &PkgLen, &ByyHead, msg);
    
    ///////////数据包发送////////////////
    // Pkg：数据指针  PkgLen：数据长度
    string buf;
    buf.assign(Pkg,PkgLen);
    
    
    TcpPbClient::getInstance()->send_data(buf);
    /////////////////////////////////////
    
    free(Pkg);  //发送完需要释放内存
    return 0;

}
void MajiangServer::update(float dt)
{
    CCTcpResponse* response = TcpPbClient::getInstance()->get();
    if(!response){
        return;
    }
    Utils::log("MajiangServer::update cmd %d",response->cmd);
    if (response->cmd == DISCONNECT_RES) //关闭
    {
        TcpPbClient::getInstance()->set_host_port("192.168.8.201",20111);
        TcpPbClient::getInstance()->connect();
        
    }
    else if (response->cmd == CONNECT_OK_RES)
    {
        Utils::log("MajiangServer::update CONNECT_OK_RES");
    }
    else if (response->cmd == CONNECT_ERROR_RES)
    {
        
    }
    else if(response->cmd == RECV_DATA_OK_RES)
    {
        CByyHead ByyHead;
        ::google::protobuf::Message* ResBody;
        ByyUnpackPkg(response->data.c_str(), (int)response->data.size(), &ByyHead, ResBody);  //协议解包
        
        unsigned int main_cmd = ByyHead.main_cmd();
        unsigned int sub_cmd  = ByyHead.sub_cmd();
        unsigned int sequence = ByyHead.sequence();  //获取序列号,需要优先处理，要验证包序号
        unsigned int uid      = ByyHead.from_uid();
        int          result   = ByyHead.result();    //业务返回的错误码   参照 mahjong_proto.h
        string       err_msg  = ByyHead.err_msg();   //业务返回的错误描述
        Utils::log( "DemoUnPkg:sequence = %ud ,main_cmd = %u,sub_cmd = %u,uid = %u,result = %d, err_msg = %s\n",sequence,main_cmd,sub_cmd,uid,result,err_msg.c_str());
        if(main_cmd == 10000)
        {
            std::string pStr;
            ResBody->SerializeToString(&pStr);
            Utils::log( "%s",pStr.c_str());
            if(sub_cmd == 0)
            {
                
                
            }
        }
        
    }
    else if(response->cmd == 4004)
    {

    }
    
}

int MajiangServer::getMainCmdByRoomId(int roomid)
{
    int cmd = 0;
    if(roomid == 1000){
        cmd = 10000;
    }else if(roomid == 1001){
        cmd = 10000;
    }else if(roomid == 1002){
        cmd = 10000;
    }else if(roomid == 1003){
        cmd = 10000;
    }else if(roomid == 1004){
        cmd = 10000;
    }else if(roomid == 2000){//喇叭
        cmd = 10000;
    }
    return cmd;
}



//========================
void MajiangServer::test()
{
    TcpPbClient::getInstance()->set_host_port("192.168.8.201",20111);
    TcpPbClient::getInstance()->connect();
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(MajiangServer::update), this,0.02f, false);
    
    m_uid = 1000141;
    //
    send_loginRequest("c3d88b111d8a4f17ff7cc3365387fd9f","v1.0");
}
int MajiangServer::send_loginRequest(string skey,string version){
    Utils::log( "send_loginRequest");
    loginRequest ReqBody;  //登陆请求
    //消息体封包
    ReqBody.set_skey(skey);
    ReqBody.set_version(version);
    MajiangServerInst->send_data(10000, 0, &ReqBody);
    
    
    return 0;
}

int MajiangServer::revc_loginResponse(string &revc_data)
{
    //////////////数据包接收//////////////
    //revc_data接收的数据
    

    //////////////////////////////////////
    
    CByyHead ByyHead;
    loginResponse ResBody;  //登陆反馈
    
    ByyUnpackPkg(revc_data.c_str(), (int)revc_data.size(), &ByyHead, &ResBody);  //协议解包
    
    unsigned int sequence = ByyHead.sequence();  //获取序列号,需要优先处理，要验证包序号
    unsigned int main_cmd = ByyHead.main_cmd();
    unsigned int sub_cmd  = ByyHead.sub_cmd();
    unsigned int uid      = ByyHead.from_uid();
    
    int          result   = ByyHead.result();    //业务返回的错误码   参照 mahjong_proto.h
    string       err_msg  = ByyHead.err_msg();   //业务返回的错误描述
    
    int          vid      = ResBody.vid();       //返回的房间号
    Utils::log( "DemoUnPkg:sequence = %ud ,main_cmd = %u,sub_cmd = %u,uid = %u,result = %d, err_msg = %s,vid = %d\n",sequence,main_cmd,sub_cmd,uid,result,err_msg.c_str(),vid);
    return 0;

}

std::string MajiangServer::SerializeTo(google::protobuf::Message* msg)
{
    std::string pStr="";
    //序列化
    if (!msg->SerializeToString(&pStr)) {
        Utils::log( "Faild to SerializeToString");
        return "";
    }
    return pStr;
}
bool MajiangServer::ParseFrom(const std::string& str,google::protobuf::Message* out_msg)
{
    //反序列化
    if (!out_msg->ParseFromString(str)) {
        Utils::log( "Faild to ParseFromString msg ");
        return false;
    }
    return true;
}



