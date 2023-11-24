#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include "CurrentLevel.h"

#define DATABASE_NAME "Database.sqlite"

//LEVELS TABLOSUNA AİT TANIMLAMALAR//

#define LEVELS_TABLE_NAME "LEVELS"
#define LEVELS_ROW_COUNT 50
#define COLUMN_NAME_LEVEL_ID "LEVEL_ID"
#define COLUMN_NAME_MOSQUITO_COUNT "MOSQUITO_COUNT"
#define COLUMN_NAME_MOTH_COUNT "MOTH_COUNT"
#define COLUMN_NAME_CRANE_COUNT "CRANE_COUNT"
#define COLUMN_NAME_LADYBIRD_COUNT "LADYBIRD_COUNT"
#define COLUMN_NAME_DRAGONFLY_COUNT "DRAGONFLY_COUNT"
#define COLUMN_NAME_GADFLY_COUNT "GADFLY_COUNT"
#define COLUMN_NAME_BUMBLEBEE_COUNT "BUMBLEBEE_COUNT"
#define COLUMN_NAME_ROBBERFLY_COUNT "ROBBERFLY_COUNT"
#define COLUMN_NAME_WEB_COUNT "WEB_COUNT"
#define COLUMN_NAME_BONUS_WEB_COUNT "BONUS_WEB_COUNT"
#define COLUMN_NAME_MIN_RATE_WEB_BUG "MIN_RATE_WEB_BUG"
#define COLUMN_NAME_ODDS_BONUS_MIGHTY "ODDS_BONUS_MIGHTY"
#define COLUMN_NAME_ODDS_BONUS_WEB "ODDS_BONUS_WEB"
#define COLUMN_NAME_MAX_BUG_COUNT_ON_STAGE "MAX_BUG_COUNT_ON_STAGE"

#define SQL_ROW_COUNT_LEVELS "SELECT COUNT(" COLUMN_NAME_LEVEL_ID ") FROM " LEVELS_TABLE_NAME

#define SQL_CREATE_TABLE_LEVELS "CREATE TABLE IF NOT EXISTS " LEVELS_TABLE_NAME "(" \
 /*seviye numarası int*/                            COLUMN_NAME_LEVEL_ID " INT PRIMARY KEY," \
 /*mosquito sayısı int*/                            COLUMN_NAME_MOSQUITO_COUNT " INT," \
/*moth sayısı int*/                                 COLUMN_NAME_MOTH_COUNT " INT," \
/*crane sayısı int*/                                COLUMN_NAME_CRANE_COUNT " INT," \
/*ladybird sayısı int*/                             COLUMN_NAME_LADYBIRD_COUNT " INT," \
/*dragonfly sayısı int*/                            COLUMN_NAME_DRAGONFLY_COUNT " INT," \
/*gadfly sayısı int*/                               COLUMN_NAME_GADFLY_COUNT " INT," \
/*bumblebee sayısı int*/                            COLUMN_NAME_BUMBLEBEE_COUNT " INT," \
/*robberfly sayısı int*/                            COLUMN_NAME_ROBBERFLY_COUNT " INT," \
/*web sayısı int*/                                  COLUMN_NAME_WEB_COUNT " INT," \
/*bonus web sayısı int*/                            COLUMN_NAME_BONUS_WEB_COUNT " INT," \
/*web/bug oranıo float*/                            COLUMN_NAME_MIN_RATE_WEB_BUG " REAL," \
/*bonusBugMighty olasılığı 1/100 ise 1 int*/        COLUMN_NAME_ODDS_BONUS_MIGHTY " INT," \
/*bonusBugWeb olasılığı 1/100 ise 1 int*/           COLUMN_NAME_ODDS_BONUS_WEB " INT," \
/*sahnede bulunabilecek max bug sayısı int*/        COLUMN_NAME_MAX_BUG_COUNT_ON_STAGE " INT" \
                                                    ");"

