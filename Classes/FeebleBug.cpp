#include "Boundries.h"
#include "Web.h"
#include "FeebleBug.h"
#include "Moth.h"
#include "Ladybird.h"

USING_NS_CC;

FeebleBug::FeebleBug(){}
FeebleBug::~FeebleBug(){}
FeebleBug* FeebleBug::createFeebleBug(BugType bugType,float velocity){
    FeebleBug* feebleBug;
    std::string pngFile;
    std::string plistFile;
    if(bugType==moth){
        feebleBug=new Moth();
        pngFile=MOTH_PNG_FILE;
        plistFile=MOTH_PLIST_FILE;
    }else if(bugType==ladybird){
        feebleBug=new Ladybird();
        pngFile=LADYBIRD_PNG_FILE;
        plistFile=LADYBIRD_PLIST_FILE;
    }

    if(feebleBug&& feebleBug->initFeebleBug(bugType,pngFile,plistFile,velocity)){
        feebleBug->autorelease();
        return feebleBug;
    }
    CC_SAFE_DELETE(feebleBug);

    return NULL;
}
bool FeebleBug::initFeebleBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity){
    if(!this->initBug(pngFile,plistFile,velocity))return false;

    float rateWidtHeight=0;
    if(Boundries::getPointRight()<Boundries::getTheoricWidth())
        rateWidtHeight=Boundries::getPointRight()/Boundries::getPointTop();
    else
        rateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();

    float sinPeriodFactor=rateWidtHeight*SIN_PERIOD_FACTOR_CONSTANT;
    float sinAmplitudeFactor=rateWidtHeight*SIN_AMPLITUDE_FACTOR_CONSTANT*1/rateWidtHeight;

    this->_sinAmplitudeVarAlterationMax=Boundries::getPointTop()/SIN_AMPLITUDE_VAR_ALTERATION_MAX_CONSTANT;//_sinAmplitudeVar değeri bu değerin üzerine çıkmıyor
    this->_sinAmplitudeVarAlterationMin=this->_sinAmplitudeVarAlterationMax/SIN_AMPLITUDE_VAR_ALTERATION_MIN_CONSTANT;//_sinAmplitudeVar değeri bu değerin altına inmiyor
    this->_sinAmplitudeVar=this->_sinAmplitudeVarAlterationMax;//bu negatif iken alttaki değişkenini + olması lazım
    this->_sinAmplitudeVarAlteration=-(Boundries::getPointTop()/SIN_AMPLITUDE_VAR_ALTERATION_CONSTANT);
    this->_sinPeriodFactor=Web::getActiveR()*sinPeriodFactor;
    this->_sinAmplitudeFactor=(sinAmplitudeFactor*(MIN_SCREEN_HEIGHT/Boundries::getPointTop()));//2.3 sayısı ekran yükseklik değerlerine göre deneyerek bulundu

    this->_bugType=bugType;
    this->_mighty=false;
    this->_initVectorRandom();//this->_vector...
    this->_initFlip();//hareket edeceği yöne göre döndür

    this->_arrangeSinAmplitudeFactorRandom(bugType);
    this->_initEntrance();

    return true;
}
float FeebleBug::_getSinAmplitude(){
    float amp=0.0f;
    float vectorX=std::abs(this->_vector->x);//abs çünkü _vector->x negatif olabilir,

    for(float i=0;i<=180;i+=vectorX){ //180 derece çünkü sin(x)in alabileceği en büyük değerler o aralıkta, sürekli eklendiği için 90-180 arası da hesaplanmalı
        amp+=this->_sinAmplitudeVar*vectorX*sin(i*PI/180.0f);
    }
    return amp;
}
void FeebleBug::_arrangeSinAmplitudeFactorRandom(BugType bugType){
    float amp;
    float initialYMin;
    float initialYMax;
    float boundarySinBottomMax;
    float boundarySinTopMax;
    float boundarySinBottomMin;
    float boundarySinTopMin;

    bool err=true;
    while(err){
        amp=this->_getSinAmplitude();

        if((int)(this->_boundaryTop-this->_boundaryBottom-amp)<1){//rasgele üretilecek sayınını en az 1 olması lazım burada hata varsa aşağıya gitmeden continue, yoksa kırılıyor program initialY yi güncellerken
            this->_updateSinAmplitudeFactor();
            amp=this->_getSinAmplitude();//this->_sinAmplitudeVar güncellendiği için amp'nin de güncellenmesi lazım
            //log("err0");
            continue;
        }

        initialYMin=this->_boundaryBottom;
        initialYMax=this->_boundaryBottom+amp+this->_boundaryTop-this->_boundaryBottom-2*amp;

        boundarySinBottomMax=initialYMax;
        boundarySinTopMax=initialYMax+amp;

        boundarySinBottomMin=initialYMin;
        boundarySinTopMin=initialYMin+amp;

        err=false;
        if(initialYMax<initialYMin){
            err=true;
        }else if(boundarySinBottomMin>boundarySinBottomMax){
            err=true;
        }else if(boundarySinTopMin>boundarySinTopMax){
            err=true;
        }else if(boundarySinBottomMax<this->_boundaryBottom){
            err=true;
        }else if(boundarySinTopMax>this->_boundaryTop){
            err=true;
        }else if(boundarySinBottomMin<this->_boundaryBottom){
            err=true;
        }else if(boundarySinTopMin>this->_boundaryTop){
            err=true;
        }

        if(err){
            this->_updateSinAmplitudeFactor();
        }
    }

    int halfSinAmplitudeMultiplier;
    halfSinAmplitudeMultiplier=1000*(this->_sinAmplitudeVar/2.0f);//rasgele tamsayı üretileceği için 1000 ile çarpılıyor,sonra yarısı alınıyor

    if(bugType==moth){
        this->_sinAmplitudeVar=((std::rand()%halfSinAmplitudeMultiplier)+halfSinAmplitudeMultiplier)/1000.0f;
    }else if(bugType==ladybird){
        this->_sinAmplitudeVar=((std::rand()%halfSinAmplitudeMultiplier)+halfSinAmplitudeMultiplier/2)/1000.0f;//ladybird için daha küçük sin genliği oluşturuluyor
    }
}
float FeebleBug::_updateSinAmplitudeFactor(){
    if(this->_sinAmplitudeVar<this->_sinAmplitudeVarAlterationMin && this->_sinAmplitudeVarAlteration<0){
        this->_sinAmplitudeVar=this->_sinAmplitudeVarAlterationMin;
        this->_sinAmplitudeVarAlteration*=-1;
    }else if(this->_sinAmplitudeVar>this->_sinAmplitudeVarAlterationMax && this->_sinAmplitudeVarAlteration>0){
        this->_sinAmplitudeVar=this->_sinAmplitudeVarAlterationMax;
        this->_sinAmplitudeVarAlteration*=-1;
    }
    this->_sinAmplitudeVar+=this->_sinAmplitudeVarAlteration;

    return this->_sinAmplitudeVar;
}
