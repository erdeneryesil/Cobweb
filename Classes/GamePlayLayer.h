#ifndef __GAMEPLAYLAYER_H__
#define __GAMEPLAYLAYER_H__

#include "cocos2d.h"
#include "Spider.h"
#include "GameInfoLayer.h"
#include "CurrentLevel.h"

#define BUG_TYPE_COUNT 8
#define SPARSE_OPERATIONS_UPDATE_INTERVAL 9.5
#define BUG_TAKE_TO_STAGE_UPDATE_INTERVAL 1.5//sinekler kaç saniye aralıkla sahneye çıksın
#define FACTOR_WEAKEST_BUG_VELOCITY 1.2 //mosquito ve crane çok yavaş olduğu için onları bu çarpan ile arttırıyoruz
#define BUG_VELOCITY_DIVIDING 320// 180.0//_bugVelocitiy değerinmi belirlerken, ekran genişliğinin kaça bölüneceğini belirler
#define BUG_VELOCITY_MIN 2.0 //_bugVelocity en az 2 olması lazım çok önemli
#define MAX_BUG_COUNT_ON_SAME_LINE 3//aynı hizada bulunabilecek max dragonfly veya gadfly sayısı
#define SAME_LINE_BUFFER_AND_WEB_R_RATION 1.0 //_sameLİneBuffer ile web yarıçap oranı
#define GAME_INFO_LAYER_ZORDER 3
#define SPIDER_ZORDER 2
#define BUG_ZORDER 2
#define ODDS_BONUS_BUG_MIGHTY_TOP 100 //bonusBugDisableMighty olasılığı üst sınır mesela olasılık 1/100 ise bu değişken 100
#define ODDS_BONUS_BUG_WEB_TOP 100//bonusBugIncreaseWeb olasılığı üst sınır mesela olasılık 1/100 ise bu değişken 100
#define SCENE_TRANSITION_TIME 1.0 //oyun kazanılırsa ya da kaybedilirse; GameOver ya da sonraki level yükelme sahnesine ne kadar süre sonra geçiş yapılsın
#define SCORE_FACTOR_ONE_BUG 3 //böceklerin teker kteker yakalandığı seviyelerde skorun kaç ile çarpılacağı

//new-delete ile kullanılacak
class GamePlayLayer:public cocos2d::Layer{
public:
    GamePlayLayer();
    ~GamePlayLayer();

    void update(float dt);
    void setWebCountLabel();//web sayısı azaldığında çalıştırılacak, Web sınıfı içinden de çağırılacak, bu sınıf içinden de...
    void setBugCountLabel();
    void setScoreLabel();
    void increaseBugCount();//sadece BonusBug içerisinden çağırılacak, BonusBug yakalandığında _bugCount 1 arttırılacak
    void disableMightyByBonus();//sadece BonusBug içerisinden çağırılacak, bonus kazanıldıysa ekrandaki (sadece _flyingBugs) güçlü sineklerin gücünü pasif eder

    bool isGameOver();//Web içerisinden çağırılacak, Son Web yok olduğunda hala sinek varsa true döndürecek
    void scoreAnimationGameOver();//Web içerisinden çağırılacak. Eğer gameOver olmuşsa yapılacak işlemler
    void gameOverOperations();//GameInfoLayer içinden çağırılacak, Score animasyonu bittikten sonra

    bool isWin();
    void scoreAnimationWin();
    void winOperations();//GameInfoLayer içinden çağırılacak, Score animasyonu bittikten sonra
private:
    GameInfoLayer* _gameInfoLayer;//new-delete
    Spider* _spider;//retrain-release

