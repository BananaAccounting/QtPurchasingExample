#ifndef SHOPMANAGER_H
#define SHOPMANAGER_H

#include <QObject>
#include <QString>
#include <QStringLiteral>

#if !defined Q_OS_WIN
#include <QtPurchasing>
#else
#include "microsoftshop.h"
#endif

class ShopManager : public QObject
{
    Q_OBJECT

    QInAppStore *m_myStore;
    void setupConnections();

public:
    enum Products { banana_product, banana_once_product, banana_subscription };
    explicit ShopManager(QWindow* mainWindow, QObject *parent = nullptr);

    void restorePurchases();
    void doPurchase(Products product);
    void checkIsTrial();

signals:
    void productPurchased(Products product);
    void error(QString errorMessage);
    void isTrial(bool);

    
public slots:
    void handleErrorGracefully(QInAppProduct*);
    void handleTransaction(QInAppTransaction*);
};

#endif // SHOPMANAGER_H
