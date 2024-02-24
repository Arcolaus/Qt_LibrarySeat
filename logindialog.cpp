#include "logindialog.h"

LoginDialog::LoginDialog(QWidget* parent) {
    this->setFixedSize(400, 400);
    this->setWindowTitle("图书馆座位预约");

    gridLayout = new QGridLayout(this);

    initUI();
    initSignalSlots();
}
void LoginDialog::loginCheck() {
    QString inputName = username_edit->text().trimmed();
    QString inputPasswd = password_edit->text().trimmed();

    QString rightPasswd;
    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        QSqlQuery que;
        if (userType() == 1)
            que.exec(QString("select * from user where user_id='%1';").arg(inputName));
        else
            que.exec(QString("select * from user_admin where admin_id='%1';").arg(inputName));

        while (que.next()) {
            rightPasswd = que.value("password").toString();
            qDebug()<<rightPasswd;
        }
    }
    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    if (inputPasswd == rightPasswd) {
        accept();
    } else {
        QMessageBox::warning(this, "警告!", "用户名和密码错误!", QMessageBox::Yes);
    }
}

void LoginDialog::initUI() {
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(50, 50, 30, 50);

    username_label = new QLabel("用户名:", this);
    gridLayout->addWidget(username_label, 1, 1, 1, 1);
    // username_label->setAlignment(Qt::AlignRight);

    username_edit = new QLineEdit("", this);
    gridLayout->addWidget(username_edit, 1, 2, 1, 3);

    password_label = new QLabel("密码:", this);
    // password_label->setAlignment(Qt::AlignRight);
    gridLayout->addWidget(password_label, 2, 1, 1, 1);

    password_edit = new QLineEdit(this);
    gridLayout->addWidget(password_edit, 2, 2, 1, 3);

    student_radio = new QRadioButton("学生", this);
    student_radio->setChecked(true);
    gridLayout->addWidget(student_radio, 3, 2, 1, 1);

    admin_radio = new QRadioButton("管理员", this);
    gridLayout->addWidget(admin_radio, 3, 3, 1, 1);

    loginBtn = new QPushButton("登录", this);
    gridLayout->addWidget(loginBtn, 4, 1, 1, 2);

    exitBtn = new QPushButton("退出", this);
    gridLayout->addWidget(exitBtn, 4, 3, 1, 2);
}

void LoginDialog::initSignalSlots() {
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::loginCheck);
    connect(exitBtn, &QPushButton::clicked, this, &QDialog::close);
}

int LoginDialog::userType() {
    if (admin_radio->isChecked())
        return 0;
    else if (student_radio->isChecked())
        return 1;
    else {
        QMessageBox::warning(this, "警告!", "请选择登录类型!", QMessageBox::Yes);
        return -1;
    }
}

QString LoginDialog::userName() {
    return username_edit->text().trimmed();
}

void LoginDialog::closeEvent(QCloseEvent* event) {
    QDialog::closeEvent(event);
    QApplication::quit(); // 退出应用程序
}

LoginDialog::~LoginDialog() {
}
