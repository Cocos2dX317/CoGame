
#ifndef __ZJH__ZJH_GF_ChannelManager__
#define __ZJH__ZJH_GF_ChannelManager__

#include <stdint.h>
#include <string>
#include <map>
#include "cocos2d.h"
#include <bitset>
#include "GF_GlobalDefine.h"

using namespace std;



class GF_ChannelManager : public cocos2d::Node
{
public:
    enum OS_TYPE{
        ktOS_IOS = 0,
        ktOS_ANDROID,
        ktOS_OTHER,
    };
    enum GAME_PAY_TYPE{
        kPAYT_IOS = 0,
        kPAYT_THIRD,
        kkPAYT_WEB,
    };
    
    GF_ChannelManager();
    virtual ~GF_ChannelManager();
    /** Return the shared instance **/
    static GF_ChannelManager *getInstance();
    bool init();
    void initChannelConfig();//配置渠道信息
    bool isEqualNowChannel(std::string channel);
    bool checkOpen(function_switch tag);
    bool is_osType(OS_TYPE ret);
public:
    int g_checkapp; //审核apk模式
    int g_log_debug;
    bool g_pay_debug;
    bool g_isthird_login;
    bool g_third_logout;
    int g_real_certif_var;
    int g_select_ui;
    int g_main_game;
    unsigned long long g_function_switch;
    string g_config_channel;
    string g_umeng_appid;         //umeng appid
    bool g_useWXLogin;//
    
};

#define GF_ChannelManager_ins GF_ChannelManager::getInstance()
#define isEqualNC(s) GF_ChannelManager::getInstance()->isEqualNowChannel(#s)
#define isEqualOS(s) GF_ChannelManager::getInstance()->is_osType(s)
#define WXAppID "wxf92e51752341cf2f"
#define WXAppSecret "0d27e88ce85207d33115613063bf7ffc"




#endif /* defined(__ZJH__LotteryDataMananger__) */
