#ifndef RBDATABASE_H
#define RBDATABASE_H

#include "../BasicHeader/RBLog.h"
#include "../BasicHeader/RBDataType.h"

//For Database File
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>



class RBDataBase
{
public:
    RBDataBase();

    void    SetFilename(QString name);
    bool    OpenDB();

    static DB_GENERAL      _DB_GENERAL;
    static DB_MC           _DB_MC[MAX_MC];
    static DB_FT           _DB_FT[MAX_FT];


private:
    QString         filename;
    QSqlDatabase    dbCore;
};

#endif // RBDATABASE_H
