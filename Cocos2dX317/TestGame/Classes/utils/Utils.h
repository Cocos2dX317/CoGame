
#ifndef My_Utils_h
#define My_Utils_h

#include "cocos-ext.h"
#include "cocos2d.h"
#include <string>
#include <algorithm>
#include <sstream>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <uuid/uuid.h>
#endif
using namespace std;
using namespace cocos2d;
using namespace extension;

class Utils
{
public:
    static std::string base64_encode(unsigned char const* , unsigned int len);
    static std::string base64_decode(std::string const& s);
    
    static void log(const char * pszFormat, ...) {
#if LOG_DEBUG == true
        char szBuf[kMaxLogLen+1] = {0};
        va_list ap;
        va_start(ap, pszFormat);
        vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
        va_end(ap);
        CCLog(szBuf);
#endif

    }
    //获取全局的UUID
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static std::string getUUID(){
        uuid_t uuid;
        char str[36];
        
        uuid_generate(uuid);
        uuid_unparse(uuid, str);
        
        return str;
    }
#endif
    
    
};



#endif
