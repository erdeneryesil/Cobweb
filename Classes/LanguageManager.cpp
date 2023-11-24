#include "LanguageManager.h"

USING_NS_CC;
using std::string;
using namespace rapidjson;

LanguageManager* LanguageManager::_instance=0;
LanguageManager::LanguageManager(){
    string fileName;
    this->_document=new Document();

    switch(Application::getInstance()->getCurrentLanguage()){
    case LanguageType::ENGLISH:
        fileName="languages/en.json";
        break;
    case LanguageType::TURKISH:
        fileName="languages/tr.json";
        break;
    case LanguageType::FRENCH:
        fileName="languages/fr.json";
        break;
    case LanguageType::GERMAN:
        fileName="languages/de.json";
        break;
    case LanguageType::SPANISH:
        fileName="languages/sp.json";
        break;
    case LanguageType::PORTUGUESE:
        fileName="languages/pr.json";
        break;
    default:
        fileName="languages/en.json";
        break;
    }

    string clearContent = FileUtils::getInstance()->getStringFromFile(fileName);

    this->_document->Parse<0>(clearContent.c_str());
    if(this->_document->HasParseError())
    {
        return;
    }
}
LanguageManager::~LanguageManager(){
    delete this->_document;
}
LanguageManager* LanguageManager::getInstance(){
    if(!_instance)_instance=new LanguageManager();
    return _instance;
}
void LanguageManager::deleteInstance(){
    if(!_instance)delete _instance;
}
string LanguageManager::getStringForKey(string key){
    return (*this->_document)[key.c_str()].GetString();
}
