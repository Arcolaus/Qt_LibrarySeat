#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    this->resize(1600, 900);

    bookSignalMapper = new QSignalMapper(this);
    cancelBookingSignalMapper=new QSignalMapper(this);

    initDB();
    setToolBar();
}

void MainWindow::initDB() {
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
                connect(cancelButton,&QPushButton::clicked,cancelBookingSignalMapper,qOverload<>(&QSignalMapper::map));
                cancelBookingSignalMapper->setMapping(cancelButton,sqlQue.value("seat_id").toString());

            } else {
                tableWidget->setItem(i, 6, new QTableWidgetItem("已过期"));
                tableWidget->item(i, 6)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }

    tableWidget->resizeColumnsToContents();

    connect(cancelBookingSignalMapper,&QSignalMapper::mappedString,this,&MainWindow::cancelBooking);
    mysql.close();
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
        }
    }

    mysql.close();
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
}

void MainWindow::bookingRecordView() {
    QTableWidget* tableWidget = new QTableWidget(50, 7, this);

    QStringList tableHeaders { "预定记录编号", "用户ID", "座位ID", "座位楼层", "座位区域", "开始时间", "结束时间" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setCentralWidget(tableWidget);
}

void MainWindow::manageSeatView() {
    QTableWidget* tableWidget = new QTableWidget(50, 4, this);
    QStringList tableHeaders { "座位ID", "楼层", "区域", "是否可用" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setCentralWidget(tableWidget);
}

MainWindow::~MainWindow() {
    // mysql.close();
}
