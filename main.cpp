#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QTimer>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog loginDialog;

    QObject::connect(&loginDialog, &LoginDialog::loginExit, &QApplication::quit);

    if (loginDialog.exec() == QDialog::Accepted) {
        w.setUserName(loginDialog.userName());
        if (loginDialog.userType() == 0)
            w.setAdminUI();
        else if (loginDialog.userType() == 1)
            w.setStudentUI();
        w.show();
    } else {
        QTimer::singleShot(1000, qApp, SLOT(quit()));
    }
    return a.exec();
}
