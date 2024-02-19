#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class DBsqlite : public QObject {
    Q_OBJECT
public:
    explicit DBsqlite(QObject* parent = nullptr);
    ~DBsqlite();

    void showAllSeats();

    bool sqlExec(QString sql);

private:
    QString queryString;
    QSqlDatabase DB;
    QSqlQuery que;
signals:
};

#endif // DBSQLITE_H
