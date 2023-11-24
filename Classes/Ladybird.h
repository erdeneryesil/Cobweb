#ifndef __LADYBIRD_H__
#define __LADYBIRD_H__

#include "FeebleBug.h"
#define LADYBIRD_POINT 3

class Ladybird : public FeebleBug{
private:
    void _initVectorRandom() override;
    void _flyUpdate() override;
    static int _ladybirdPoint;
public:
    Ladybird();
    ~Ladybird();
    int getPoint()override;
};

#endif



