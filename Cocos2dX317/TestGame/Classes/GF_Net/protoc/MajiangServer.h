//
//  MajiangServer.h
//  YSZ
//
//  Created by boyiu123 on 2017/4/18.
//
//

#ifndef MajiangServer_h
#define MajiangServer_h
#include "protocol.h"
#include "mahjong_msg.pb.h"
#include "byyhead.pb.h"

#include "Singleton.h"
#include "cocos2d.h"
using namespace cocos2d;

class MajiangServer :public CoaSingleton<MajiangServer> ,Node
{
public:
    MajiangServer();
    ~MajiangServer();
    bool init();
    
    void set_uid(int uid);
    int send_data(int main_cmd,int cmd,const google::protobuf::Message* msg);
    int getMainCmdByRoomId(int roomid);
    
    
    
    

    
    
    

private:
    int m_uid;
    void update(float dt);
    
    
    
    //
public:
    void test();
    int send_loginRequest(string skey,string version);
    int revc_loginResponse(string &);
    
    std::string SerializeTo(google::protobuf::Message* msg);
    bool ParseFrom(const std::string& str,google::protobuf::Message* out_msg);
};
#define MajiangServerInst MajiangServer::getInstance()
#endif /* MajiangServer_h */
