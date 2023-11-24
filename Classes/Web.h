#ifndef __WEB_H__
#define __WEB_H__

#include "cocos2d.h"
#include "Bug.h"

#define RISE_DURATION 1.0f          //Spider KNIT_DURATION ile aynı olmalı
#define ACTIVE_INTERVAL 0.03f
#define ACTIVE_SCALE_DEC_RATE 0.96f
#define ACTIVE_SCALE_INC_RATE 1.0f
#define ACTIVE_ROTATE_ANGLE 0.5f
#define DEATH_DURATION 2.0f
#define ACTIVE_SCALE_RATE 0.8f   //büyüme efekti de olduğu için png dosyası orijinal boyuttan büyük, orijinal boyuta ulaşmak için 0.8 scale et
#define MIN_SCALE_RATE 0.1f      //en küçük olabileceği ebat oranı
#define MAX_SCALE_RATE 1.0f     //en büyük olabileceği ebat oranı
#define DEATH_SUDDEN_DURATION_RATE 0.5f  //hızlı death süresinin, normal death süresine oranı
#define PRERISE_DURATION_RATE 0.5f   //prerise süresi _riseDuration % kaçı 0-1 arası, rotate ile uyumlu olmalı ikisinin toplamı 1.0
#define ROTATE_DURATIONRATE 0.5f     //rise içindeki rotate süresi _riseDuration % kaçı 0-1 arası, prerise ile uyumlu olmalı ikisinin toplamı 1.0
#define RISE_ROTATE_ANGLE 90.0f
#define DEATH_ROTATE_ANGLE -720.0f
#define GROW_DURATION_RATE 0.2f      //ağ death olurken başlangıçta büyüyeceği süre yani _deathDuration % kaçı 0-1 arası, shrink ile uyumlu olmalı ikisinin toplamı 1.0
#define SHRINK_DURATION_RATE 0.8f    //ağ death olurken başlangıçta küçüleceği süre yani _deathDuration % kaçı 0-1 arası, grow ile uyumlu olmalı ikisinin toplamı 1.0
#define ACTIVE_DURATION 3.5f //ağın aktif kalacağı süre
#define INNER_BOUNDARY_RATE 0.22f // yakalanan Bug'ların ağın dışındaymış gibi görünmemesi için _innerBoundaryTop,_innerBoundaryBottom, _innerBoundaryLeft, _innerBoundaryRight hesaplamaları için
#define WEB_PNG_FILE "web/web.png"

class Spider;

class Web : public cocos2d::Sprite
{
public:
    static float getActiveR();
    static float getOriginalR();
    static void setOwnerSpider(Spider* spider);
    static Spider* getOwnerSpider();
    static void setOneBug(bool oneBug);
    enum Status{idle=0,triggerRise,rising,risingInterrupt,active,dying,dyingSudden,dead};

    Web();
    ~Web();
    static Web* createWeb(const cocos2d::Vec2*const location);
    bool initWeb(const cocos2d::Vec2*const location);
    void rise();
    void riseInterrupt();//kendisi normal süresinde bitecek zaten ama erken bitirmek istersen kullanılabilir
    void bugPushBack(Bug* bug);
    void bugPutIn(Bug* bug); //Bug'ı ağın içinde göstermek için, konumlandırır, Bug içinden çağırılır

    Status getStatus();

private:
    static Spider* _ownerSpider;//bütün Web nesnelerinin sahibi tek bir Spider, o yüzden static
    static bool _oneBug;//sadece tek bir sinek yakalayabilir

    float _activeScaleRate;//ağın aktif iken sahip olduğu abatın oranı
    float _minScaleRate;//alabileceği en küçük boyut oranı, küçülürken en az ne kadar olacaksa
    float _maxScaleRate;//alabileceği en küçük boyut oranı, 1.0 olacak

    float _innerBoundaryTop;//yakalanan Bug'ın, ağ içerisinde, dış kısma en yakın olabileceği üst nokta (Bug'ın ağ dışında görünmemesi için)
    float _innerBoundaryBottom;//yakalanan Bug'ın, ağ içerisinde, dış kısma en yakın olabileceği alt nokta (Bug'ın ağ dışında görünmemesi için)
    float _innerBoundaryLeft;//yakalanan Bug'ın, ağ içerisinde, dış kısma en yakın olabileceği sol nokta (Bug'ın ağ dışında görünmemesi için)
    float _innerBoundaryRight;//yakalanan Bug'ın, ağ içerisinde, dış kısma en yakın olabileceği sağ nokta (Bug'ın ağ dışında görünmemesi için)

    void update(float dt);//sadece Web actionlarını kontrol etmekte kullanılacak

    Status _status;
    cocos2d::Vec2* _location;

    float _activeDuration;
    float _activeTimer;
    float _activeTimerDefault;
    cocos2d::Repeat* _activeRepeat;   //retain-release
    void _initActiveAction();
    void _preStartActiveSettings();


    float _riseDuration;////tek bir döngü bu süre
    cocos2d::Sequence* _riseSequence;//retain-release
    void _initRiseAction();
    void _preStartRiseSettings();
    void _finishedRiseSettings();

    float _deathDuration;
    cocos2d::Sequence* _deathSequence;//retain-release
    void _initDeathAction();
    void _finishedDeathSettings();
    void _preStartDeathSettings();

    float _deathSuddenDuration;
    cocos2d::Sequence* _deathSuddenSequence;//retain-release
    void _initDeathSuddenAction();
    void _finishedDeathSuddenSettings();

    cocos2d::Vector<Bug*>* _bugs;//new-delete, içindeki bug nesneler silinmeyecek burada
};

#endif
