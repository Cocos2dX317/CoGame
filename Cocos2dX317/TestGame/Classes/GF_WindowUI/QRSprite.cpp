#include "QRSprite.h"
#include "qrencode.h"
using namespace cocos2d;

const static Color3B foreColor = Color3B(48, 131, 201);
const static Color3B backColor = Color3B::WHITE;

bool QRSprite::initWithString(const std::string& code, int size)
{
    QRcode* qrcodeObj = QRcode_encodeString(code.c_str(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    if (qrcodeObj != nullptr)
    {
        auto w = qrcodeObj->width;
        auto len = w * w;
        for (int i = 0; i < w; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                auto idx = i * w + j;
                auto color = (qrcodeObj->data[idx] & 1) == 1 ? 255 : 0;
                qrcodeObj->data[idx] = color;
            }
        }
        
        auto multi = size / w;
        w *= multi;
        CC_SAFE_DELETE(m_buff);
        m_buff = multiBuff(qrcodeObj->data, qrcodeObj->width, multi);
        m_width = w;
        updateTexture();
        setScale(size*1.0f / w);
        QRcode_free(qrcodeObj);
        return true;
    }
    
    return false;
}

QRSprite* QRSprite::create(const std::string& code, int size)
{
    QRSprite* ret = new QRSprite();
    
    if (ret && ret->initWithString(code, size))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    
    return nullptr;
}

QRSprite::QRSprite()
{
    m_buff = nullptr;
    m_width = 0;
    NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(QRSprite::onEnterForeground), EVENT_COME_TO_FOREGROUND, nullptr);
}

QRSprite::~QRSprite()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
    CC_SAFE_DELETE(m_buff);
}

unsigned char* QRSprite::multiBuff(unsigned char * src, int w, int multiple)
{
    if (!src || multiple <= 1 || w == 0) return src;
    unsigned char * buff = new unsigned char[w * w * multiple * multiple];
    const auto width = multiple * w;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            auto pix = src[i * w + j];
            const auto row = i * multiple;
            const auto column = j * multiple;
            for (int k = 0; k < multiple; k++)
            {
                for (int m = 0; m < multiple; m++)
                {
                    buff[(row + k) * width + column + m] = pix;
                }
            }
        }
    }
    return buff;
}

void QRSprite::onEnterForeground(Ref*)
{
    updateTexture();
}

void QRSprite::updateTexture()
{
    if (!m_buff) return;
    auto txtr = new Texture2D;
    txtr->initWithData(m_buff, m_width * m_width, Texture2D::PixelFormat::A8, m_width, m_width, Size(m_width, m_width));
    txtr->autorelease();
    Sprite::initWithTexture(txtr);
}
