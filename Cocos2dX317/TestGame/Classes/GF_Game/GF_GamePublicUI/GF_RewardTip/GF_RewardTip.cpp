//
//  GF_RewardTip.cpp
//  MyCppGame
//
//  Created by boyiu123 on 2018/2/2.
//
//

#include "GF_RewardTip.h"
#include "GF_WindowsManager.h"

GF_RewardTip::GF_RewardTip()
{
    
}
GF_RewardTip::~GF_RewardTip()
{
    
}

bool GF_RewardTip::init()
{
    if ( !GF_WinUI::init() )
    {
        return false;
    }
    CCLOG("GF_RewardTip::init");
    
    return true;
}

void GF_RewardTip::myinit(int type)
{
    CCLOG("GF_RewardTip::myinit");
    setBgVisable(true);
    setKeyRelease(true);
    //Widget
    m_pWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("task/majaing_reward_dia.json"));
    m_pWidget->setAnchorPoint(Vec2(0.5,0.5));
    m_pWidget->setOpacity(80);
    this->addChild(m_pWidget);

    
    //

    setCurTip(type);
    
}


void GF_RewardTip::setCurTip(int index)
{
    ImageView* rewardTip1 = static_cast<ImageView*>(m_pWidget->getChildByName("type1"));
    rewardTip1->setVisible(false);
    rewardTip1->setTouchEnabled(false);
    ImageView* rewardTip2 = static_cast<ImageView*>(m_pWidget->getChildByName("type2"));
    rewardTip2->setVisible(false);
    rewardTip2->setTouchEnabled(false);
    
    if(index == 1){
        rewardTip1->setVisible(true);
        rewardTip1->setTouchEnabled(true);
        initTip1();
    }else if(index == 2){
        rewardTip2->setVisible(true);
        rewardTip2->setTouchEnabled(true);
        initTip2();
    }
}
void GF_RewardTip::initTip1()
{
//    ImageView* rewardTip1 = static_cast<ImageView*>(m_pWidget->getChildByName("type1"));
//    ImageView* ok_btn  = static_cast<ImageView*>(rewardTip1->getChildByName("ok_btn"));
//    ok_btn->setTouchEnabled(true);
//    ok_btn->addTouchEventListener(this, toucheventselector(GF_RewardTip::touchOkEvent));
//    ImageView* cancel_btn  = static_cast<ImageView*>(rewardTip1->getChildByName("cancel_btn"));
//    cancel_btn->setTouchEnabled(true);
//    cancel_btn->addTouchEventListener(this, toucheventselector(GF_RewardTip::touchCancelEvent));
//    CheckBox* tipcheck  = static_cast<CheckBox*>(rewardTip1->getChildByName("check_tip"));
//    tipcheck->setTouchEnabled(true);
//    tipcheck->addEventListenerCheckBox(this, checkboxselectedeventselector(GF_RewardTip::touchCheckEvent));
//    static_cast<Label*>(tipcheck->getChildByName("label"))->setTouchEnabled(true);
//    static_cast<Label*>(tipcheck->getChildByName("label"))->addTouchEventListener(this, toucheventselector(GF_RewardTip::touchLabelEvent));
//
//    ImageView* propbg  = static_cast<ImageView*>(rewardTip1->getChildByName("propbg"));
//    ImageView* icon  = static_cast<ImageView*>(propbg->getChildByName("icon"));
//    Label* pname  = static_cast<Label*>(propbg->getChildByName("label"));
//    sprintf(mBuf, "%d金券",10);
//    pname->setText(mBuf);
//    memset(mBuf, 0, sizeof(mBuf));
//
//
//    Label* lb_1  = static_cast<Label*>(rewardTip1->getChildByName("lb_1"));
//    Label* lb_2  = static_cast<Label*>(rewardTip1->getChildByName("lb_2"));
//    sprintf(mBuf, "%lld金券",GF_MajiangDataManager::getInstance()->quan_money);
//    lb_2->setText(mBuf);
//    memset(mBuf, 0, sizeof(mBuf));
//    lb_1->setPositionX(-(lb_1->getContentSize().width+lb_2->getContentSize().width+5)/2+lb_1->getContentSize().width/2);
//    lb_2->setPositionX(+(lb_1->getContentSize().width+lb_2->getContentSize().width+5)/2-lb_2->getContentSize().width/2);
//
//
//    Label* lb_3  = static_cast<Label*>(rewardTip1->getChildByName("lb_3"));
//    Label* lb_4  = static_cast<Label*>(rewardTip1->getChildByName("lb_4"));
//    Label* lb_5  = static_cast<Label*>(rewardTip1->getChildByName("lb_5"));
//    Label* lb_6  = static_cast<Label*>(rewardTip1->getChildByName("lb_6"));
//    Label* lb_7  = static_cast<Label*>(rewardTip1->getChildByName("lb_7"));
//
//
//    float allwidth = lb_3->getContentSize().width+lb_4->getContentSize().width+lb_5->getContentSize().width
//    +lb_6->getContentSize().width+lb_7->getContentSize().width+20;
//    lb_3->setPositionX(-allwidth/2+lb_3->getContentSize().width/2);
//    lb_4->setPositionX(-allwidth/2+lb_3->getContentSize().width+5+lb_4->getContentSize().width/2);
//    lb_5->setPositionX(-allwidth/2+lb_3->getContentSize().width+lb_4->getContentSize().width+lb_5->getContentSize().width/2);
//    lb_6->setPositionX(-allwidth/2+lb_3->getContentSize().width+lb_4->getContentSize().width+lb_5->getContentSize().width+lb_6->getContentSize().width/2);
//    lb_7->setPositionX(-allwidth/2+lb_3->getContentSize().width+lb_4->getContentSize().width+lb_5->getContentSize().width+lb_6->getContentSize().width+lb_7->getContentSize().width/2);
    
    
}
void GF_RewardTip::initTip2()
{
    
}

void GF_RewardTip::onEnter()
{
    CCLayer::onEnter();
    CCLOG("GF_RewardTip::onEnter");
}

void GF_RewardTip::onExit()
{
    CCLayer::onExit();
    CCLOG("GF_RewardTip::onExit");
}





