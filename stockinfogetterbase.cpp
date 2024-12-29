#include "stockinfogetterbase.h"



StockInfoGetterBase::StockInfoGetterBase()
{
    m_stockCode = 1;
}

StockInfoGetter::StockInfoGetter()
{
    m_netWorkManager = new QNetworkAccessManager();
}

int StockInfoGetter::HttpGetRequest(QString api)
{
    int iReturn = -1;
    if(api.isEmpty())
        return iReturn;
    QNetworkRequest request((QUrl(api)));

    m_netWorkManager->get(request);
    iReturn = 0;
    return iReturn;
}
