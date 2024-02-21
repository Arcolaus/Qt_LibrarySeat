#ifndef CONNMYSQL_H
#define CONNMYSQL_H

#include <QDebug>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class ConnMysql : public QObject {
    Q_OBJECT
public:
    explicit ConnMysql(QObject* parent = nullptr);

private:
    QSqlDatabase mysql;
signals:
};

#endif // CONNMYSQL_H
