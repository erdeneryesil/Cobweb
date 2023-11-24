#ifndef __GAMEMENU_SCENE_H__
#define __GAMEMENU_SCENE_H__

#include "cocos2d.h"

#define GAME_MENU_FONT_FILE "fonts/menuFont.fnt"
#define GAME_MENU_BACKGROUND_PNG_FILE "backgrounds/menuScene.png"
#define HIGSCORE_LABEL_SCALE 0.75 //highScore etiketi biraz daha küçük olsun

class GameMenuScene : public cocos2d::Scene
{
private:
    enum MenuItem{playAgain=0,playNew};//eğer menü seçeneklerine ekleme yapılırsa buraya da ekle
    cocos2d::Sprite* _background;//retian-release
    void menuItemClick(cocos2d::Ref* sender);
    int _currentLevel;//veritabanından alınacak
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameMenuScene);
    ~GameMenuScene();
};

#endif // __GAMEMENU_SCENE_H__
