//
//  GF_WindowsManager.cpp
//  YSZ
//
//  Created by boyiu123 on 2018/2/2.
//
//

#include "GF_WindowsManager.h"

GF_Windows::GF_Windows()
{
    
}
GF_Windows::~GF_Windows()
{
    
}

Layer* GF_Windows::windows()
{
    GF_Windows_inis->m_rootLayer = LayerColor::create(Color4B(255,255,255,0));
    return GF_Windows_inis->m_rootLayer;
}
bool GF_Windows::init()
{
    
    return true;
}

void GF_Windows::addWindow(GF_WinUI* winui,WindowType type,ZoderType zoder)
{
    m_rootLayer->addChild(winui,zoder);
    winui->setTag(type);
    
    winui->myinit();
    m_windows.push_back(winui);
    
}

void GF_Windows::popWindow()
{
    if(m_windows.empty())
    {
        return;
    }
    GF_WinUI* last = m_windows.back();
    m_rootLayer->removeChild(last);
    
    
    m_windows.pop_back();
    
}

void GF_Windows::removeWindow(GF_WinUI* winui)
{
    vector<GF_WinUI*>::iterator iter = m_windows.begin();
    for (; iter!=m_windows.end(); )
    {
        if((*iter) == winui)
        {
            m_rootLayer->removeChild(winui);
            iter = m_windows.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void GF_Windows::hideWindow(GF_WinUI* winui)
{
    if(winui)
    {
        winui->setVisible(false);
    }
}

void GF_Windows::showWindow(GF_WinUI* winui)
{
    if(winui)
    {
        winui->setVisible(true);
    }
}

GF_WinUI* GF_Windows::getWindow(WindowType type)
{
    GF_WinUI* findTag = NULL;
    vector<GF_WinUI*>::iterator iter = m_windows.begin();
    for (; iter!=m_windows.end(); iter++)
    {
        if((*iter)->getTag() == type)
        {
            findTag = (*iter);
            break;
        }
    }
    
    return findTag;
}
















