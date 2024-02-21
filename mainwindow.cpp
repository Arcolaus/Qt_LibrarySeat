#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    this->resize(800, 600);

    signalMapper = new QSignalMapper(this);

    initDB();
    setToolBar();
}

void MainWindow::initDB() {
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

    QTableWidget* tableWidget = new QTableWidget(0, 5, this);
    QStringList tableHeaders { "座位ID", "楼层", "区域", "是否可用", "预约" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPushButton* btn = new QPushButton("book", this);
    tableWidget->setCellWidget(0, 4, btn);

    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");

    mysql.setHostName("localhost"); // 数据库地址，一般都是本地，填localhost就可以(或者填写127.0.0.1)
    mysql.setDatabaseName("libraryseat"); // 数据库名，根据你Mysql里面的数据库名称来填写，比如我的Mysql里面有个数据库叫school
    mysql.setUserName("root"); // 登录用户名，一般是本地用户，填root就可以
    mysql.setPassword("456949"); // 登录密码，填写你自己Mysql登陆密码
    mysql.setPort(3306); // 端口，默认是3306

    if (!mysql.open()) { // 数据库打开失败
        qDebug() << ("警报 " + mysql.lastError().text()); // 显示错误信息
    } else {
        qDebug() << ("数据库连接成功");
    }

    QSqlQuery sqlQue;

    sqlQue.exec("SELECT * FROM Seat;");
    for (int i = 0; sqlQue.next(); i++) {
        tableWidget->insertRow(i);
        tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("seat_id").toString()));
        tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("floor").toString()));
        tableWidget->setItem(i, 2, new QTableWidgetItem(sqlQue.value("area").toString()));

        if (sqlQue.value("availability").toInt() == 1) {
            qDebug() << "Yes";

            tableWidget->setItem(i, 3, new QTableWidgetItem("可用"));
            tableWidget->item(i, 3)->setBackground(QColor(Qt::green));
            QPushButton* newBtn = new QPushButton("book", this);

            pushButtonVector.push_back(newBtn);
            tableWidget->setCellWidget(i, 4, pushButtonVector[i]);

            connect(newBtn, &QPushButton::clicked, signalMapper, qOverload<>(&QSignalMapper::map));
            signalMapper->setMapping(newBtn, sqlQue.value("seat_id").toString());
        } else {
            tableWidget->setItem(i, 3, new QTableWidgetItem("不可用"));
            tableWidget->item(i, 3)->setBackground(QColor(Qt::red));
            qDebug() << "no";
        }
    }

    mysql.close();

    connect(signalMapper, &QSignalMapper::mappedString, this, &MainWindow::bookSeat);
    this->setCentralWidget(tableWidget);
}

void MainWindow::checkMyBookingView() {
}

void MainWindow::bookSeat(const QString& text) {
    qDebug() << text;
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