    int _levelId;//kaçıncı seviye
    int _playCount;//bu seviye kaçıncı kez oynanıyor
    int _score;//kayıtlı olan skor
    int _highScore;//kayıtlı olan rekor skor
    int _scoreLevel;//bu seviyede kazanılan puan
    int _oddsBonusMightyTop;//bonusBugDisableMighty olasılığı üst sınır mesela olasılık 1/100 ise bu değişken 100
    int _oddsBonusMighty;//bonusBugDisableMighty olasılığı üst sınır mesela olasılık 1/100 ise bu değişken 1
    int _oddsBonusWebTop;//bonusBugIncreaseWeb olasılığı üst sınır mesela olasılık 1/100 ise bu değişken 100
    int _oddsBonusWeb;//bonusBugIncreaseWeb olasılığı üst sınır mesela olasılık 1/100 ise bu değişken 1
    bool _bonusMighty;//Bonus Mighty varmı? ona göre bonusbug gönderilecek veya gönderilmeyecek
    bool _bonusWeb;//Bonus Web varmı? ona göre bonusbug gönderilecek veya gönderilmeyecek
    Bug* _bonusBugMighty;//retain-release, diğer Bug lar retain-release değil çünkü onlar create edildiği anda Vector içine konuluyor
    Bug* _bonusBugWeb;//retain-release, diğer Bug lar retain-release değil çünkü onlar create edildiği anda Vector içine konuluyor
    bool _bonusBugMightyTakeToStage;//sahneye bir kere çıkacağı için, çıkıp çıkmadığı bu değişken ile kontrol edilecek
    bool _bonusBugWebTakeToStage;//sahneye bir kere çıkacağı için, çıkıp çıkmadığı bu değişken ile kontrol edilecek
    int _maximumBugCountOnStage;//aynı anda sahnede bulunabilecek bug sayısı
    void _initBonusBugs();
    void _bonusBugOperations();

    float _bugVelocity;//Bug nesnelerinin hızı, en az 2 olmalı..
    int _bugCount;//BonusBug haricindeki tüm Bug nesnelerinin sayısı
    int _bugTypeCount;
    int _mosquitoCount;
    int _craneCount;
    int _mothCount;
    int _ladybirdCount;
    int _dragonflyCount;
    int _bumblebeeCount;
    int _gadflyCount;
    int _robberflyCount;
    cocos2d::Vector<Bug*>*_repositoryBugs;//new delete
    void _initRepositoryBugs();

    cocos2d::Vector<Bug*>* _flyingBugs; //new delete
    cocos2d::Vector<Bug*>* _catchedBugs; //new delete
    std::vector<float>* _sameLineBugsY;//new-delete
    float _webActiveR;
    float _webOriginalR;

    bool _isBugOnWeb(const Bug*const bug, Web*const web,float webR);
    void _sparseOperationsUpdate(float dt);//seyrek aralıklarla çalışacak.   this->schedule(schedule_selector(GamePlayLayer::_sparseOperationsUpdate),5.0f);
    void _removeDeadBugs(); //Status==dead olmuş bug neslerini sahneden ve _catchedBugs içinden sil
    void _refreshSpiderCurrentSide();//spider Side değerini değiştir walk veya sameSide ile
    Spider::Side _calculateBugsHorizontalWeight();//uçan böceklerin çoğunun ekranın sağında mı yoksa solunda mı olduğunu hesaplar

    int _maxBugCountOnSameLine;
    bool _areBugsOnSameLine();//yakın y hizasında, belli sayıda dragonfly ve gadfly varsa ağ yapılamaz, bunu tespit eder bu fonkdiyon
    float _sameLineBuffer; //yakın y hizasında, belli sayıda dragonfly ve gadfly ın max uzaklığı

    bool _rateWebBug();//kalan web sayısı ile böcek sayısının oranını hesaplar
    float _minRateWebBug;

    bool touchBegan(cocos2d::Touch*touch,cocos2d::Event*event);

    bool _pause;
    void _pauseGame();
    void _resumeGame();

    void _bugTakeToStageUpdate(float dt);

    void _transitionGameOverScene(float dt);
    void _transitionCongratulationsScene(float dt);
    void _transitionGameLoadScene(float dt);
};
#endif
