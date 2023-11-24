#ifndef __WEAKESTBUG_H__
#define __WEAKESTBUG_H__

#include "Bug.h"

#define MOSQUITO_PNG_FILE "bugs/spritesheet/mosquito.png"
#define MOSQUITO_PLIST_FILE "bugs/spritesheet/mosquito.plist"
#define CRANE_PNG_FILE "bugs/spritesheet/crane.png"
#define CRANE_PLIST_FILE "bugs/spritesheet/crane.plist"

class WeakestBug: public Bug
{
private:
    void _initVectorRandom()override=0;
    void _flyUpdate() override;

public:
    WeakestBug();
    ~WeakestBug();
    static WeakestBug* createWeakestBug(BugType bugType,float velocity);
    bool initWeakestBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity);

    int getPoint()override=0;
};

#endif
