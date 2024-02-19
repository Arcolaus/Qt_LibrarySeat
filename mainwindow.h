#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    void initDB();

    void setToolBar();

    void setAdminUI();
    void setStudentUI();

    ~MainWindow();

private:
    QToolBar* toolBar;

    QAction* stuBook;
    QAction* adminBookingRecord;
    QAction* adminManageSeat;

    QSqlDatabase sqlite;

public slots:
    void bookView();

    void bookingRecordView();

    void manageSeatView();
};
#endif // MAINWINDOW_H
