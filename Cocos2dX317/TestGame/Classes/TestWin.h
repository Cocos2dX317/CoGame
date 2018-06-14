//
//  TestWin.hpp
//  MyCppGame
//
//  Created by boyiu123 on 2018/2/2.
//
//

#ifndef TestWin_hpp
#define TestWin_hpp

#include <stdio.h>
#include "GF_WinUI.h"

class TestWin : public GF_WinUI
{
public:
    //默认初始化
    TestWin();
    ~TestWin();
    virtual bool init();
    CREATE_FUNC(TestWin);
    
    void myinit();
private:
    void onEnter();
    void onExit();
    
    Widget* m_pWidget;
    
};

#endif /* TestWin_hpp */
