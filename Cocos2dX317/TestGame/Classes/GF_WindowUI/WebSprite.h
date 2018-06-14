//
//  WebSprite.hpp
//  MyCppGame-mobile
//
//  Created by zhanghui on 2018/6/8.
//
//  下载并加载图片，下载失败使用默认图片

#ifndef WebSprite_hpp
#define WebSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Singleton.h"
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::network;

//WebSpriteAgent
class WebSprite;
class WebSpriteAgent : public Node,CoaSingleton<WebSpriteAgent>
{
public:
    bool init();
    WebSprite* createWebSprite(const std::string& name,const std::string& url);
    
private:
    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    
    std::map<std::string , WebSprite*> imgs;
};
typedef CoaSingleton<WebSpriteAgent> WebSpriteAgent_Singleton;
#define WebSpriteAgentInst WebSpriteAgent_Singleton::getInstance()

class WebSprite : public Sprite
{
public:
    //默认初始化
    WebSprite();
    ~WebSprite();
    CREATE_FUNC(WebSprite);
    virtual bool init();

    std::string url;
    std::string name;
    bool isSave;
    
};


#endif /* WebSprite_hpp */
