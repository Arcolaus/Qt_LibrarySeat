#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    LoginDialog(QWidget* parent = nullptr);
    int userType();
    ~LoginDialog();

private:
    QGridLayout* gridLayout ;
    QLabel* username_label;
    QLabel* password_label;

    QLineEdit* username_edit;
    QLineEdit* password_edit;

    QPushButton* loginBtn;
    QPushButton* exitBtn;

    QRadioButton* student_radio;
    QRadioButton* admin_radio;

    void initUI();
    void initSignalSlots();

public slots:
    void loginCheck();

};

#endif // LOGINDIALOG_H