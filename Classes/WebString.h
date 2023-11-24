#ifndef __WEB_STRING_H__
#define __WEB_STRING_H__

#include "cocos2d.h"

#define MIN_SCALE_RATE_WEBSTRING 0.0 //başlangıç hali, en küçük olabileceği ebat oranı
#define SCAT_DURATION 0.1  //uzama süresi
#define SHORTEN_DURATION 0.1  //kısalma süresi
#define WEBSTRING_PNG_FILE "web/webString.png"

class WebString : public cocos2d::Sprite
{
public:
    WebString();
    ~WebString();
    static WebString* createWebString();
    bool initWebString();

    void scat(float x,float yBottom, float yTop);//alttan üste doğru uzayacak WebString
    void shorten();

private:
    float _originalHeight;
    float _minScaleRate;

    float _scatDuration;
    void _initScatAction();
    void _finishedScatAction();

    float _shortenDuration;
    void _initShortenAction();
    void _finishedShortenAction();
};
#endif
