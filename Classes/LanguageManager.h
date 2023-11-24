#ifndef __LANGUAGE_MANAGER_H__
#define __LANGUAGE_MANAGER_H__
#include<string>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

//getInstance() ile çağırılacak ve AppDelegate yıkıcı fonksiyonunda deleteInstance() ile yok edilecek
class LanguageManager{
    private:
        rapidjson::Document* _document;
        static LanguageManager* _instance;
        LanguageManager();
        ~LanguageManager();
    public:
        std::string getStringForKey(std::string key);
        static LanguageManager* getInstance();
        static void deleteInstance();

};

#endif
