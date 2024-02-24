#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    this->resize(1280, 768);

    adminCancelBookingSignalMapper = new QSignalMapper(this);
    bookSignalMapper = new QSignalMapper(this);
    cancelBookingSignalMapper = new QSignalMapper(this);
    deleteSeatSignalMapper = new QSignalMapper(this);
    modifySeatSignalMapper = new QSignalMapper(this);

    deleteUserSignalMapper = new QSignalMapper(this);
    modifyUserSignalMapper = new QSignalMapper(this);
    addUserSignalMapper = new QSignalMapper(this);

    setToolBar();
}

void MainWindow::setUserName(QString username) {
    this->userName = username;
}

void MainWindow::setToolBar() {
    this->toolBar = new QToolBar(this);
    this->toolBar->setFloatable(false);
    this->toolBar->setMovable(false);

    this->addToolBar(this->toolBar);
}

void MainWindow::setAdminUI() {
    this->adminBookingRecord = new QAction("预约记录", this);
    this->toolBar->addAction(this->adminBookingRecord);

    this->adminManageSeat = new QAction("管理座位", this);
    this->toolBar->addAction(adminManageSeat);

    this->adminManageUser = new QAction("管理用户", this);
    this->toolBar->addAction(adminManageUser);

    connect(adminManageUser, &QAction::triggered, this, &MainWindow::manageUsersView);
    connect(this->adminBookingRecord, &QAction::triggered, this, &MainWindow::bookingRecordView);
    connect(this->adminManageSeat, &QAction::triggered, this, &MainWindow::manageSeatView);
}

void MainWindow::setStudentUI() {
    this->stuBook = new QAction("预约", this->toolBar);
    this->stuCheckMyBooking = new QAction("查看我的预约记录", this->toolBar);

    this->toolBar->addAction(this->stuBook);
    this->toolBar->addAction(this->stuCheckMyBooking);

    connect(this->stuBook, &QAction::triggered, this, &MainWindow::bookView);
    connect(this->stuCheckMyBooking, &QAction::triggered, this, &MainWindow::checkMyBookingView);
}

void MainWindow::bookView() {
    QVector<QPushButton*> pushButtonVector;

    QTableWidget* tableWidget = new QTableWidget(100, 5, this);
    QStringList tableHeaders { "座位ID", "楼层", "区域", "是否可用", "预约" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        QSqlQuery sqlQue;

        sqlQue.exec("SELECT * FROM seat;");
        for (int i = 0; sqlQue.next(); i++) {
            tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("seat_id").toString()));
            tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("floor").toString()));
            tableWidget->setItem(i, 2, new QTableWidgetItem(sqlQue.value("area").toString()));

            if (sqlQue.value("availability").toInt() == 1) {
                tableWidget->setItem(i, 3, new QTableWidgetItem("可用"));
                tableWidget->item(i, 3)->setBackground(QColor(Qt::green));

                QPushButton* bookButton = new QPushButton("book", this);
                pushButtonVector.push_back(bookButton);
                tableWidget->setCellWidget(i, 4, bookButton);

                connect(bookButton, &QPushButton::clicked, bookSignalMapper, qOverload<>(&QSignalMapper::map));
                bookSignalMapper->setMapping(bookButton, sqlQue.value("seat_id").toString());
            } else {
                tableWidget->setItem(i, 3, new QTableWidgetItem("不可用"));
                tableWidget->item(i, 3)->setBackground(QColor(Qt::red));
            }
        }
    }

    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    connect(bookSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::bookSeat);
    this->setCentralWidget(tableWidget);
}

