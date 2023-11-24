#ifndef __CRANE_H__
#define __CRANE_H__

#include "WeakestBug.h"
#define CRANE_POINT 2

class Crane : public WeakestBug
{
private:
    void _initVectorRandom() override;
    static int _cranePoint;
public:
    Crane();
    ~Crane();
    int getPoint()override;
};
#endif
