//
//  GF_WinUI.cpp
//  YSZ
//
//  Created by boyiu123 on 2018/2/2.
//
//

#include "GF_WinUI.h"
#include "GF_WindowsManager.h"

GF_WinUI::GF_WinUI()
{
    
}
GF_WinUI::~GF_WinUI()
{
    
}

bool GF_WinUI::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("GF_WinUI::init");
    
    //public CCSpriteFrameCache
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("task/alerts_materials.plist", "task/alerts_materials.png");
    cache->addSpriteFramesWithFile("task/common_fnt.plist", "task/common_fnt.png");
    cache->addSpriteFramesWithFile("task/common_title.plist", "task/common_title.png");
    
    //
    m_bg  = ImageView::create();
    m_bg->loadTexture("bg.jpg");
    addChild(m_bg);
    
    m_bg->setScale(Director::getInstance()->getContentScaleFactor());
    
    
    return true;
}
void GF_WinUI::onEnter()
{
    Layer::onEnter();
    CCLOG("GF_WinUI::onEnter");
    
    //对手机返回键的监听
    auto listener = EventListenerKeyboard::create();
    //和回调函数绑定
    listener->onKeyReleased = CC_CALLBACK_2(GF_WinUI::onKeyReleased,this);
    //添加到事件分发器中
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

}

void GF_WinUI::onExit()
{
    Layer::onExit();
    CCLOG("GF_WinUI::onExit");
}

void GF_WinUI::setBgFile(string file)
{
    m_bg->loadTexture(file.c_str());
}

void GF_WinUI::setBgVisable(bool visable)
{
    m_bg->setVisible(visable);
}

void GF_WinUI::onClose()
{
    
    GF_Windows_inis->removeWindow(this);
    if(m_backWin)
    {
        GF_Windows_inis->showWindow(m_backWin);
    }
}

void GF_WinUI::setShowBack(GF_WinUI* winui)
{
    m_backWin = winui;
}

void GF_WinUI::setKeyRelease(bool enable)
{
    isKeyRelease = enable;
}

void GF_WinUI::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event){
    if(!isKeyRelease){
        return;
    }
    CCLOG("GF_WinUI::onKeyReleased: keycode = %d,unused_event = ?",keyCode);
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
        onClose();
    }

}



