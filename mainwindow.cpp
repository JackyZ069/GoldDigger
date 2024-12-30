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
    m_mysql = new mysqlStorage();
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

void MainWindow::on_connectDataBaseBtn_clicked()
{
    //打开数据库
    bool b = m_mysql->connectDatabase(ui->databaseAddress->text(),ui->databasePort->text().toInt(),ui->databaseName->text(),ui->databaseUserName->text(),ui->databasePassword->text());
    if(!b)
    {
        QMessageBox::warning(this,"error","open database error,please check!");
        return;
    }
    ui->databaseStatu->setText("数据库连接成功！");
    //创建表格
    b = m_mysql->creatStockBaseTable();
    if(!b)
    {
        QMessageBox::warning(this,"error","creat table error,please check!");
        return;
    }
    //打开表格
    b = m_mysql->openStockBaseTable();
    if(!b)
    {
        QMessageBox::warning(this,"error","open table error,please check!");
        return;
    }
    //获取数据

}

void MainWindow::on_stockConnectBtn_clicked()
{
    QString stockNum = ui->stockCodeLineEdit->text();
    if(stockNum.isEmpty())
        return;

    ui->stockCancelBtn->setText("获取数据中...");

    m_StockThread = new getStockWorkThread(stockNum,ui->databaseAddress->text(),ui->databasePort->text().toInt(),ui->databaseName->text(),ui->databaseUserName->text(),ui->databasePassword->text());
    m_StockThread->start();

    connect(m_StockThread,&getStockWorkThread::workDoneOneTime,this,&MainWindow::stock_work_done);
}


void MainWindow::on_stockCancelBtn_clicked()
{

}

void MainWindow::stock_work_done()
{
    ui->databaseRecord->setText(QString("数据库记录条数：%1").arg(m_mysql->showRecords()));
    double currStockPrice = m_mysql->getDoubleDataFromDB("stockCurrPrice");
    QFont font = ui->StockCurrPriceLabel->font();
    font.setPointSize(20);
    ui->StockCurrPriceLabel->setFont(font);
    if(m_mysql->getDoubleDataFromDB("riseFallPrice")>0)
        ui->StockCurrPriceLabel->setStyleSheet("color:red;");
    else
        ui->StockCurrPriceLabel->setStyleSheet("color:green;");
    ui->StockCurrPriceLabel->setText(QString::number(currStockPrice));

}

