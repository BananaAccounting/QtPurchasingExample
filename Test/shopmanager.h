#ifndef SHOPMANAGER_H
#define SHOPMANAGER_H

#include <QObject>
#include <QString>
#include <QStringLiteral>
#include <QWindow>

#if !defined Q_OS_WIN
#include <QtPurchasing>
#else
//#include "microsoftshop.h"
#include "qinappstore.h"
#include "qinapptransaction.h"
#include "qinappproduct.h"
#endif
#include "mylogger.h"

class ShopManager : public QObject
{
    Q_OBJECT

public:
    enum Products { banana_product, banana_once_product, banana_subscription, banana_consumable};
    explicit ShopManager(QWindow* mainWindow, QObject *parent = nullptr);

    void restorePurchases();
	void doPurchase(Products product); 
	void useProduct(Products product, int quantity);
	void initShop();
	bool event(QEvent *e) override;
private:
	int myConsumables = 0;
	QInAppStore *m_myStore;
	void setupConnections();
	QString productIdentifier(Products);
signals:
    void productPurchased(Products product);
    void error(const QString& errorMessage);
	void isTrial(bool);
	void isActive(bool);
	void isDurableActive(bool);
	void isSubscriptionActive(bool);
	void updateConsumable(const QString&);
    
public slots:
    void handleErrorGracefully(QInAppProduct*);
	void handleStringResponse(const QString&);

	void handleCorrectProduct(QInAppProduct*);
	void handleUnknownProduct(QInAppProduct::ProductType, const QString &);
	void handleTransactions(QInAppTransaction*);
};

#endif // SHOPMANAGER_H
