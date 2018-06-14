//
//  TestWin.cpp
//  MyCppGame
//
//  Created by boyiu123 on 2018/2/2.
//
//

#include "TestWin.h"

TestWin::TestWin()
{
    
}
TestWin::~TestWin()
{
    
}

bool TestWin::init()
{
    if ( !GF_WinUI::init() )
    {
        return false;
    }
    CCLOG("TestWin::init");
    
    return true;
}
void TestWin::myinit()
{
    CCLOG("Hall_Test::myinit");
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_pWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("hallui_v1/majiang_hall_check.json"));
    m_pWidget->setAnchorPoint(Vec2(0.5,0.5));
    m_pWidget->setPosition(Vec2(0,0));

    this->addChild(m_pWidget);
    
    Sprite* hello = Sprite::create("HelloWorld.png");
    hello->setPosition(Vec2(visibleSize.width/2+10, visibleSize.height/2));
    m_pWidget->addChild(hello,100);
    
    addChild(Sprite::create("HelloWorld.png"));
    
    QRSprite* qr = QRSprite::create("hello world",256);
    qr->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    m_pWidget->addChild(qr,200);
}
void TestWin::onEnter()
{
    Layer::onEnter();
    CCLOG("TestWin::onEnter");
}

void TestWin::onExit()
{
    Layer::onExit();
    CCLOG("TestWin::onExit");
}





