#ifndef __STRING_RES_H__
#define __STRING_RES_H__
#include "Singleton.h"
#include <map>
#include <string>

using namespace std;

class string_res : public CoaSingleton<string_res>
{
public:
    bool add_file(const char* file);
    const char* get_string(const char* file,const char* key);
    void clear();
    bool isEncrypted(std::string path);
    void decryptfile(const char *in_filename,const char *out_file);
    void deEncryptData(unsigned char** copyData, const char* key, ssize_t dataLen);
private:
    map<string, map<string,string> > m_dict;
};

typedef CoaSingleton<string_res> string_res_singleton;

#define XML_GAME_CONFIG(s) string_res_singleton::getInstance()->get_string("game_config.xml",(s))
#define XML_PTAG(s) string_res_singleton::getInstance()->get_string("ptag.xml",(s))

#endif // __STRING_RES_H__
