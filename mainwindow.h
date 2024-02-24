#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDateTime>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QSpinBox>
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
    QAction * adminManageUser;

    QAction* adminAddUser;
    QAction* adminDeletUser;
    QAction* adminModifyUser;

    QSqlDatabase mysql;

    QSignalMapper* bookSignalMapper;
    QSignalMapper* cancelBookingSignalMapper;

    QSignalMapper* addSeatSignalMapper;
    QSignalMapper* deleteSeatSignalMapper;
    QSignalMapper* modifySeatSignalMapper;

    QSignalMapper* adminCancelBookingSignalMapper;

    QSignalMapper* deleteUserSignalMapper;
    QSignalMapper* modifyUserSignalMapper;
    QSignalMapper* addUserSignalMapper;

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
    void manageUsersView();

    void adminBookSeat(const QString& text);
    void addSeat();
    void deleteSeat(const QString& text);
    void modifySeat(const QString& text);

    void addUser();
    void deleteUser(const QString& text);
    void modifyUser(const QString& text);
};
#endif // MAINWINDOW_H
