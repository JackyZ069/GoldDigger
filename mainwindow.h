#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stockinfogetterbase.h"
#include "mysqlstoragebase.h"
#include "work_thread_base.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRequestFinished(QNetworkReply *reply);

    void on_connectDataBaseBtn_clicked();

    void on_stockConnectBtn_clicked();

    void on_stockCancelBtn_clicked();

    void stock_work_done();

    void on_stackDataBaseBtn_clicked();

    void on_stackStockBtn_clicked();

private:
    Ui::MainWindow *ui;
    mysqlStorage *m_mysql;
    getStockWorkThread *m_StockThread;
};
#endif // MAINWINDOW_H
