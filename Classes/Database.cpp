#include "Database.h"
USING_NS_CC;

Database* Database::_instance=nullptr;

Database::Database(){
    dropTable("LEVELS");
    dropTable("CURRENT_LEVEL");


    std::string sqlCreateLevels=SQL_CREATE_TABLE_LEVELS;
    this->_createTable(sqlCreateLevels);
    std::string sqlgetRowCountLevels=SQL_ROW_COUNT_LEVELS;
    int rowCountLevels=this->_getRowCount(sqlgetRowCountLevels);
    if(rowCountLevels==0){
        int rowCount=LEVELS_ROW_COUNT;
        std::string sql[rowCount]=SQL_LEVELS_INSERTS;
        this->_insertIntoTable(rowCount,sql);
    }

    std::string sqlCreateCurrentLevel=SQL_CREATE_TABLE_CURRENT_LEVEL;
    this->_createTable(sqlCreateCurrentLevel);
    std::string sqlgetRowCountCurrentLevel=SQL_ROW_COUNT_CURRENT_LEVEL;
    int rowCountCurrentLevel=this->_getRowCount(sqlgetRowCountCurrentLevel);
    if(rowCountCurrentLevel==0){
        int rowCount=CURRENT_LEVEL_ROW_COUNT;
        std::string sql[rowCount]=SQL_CURRENT_LEVEL_INSERTS;
        this->_insertIntoTable(rowCount,sql);
    }

}


Database::~Database(){}

void Database::open(){
    std::string path=FileUtils::getInstance()->getWritablePath()+DATABASE_NAME;
    sqlite3_open(path.c_str(),&(this->_db));
}

void Database::close(){
    sqlite3_close(this->_db);
}
int Database::getLastLevelId(){
    return LEVELS_ROW_COUNT;
}
Database* Database::getInstance(){
    if(Database::_instance==nullptr){
        Database::_instance=new Database();
    }
    return Database::_instance;
}
void Database::deleteInstance(){
    if(!Database::_instance)delete _instance;
}

Database::CurrentLevelDetailed* Database::getCurrentLevelDetailed(){
    CurrentLevelDetailed* currentLevelDetailed=new CurrentLevelDetailed();//gittiği yerde silinecek

    this->_getDataFromCurrentLevel(currentLevelDetailed);
    this->_getDataFromLevels(currentLevelDetailed);

    return currentLevelDetailed;
}
Database::CurrentLevelBasic* Database::getCurrentLevelBasic(){
    CurrentLevelBasic* currentLevelBasic=new CurrentLevelBasic();//gittiği yerde silinecek
    this->_getDataFromCurrentLevel(currentLevelBasic);

    return currentLevelBasic;
}

void Database::dropTable(std::string tableName){
    this->open();
    sqlite3_stmt* stmt;

    std::string sql=((std::string)("DROP TABLE IF EXISTS ")).append(tableName).append(";");

    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            //log("Error dropTable");
        }else{}
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}

