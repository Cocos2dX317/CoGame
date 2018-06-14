#ifndef __QRSPRITE_H__
#define __QRSPRITE_H__
#include "cocos2d.h"

class QRSprite : public cocos2d::Sprite
{
public:
    static QRSprite* create(const std::string& code, int size = 256);
    bool initWithString(const std::string& code, int size = 256);
    
private:
    unsigned char* multiBuff(unsigned char * src, int w, int multiple);
    void onEnterForeground(Ref*);
    void updateTexture();
    
private:
    QRSprite();
    virtual ~QRSprite();
    unsigned char *m_buff;
    int m_width;
};
#endif //__QRSPRITE_H__