#define SQL_LEVELS_COLUMNS "("\
                              COLUMN_NAME_LEVEL_ID ","\
                              COLUMN_NAME_MOSQUITO_COUNT ","\
                              COLUMN_NAME_MOTH_COUNT ","\
                              COLUMN_NAME_CRANE_COUNT ","\
                              COLUMN_NAME_LADYBIRD_COUNT ","\
                              COLUMN_NAME_DRAGONFLY_COUNT ","\
                              COLUMN_NAME_GADFLY_COUNT ","\
                              COLUMN_NAME_BUMBLEBEE_COUNT ","\
                              COLUMN_NAME_ROBBERFLY_COUNT ","\
                              COLUMN_NAME_WEB_COUNT ","\
                              COLUMN_NAME_BONUS_WEB_COUNT ","\
                              COLUMN_NAME_MIN_RATE_WEB_BUG ","\
                              COLUMN_NAME_ODDS_BONUS_MIGHTY ","\
                              COLUMN_NAME_ODDS_BONUS_WEB ","\
                              COLUMN_NAME_MAX_BUG_COUNT_ON_STAGE \
                              ")"

#define SQL_LEVELS_VALUES_ROW1 "VALUES(1,30,0,0,0,0,0,0,0,9,0,0.00,0,0,20);"
#define SQL_LEVELS_VALUES_ROW2 "VALUES(2,30,0,0,0,0,0,0,0,8,0,0.00,0,0,10);"
#define SQL_LEVELS_VALUES_ROW3 "VALUES(3,30,0,0,0,0,0,0,0,11,0,0.00,0,0,5);"
#define SQL_LEVELS_VALUES_ROW4 "VALUES(4,10,0,0,0,0,0,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW5 "VALUES(5,15,0,0,0,0,0,0,0,10,0,0.00,0,0,3);"
#define SQL_LEVELS_VALUES_ROW6 "VALUES(6,25,5,0,0,0,0,0,0,8,0,0.00,0,0,10);"
#define SQL_LEVELS_VALUES_ROW7 "VALUES(7,25,5,0,0,0,0,0,0,11,0,0.00,0,0,5);"
#define SQL_LEVELS_VALUES_ROW8 "VALUES(8,0,10,0,0,0,0,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW9 "VALUES(9,20,10,0,0,0,0,0,0,11,0,0.00,0,0,5);"
#define SQL_LEVELS_VALUES_ROW10 "VALUES(10,0,15,0,0,0,0,0,0,10,0,0.00,0,0,3);"
#define SQL_LEVELS_VALUES_ROW11 "VALUES(11,5,5,0,0,0,0,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW12 "VALUES(12,10,15,5,0,0,0,0,0,8,3,0.23,0,3,10);"
#define SQL_LEVELS_VALUES_ROW13 "VALUES(13,10,15,5,0,0,0,0,0,11,4,0.31,0,4,5);"
#define SQL_LEVELS_VALUES_ROW14 "VALUES(14,0,0,10,0,0,0,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW15 "VALUES(15,10,10,10,0,0,0,0,0,11,4,0.31,0,4,5);"
#define SQL_LEVELS_VALUES_ROW16 "VALUES(16,0,0,15,0,0,0,0,0,10,3,0.57,0,3,3);"
#define SQL_LEVELS_VALUES_ROW17 "VALUES(17,5,5,5,0,0,0,0,0,15,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW18 "VALUES(18,5,10,10,5,0,0,0,0,8,3,0.23,0,3,10);"
#define SQL_LEVELS_VALUES_ROW19 "VALUES(19,5,10,10,5,0,0,0,0,11,4,0.31,0,4,5);"
#define SQL_LEVELS_VALUES_ROW20 "VALUES(20,0,0,0,10,0,0,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW21 "VALUES(21,5,5,10,10,0,0,0,0,11,4,0.31,0,4,5);"
#define SQL_LEVELS_VALUES_ROW22 "VALUES(22,0,0,0,15,0,0,0,0,10,3,0.57,0,3,3);"
#define SQL_LEVELS_VALUES_ROW23 "VALUES(23,5,5,5,5,0,0,0,0,20,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW24 "VALUES(24,5,5,5,10,5,0,0,0,8,3,0.23,9,3,10);"
#define SQL_LEVELS_VALUES_ROW25 "VALUES(25,5,5,5,5,5,5,5,5,11,4,0.31,12,4,8);"
#define SQL_LEVELS_VALUES_ROW26 "VALUES(26,0,0,0,0,10,0,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW27 "VALUES(27,5,5,5,5,10,0,0,0,11,4,0.31,12,4,5);"
#define SQL_LEVELS_VALUES_ROW28 "VALUES(28,0,0,0,0,15,0,0,0,10,3,0.57,9,3,3);"
#define SQL_LEVELS_VALUES_ROW29 "VALUES(29,5,5,5,5,5,0,0,0,25,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW30 "VALUES(30,5,5,5,5,5,5,0,0,8,3,0.23,9,3,10);"
#define SQL_LEVELS_VALUES_ROW31 "VALUES(31,5,5,5,5,5,5,0,0,11,4,0.31,12,4,5);"
#define SQL_LEVELS_VALUES_ROW32 "VALUES(32,0,0,0,0,0,10,0,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW33 "VALUES(33,5,5,5,5,5,10,0,0,11,4,0.27,12,4,8);"
#define SQL_LEVELS_VALUES_ROW34 "VALUES(34,5,5,5,5,5,10,0,0,14,5,0.34,15,5,5);"
#define SQL_LEVELS_VALUES_ROW35 "VALUES(35,0,0,0,0,0,15,0,0,10,3,0.57,9,3,3);"
#define SQL_LEVELS_VALUES_ROW36 "VALUES(36,5,5,5,5,5,5,0,0,30,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW37 "VALUES(37,5,5,5,5,5,5,5,0,11,4,0.27,12,4,8);"
#define SQL_LEVELS_VALUES_ROW38 "VALUES(38,5,5,5,5,5,5,5,0,14,5,0.34,15,5,5);"
#define SQL_LEVELS_VALUES_ROW39 "VALUES(39,0,0,0,0,0,0,10,0,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW40 "VALUES(40,3,4,4,4,5,5,10,0,14,5,0.34,15,5,7);"
#define SQL_LEVELS_VALUES_ROW41 "VALUES(41,0,0,0,0,0,0,15,0,12,3,0.68,0,3,3);"
#define SQL_LEVELS_VALUES_ROW42 "VALUES(42,5,5,5,5,5,5,5,0,35,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW43 "VALUES(43,0,2,3,4,8,8,5,5,16,6,0.39,18,6,6);"
#define SQL_LEVELS_VALUES_ROW44 "VALUES(44,0,0,3,3,8,8,8,5,16,6,0.39,18,6,6);"
#define SQL_LEVELS_VALUES_ROW45 "VALUES(45,0,0,0,0,0,0,0,10,10,0,0.00,0,0,1);"
#define SQL_LEVELS_VALUES_ROW46 "VALUES(46,0,0,0,3,8,8,8,8,16,6,0.39,18,6,6);"
#define SQL_LEVELS_VALUES_ROW47 "VALUES(47,0,0,0,0,8,10,8,9,20,7,0.49,21,7,5);"
#define SQL_LEVELS_VALUES_ROW48 "VALUES(48,0,0,0,0,0,0,0,15,12,3,0.68,0,3,3);"
#define SQL_LEVELS_VALUES_ROW49 "VALUES(49,0,0,0,0,7,8,8,12,20,7,0.49,21,7,5);"
#define SQL_LEVELS_VALUES_ROW50 "VALUES(50,5,5,5,5,5,5,5,5,40,0,0.00,0,0,1);"


