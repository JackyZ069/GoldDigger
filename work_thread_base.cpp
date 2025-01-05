#include "work_thread_base.h"
#include <QProcess>
#include <QTime>

work_thread_base::work_thread_base() {}

work_thread_base::~work_thread_base()
{

}

getStockWorkThread::getStockWorkThread(QString stocknum, const QString &address, const int &port, const QString &dbName, const QString &dbUser, const QString &dbPassWord)
{
    m_stockNum = stocknum;
    m_stop = false;
    m_mysql = new mysqlStorage();
    //打开数据库
    bool b = m_mysql->connectDatabase(address,port,dbName,dbUser,dbPassWord,"SecondThread");
    //打开表格
    b = m_mysql->openStockBaseTable();
    if(!b)
    {
        qDebug() << "open table thread error,please check!";
        return;
    }

}

getStockWorkThread::~getStockWorkThread()
{

}

void getStockWorkThread::run()
{
    if(m_mysql == nullptr)
    {
        qDebug()<<"mysql is null";
        return;
    }

    //设置定时器
    while(!m_stop)
    {
        ///调用 python脚本
        QProcess process;
        process.start("/Users/jackyz/PythonProject/.venv/bin/python", QStringList() << "/Users/jackyz/PythonProject/stockInfo.py" << m_stockNum);

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
        if(m_mysql->isTableVaild())
            m_mysql->InsertStockBaseTable(outputStrList);

        //发出信号后休眠
        emit workDoneOneTime();
        //股票数据基本上是3s更新一次
        msleep(1888);

        // 获取当前时间
        QTime currentTime = QTime::currentTime();

        // 直接比较当前时间和15点
        if (currentTime > QTime(15, 0, 0))
            m_stop = true;
    }
    return;
}
