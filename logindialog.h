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
    int userType();
    QString userName();
    ~LoginDialog();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QGridLayout* gridLayout;
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
signals:
    void loginExit();
};

#endif // LOGINDIALOG_H