void MainWindow::checkMyBookingView() {
    QVector<QPushButton*> pushButtonVector;
    QTableWidget* tableWidget = new QTableWidget(100, 7, this);
    QStringList tableHeaders { "预定编号", "座位ID", "楼层", "区域", "起始时间", "结束时间", "是否已过期" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        QSqlQuery sqlQue;

        sqlQue.exec(QString("select * from booking left join libraryseat.seat as s on s.seat_id = booking.seat_id where booking.user_id='%1';").arg(this->userName));

        for (int i = 0; sqlQue.next(); i++) {

            tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("booking_id").toString()));
            tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("seat_id").toString()));
            tableWidget->setItem(i, 2, new QTableWidgetItem(sqlQue.value("floor").toString()));
            tableWidget->setItem(i, 3, new QTableWidgetItem(sqlQue.value("area").toString()));
            tableWidget->setItem(i, 4, new QTableWidgetItem(sqlQue.value("start_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            tableWidget->setItem(i, 5, new QTableWidgetItem(sqlQue.value("end_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));

            QDateTime endTime = sqlQue.value("end_time").toDateTime();

            if (endTime > QDateTime::currentDateTime()) {

                QPushButton* cancelButton = new QPushButton("取消预定", this);
                pushButtonVector.push_back(cancelButton);
                tableWidget->setCellWidget(i, 6, cancelButton);
                connect(cancelButton, &QPushButton::clicked, cancelBookingSignalMapper, qOverload<>(&QSignalMapper::map));
                cancelBookingSignalMapper->setMapping(cancelButton, sqlQue.value("seat_id").toString());

            } else {
                tableWidget->setItem(i, 6, new QTableWidgetItem("已过期"));
                tableWidget->item(i, 6)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }

    tableWidget->resizeColumnsToContents();

    connect(cancelBookingSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::cancelBooking);
    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    this->setCentralWidget(tableWidget);
}

void MainWindow::bookSeat(const QString& text) {
    bool ok = false;
    int bookDuration = QInputDialog::getInt(this, "请输入预定时长", "时长(小时,至多8小时):", 1, 0, 8, 1, &ok);

    QDateTime nowTime = QDateTime::currentDateTime();
    QDateTime endTime = nowTime.addSecs(bookDuration * 3600);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        if (ok) {
            QSqlQuery que;
            que.exec(QString("insert into booking values(null,'%1',%2,'%3','%4');").arg(this->userName, text, nowTime.toString("yyyy-MM-dd hh:mm:ss"), endTime.toString("yyyy-MM-dd hh:mm:ss")));
            bookView();
        }
    }
    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    bookView();
}

void MainWindow::cancelBooking(const QString& text) {
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
        que.exec(QString("delete from booking where seat_id=%1 and end_time > now();").arg(text));
    }

    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    checkMyBookingView();
}

void MainWindow::bookingRecordView() {
    QVector<QPushButton*> pushButtonVector;
    QTableWidget* tableWidget = new QTableWidget(50, 8, this);

    QStringList tableHeaders { "预定记录编号", "用户ID", "座位ID", "座位楼层", "座位区域", "开始时间", "结束时间", "是否已过期" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        QSqlQuery sqlQue;

        sqlQue.exec("select * from booking left join libraryseat.seat as s on booking.seat_id = s.seat_id;");

        for (int i = 0; sqlQue.next(); i++) {

            tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("booking_id").toString()));
            tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("user_id").toString()));
            tableWidget->setItem(i, 2, new QTableWidgetItem(sqlQue.value("booking.seat_id").toString()));
            tableWidget->setItem(i, 3, new QTableWidgetItem(sqlQue.value("floor").toString()));
            tableWidget->setItem(i, 4, new QTableWidgetItem(sqlQue.value("area").toString()));
            tableWidget->setItem(i, 5, new QTableWidgetItem(sqlQue.value("start_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            tableWidget->setItem(i, 6, new QTableWidgetItem(sqlQue.value("end_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));

            QDateTime endTime = sqlQue.value("end_time").toDateTime();

            if (endTime > QDateTime::currentDateTime()) {

                QPushButton* cancelButton = new QPushButton("取消预定", this);
                pushButtonVector.push_back(cancelButton);
                tableWidget->setCellWidget(i, 7, cancelButton);
                connect(cancelButton, &QPushButton::clicked, adminCancelBookingSignalMapper, qOverload<>(&QSignalMapper::map));
                adminCancelBookingSignalMapper->setMapping(cancelButton, sqlQue.value("seat_id").toString());

            } else {
                tableWidget->setItem(i, 7, new QTableWidgetItem("已过期"));
                tableWidget->item(i, 7)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }
    connect(adminCancelBookingSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::adminCancelBooking);
    tableWidget->resizeColumnsToContents();
    this->setCentralWidget(tableWidget);

    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void MainWindow::adminCancelBooking(const QString& text) {
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
        que.exec(QString("delete from booking where seat_id=%1 and end_time > now();").arg(text));
    }

    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    bookingRecordView();
}

