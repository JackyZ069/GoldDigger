#ifndef STOCKINFOGETTERBASE_H
#define STOCKINFOGETTERBASE_H
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class StockInfoGetterBase
{
public:
    StockInfoGetterBase();


private:
    int m_stockCode;
};

class StockInfoGetter : public virtual StockInfoGetterBase
{
public:
    StockInfoGetter();

public:
    int HttpGetRequest(QString api);
    QNetworkAccessManager* GetManager(){return m_netWorkManager;}
private:

    QNetworkAccessManager *m_netWorkManager;

private slots:


};



#endif // STOCKINFOGETTERBASE_H
