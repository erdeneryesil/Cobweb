#include "GameSureScene.h"
#include "GameLoadScene.h"
#include"GameMenuScene.h"
#include "Database.h"
#include "LanguageManager.h"
#include "Boundries.h"

USING_NS_CC;


Scene* GameSureScene::createScene()
{
    return GameSureScene::create();
}
GameSureScene::~GameSureScene(){
    this->_background->release();
}
bool GameSureScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    this->_background=Sprite::create(GAME_SURE_BACKGROUND_PNG_FILE);
    this->_background->retain();
    this->_background->setPosition(visibleSize/2);
    this->addChild(this->_background,-1);

    Database::CurrentLevelBasic* currentLevelBasic=Database::getInstance()->getCurrentLevelBasic();//bu fonksiyon içinde delete
    int playCount=currentLevelBasic->playCount;
    this->_currentLevel=currentLevelBasic->levelId;//veritabanından alınacak
    std::stringstream ss;
    ss<<this->_currentLevel;
    std::string currentLevelStr=ss.str();

    float rateWidtHeight=0;
    float theoricRateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();
    float textScale;
    if(Boundries::getPointRight()<Boundries::getTheoricWidth())
        rateWidtHeight=Boundries::getPointRight()/Boundries::getPointTop();
    else
        rateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();

    textScale=rateWidtHeight/theoricRateWidtHeight;

    Label* labelTitle=Label::createWithBMFont(GAME_SURE_FONT_FILE,LanguageManager::getInstance()->getStringForKey("labelSure"));
    labelTitle->setContentSize(labelTitle->getContentSize()*textScale);
    labelTitle->setScale(textScale);
    Label* labelYes=Label::createWithBMFont(GAME_SURE_FONT_FILE,(std::string(LanguageManager::getInstance()->getStringForKey("labelYes"))));
    labelYes->setContentSize(labelYes->getContentSize()*textScale);
    labelYes->setScale(textScale);
    Label* labelNo=Label::createWithBMFont(GAME_SURE_FONT_FILE,LanguageManager::getInstance()->getStringForKey("labelNo"));
    labelNo->setContentSize(labelNo->getContentSize()*textScale);
    labelNo->setScale(textScale);

    delete currentLevelBasic;

    MenuItemLabel* itemTitle=MenuItemLabel::create(labelTitle);
    itemTitle->setEnabled(false);

    MenuItemLabel* itemYes=MenuItemLabel::create(labelYes,CC_CALLBACK_1(GameSureScene::menuItemClick,this));
    itemYes->setTag(yes);
    MenuItemLabel* itemNo=MenuItemLabel::create(labelNo,CC_CALLBACK_1(GameSureScene::menuItemClick,this));
    itemNo->setTag(no);

    Menu* menu=Menu::create(itemTitle,itemYes,itemNo,nullptr);
    menu->alignItemsVertically();
    menu->setPosition(visibleSize/2);
    this->addChild(menu);

    return true;
}

void GameSureScene::menuItemClick(Ref* sender){
    int tag=((Label*)sender)->getTag();

    if(tag==yes){
        Database::getInstance()->updateAllInCurrentLevel(1,1,0);
        Director::getInstance()->replaceScene(GameLoadScene::createScene());//burada setCurrentLevel gerek yok çünkü GameLoadScene de varsayılan olarak _currentLevel=1 oluyor
    }else if(tag==no){
        Director::getInstance()->replaceScene(GameMenuScene::createScene());
    }

}
