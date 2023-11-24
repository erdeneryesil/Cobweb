#ifndef __MOSQUITO_H__
#define __MOSQUITO_H__

#include "WeakestBug.h"

#define MOSQUITO_POINT 1

class Mosquito: public WeakestBug{
private:
    void _initVectorRandom() override;
    static int _mosquitoPoint;

public:
    Mosquito();
    ~Mosquito();
    int getPoint()override;
};

#endif