void MainWindow::manageSeatView() {
    QVector<QPushButton*> bookButtons;
    QVector<QPushButton*> deleteButtons;
    QVector<QPushButton*> modifyButtons;

    QTableWidget* tableWidget = new QTableWidget(50, 8, this);
    QStringList tableHeaders { "座位ID", "楼层", "区域", "是否可用", "预约", "删除座位", "修改座位信息", "增加座位" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        QSqlQuery sqlQue;

        sqlQue.exec("SELECT * FROM seat;");
        for (int i = 0; sqlQue.next(); i++) {
            tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("seat_id").toString()));
            tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("floor").toString()));
            tableWidget->setItem(i, 2, new QTableWidgetItem(sqlQue.value("area").toString()));

            if (sqlQue.value("availability").toInt() == 1) {
                tableWidget->setItem(i, 3, new QTableWidgetItem("可用"));
                tableWidget->item(i, 3)->setBackground(QColor(Qt::green));

                QPushButton* bookButton = new QPushButton("book", this);
                bookButtons.push_back(bookButton);
                tableWidget->setCellWidget(i, 4, bookButton);
                connect(bookButton, &QPushButton::clicked, bookSignalMapper, qOverload<>(&QSignalMapper::map));
                bookSignalMapper->setMapping(bookButton, sqlQue.value("seat_id").toString());

            } else {
                tableWidget->setItem(i, 3, new QTableWidgetItem("不可用"));
                tableWidget->item(i, 3)->setBackground(QColor(Qt::red));
            }

            QPushButton* deleteButton = new QPushButton("删除座位", this);
            deleteButtons.push_back(deleteButton);
            tableWidget->setCellWidget(i, 5, deleteButton);
            connect(deleteButton, &QPushButton::clicked, deleteSeatSignalMapper, qOverload<>(&QSignalMapper::map));
            deleteSeatSignalMapper->setMapping(deleteButton, sqlQue.value("seat_id").toString());

            QPushButton* modifyButton = new QPushButton("修改信息", this);
            modifyButtons.push_back(modifyButton);
            tableWidget->setCellWidget(i, 6, modifyButton);
            connect(modifyButton, &QPushButton::clicked, modifySeatSignalMapper, qOverload<>(&QSignalMapper::map));
            modifySeatSignalMapper->setMapping(modifyButton, sqlQue.value("seat_id").toString());
        }
        QPushButton* addSeatButton = new QPushButton("增加座位", this);
        tableWidget->setCellWidget(sqlQue.size(), 7, addSeatButton);
        connect(addSeatButton, &QPushButton::clicked, this, &MainWindow::addSeat);
    }

    connect(modifySeatSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::modifySeat);
    connect(deleteSeatSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::deleteSeat);

    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    tableWidget->resizeColumnsToContents();
    this->setCentralWidget(tableWidget);
}

void MainWindow::adminBookSeat(const QString& text) {
    bool ok = false;
    int bookDuration = QInputDialog::getInt(this, "请输入预定时长", "时长(小时,至多8小时):", 1, 0, 8, 1, &ok);

    QDateTime nowTime = QDateTime::currentDateTime();
    QDateTime endTime = nowTime.addSecs(bookDuration * 3600);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        if (ok) {
            QSqlQuery que;
            que.exec(QString("insert into booking values(null,'%1',%2,'%3','%4');").arg(this->userName, text, nowTime.toString("yyyy-MM-dd hh:mm:ss"), endTime.toString("yyyy-MM-dd hh:mm:ss")));
            bookView();
        }
    }
    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    manageSeatView();
}

void MainWindow::addSeat() {
    QDialog dialog;
    dialog.setWindowTitle("增加座位");

    QFormLayout layout(&dialog);

    QLineEdit* lineEdit1 = new QLineEdit(&dialog);
    QLineEdit* lineEdit2 = new QLineEdit(&dialog);

    layout.addRow("楼层:", lineEdit1);
    layout.addRow("区域:", lineEdit2);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString text1 = lineEdit1->text();
        QString text2 = lineEdit2->text();

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
            que.exec(QString("insert into seat values(null,'%1','%2',1);").arg(text1, text2));
        }
        mysql.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        manageSeatView();
    }
}

void MainWindow::deleteSeat(const QString& text) {
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
        que.exec(QString("delete from seat where seat_id=%1;").arg(text));
    }
    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    manageSeatView();
}

