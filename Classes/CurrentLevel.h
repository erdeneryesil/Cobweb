#ifndef __CURRENT_LEVEL_H__
#define __CURRENT_LEVEL_H__

//new-delete
class CurrentLevel{
    private:
        int _levelId;
        int _playCount;
        int _score;
        int _highScore;
        int _mosquitoCount;
        int _mothCount;
        int _craneCount;
        int _ladybirdCount;
        int _dragonflyCount;
        int _gadflyCount;
        int _bumblebeeCount;
        int _robberflyCount;
        int _webCount;
        int _bonusWebCount;
        double _minRateWebBug;
        int _oddsBonusMighty;
        int _oddsBonusWeb;
        int _maxBugCountOnStage;

    public:
        CurrentLevel();
        ~CurrentLevel();

        int levelId();
        int playCount();
        int score();
        int highScore();
        int mosquitoCount();
        int mothCount();
        int craneCount();
        int ladybirdCount();
        int dragonflyCount();
        int gadflyCount();
        int bumblebeeCount();
        int robberflyCount();
        int webCount();
        int bonusWebCount();
        double minRateWebBug();
        int oddsBonusMighty();
        int oddsBonusWeb();
        int maxBugCountOnStage();
};

#endif
