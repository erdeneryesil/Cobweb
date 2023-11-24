#ifndef __BONUSBUG_H__
#define __BONUSBUG_H__

#include "Bug.h"

#define BONUSBUG_PNG_FILE "bugs/spritesheet/glowworm.png"
#define BONUSBUG_PLIST_FILE "bugs/spritesheet/glowworm.plist"
#define BONUSBUG_POINT 0
#define BONUSBUG_FLY_STATUS_COUNT 4
#define BONUSBUG_CONSTANT_1 3000.0
#define BONUSBUG_CONSTANT_2 100.0
#define BONUSBUG_CONSTANT_3 1.8
#define BONUSBUG_CONSTANT_4 0.4
#define BONUSBUG_FLY_SPRITE_PNG_FILE "bugs/spritesheet/glowwormFlySprite.png"
#define BONUSBUG_FLY_SPRITE_PLIST_FILE "bugs/spritesheet/glowwormFlySprite.plist"
#define BONUSBUG_FLY_SPRITE_FRAME_COUNT 8
#define BONUSBUG_FLY_SPRITE_DURATION_PER_FRAME 0.05f
#define BONUSBUG_FLY_SPRITE_POS_H_RATE 1.1f
#define BONUSBUG_FLY_SPRITE_POS_V_RATE 2.0f

class BonusBug : public Bug{
public:
    enum BonusType{disableMighty=0,increaseWeb};
    BonusBug ();
    ~BonusBug ();
    static BonusBug * createBonusBug(BonusType bonusType,float velocity);
    bool initBonusBug(std::string pngFile,std::string plistFile,BonusType bonusType,float velocity);

    BonusType getBonusType();
    void catchStart(Web* web)override;
    int getPoint()override;

protected:
    BonusType _bonusType;
    cocos2d::Size _visibleSize;
    float _vectorYUpdate;
    float _vectorYUpdateDefault;
    float _vectorYUpdateDecrease;
    enum FlyStatus{descent=0,rise,run};
    FlyStatus _flayStatusArray[4];
    int _flyStatusIndex;
    void _changeDescentRiseParametersRandom();
    void _changeRunParametersRandom();

    void _initVectorRandom()override;
    void _flyUpdate() override;

    FlyStatus _flyStatus;
    int _flyStatusTimer;
    void _setRunTimerRandom();
    void _setDescenRiseTimerRandom();

    typedef void(BonusBug ::*FunctionPointer)();
    FunctionPointer _flyStatusFunctionPointer; //fly status a göre bu pointer _flyFunc, _descentFunc ya da _riseFunc fonksiyonlardan birine işaret edecek

    void _setFlyStatus(FlyStatus flyStatus);
    void _setFlyStatusFunctionPointer();
    void _descentFunction();
    void _descentOverflowControl();
    void _riseFunction();
    void _riseOverflowControl();

    void _runFunction();

    static int _bonusBugPoint;

    cocos2d::Sprite* _flySprite;//retain-release
    cocos2d::Sequence* _flySpriteSequence;//retain-release
    void _initFlySprite();
};

#endif



