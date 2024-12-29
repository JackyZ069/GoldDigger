#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mysql = new mysqlStorage();
    ui->databaseAddress->setText("localhost");
    ui->databasePort->setText("3306");
    ui->databaseName->setText("stockDB");
    ui->databaseUserName->setText("root");
    ui->databasePassword->setText("mp112233");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRequestFinished(QNetworkReply *reply)
{

    if (reply->error()) {
        // 处理错误
        qDebug() << "Error:" << reply->errorString();
    } else {
        // 获取HTTP响应头
        QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();

        // 查找Content-Type字段
        for (const QNetworkReply::RawHeaderPair &header : headers) {
            if (header.first == "Content-Type") {
                qDebug() << "Content-Type:" << header.second;
                break;
            }
        }

        // 读取响应数据
        QByteArray data = reply->readAll();

        // 输出响应数据
        qDebug() << "响应数据:" << QString::fromUtf8(data.trimmed());
    }
    // 不要忘记删除QNetworkReply对象
    reply->deleteLater();
}


void MainWindow::on_pushButtonConfrim_clicked()
{
    if(ui->stockCodeLineEdit->text().isEmpty())
        return;
    else
    {
        ui->textEdit->clear();
        ///调用 python脚本
        QProcess process;
        // process.setEnvironment(QProcess::systemEnvironment());
        // process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
        // process.setProcessChannelMode(QProcess::MergedChannels);
        process.start("/Users/jackyz/PythonProject/.venv/bin/python", QStringList() << "/Users/jackyz/PythonProject/stockInfo.py" << ui->stockCodeLineEdit->text());

        if (!process.waitForFinished()) {
            qDebug() << "进程执行失败";
            return;
        }

        QByteArray output = process.readAllStandardOutput();
        //QByteArray errorOutput = process.readAllStandardError();
        //qDebug() << "Python脚本返回值:" <<QString::fromUtf8(output.trimmed());
        //qDebug() << "Python脚本错误信息:" << errorOutput.trimmed();

        //切割获得的数据；
        QStringList outputStrList = QString(output).split('~');
        int size = outputStrList.size();
        ui->textEdit->setText(QString::fromUtf8(output.trimmed()));
        if(mysql->isTableVaild())
            mysql->InsertStockBaseTable(outputStrList);
    }
}



void MainWindow::on_connectDataBaseBtn_clicked()
{
    //打开数据库
    bool b = mysql->connectDatabase(ui->databaseAddress->text(),ui->databasePort->text().toInt(),ui->databaseName->text(),ui->databaseUserName->text(),ui->databasePassword->text());
    if(!b)
    {
        QMessageBox::warning(this,"error","open database error,please check!");
        return;
    }
    ui->databaseStatu->setText("数据库连接成功！");
    //创建表格
    b = mysql->creatStockBaseTable();
    if(!b)
    {
        QMessageBox::warning(this,"error","creat table error,please check!");
        return;
    }
    //打开表格
    b = mysql->openStockBaseTable();
    if(!b)
    {
        QMessageBox::warning(this,"error","open table error,please check!");
        return;
    }
    //获取数据

}