void Database::_createTable(const std::string& sql){
    this->open();
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            //log("Error1 _createTable");
        }
    }else {
        //log("Error2 _createTable");
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
int Database::_getRowCount(const std::string& sql){
    int result=0;

    this->open();
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,NULL)==SQLITE_OK){
        if(sqlite3_step(stmt)==SQLITE_ROW){
            result=sqlite3_column_int(stmt,0);
        }else{
            //log("Error1 _getRowCount %s",sqlite3_errmsg(this->_db));
        }
    }else{
        //log("Error2 _getRowCount %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();

    return result;
}
void Database::_insertIntoTable(int rowCount,const std::string*const sql){
    this->open();
    sqlite3_stmt* stmt;
    for(int i=0;i<rowCount;i++){
        if(sqlite3_prepare_v2(this->_db,sql[i].c_str(),-1,&stmt,nullptr)==SQLITE_OK){
            if(sqlite3_step(stmt)!=SQLITE_DONE){
                //log("Error1 _insertIntoTable, %s",sqlite3_errmsg(this->_db));
            }
        }else {
            //log("Error2 _insertIntoTable, %s",sqlite3_errmsg(this->_db));
        }
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
void Database::updateLevelIdInCurrentLevel(int levelId){
    std::string sql=SQL_CURRENT_LEVEL_UPDATE_LEVEL_ID;

    this->open();
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        sqlite3_bind_int(stmt,1,levelId);
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            //log("Error1 updateLevelIdInCurrentLevel, %s",sqlite3_errmsg(this->_db));
        }
    }else {
        //log("Error2 updateLevelIdInCurrentLevel, %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
void Database::updatePlayCountInCurrentLevel(int playCount){
    std::string sql=SQL_CURRENT_LEVEL_UPDATE_PLAY_COUNT;

    this->open();
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        sqlite3_bind_int(stmt,1,playCount);
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            //log("Error1 updatePlayCountInCurrentLevel, %s",sqlite3_errmsg(this->_db));
        }
    }else {
        //log("Error2 updatePlayCountInCurrentLevel, %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
void Database::updateScoreInCurrentLevel(int score){
    std::string sql=SQL_CURRENT_LEVEL_UPDATE_SCORE;

    this->open();
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        sqlite3_bind_int(stmt,1,score);
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            //log("Error1 updateScoreInCurrentLevel, %s",sqlite3_errmsg(this->_db));
        }
    }else {
        //log("Error2 updateScoreInCurrentLevel, %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
void Database::updateHighScoreInCurrentLevel(int highScore){
    std::string sql=SQL_CURRENT_LEVEL_UPDATE_HIGHSCORE;

    this->open();
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        sqlite3_bind_int(stmt,1,highScore);
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            //log("Error1 updateHighScoreInCurrentLevel, %s",sqlite3_errmsg(this->_db));
        }
    }else {
        //log("Error2 updateHighScoreInCurrentLevel, %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
void Database::updateAllInCurrentLevel(int levelId,int playCount,int score){//highScore güncellenmeyecek
    std::string sql[]=SQL_CURRENT_LEVEL_UPDATE_ALL;
    int parameters[]{levelId,playCount,score};
    int rowCount=CURRENT_LEVEL_ROW_COUNT-1;//highScore güncellenmeyecek

    this->open();
    sqlite3_stmt* stmt;
    for(int i=0;i<rowCount;i++){
        if(sqlite3_prepare_v2(this->_db,sql[i].c_str(),-1,&stmt,nullptr)==SQLITE_OK){
            sqlite3_bind_int(stmt,1,parameters[i]);
            if(sqlite3_step(stmt)!=SQLITE_DONE){
                //log("Error1 updateAllInCurrentLevel, %s",sqlite3_errmsg(this->_db));
            }
        }else {
            //log("Error2 updateAllInCurrentLevel, %s",sqlite3_errmsg(this->_db));
        }
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
void Database::_getDataFromCurrentLevel(CurrentLevelBasic* currentLevelBasic){
    this->open();

    std::string key;
    int value;
    sqlite3_stmt* stmt;
    std::string sql=SQL_CURRENT_LEVEL_SELECT;

    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,NULL)==SQLITE_OK){

        while(sqlite3_step(stmt)==SQLITE_ROW){
            const unsigned char* keyCharP=sqlite3_column_text(stmt,0);
            key=std::string((char*)keyCharP);

            if(key==KEY1STR)currentLevelBasic->levelId=sqlite3_column_int(stmt,1);
            else if(key==KEY2STR)currentLevelBasic->playCount=sqlite3_column_int(stmt,1);
            else if(key==KEY3STR)currentLevelBasic->score=sqlite3_column_int(stmt,1);
            else if(key==KEY4STR)currentLevelBasic->highScore=sqlite3_column_int(stmt,1);
        }
    }else{
        //log("Error _getDataFromCurrentLevel %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}

void Database::_getDataFromLevels(CurrentLevelDetailed* currentLevelDetailed){
    this->open();

    sqlite3_stmt* stmt;
    std::string sql=SQL_LEVELS_SELECT;

    if(sqlite3_prepare_v2(this->_db,sql.c_str(),-1,&stmt,NULL)==SQLITE_OK){
        sqlite3_bind_int(stmt,1,currentLevelDetailed->levelId);

        if(sqlite3_step(stmt)==SQLITE_ROW){
            currentLevelDetailed->mosquitoCount=sqlite3_column_int(stmt,0);
            currentLevelDetailed->mothCount=sqlite3_column_int(stmt,1);
            currentLevelDetailed->craneCount=sqlite3_column_int(stmt,2);
            currentLevelDetailed->ladybirdCount=sqlite3_column_int(stmt,3);
            currentLevelDetailed->dragonflyCount=sqlite3_column_int(stmt,4);
            currentLevelDetailed->gadflyCount=sqlite3_column_int(stmt,5);
            currentLevelDetailed->bumblebeeCount=sqlite3_column_int(stmt,6);
            currentLevelDetailed->robberflyCount=sqlite3_column_int(stmt,7);
            currentLevelDetailed->webCount=sqlite3_column_int(stmt,8);
            currentLevelDetailed->bonusWebCount=sqlite3_column_int(stmt,9);
            currentLevelDetailed->minRateWebBug=sqlite3_column_double(stmt,10);
            currentLevelDetailed->oddsBonusMighty=sqlite3_column_int(stmt,11);
            currentLevelDetailed->oddsBonusWeb=sqlite3_column_int(stmt,12);
            currentLevelDetailed->maxBugCountOnStage=sqlite3_column_int(stmt,13);
        }
    }else{
        //log("Error _getDataFromLevels %s",sqlite3_errmsg(this->_db));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    this->close();
}
