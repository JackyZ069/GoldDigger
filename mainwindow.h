#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stockinfogetterbase.h"
#include "mysqlstoragebase.h"

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

    void on_pushButtonConfrim_clicked();

    void on_connectDataBaseBtn_clicked();

private:
    Ui::MainWindow *ui;
    mysqlStorage *mysql;
};
#endif // MAINWINDOW_H
