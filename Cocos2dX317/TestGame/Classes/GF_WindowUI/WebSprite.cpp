//
//  WebSprite.cpp
//  MyCppGame-mobile
//
//  Created by zhanghui on 2018/6/8.
//

#include "WebSprite.h"

WebSprite::WebSprite()
{
    
    url = "";
    name = "defult.png";
    isSave = false;
}
WebSprite::~WebSprite()
{
    
}
bool WebSprite::init()
{
    if ( !Sprite::init() )
    {
        return false;
    }
    initWithFile(name.c_str());
    return true;
}

//======================WebSpriteAgent========================
bool WebSpriteAgent::init()
{
    Node::init();
    return true;
}
WebSprite* WebSpriteAgent::createWebSprite(const std::string& name,const std::string& url)
{
    WebSprite* img = WebSprite::create();
    img->url = url;
    img->name = name;
    
    std::string path = FileUtils::getInstance()->getWritablePath()+name;
    log("path=%s", path.c_str());
    if(FileUtils::getInstance()->isFileExist(path.c_str()))
    {
        img->initWithFile(path.c_str());
        return img;
    }
    img->retain();

    imgs.insert(std::make_pair(name.c_str(), img));
    
    //加载
    HttpRequest* request = new HttpRequest();
    request->setTag(name.c_str());
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(WebSpriteAgent::onHttpRequestCompleted));
    HttpClient::getInstance()->send(request);
    request->release();
    
    return img;
}

void WebSpriteAgent::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
        return;
    }
    std::string name = response->getHttpRequest()->getTag();
    // You can get original request type from: response->request->reqType
    if (!response->isSucceed())
    {
        log("%s completed", response->getHttpRequest()->getTag());
    }
    long statusCode = response->getResponseCode();
    if (!response->isSucceed())
    {
        log("response failed");
        log("code = %ld,error buffer: %s",statusCode, response->getErrorBuffer());
        return;
    }
    
    //get data
    std::vector<char> *buffer = response->getResponseData();
    
    //create image
    Image* img = new Image();
    img->initWithImageData((unsigned char*)buffer->data(),buffer->size());
    //create texture
    Texture2D* texture = new Texture2D();
    bool isImg = texture->initWithImage(img);
    img->release();
    //create WebSprite
    if(imgs.find(name.c_str())!=imgs.end()&&isImg){
        WebSprite* webimg = imgs.find(name.c_str())->second;
        if(webimg)
        {
            webimg->initWithTexture(texture);
            
            //save file
            if(webimg->isSave){
                std::string path = FileUtils::getInstance()->getWritablePath();
                path+=webimg->name;
                
                FILE *fp = fopen(path.c_str(), "wb+");
                fwrite(buffer->data(), 1,buffer->size(), fp);
                fclose(fp);
            }
            
            
        }
        webimg->release();
        imgs.erase(name.c_str());
    }
    texture->release();
    
    
   
    
    buffer->clear();
}