#define SQL_LEVELS_INSERTS  {\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW1,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW2,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW3,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW4,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW5,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW6,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW7,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW8,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW9,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW10,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW11,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW12,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW13,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW14,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW15,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW16,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW17,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW18,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW19,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW20,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW21,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW22,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW23,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW24,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW25,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW26,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW27,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW28,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW29,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW30,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW31,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW32,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW33,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW34,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW35,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW36,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW37,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW38,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW39,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW40,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW41,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW42,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW43,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW44,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW45,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW46,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW47,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW48,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW49,\
                                "INSERT INTO " LEVELS_TABLE_NAME SQL_LEVELS_COLUMNS " " SQL_LEVELS_VALUES_ROW50,\
                                }


#define SQL_LEVELS_SELECT "SELECT " \
                                    COLUMN_NAME_MOSQUITO_COUNT "," \
                                    COLUMN_NAME_MOTH_COUNT "," \
                                    COLUMN_NAME_CRANE_COUNT "," \
                                    COLUMN_NAME_LADYBIRD_COUNT ","\
                                    COLUMN_NAME_DRAGONFLY_COUNT "," \
                                    COLUMN_NAME_GADFLY_COUNT "," \
                                    COLUMN_NAME_BUMBLEBEE_COUNT "," \
                                    COLUMN_NAME_ROBBERFLY_COUNT "," \
                                    COLUMN_NAME_WEB_COUNT "," \
                                    COLUMN_NAME_BONUS_WEB_COUNT "," \
                                    COLUMN_NAME_MIN_RATE_WEB_BUG "," \
                                    COLUMN_NAME_ODDS_BONUS_MIGHTY "," \
                                    COLUMN_NAME_ODDS_BONUS_WEB "," \
                                    COLUMN_NAME_MAX_BUG_COUNT_ON_STAGE \
                                    " from " LEVELS_TABLE_NAME " WHERE " COLUMN_NAME_LEVEL_ID "=?;"

