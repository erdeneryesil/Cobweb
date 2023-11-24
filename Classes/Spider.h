#ifndef __SPIDER_H__
#define __SPIDER_H__

#include "cocos2d.h"
#include "Web.h"
#include "WebString.h"

#define SPIDER_SPRITE_SPRITESHEET_PNG_FILE "spider/spritesheet/sprite.png"
#define SPIDER_SPRITE_SPRITESHEET_PLIST_FILE "spider/spritesheet/sprite.plist"
#define SPIDER_SLOWWALK_SPRITESHEET_PNG_FILE "spider/spritesheet/slowWalk.png"
#define SPIDER_SLOWWALK_SPRITESHEET_PLIST_FILE "spider/spritesheet/slowWalk.plist"
#define SPIDER_JUMP_SPRITESHEET_PNG_FILE "spider/spritesheet/jump.png"
#define SPIDER_JUMP_SPRITESHEET_PLIST_FILE "spider/spritesheet/jump.plist"
#define SPIDER_SAMESIDELEFT_SPRITESHEET_PNG_FILE "spider/spritesheet/sameSideLeft.png"
#define SPIDER_SAMESIDELEFT_SPRITESHEET_PLIST_FILE "spider/spritesheet/sameSideLeft.plist"
#define SPIDER_SAMESIDERIGHT_SPRITESHEET_PNG_FILE "spider/spritesheet/sameSideRight.png"
#define SPIDER_SAMESIDERIGHT_SPRITESHEET_PLIST_FILE "spider/spritesheet/sameSideRight.plist"
#define SPIDER_KNIT_SPRITESHEET_PNG_FILE "spider/spritesheet/knit.png"
#define SPIDER_KNIT_SPRITESHEET_PLIST_FILE "spider/spritesheet/knit.plist"
#define SPIDER_CLIMB_SPRITESHEET_PNG_FILE "spider/spritesheet/climb.png"
#define SPIDER_CLIMB_SPRITESHEET_PLIST_FILE "spider/spritesheet/climb.plist"
#define LOCATION_Y_BODY_HEIGHT 2.5 //bridgeY noktasından, spider yüksekliğinin yarısı kadar daha yukarı çıkmalı, ama /2 yerine /2.5
#define WALK_DURATION 2.0
#define WALK_FRAME_COUNT 13
#define GO_BACK_DURATION 0.5
#define JUMP_DURATION 0.7
#define JUMP_SHRINK_RATE 10
#define JUMP_FLY_FRAME_COUNT 13
#define JUMP_SHRINK_FRAME_COUNT 4
#define SAME_SIDE_DURATION 2.0
#define SAME_SIDE_FRAME_COUNT 13
#define INNER_KNIT_DURATION 1.0           //web RISE_DURATION ile aynı olmalı
#define INNER_KNIT_FRAME_COUNT 14
#define INNER_KNIT_ANIMATION_LOOPS_COUNT 5
#define CLIMB_DURATION 0.7
#define CLIMB_FRAME_COUNT 12
#define REMOVE_WEB_FROM_LAYER_UPDATE_INTERVAL 2.5
#define WEB_ZORDER 0
#define WEBSTRING_ZORDER 0

class GamePlayLayer;

class Spider : public cocos2d::Sprite
{
public:
    enum Side{left=0,right};
    enum Status{waiting=0,triggerWalk,walking,triggerSameSide,sameSiding,triggerKnit,jumping,innerKnitting,interruptKnit,climbing,goingBack};

     Spider();
    ~Spider();
    static Spider* createSpider(GamePlayLayer* parentLayer,int webCount,int bonusWebCount);
    bool initSpider(GamePlayLayer* parentLayer,int webCount,int bonusWebCount);

    void knit(cocos2d::Vec2& knitPos);//ağ yapılacak yere jump, innerKnit, climb, goBack
    void knitInterrupt();//elle bitir

    void walk();
    void sameSide();

    void addWebStringToLayer();
    void moveWebFromLiveWebsToDeadWebs(Web*const web);
    cocos2d::Vector<Web*>* getLiveWebs();

    int getWebCount();
    int getBonusWebCount();
    void decreaseWebCount();//ağ örmesi güçlü sineklere yakalanmadan tamamlandıysa ağ sayısını bir azalt, bu fonksiyon Web sınıfı içinden çağırılacak
    void increaseWebCountByBonus();//bonus kazanılırsa _webCount değerini arttır _bonusWebCount kadar...

    WebString* getWebString();
    GamePlayLayer* getParentLayer();

    Status getStatus();
    Side getCurrentSide();

private:
    GamePlayLayer* _parentLayer;//silinmeyecek, spider in içinde bulunduğu Layer ın referansı
    void update(float dt);//sadece Spider actionlarını kontrol etmekte kullanılacak

