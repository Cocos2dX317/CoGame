//
//  GF_RewardTip.hpp
//  MyCppGame
//
//  Created by boyiu123 on 2018/2/2.
//
//  任务奖励提示

#ifndef GF_RewardTip_hpp
#define GF_RewardTip_hpp

#include <stdio.h>
#include "GF_WinUI.h"
USING_NS_CC_EXT;

class GF_RewardTip : public GF_WinUI
{
public:
    //默认初始化
    GF_RewardTip();
    ~GF_RewardTip();
    virtual bool init();
    CREATE_FUNC(GF_RewardTip);
    
    //获得金券展示类型
    void myinit(int type);
private:
    void onEnter();
    void onExit();
    
    void setCurTip(int index);
    void initTip1();
    void initTip2();
     
    Widget* m_pWidget;
    
    
    
};

#endif /* GF_RewardTip_hpp */
