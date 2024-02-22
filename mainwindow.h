#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDebug>
#include <QDateTime>
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
#include <QInputDialog>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    void initDB();

    void setToolBar();

    void setAdminUI();
    void setStudentUI();

    void setUserName(QString userName);

    ~MainWindow();

private:
    QString userName;
    QToolBar* toolBar;

    QAction* stuBook;
    QAction* stuCheckMyBooking;

    QAction* adminBookingRecord;
    QAction* adminManageSeat;

    QSqlDatabase mysql;

    QSignalMapper* bookSignalMapper;
    QSignalMapper* cancelBookingSignalMapper;
    QSignalMapper* deleteSeatSignalMapper;
    QSignalMapper* modifySeatSignalMapper;
    QSignalMapper* adminCancelBookingSignalMapper;


public slots:
    // 学生功能
    void bookView();
    void bookSeat(const QString& text);
    void cancelBooking(const QString& text);
    void checkMyBookingView();

    // 管理员功能
    void bookingRecordView();
    void manageSeatView();
    void adminCancelBooking(const QString& text);

    void adminBookSeat(const QString& text);
    void addSeat();
    void deleteSeat(const QString& text);
    void modifySeat(const QString& text);
};
#endif // MAINWINDOW_H
