#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    LoginDialog(QWidget* parent = nullptr);
    int userType(); // 返回登录的用户类型
    QString userName(); // 返回登录的用户名
    ~LoginDialog();

protected:
    void closeEvent(QCloseEvent* event) override; // 重载登录对话框关闭时的动作

private:
    QGridLayout* gridLayout; // 登录界面网格布局
    QLabel* username_label; 
    QLabel* password_label;

    QLineEdit* username_edit; // 用户名输入
    QLineEdit* password_edit; // 密码输入

    QPushButton* loginBtn; // 登录按键
    QPushButton* exitBtn; // 退出按键

    QRadioButton* student_radio; // 选择学生登录身份
    QRadioButton* admin_radio; // 选择管理员登录身份

    void initUI(); // 初始化登录界面UI
    void initSignalSlots(); // 初始化信号槽

public slots:
    void loginCheck(); // 检查用户名和密码是否正确
signals:
    void loginExit();
};

#endif // LOGINDIALOG_H
