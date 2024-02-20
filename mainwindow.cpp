#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    this->resize(800, 600);

    signalMapper = new QSignalMapper(this);

    initDB();
    setToolBar();
}

void MainWindow::initDB() {
    sqlite = QSqlDatabase::addDatabase("QSQLITE");

    sqlite.setDatabaseName("E:\\LifeCodeProj\\240214_LibarySeat\\LibarySeat\\database\\libraryseat.db");

    if (this->sqlite.open()) {
        qDebug() << "Database connected";
    } else {
        qDebug() << "Connect failed" << this->sqlite.lastError();
        // QMessageBox msgBox;
        // msgBox.setText("数据库连接失败");
        // msgBox.setDetailedText(DB.lastError().text());
        // msgBox.exec();
    }
    sqlite.close();
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
    this->toolBar->addAction(this->stuBook);
    connect(this->stuBook, &QAction::triggered, this, &MainWindow::bookView);
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

    QSqlQuery sqlQue(this->sqlite);

    sqlite.open();
    sqlQue.exec("SELECT * FROM Seat;");
    for (int i = 0; sqlQue.next(); i++) {
        tableWidget->insertRow(i);
        tableWidget->setItem(i, 0, new QTableWidgetItem(sqlQue.value("seat_id").toString()));
        tableWidget->setItem(i, 1, new QTableWidgetItem(sqlQue.value("floor").toString()));
        tableWidget->setItem(i, 2, new QTableWidgetItem(sqlQue.value("area").toString()));

        if (sqlQue.value("availability").toInt() == 1) {
            qDebug()<<"Yes";

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
            qDebug()<<"no";
        }
    }

    connect(signalMapper, &QSignalMapper::mappedString, this, &MainWindow::bookSeat);
    this->setCentralWidget(tableWidget);
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
    sqlite.close();
    // delete this->sqlite;
}
