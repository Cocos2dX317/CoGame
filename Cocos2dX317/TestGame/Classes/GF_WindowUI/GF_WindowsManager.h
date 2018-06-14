//
//  GF_WindowsManager.hpp
//  YSZ
//
//  Created by boyiu123 on 2018/2/2.
//
//  窗口管理类

#ifndef GF_WindowsManager_hpp
#define GF_WindowsManager_hpp

#include "Singleton.h"
#include <stdio.h>
#include "cocos2d.h"
#include "GF_WinUI.h"
USING_NS_CC;
using namespace std;

enum ZoderType{
    kcNormal = 0,
    kcMaxTop,
};
enum WindowType{
    kcTest = 0,
    kcLogin,
};

class GF_Windows : public CoaSingleton<GF_Windows>
{
public:
    //默认初始化
    static Layer* windows();
    GF_Windows();
    ~GF_Windows();
    bool init();
    
    /**
     添加窗口

     @param winui 对象
     @param zoder 层级
     */
    void addWindow(GF_WinUI* winui,WindowType type,ZoderType zoder);
    
    /**
     pop销毁窗口
     */
    void popWindow();
    
    /**
     销毁窗口
     */
    void removeWindow(GF_WinUI* winui);
    
    /**
     隐藏窗口
     */
    void hideWindow(GF_WinUI* winui);
    
    /**
     显示窗口
     */
    void showWindow(GF_WinUI* winui);
    
    /**
     get窗口

     @param type WindowType
     @return GF_WinUI
     */
    GF_WinUI* getWindow(WindowType type);
    
    
    
    //
private:
    Layer* m_rootLayer;
    vector<GF_WinUI*> m_windows;
};

typedef CoaSingleton<GF_Windows> GF_Windows_singleton;

/**
 单例

 @return GF_Windows
 */
#define GF_Windows_inis GF_Windows_singleton::getInstance()

#endif /* GF_WindowsManager_hpp */
