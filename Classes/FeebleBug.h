#ifndef __FEEBLEBUG_H__
#define __FEEBLEBUG_H__

#include "Bug.h"

#define PI 3.14159265
#define MOTH_PNG_FILE "bugs/spritesheet/moth.png"
#define MOTH_PLIST_FILE "bugs/spritesheet/moth.plist"
#define LADYBIRD_PNG_FILE "bugs/spritesheet/ladybird.png"
#define LADYBIRD_PLIST_FILE "bugs/spritesheet/ladybird.plist"
#define SIN_PERIOD_FACTOR_CONSTANT 0.7
#define SIN_AMPLITUDE_FACTOR_CONSTANT 2.2
#define MIN_SCREEN_HEIGHT 360 //olabilecek en küçük ekran yüksekliği (640x360)
#define SIN_AMPLITUDE_VAR_ALTERATION_MAX_CONSTANT 100
#define SIN_AMPLITUDE_VAR_ALTERATION_MIN_CONSTANT 4
#define SIN_AMPLITUDE_VAR_ALTERATION_CONSTANT 10000

class FeebleBug : public Bug
{
public:
    FeebleBug();
    ~FeebleBug();
    static FeebleBug* createFeebleBug(BugType bugType,float velocity);
    virtual bool initFeebleBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity);
     int getPoint()override=0;

protected:
    float _sinAmplitudeVar;//yapılacak ayarlarda bu kullanılacak
    float _sinAmplitudeVarAlteration;//_sinAmplitudeMultiplier değişkeninin kaçar kaçar arttırılıp, azaltılacağı
    float _sinAmplitudeVarAlterationMin;
    float _sinAmplitudeVarAlterationMax;
    float _sinPeriodFactor;//bütün ekran çözünürlüklerinde aynı peryoda sahip olması için kullanılacak katsayı
    float _sinAmplitudeFactor;//bütün ekran çözünürlüklerinde aynı genliğe sahip olması için kullanılacak katsayı
    float _getSinAmplitude();//sin(x) genliği, en küçük en büyük Y değeri
    void _initVectorRandom()override=0;
    void _flyUpdate()override=0;
    void _arrangeSinAmplitudeFactorRandom(BugType bugType);//max genliği hesapladıktan sonra, maxgenlik--0.5*maxgenlik arasında rasgele genlik üretiliyor. Hem moth için hem de ladybird için aynı yöntemle genlik oluşturulabilir, öyle de yapılıyor
    float _updateSinAmplitudeFactor();
};
#endif
