#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Accepted) {
        if (loginDialog.userType() == 0)
            w.setAdminUI();
        else if (loginDialog.userType() == 1)
            w.setStudentUI();
        w.show();
    } else {
        w.close();
    }

    return a.exec();
}
