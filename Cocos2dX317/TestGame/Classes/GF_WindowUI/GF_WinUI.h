//
//  GF_WinUI.hpp
//  YSZ
//
//  Created by boyiu123 on 2018/2/2.
//
//  窗口基类

#ifndef GF_WinUI_hpp
#define GF_WinUI_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "QRSprite.h"
#include "GF_ChannelManager.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;
using namespace std;

class GF_WinUI : public cocos2d::Layer
{
public:
    //默认初始化
    GF_WinUI();
    ~GF_WinUI();
    virtual bool init();
    CREATE_FUNC(GF_WinUI);
    
    /**
     设置背景
     
     @param file 文件名
     */
    void setBgFile(string file);
    
    //设置背景可见
    void setBgVisable(bool visable);
    
    //关闭
    void onClose();
    
    //关闭
    void setKeyRelease(bool enable);
    
    
    /**
     在关闭后弹出winui
     
     @param winui 窗体
     */
    void setShowBack(GF_WinUI* winui);
    
    /**
     自定义初始化
     */
    virtual void myinit(){};
    //返回键
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event);
private:
    void onEnter();
    void onExit();
    
    ImageView* m_bg;    //背景
    GF_WinUI* m_backWin; //
    bool isKeyRelease;
    
};

#endif /* GF_WinUI_hpp */
