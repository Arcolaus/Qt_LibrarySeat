#include "dbsqlite.h"

DBsqlite::DBsqlite(QObject* parent)
    : QObject { parent } {
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("./database/libraryseat.db"); //打开数据库

    if (DB.open()) {

    } else {
        QMessageBox msgBox;
        msgBox.setText("数据库连接失败");
        msgBox.setDetailedText(DB.lastError().text());
        msgBox.exec();
    }
}

DBsqlite::addRecord() {

}

DBsqlite::~DBsqlite() {
    DB.close();
}