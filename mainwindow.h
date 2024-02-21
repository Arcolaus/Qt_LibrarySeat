#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
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
    QAction* stuCheckMyBooking;

    QAction* adminBookingRecord;
    QAction* adminManageSeat;

    QSqlDatabase mysql;

    QSignalMapper* signalMapper;

public slots:
    // 学生功能
    void bookView();
    void bookSeat(const QString& text);
    void checkMyBookingView();

    // 管理员功能
    void bookingRecordView();
    void manageSeatView();
};
#endif // MAINWINDOW_H