    cocos2d::SpriteFrame*_spriteFrameLeft;
    cocos2d::SpriteFrame*_spriteFrameRight;
    cocos2d::SpriteFrame*_spriteFrameCenter;
    void _initSprite(std::string pngFile,std::string plistFile);

    Status _status;
    Side _currentSide;
    float _webActiveR;//ağın ölçüleri
    cocos2d::Vec2* _attackLocationLeft;//new delete     //örümceğin beklediği sol nokta
    cocos2d::Vec2* _attackLocationRight;//new delete    //örümceğin beklediği sağ nokta
    cocos2d::Vec2* _knitPos; //new delete //ağ yapacağı nokta, her defasında x,y değerleri değişecek
    float _attackLocationY;//örümceğin beklediği yerin Y koordinatı
    float _boundaryTop;     //ağ yapabileceği üst sınır
    float _boundaryBottom;  //ağ yapabileceği alt sınır
    float _boundaryLeft;       //ağ yapabileceği sol sınır
    float _boundaryRight;   //ağ yapabileceği sağ sınır

    void _adaptKnitPos(cocos2d::Vec2& knitPos);

    WebString* _webString;
    cocos2d::Vector<Web*>* _liveWebs;   //new delete
    cocos2d::Vector<Web*>* _deadWebs;   //new delete
    void _removeWebFromLayerUpdate(float dt); //seyrek aralıklarla çalışacak. Status==dead olmuş web neslerini sahneden ve _catchedBugs içinden sil  this->schedule(schedule_selector(Spider::_removeWebFromLayerUpdate),5.0f);
    int _webCount;
    int _bonusWebCount;

    float _walkDuration;
    int _walkFrameCount;
    cocos2d::Spawn* _walkLSpawn;//retain-release
    cocos2d::Spawn* _walkRSpawn;//retain-release
    cocos2d::Spawn* _walkSpawn;//YOK retain-release _currentSide, durumuna göre _walkLSpawn veya _walkRSpawn'dan birini işaret edecek
    void _initWalkActions(std::string pngFile,std::string plistFile);
    void _finishedWalkSettings();

    float _sameSideDuration;
    int _sameSideFrameCount;
    cocos2d::Animate* _sameSideAnimateL; //retain-release
    cocos2d::Animate* _sameSideAnimateR; //retain-release
    cocos2d::Animate* _sameSideAnimate;//YOK retain-release _currentSide, durumuna göre _sameSideAnimateL veya _sameSideAnimateR'dan birini işaret edecek
    void _initSameSideActions(std::string pngFileL,std::string plistFileL,std::string pngFileR,std::string plistFileR);

    float _knitDistanceL;//ağ örülen noktanın sol noktaya mesafesi _knitLSequence ve _knitInterruptLSequence tespiti için
    float _knitDistanceR;//ağ örülen noktanın sağ noktaya mesafesi _knitRSequence ve _knitInterruptRSequence tespiti için

    float _jumpDuration;
    float _jumpDurationPerFrame;
    int _jumpFlyFrameCount;
    int _jumpShrinkFrameCount;
    float _jumpShrinkRate;
    cocos2d::ccBezierConfig _jumpBezierConf;
    cocos2d::BezierTo* _jumpBezier;     //retain-release
    cocos2d::Sequence* _jumpSequence;   //retain-release
    void _initJumpAction(std::string pngFile,std::string plistFile);
    void _preStartJumpSettings();
    void _finishedJumpSettings();

    float _innerKnitDuration;//tek bir döngü değil bu süre, knit animasyonu birden fazla kere tekrar edecek muhtemelen
    float _innerKnitAnimationLoopsCount;
    int _innerKnitFrameCount;
    cocos2d::Animate* _innerKnitAnimate;//retain-release
    void _initInnerKnitAction(std::string pngFile,std::string plistFile);
    void _preStartInnerKnitSettings();

    float _climbDuration;
    int _climbFrameCount;
    cocos2d::MoveBy* _climbMoveBy;      //retain-release
    cocos2d::Spawn* _climbSpawn;//retain-release
    void _initClimbAction(std::string pngFile,std::string plistFile);
    void _preStartClimbSettings();
    void _finishedClimbSettings();

    //ağı örüp yukarıya çıktıktan sonra olması gereken yere dönmesi için
    int _goBackFrameCount;
    float _goBackDuration;
    cocos2d::Spawn* _goBackSpawnL;//retain-release
    cocos2d::Spawn* _goBackSpawnR;//retain-release
    cocos2d::Spawn* _goBackSpawn;//YOK retain-release, _knitDistanceL,_knitDistanceR durumuna göre _goBackInterruptSpawnR veya _goBackInterruptSpawnL den birisini gösterecek
    void _initGoBackActions(std::string pngFile,std::string plistFile);
};

#endif