void MainWindow::modifySeat(const QString& text) {
    QDialog dialog;
    dialog.setWindowTitle("修改座位信息");

    QFormLayout layout(&dialog);

    QLineEdit* lineEdit1 = new QLineEdit(&dialog);
    QLineEdit* lineEdit2 = new QLineEdit(&dialog);

    layout.addRow("楼层:", lineEdit1);
    layout.addRow("区域:", lineEdit2);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString text1 = lineEdit1->text();
        QString text2 = lineEdit2->text();

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
            que.exec(QString("update seat set floor='%2',area='%3' where seat_id=%1;").arg(text, text1, text2));
        }
        mysql.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        manageSeatView();
    }
}

void MainWindow::manageUsersView() {
    QVector<QPushButton*> deleteButtons;
    QVector<QPushButton*> modifyButtons;

    QTableWidget* tableWidget = new QTableWidget(50, 5, this);
    QStringList tableHeaders { "用户ID", "密码", "删除用户", "修改信息", "增加用户" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost");
    mysql.setDatabaseName("libraryseat");
    mysql.setUserName("root");
    mysql.setPassword("456949");
    mysql.setPort(3306);

    if (!mysql.open()) {
        qDebug() << ("Alert " + mysql.lastError().text());
    } else {
        QSqlQuery sqlQue;

        sqlQue.exec("SELECT * FROM user;");
        for (int i = 0; sqlQue.next(); i++) {
            tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("user_id").toString()));
            tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("password").toString()));

            QPushButton* deleteButton = new QPushButton("删除", this);
            deleteButtons.push_back(deleteButton);
            tableWidget->setCellWidget(i, 2, deleteButton);
            connect(deleteButton, &QPushButton::clicked, deleteUserSignalMapper, qOverload<>(&QSignalMapper::map));
            deleteUserSignalMapper->setMapping(deleteButton, sqlQue.value("user_id").toString());

            QPushButton* modifyButton = new QPushButton("修改密码", this);
            modifyButtons.push_back(modifyButton);
            tableWidget->setCellWidget(i, 3, modifyButton);
            connect(modifyButton, &QPushButton::clicked, modifyUserSignalMapper, qOverload<>(&QSignalMapper::map));
            modifyUserSignalMapper->setMapping(modifyButton, sqlQue.value("user_id").toString());
        }
        QPushButton* addSeatButton = new QPushButton("增加", this);
        tableWidget->setCellWidget(sqlQue.size(), 4, addSeatButton);
        connect(addSeatButton, &QPushButton::clicked, this, &MainWindow::addUser);
    }

    connect(deleteUserSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::deleteUser);
    connect(modifyUserSignalMapper, &QSignalMapper::mappedString, this, &MainWindow::modifyUser);

    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    tableWidget->resizeColumnsToContents();
    this->setCentralWidget(tableWidget);
}

void MainWindow::addUser() {
    QDialog dialog;
    dialog.setWindowTitle("增加用户");

    QFormLayout layout(&dialog);

    QLineEdit* lineEdit1 = new QLineEdit(&dialog);
    QLineEdit* lineEdit2 = new QLineEdit(&dialog);

    layout.addRow("用户名:", lineEdit1);
    layout.addRow("密码:", lineEdit2);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString text1 = lineEdit1->text();
        QString text2 = lineEdit2->text();

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
            que.exec(QString("insert into user values('%1','%2');").arg(text1, text2));
        }
        mysql.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        manageUsersView();
    }
}

void MainWindow::deleteUser(const QString& text) {
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
        qDebug() << QString("delete from user where user_id=%1;").arg(text);
        que.exec(QString("delete from user where user_id='%1';").arg(text));
    }
    mysql.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    manageUsersView();
}

void MainWindow::modifyUser(const QString& text) {
    QDialog dialog;
    dialog.setWindowTitle("修改密码");

    QFormLayout layout(&dialog);

    QLineEdit* lineEdit1 = new QLineEdit(&dialog);

    layout.addRow("新密码:", lineEdit1);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString text1 = lineEdit1->text();

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
            qDebug()<<QString("update user set password='%2' where user_id='%1';").arg(text, text1);
            que.exec(QString("update user set password='%2' where user_id='%1';").arg(text, text1));
        }
        mysql.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        manageUsersView();
    }
}

MainWindow::~MainWindow() {
}