//LEVELS TABLOSUNA AİT TANIMLAMALAR BİTTİ//


//CURRENT_LEVEL TABLOSUNA AİT TANIMLAMALAR//
#define CURRENT_LEVEL_TABLE_NAME "CURRENT_LEVEL"
#define CURRENT_LEVEL_ROW_COUNT 4
#define COLUMN_NAME_KEY "KEY"
#define COLUMN_NAME_VALUE "VALUE"
#define KEY1 "'LEVEL_ID'"   //mevcut level in değeri
#define KEY2 "'PLAY_COUNT'" //mevcut level i kaçıncı kere oynayacağı, başlangıçta 1
#define KEY3 "'SCORE'"//mevcut skor başlangıçta 0
#define KEY4 "'HIGHSCORE'"//rekor skor ilk kurulumda 0
#define KEY1STR "LEVEL_ID" //tek tırnak işareti yok
#define KEY2STR "PLAY_COUNT" //tek tırnak işareti yok
#define KEY3STR "SCORE" //tek tırnak işareti yok
#define KEY4STR "HIGHSCORE" //tek tırnak işareti yok

#define SQL_CREATE_TABLE_CURRENT_LEVEL "CREATE TABLE IF NOT EXISTS " CURRENT_LEVEL_TABLE_NAME "(" \
 /*mevcut level deki ayarın adı, LEVEL_ID gibi TEXT(20)*/                               COLUMN_NAME_KEY " TEXT(20) PRIMARY KEY," \
 /*mevcut level deki ayarın değeri, LEVEL_IDnin 5 olması gibi int*/                     COLUMN_NAME_VALUE " INT" \
                                                                                        ");"

#define SQL_ROW_COUNT_CURRENT_LEVEL "SELECT COUNT(" COLUMN_NAME_KEY ") FROM " CURRENT_LEVEL_TABLE_NAME

#define SQL_CURRENT_LEVEL_COLUMNS "("\
                              COLUMN_NAME_KEY ","\
                              COLUMN_NAME_VALUE \
                              ")"

#define SQL_CURRENT_LEVEL_VALUES_ROW1 "VALUES(" KEY1 ",1);"
#define SQL_CURRENT_LEVEL_VALUES_ROW2 "VALUES(" KEY2 ",1);"
#define SQL_CURRENT_LEVEL_VALUES_ROW3 "VALUES(" KEY3 ",0);"
#define SQL_CURRENT_LEVEL_VALUES_ROW4 "VALUES(" KEY4 ",0);"

#define SQL_CURRENT_LEVEL_INSERTS  {\
                                "INSERT INTO " CURRENT_LEVEL_TABLE_NAME SQL_CURRENT_LEVEL_COLUMNS " " SQL_CURRENT_LEVEL_VALUES_ROW1,\
                                "INSERT INTO " CURRENT_LEVEL_TABLE_NAME SQL_CURRENT_LEVEL_COLUMNS " " SQL_CURRENT_LEVEL_VALUES_ROW2,\
                                "INSERT INTO " CURRENT_LEVEL_TABLE_NAME SQL_CURRENT_LEVEL_COLUMNS " " SQL_CURRENT_LEVEL_VALUES_ROW3,\
                                "INSERT INTO " CURRENT_LEVEL_TABLE_NAME SQL_CURRENT_LEVEL_COLUMNS " " SQL_CURRENT_LEVEL_VALUES_ROW4\
                            }

