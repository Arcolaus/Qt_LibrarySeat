#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    this->resize(800, 600);
    setToolBar();
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
    QTableWidget* tableWidget = new QTableWidget(50, 5, this);
    QStringList tableHeaders { "座位ID", "楼层", "区域", "是否可用", "预约" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPushButton* btn = new QPushButton("book", this);
    tableWidget->setCellWidget(0, 4, btn);

    this->setCentralWidget(tableWidget);
}

void MainWindow::bookingRecordView() {
    QTableWidget* tableWidget = new QTableWidget(50, 7, this);
    QStringList tableHeaders { "预定记录编号", "用户ID", "座位ID", "座位楼层", "座位区域", "开始时间", "结束时间" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setCentralWidget(tableWidget);
}

void MainWindow::manageSeatView(){
    QTableWidget* tableWidget = new QTableWidget(50, 4, this);
    QStringList tableHeaders { "座位ID", "楼层", "区域", "是否可用" };
    tableWidget->setHorizontalHeaderLabels(tableHeaders);

    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setCentralWidget(tableWidget);
}

MainWindow::~MainWindow() { }
