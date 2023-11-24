#ifndef __MOTH_H__
#define __MOTH_H__

#include "FeebleBug.h"

#define MOTH_POINT 1

class Moth : public FeebleBug{
private:
    float _boundarySinTop;//Bug sınıfının _boundaryTop, değişkenine ek olarak bu _boundarySinTop, sinüs sınırını belirleyecek
    float _boundarySinBottom;//Bug sınıfının _boundaryBottom, değişkenine ek olarak bu _boundarySinTop, sinüs sınırını belirleyecek
    float _adaptX;//ekrana soldan girerse, x=0 değil, x=_boundaryLeft olduğu için sinüs dalgasında bozulma oluyor, bunu önlemek için soldan girerse _overage=_boundaryLeft, sağdan girerse _overage=(this->_boundaryRight)%360
    void _initVectorRandom() override;
    void _flyUpdate() override;
    bool initFeebleBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity) override;
    float _generateEntranceYRandom() override;
    void _sinusOverflowYControl();

    static int _mothPoint;
public:
    Moth();
    ~Moth();
    int getPoint()override;
};

#endif
