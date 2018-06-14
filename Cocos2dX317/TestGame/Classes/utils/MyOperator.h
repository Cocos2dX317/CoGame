/**
 * 重载操作 
 * add: wujunru
 * date: 2014.03.31
 */

#ifndef ZJH_MyOperator_h
#define ZJH_MyOperator_h

#include "cocos2d.h"

using namespace cocos2d;

// -- operator +

inline cocos2d::CCPoint operator + ( const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2 ) {
    return ccp( p1.x + p2.x, p1.y + p2.y );
}

inline cocos2d::CCPoint operator + ( const cocos2d::CCPoint& p1, const cocos2d::CCSize& s2 ) {
    return ccp( p1.x + s2.width, p1.y + s2.height );
}

inline cocos2d::CCPoint operator + ( const cocos2d::CCSize& s1, const cocos2d::CCPoint& p2 ) {
    return ccp( s1.width + p2.x, s1.height + p2.y );
}

inline cocos2d::CCPoint operator + ( const cocos2d::CCSize& s1, const cocos2d::CCSize& s2 ) {
    return ccp( s1.width + s2.width, s1.height + s2.height );
}

// -- operator -

inline cocos2d::CCPoint operator - ( const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2 ) {
    return ccp( p1.x - p2.x, p1.y - p2.y );
}

inline cocos2d::CCPoint operator - ( const cocos2d::CCPoint& p1, const cocos2d::CCSize& s2 ) {
    return ccp( p1.x - s2.width, p1.y - s2.height );
}

inline cocos2d::CCPoint operator - ( const cocos2d::CCSize& s1, const cocos2d::CCPoint& p2 ) {
    return ccp( s1.width - p2.x, s1.height - p2.y );
}

inline cocos2d::CCPoint operator - ( const cocos2d::CCSize& s1, const cocos2d::CCSize& s2 ) {
    return ccp( s1.width - s2.width, s1.height - s2.height );
}

// -- operator *

inline cocos2d::CCPoint operator * ( const cocos2d::CCPoint& p1, const float scale ) {
    return ccp( p1.x * scale, p1.y * scale );
}

inline cocos2d::CCPoint operator * ( const cocos2d::CCSize& s1, const float scale ) {
    return ccp( s1.width * scale, s1.height * scale );
}

inline cocos2d::CCPoint operator * ( const float scale, const cocos2d::CCPoint& p2 ) {
    return ccp( scale * p2.x, scale * p2.y );
}

inline cocos2d::CCPoint operator * ( const float scale, const cocos2d::CCSize& s2 ) {
    return ccp( scale * s2.width, scale * s2.height );
}

// -- operator /

inline cocos2d::CCPoint operator / ( const cocos2d::CCPoint& p1, const float scale ) {
    return ccp( p1.x/scale, p1.y/scale );
}

inline cocos2d::CCPoint operator / ( const cocos2d::CCSize& s1, const float scale ) {
    return ccp( s1.width/scale, s1.height/scale );
}

inline cocos2d::CCPoint operator / ( const float scale, const cocos2d::CCPoint& p2 ) {
    return ccp( scale/p2.x, scale/p2.y );
}

inline cocos2d::CCPoint operator / ( const float scale, const cocos2d::CCSize& s2 ) {
    return ccp( scale/s2.width, scale/s2.height );
}

inline cocos2d::CCSpriteFrame* getMySpriteFrame( const std::string name ) {
    CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( name.c_str() );
    
    if( pFrame ) {
        return pFrame;
    }
    CCLOG("spriteFrame create is fail by getMySpriteFrame(/*name*/) %s ", name.c_str() );
    
    return NULL;
}

inline cocos2d::CCSprite* getMySprite( const std::string name ) {
    CCSprite* pSprite = CCSprite::createWithSpriteFrame( getMySpriteFrame( name ) );
    if( pSprite ) {
        return pSprite;
    }
    CCLOG("sprite create is fail by getMySprite(/*name*/) %s ", name.c_str() );
    return NULL;
}

#endif

