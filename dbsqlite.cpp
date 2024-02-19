#include "dbsqlite.h"

DBsqlite::DBsqlite(QObject* parent)
    : QObject { parent } {
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("./database/libraryseat.db"); //打开数据库

    if (DB.open()) {

        /*
        QSqlQuery q;
        q.exec("SELECT * FROM loginusers");
        while(q.next()){
            qDebug()<<q.value("name").toString();
        }
        */
    } else {
        QMessageBox msgBox;
        msgBox.setText("数据库连接失败");
        msgBox.setDetailedText(DB.lastError().text());
        msgBox.exec();
    }
}

bool DBsqlite::sqlExec(QString sql) {
    return que.exec(sql);
}

DBsqlite::~DBsqlite() {
    DB.close();
}