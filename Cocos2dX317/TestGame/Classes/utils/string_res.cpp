#include "string_res.h"
#include "tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"
#include "cocos2d.h"
#include <stdio.h>
#include "md5.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif

using namespace std;
using namespace tinyxml2;
using namespace cocos2d;
bool string_res::add_file(const char* file)
{
    string fullPath=FileUtils::getInstance()->fullPathForFilename(file);
    
    Data data = FileUtils::getInstance()->getDataFromFile(fullPath.c_str());
    if (data.isNull())
    {
        return false;
    }
    
    tinyxml2::XMLDocument doc;
    XMLError error = doc.Parse((const char*)data.getBytes(), data.getSize());
    if (error != XML_SUCCESS)
        return false;
    XMLElement* dict = doc.FirstChildElement();
    if (!dict) return false;
    XMLElement* element = dict->FirstChildElement("i");
    while (element)
    {
        string k = element->Attribute("k");
        string v = element->Attribute("v");
        m_dict[file].insert(make_pair(k, v));
        element = element->NextSiblingElement();
    }
    
	return true;
}

const char* string_res::get_string(const char* file,const char* key)
{
    map<string,string>::iterator i;
	i = m_dict[file].find(key); 
    if(i != m_dict[file].end() && i->second.length() > 0)
        return i->second.c_str();
    return 0;
}

void string_res::clear()
{
    m_dict.clear();
}

/*加密/解密子函数开始*/
void string_res::deEncryptData(unsigned char** copyData, const char* key, ssize_t dataLen){
    unsigned char* _data = *copyData;
    unsigned char* de_start = _data;
    unsigned char* de_end = _data + dataLen;
    ssize_t keyLen = strlen(key);
    ssize_t keyIndex = 0;
    for (; de_start <= de_end;de_start++,keyIndex++){
        if (keyIndex >= keyLen)
            keyIndex = 0;
        *de_start ^= key[keyIndex];
    }
}



