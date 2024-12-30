#ifndef WORK_THREAD_BASE_H
#define WORK_THREAD_BASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include "mysqlstoragebase.h"

class work_thread_base : public QThread
{
public:
    work_thread_base();
    ~work_thread_base();
};

class getStockWorkThread : public work_thread_base
{
    Q_OBJECT
public:
    getStockWorkThread(QString stockNum, const QString &address, const int &port, const QString &dbName, const QString &dbUser, const QString &dbPassWord);
    ~getStockWorkThread();

protected:
    void run() override;

private:
    mysqlStorage *m_mysql;
    QString m_stockNum;
    bool m_stop;

signals:
    //每完成一次发射一次信号
    void workDoneOneTime();
};

#endif // WORK_THREAD_BASE_H
