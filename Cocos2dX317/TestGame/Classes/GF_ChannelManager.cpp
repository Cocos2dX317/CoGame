
#include "GF_ChannelManager.h"
#include "string_res.h"
#include "Utils.h"


GF_ChannelManager *channelManager;

GF_ChannelManager::GF_ChannelManager()
{
    
}

GF_ChannelManager::~GF_ChannelManager()
{
    
}

GF_ChannelManager* GF_ChannelManager::getInstance()
{
    if (channelManager == NULL) {
        channelManager = new GF_ChannelManager();
        channelManager->init();
        channelManager->retain();
    }
    
    return channelManager;
}
bool GF_ChannelManager::init(){
    g_checkapp = !strcmp(XML_GAME_CONFIG("checkapp"), "true");
    g_log_debug = !strcmp(XML_GAME_CONFIG("log_debug"), "true");
    g_pay_debug = !strcmp(XML_GAME_CONFIG("pay_debug"), "true");
    g_umeng_appid = XML_GAME_CONFIG("umeng_appid");
    g_useWXLogin = !strcmp(XML_GAME_CONFIG("g_useWXLogin"), "true");
    g_isthird_login = !strcmp(XML_GAME_CONFIG("third_login"), "true");
    g_third_logout = !strcmp(XML_GAME_CONFIG("third_logout"), "true");
    g_real_certif_var = atoi(XML_GAME_CONFIG("real_certif_var"));
    g_select_ui = atoi(XML_GAME_CONFIG("selectUI"));
    g_main_game = atoi(XML_GAME_CONFIG("main_game"));
    std::string function_str = XML_GAME_CONFIG("function_switch");
    std::bitset<64> function_bits = std::bitset<64>(function_str);
    g_function_switch = function_bits.to_ulong();
    Utils::log("g_pay_debug  = %d",g_pay_debug);
    Utils::log("g_function_switch  = %lld",g_function_switch);
    Utils::log("checkOpen  0= %d,1=%d,2=%d,3=%d",checkOpen(kfWorldChat),checkOpen(kfSafeBox),checkOpen(kfTask),checkOpen(kfHuoDong));
    //todo
    //    g_function_switch = kWorldChat|kSafeBox;
    //    g_function_switch = g_function_switch|kHuoDong;
    
    
    
    return true;
}
void GF_ChannelManager::initChannelConfig(){
    g_config_channel = XML_GAME_CONFIG("channel");
    //channelId =XML_GAME_CONFIG("channel");
    //channelName = XML_GAME_CONFIG("channel_name");
    
}//配置渠道信息

bool GF_ChannelManager::isEqualNowChannel(std::string channel)
{
    return g_config_channel == channel;
}
bool GF_ChannelManager::checkOpen(function_switch tag)
{
    return CHECK_FUNCTION_MASK(g_function_switch,tag);
}
bool GF_ChannelManager::is_osType(OS_TYPE ret){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return ret == ktOS_IOS;
# elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return ret == ktOS_ANDROID;
#else
    return ret == ktOS_OTHER;
#endif
}





