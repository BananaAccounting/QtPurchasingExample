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
#include "mylogger.h"

class ShopManager : public QObject
{
    Q_OBJECT

    QInAppStore *m_myStore;
    void setupConnections();

public:
    enum Products { banana_product, banana_durable, banana_subscription, banana_consumable};
    explicit ShopManager(QWindow* mainWindow, QObject *parent = nullptr);

    void restorePurchases();
	void doPurchase(Products product); 
	void checkSubscription();
	void checkDurable();
	void initShop();
	bool event(QEvent *e) override;

signals:
    void productPurchased(Products product);
    void error(const QString& errorMessage);
	void isTrial(bool);
	void isActive(bool);
	void isDurableActive(bool);
	void isSubscriptionActive(bool);
    
public slots:
    void handleErrorGracefully(QInAppProduct*);
    void handleTransaction(QInAppTransaction*);
	void handleStringResponse(const QString&);
};

#endif // SHOPMANAGER_H
