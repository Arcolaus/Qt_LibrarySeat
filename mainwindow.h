#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>
#include <dbsqlite.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    void setToolBar();

    void setAdminUI();
    void setStudentUI();

    ~MainWindow();

private:
    QToolBar* toolBar;

    QAction* stuBook;
    QAction* adminBookingRecord;
    QAction* adminManageSeat;

    DBsqlite dbsqlite;

public slots:
    void bookView();

    void bookingRecordView();

    void manageSeatView();
};
#endif // MAINWINDOW_H