#define SQL_CURRENT_LEVEL_UPDATE_LEVEL_ID "UPDATE " CURRENT_LEVEL_TABLE_NAME " SET " COLUMN_NAME_VALUE "=? WHERE " COLUMN_NAME_KEY "=" KEY1 ";"
#define SQL_CURRENT_LEVEL_UPDATE_PLAY_COUNT "UPDATE " CURRENT_LEVEL_TABLE_NAME " SET " COLUMN_NAME_VALUE "=? WHERE " COLUMN_NAME_KEY "=" KEY2 ";"
#define SQL_CURRENT_LEVEL_UPDATE_SCORE "UPDATE " CURRENT_LEVEL_TABLE_NAME " SET " COLUMN_NAME_VALUE "=? WHERE " COLUMN_NAME_KEY "=" KEY3 ";"
#define SQL_CURRENT_LEVEL_UPDATE_ALL {SQL_CURRENT_LEVEL_UPDATE_LEVEL_ID,SQL_CURRENT_LEVEL_UPDATE_PLAY_COUNT,SQL_CURRENT_LEVEL_UPDATE_SCORE}
#define SQL_CURRENT_LEVEL_UPDATE_HIGHSCORE "UPDATE " CURRENT_LEVEL_TABLE_NAME " SET " COLUMN_NAME_VALUE "=? WHERE " COLUMN_NAME_KEY "=" KEY4 ";"

#define SQL_CURRENT_LEVEL_SELECT "SELECT " COLUMN_NAME_KEY "," COLUMN_NAME_VALUE " from " CURRENT_LEVEL_TABLE_NAME ";"

//CURRENT_LEVEL TABLOSUNA AİT TANIMLAMALAR BİTTİ//


//getInstance() ile çağırılacak ve AppDelegate yıkıcı fonksiyonunda deleteInstance() ile yok edilecek

class Database{
    public:
        struct CurrentLevelBasic{
            int levelId;
            int playCount;
            int score;
            int highScore;
        };

        struct CurrentLevelDetailed:public CurrentLevelBasic{
            int mosquitoCount;
            int mothCount;
            int craneCount;
            int ladybirdCount;
            int dragonflyCount;
            int gadflyCount;
            int bumblebeeCount;
            int robberflyCount;
            int webCount;
            int bonusWebCount;
            double minRateWebBug;
            int oddsBonusMighty;
            int oddsBonusWeb;
            int maxBugCountOnStage;
        };
        static Database* getInstance();
        static int getLastLevelId();//son levelin numarasını döndürür
        static void deleteInstance();
        CurrentLevelDetailed* getCurrentLevelDetailed();//döndürülen pointer, gitti yerde silinecek
        CurrentLevelBasic* getCurrentLevelBasic();//döndürülen pointer, gitti yerde silinecek
        void updateLevelIdInCurrentLevel(int levelId);
        void updatePlayCountInCurrentLevel(int playCount);
        void updateScoreInCurrentLevel(int score);
        void updateHighScoreInCurrentLevel(int highScore);
        void updateAllInCurrentLevel(int levelId,int playCount,int score);//highScore güncellenmeyecek

        void dropTable(std::string tableName);//deneme

    private:
        Database();
        virtual ~Database();
        static Database* _instance;
        sqlite3* _db;
        void open();
        void close();

        void _createTable(const std::string& sql);// tablo oluştur, eğer yoksa
        int _getRowCount(const std::string& sql);//tablodaki kayıt sayısı
        void _insertIntoTable(int rowCount,const std::string*const sql);//tabloda bulunması gereken varsayılan verileri kaydeder
        void _getDataFromCurrentLevel(CurrentLevelBasic* currentLevelBasic);
        void _getDataFromLevels(CurrentLevelDetailed* currentLevelDetailed);
};

#endif
