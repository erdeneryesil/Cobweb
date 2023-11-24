#ifndef __STRONGESTBUG_H__
#define __STRONGESTBUG_H__

#include "Bug.h"

#define PI 3.14159265
#define GADFLY_PNG_FILE "bugs/spritesheet/gadfly.png"
#define GADFLY_PLIST_FILE "bugs/spritesheet/gadfly.plist"
#define ROBBERFLY_PNG_FILE "bugs/spritesheet/robberfly.png"
#define ROBBERFLY_PLIST_FILE "bugs/spritesheet/robberfly.plist"
#define REVOLVE_QUARTES_R_FACTOR 0.45
#define STRONGESTBUG_FLY_SPRITE_FRAME_COUNT 16
#define STRONGESTBUG_FLY_SPRITE_DURATION_PER_FRAME 0.05f

class StrongestBug : public Bug{
protected:
    void _initVectorRandom() override=0;
    void _flyUpdate() override;

    enum FlyStatus{run=0,revolve,revolveBack};
    enum RevolveOverflowType{top=0,bottom,left,right};
    FlyStatus _flyStatus;
    void _initFlyStatusRandom();
    int _flyStatusTimer;
    virtual void _changeRunParametersRandom()=0;
    virtual void _changeRevolveParametersRandom()=0;
    void _changeRevolveBackParameters();
    typedef void(StrongestBug::*FunctionPointer)();
    FunctionPointer _flyStatusFunctionPointer; //fly status a göre bu pointer _flyFunc ya da _revloveFunc fonksiyonlardan birine işaret edecek
    void _setFlyStatus(FlyStatus flyStatus);
    void _setFlyStatusFunctionPointer();
    void _runFunction();
    void _revolveFunction();
    void _revolveBackFunction();
    cocos2d::Vec2* _revolveCenterPoint;
    float _revolveR;
    float _revolveQuarterR;//_revolveR değeri Web nesnesine göre belirlenecek, bu sebeple kullanılacakj bu değişken
    float _revolveIterationValue;//- saat yönü, + saat yönü tersi
    float _revolveCurrentAngle;
    float _revolveStartY;
    RevolveOverflowType _revolveOverflowType;
    void _revolveOverflowControl();

    cocos2d::Sprite* _flySprite;//retain-release
    cocos2d::Sequence* _flySpriteSequence;//retain-release
    virtual void _initFlySprite()=0;
public:
    StrongestBug();
    ~StrongestBug();
    static StrongestBug* createStrongestBug(BugType bugType,float velocity);
    bool initStrongestBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity);
    void disableMighty();//Bonus kazanıldığında, güçsüz hale getirir, _powerParticle kapatılır
    void catchStart(Web* web)override;
    int getPoint()override=0;
};

#endif
