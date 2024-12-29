#include "mysqlstoragebase.h"
#include <QSqlQuery>
#include <QDateTime>

mysqlStorageBase::mysqlStorageBase() {}

mysqlStorage::mysqlStorage()
{

}

mysqlStorage::~mysqlStorage()
{
    DB.close();
    delete tabModel;
    delete selectModel;
    delete dataMapper;

}

bool mysqlStorage::connectDatabase(const QString &address, const int &port, const QString &dbName, const QString &dbUser, const QString &dbPassWord)
{
    //add a database driver
    DB = QSqlDatabase::addDatabase("QMYSQL");
    //set database info
    DB.setHostName(address); // 对于远程数据库，设置主机名或IP
    DB.setPort(port);
    DB.setDatabaseName(dbName); // 对于其他数据库则是数据库名，如果数据库不存在，应该会新建一个数据库
    DB.setUserName(dbUser); // 数据库的用户名
    DB.setPassword(dbPassWord); // 数据库的密码

    //open db
    if(!DB.open())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool mysqlStorage::openTable(const QString &tableName)
{
    if(tableName == "stock_base_table")
    {
        return  openStockBaseTable();
    }
    else
        return false;
}

bool mysqlStorage::openStockBaseTable()
{
    //open and connect the database's table;
    tabModel = new QSqlTableModel(nullptr,DB);

    //set table name
    tabModel->setTable("stock_base_table");
    //set submit strategy
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //set the sort order
    tabModel->setSort(tabModel->fieldIndex("empNo"),Qt::AscendingOrder);

    if(!tabModel->select())
        return false;

    return true;
    // showRecordCounts();
    // connect(selectModel,&QItemSelectionModel::currentChanged,this,&MainWindow::do_currentChanged);
    // connect(selectModel,&QItemSelectionModel::currentRowChanged,this,&MainWindow::do_currentRowChanged);
}

bool mysqlStorage::creatStockBaseTable()
{
    QSqlQuery query(DB);
    query.exec("CREATE TABLE  IF NOT EXISTS stock_base_table("
               "stockCode INT PRIMARY KEY,"
               "stockName VARCHAR(50) NOT NULL,"
               "stockCurrPrice DOUBLE NOT NULL,"
               "currentTime DATETIME NOT NULL,"
               "riseFallPrice DOUBLE NOT NULL,"
               "riseFallPercent DOUBLE NOT NULL,"
               "stockClosingPrice DOUBLE NOT NULL,"
               "stockOpeningPrice DOUBLE NOT NULL,"
               "currTopPrice DOUBLE NOT NULL,"
               "currBottomPrice DOUBLE NOT NULL,"
               "maybeTopPrice DOUBLE NOT NULL,"
               "maybeBottomPrice DOUBLE NOT NULL"
               ");");

    if (!query.isActive()) {
        qDebug() << "Error: Failed to create table." << query.lastError();
        return false;
    } else {
        qDebug() << "Table created!";
        return true;
    }
}

bool mysqlStorage::InsertStockBaseTable(QStringList &dataList)
{
    //插入记录说明，从tx股票上获取的信息的分布如下：
    // 0	51	     1:沪A，51:深A
    //               1	永利股份	股票名称
    //               2	300230	    股票代码
    //               3	5.7	        当前价格
    //               4	5.52	    昨收
    //               5	5.49	    今开
    //               6	132876	    成交量（手）
    //               7	73161	    外盘
    //               8	59712	    内盘
    //               9	5.68	    买一
    //               10	911	        买一量（手）
    //               11	5.67	    买二
    //               12	1420	    买二量（手）
    //               13	5.66	    买三
    //               14	223	        买三量（手）
    //               15	5.65	    买四
    //               16	1121	    买四量（手）
    //               17	5.64	    买五
    //               18	653	        买五量（手）
    //               19	5.7	        卖一
    //               20	806	        卖一量（手）
    //               21	5.71	    卖二
    //               22	687	        卖二量（手）
    //               23	5.72	    卖三
    //               24	1042	    卖三量（手）
    //               25	5.73	    卖四
    //               26	954	        卖四量（手）
    //               27	5.74	    卖五
    //               28	1460	    卖五量（手）
    //               29	11:29:52/5.70/60/B/34146/4919|11:29:49/5.69/6/S/3414/4917	最近逐笔成交
    //               30	20181114113012	时间
    //               31	0.18	    涨跌
    //               32	3.26	    涨幅%
    //               33	5.73	    最高价
    //               34	5.46	    最低价
    //               35	5.70/132876/74586032	价格/成交量（手）/成交额
    //               36	132876	    成交量（手）
    //               37	7459	    成交额（万）
    //               38	2.46	    换手率%
    //               39	10.89	    市盈率(TTM)
    //               40
    //               41	5.73	    最高价
    //               42	5.46	    最低价
    //               43	4.89	    振幅%
    //               44	30.78	    流通市值(亿)
    //               45	46.52	    总市值
    //               46	1.52	    市净率
    //               47	6.07	    涨停价
    //               48	4.97	    跌停价
    //               49	2.41	    量比
    //               50	-621	    委差
    //               51	5.61	    未知
    //               52	9.88	    市盈率(动)
    //               53	15.93	    市盈率(静)

    if(dataList.size()<50)
        return false;

    //create a query
    QSqlQuery query;
    //此处使用占位符的作用是：使用占位符和bindValue方法是一种更安全、更清晰和更易于维护的方式来执行SQL插入操作。这种方式可以帮助你避免SQL注入攻击，提高代码的可读性和可维护性，并且在某些情况下还可以提高性能。
    query.prepare("INSERT INTO stock_base_table (stockCode, stockName, stockCurrPrice, currentTime, riseFallPrice, riseFallPercent, "
                    "stockClosingPrice, stockOpeningPrice, currTopPrice, currBottomPrice, maybeTopPrice, maybeBottomPrice)"
                    "VALUES (:stockCode, :stockName, :stockCurrPrice, :currentTime, :riseFallPrice, :riseFallPercent, "
                  ":stockClosingPrice, :stockOpeningPrice, :currTopPrice, :currBottomPrice, :maybeTopPrice, :maybeBottomPrice)");
    //绑定值到占位符
    query.bindValue(":stockCode", dataList[2].toInt());
    query.bindValue(":stockName", dataList[1]);
    query.bindValue(":stockCurrPrice",dataList[3].toDouble());
    //时间戳转化
    qint64 timestamp = dataList[30].toLongLong();
    // 将时间戳转换为QDateTime对象
    QDateTime dt = QDateTime::fromString(QString::number(timestamp), "yyyyMMddHHmmss");
    // 将QDateTime对象格式化为MySQL的DATETIME格式
    QString mysqlDatetime = dt.toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":currentTime",mysqlDatetime);
    double a = dataList[31].toDouble();
    query.bindValue(":riseFallPrice",dataList[31].toDouble());
    double a1 = dataList[32].toDouble();
    query.bindValue(":riseFallPercent",dataList[32].toDouble());
    query.bindValue(":stockClosingPrice", dataList[4].toDouble());
    query.bindValue(":stockOpeningPrice",dataList[5].toDouble());
    query.bindValue(":currTopPrice",dataList[41].toDouble());
    query.bindValue(":currBottomPrice",dataList[42].toDouble());
    query.bindValue(":maybeTopPrice",dataList[47].toDouble());
    query.bindValue(":maybeBottomPrice",dataList[48].toDouble());

    //INSERT
    if(!query.exec())
    {
        qDebug() << "Insert Fail!" << query.lastError().text();
    }
    else
    {
        qDebug() << "Insert Success!" << query.lastInsertId();
    }

    return true;
}

bool mysqlStorage::isTableVaild()
{
    if(tabModel)
        return true;
    else
        return false;
}


