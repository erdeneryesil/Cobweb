#ifndef __GAMEINFOLAYER_H__
#define __GAMEINFOLAYER_H__

#include "cocos2d.h"
#include "Boundries.h"

#define GAME_INFO_FONT_FILE "fonts/infoDigit.fnt"
#define GAME_INFO_FONT_FILE_BIG  "fonts/menuFont.fnt"
#define FLY_ICON_PNG_FILE "info/fly.png"
#define WEB_ICON_PNG_FILE "info/web.png"
#define PAUSE_ICON_PNG_FILE "info/pause.png"
#define RESUME_ICON_PNG_FILE "info/resume.png"

#define SCORE_ANIMATION_INTERVAL 0.01

//new-delete ile kullanılacak
class GameInfoLayer:public cocos2d::Layer{
public:
    GameInfoLayer();
    ~GameInfoLayer();
    void flyLabelSetNumber(int number);
    void webLabelSetNumber(int number);
    void levelLabelSetNumber(int number);
    void scoreLabelSetNumber(int number);
    void setScoreLabelForWin(int score,int levelScore,int factor);
    void setScoreLabelForGameOver(int score,int levelScore);

    bool tapPause(cocos2d::Vec2& tapPos);
    bool tapResume(cocos2d::Vec2& tapPos);
    void showPauseIcon();
    void hidePauseIcon();
    void showResumeIcon();
    void hideResumeIcon();

private:
    cocos2d::Sprite* _flyIcon;//retain-release
    cocos2d::Label* _flyLabel;//retain-release
    cocos2d::Sprite* _webIcon;//retain-release
    cocos2d::Label* _webLabel;//retain-release

    cocos2d::Label* _levelLabel;//retain-release
    cocos2d::Label* _scoreLabel;//retain-release
    cocos2d::Label* _scoreFactorLabel;//retain-release

    cocos2d::Sprite* _pauseIcon;//retain-release
    cocos2d::Sprite* _resumeIcon;//retain-release

    //Score animasyonu için kullanılacak değişkenler
    int _animScore;
    int _animLevelScore;
    int _animFactor;
    int _animIndex;
    void _scoreAnimationWin(float dt);
    void _scoreAnimationGameOver(float dt);
};
#endif
