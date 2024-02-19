#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>

class DBsqlite : public QObject {
    Q_OBJECT
public:
    explicit DBsqlite(QObject* parent = nullptr);
    ~DBsqlite();
    void showAllSeats();

    void addBooking();

private:
    QString queryString;
    QSqlDatabase DB;
signals:
};

#endif // DBSQLITE_H
