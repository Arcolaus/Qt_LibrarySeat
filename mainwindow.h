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
#include <QSignalMapper>

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

    QSignalMapper * signalMapper;

public slots:
    void bookView();

    void bookingRecordView();

    void manageSeatView();

    void bookSeat(const QString &text);
};
#endif // MAINWINDOW_H
