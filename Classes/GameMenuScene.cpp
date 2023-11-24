#include "GameMenuScene.h"
#include "GameLoadScene.h"
#include "GameSureScene.h"
#include "LanguageManager.h"
#include "Database.h"
#include "Boundries.h"

USING_NS_CC;

Scene* GameMenuScene::createScene()
{
    return GameMenuScene::create();
}
GameMenuScene::~GameMenuScene(){
    this->_background->release();
}
bool GameMenuScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    this->_background=Sprite::create(GAME_MENU_BACKGROUND_PNG_FILE);
    this->_background->retain();
    this->_background->setPosition(visibleSize/2);
    this->addChild(this->_background,-1);

    Database::CurrentLevelBasic* currentLevelBasic=Database::getInstance()->getCurrentLevelBasic();//bu fonksiyon içinde delete
    int playCount=currentLevelBasic->playCount;
    int highScore=currentLevelBasic->highScore;
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

    Label* labelPlayAgain=Label::create();
    labelPlayAgain->setBMFontFilePath(GAME_MENU_FONT_FILE);
    labelPlayAgain->setString((std::string(LanguageManager::getInstance()->getStringForKey("labelPlayAgain"))).append(currentLevelStr));
    labelPlayAgain->setContentSize(labelPlayAgain->getContentSize()*textScale);
    labelPlayAgain->setScale(textScale);

    Label* labelNewGame=Label::create();
    labelNewGame->setBMFontFilePath(GAME_MENU_FONT_FILE);
    labelNewGame->setString(LanguageManager::getInstance()->getStringForKey("labelNewGame"));
    labelNewGame->setContentSize(labelNewGame->getContentSize()*textScale);
    labelNewGame->setScale(textScale);

    if(highScore>0){
        Label* labelHighScore=Label::create();
        labelHighScore->setBMFontFilePath(GAME_MENU_FONT_FILE);
        std::stringstream highScoreStr;
        highScoreStr<<LanguageManager::getInstance()->getStringForKey("labelHighScore")<<" ";
        highScoreStr<<highScore;
        labelHighScore->setString(highScoreStr.str());
        labelHighScore->setContentSize(labelHighScore->getContentSize()*textScale);
        labelHighScore->setScale(textScale*HIGSCORE_LABEL_SCALE);
        labelHighScore->setPosition(Vec2(Boundries::getPointLeft()+labelHighScore->getBoundingBox().size.width/2,Boundries::getPointBottom()+labelHighScore->getBoundingBox().size.height/2));
        this->addChild(labelHighScore);
    }

    delete currentLevelBasic;

    MenuItemLabel* itemPlayAgain=MenuItemLabel::create(labelPlayAgain,CC_CALLBACK_1(GameMenuScene::menuItemClick,this));
    if(this->_currentLevel==1 && playCount==1){
        itemPlayAgain->setEnabled(false);
        itemPlayAgain->setOpacity(100);
    }

    itemPlayAgain->setTag(playAgain);
    MenuItemLabel* itemNewGame=MenuItemLabel::create(labelNewGame,CC_CALLBACK_1(GameMenuScene::menuItemClick,this));
    itemNewGame->setTag(playNew);

    Menu* menu=Menu::create(itemPlayAgain,itemNewGame,nullptr);
    menu->alignItemsVertically();
    menu->setPosition(visibleSize/2);
    this->addChild(menu);

    return true;
}
void GameMenuScene::menuItemClick(Ref* sender){
    int tag=((Label*)sender)->getTag();

    if(tag==playAgain){
        Scene* gameLoadScene=GameLoadScene::createScene();
        ((GameLoadScene*)gameLoadScene)->setCurrentLevel(this->_currentLevel);
        Director::getInstance()->replaceScene(gameLoadScene);
    }else if(tag==playNew){
        if(this->_currentLevel==1){
            Database::getInstance()->updateAllInCurrentLevel(1,1,0);
            Director::getInstance()->replaceScene(GameLoadScene::createScene());//burada setCurrentLevel gerek yok çünkü GameLoadScene de varsayılan olarak _currentLevel=1 oluyor
        }else{
            Director::getInstance()->replaceScene(GameSureScene::createScene());//kayıtlı oyunu silmek istediğine emin misin? diye sor
        }
    }
}
