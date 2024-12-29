#ifndef MYSQLSTORAGEBASE_H
#define MYSQLSTORAGEBASE_H
#include <QtSql>
#include <QDataWidgetMapper>
#include <QSqlTableModel>


class mysqlStorageBase
{
public:
    mysqlStorageBase();
};



class mysqlStorage : public virtual mysqlStorageBase
{
public:
    mysqlStorage();
    ~mysqlStorage();

public:
    //连接数据库
    bool connectDatabase(const QString &address = "localhost", const int &port = 3306,const QString &dbName = "stockDB",
                         const QString &dbUser = "root", const QString &dbPassWord = "mp112233");
    //打开表格
    bool openTable(const QString& tableName = "stock_base_table");
    //打开股票基础表
    bool openStockBaseTable();
    //创建股票基础表
    bool creatStockBaseTable();
    //插入一条记录
    bool InsertStockBaseTable(QStringList &dataList);
    //判断表是否打开
    bool isTableVaild();

private:
    //数据库
    QSqlDatabase DB;
    //表格Model
    QSqlTableModel *tabModel;
    //记录项Model
    QItemSelectionModel *selectModel;
    QDataWidgetMapper *dataMapper;


};

#endif // MYSQLSTORAGEBASE_H

