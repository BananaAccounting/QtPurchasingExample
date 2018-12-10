#ifndef ASYNCSTORE_H
#define ASYNCSTORE_H
#include "pch.h"
#include <QRunnable>
#include <QThreadPool>
#include <QObject>
#include <QWindow>

class AsyncStore : public QObject, public QRunnable
{
	Q_OBJECT

public:
	AsyncStore(QWindow* mainWindow, QObject *parent = nullptr);
	AsyncStore();
	enum mType { getAppInfo, getAddons, getAddonsPurchasable, getUserCollection, checkIsTrial, checkSubscription, checkDurable, 
		buySubscription, buyDurable, buyProduct};
	void setOperation(mType operation) { m_operation = operation; }
	void run() override;
	void setContext(const winrt::Windows::Services::Store::StoreContext&);
	
private:
	winrt::Windows::Services::Store::StoreContext context = nullptr;

	winrt::hstring productID = L"9NDW9G6P5G6X";
	winrt::hstring subscriptionID = L"9P2QZFC6NH0M";
	winrt::hstring durableID = L"9P27DZCTDFDR";
	winrt::hstring consumableID = L"9NJBKP5835TX";

	void getStoreContext();
	mType m_operation = getAppInfo;
	void getAppInfoAsync();
	void getAddonsAsync();
	void getUserCollectionAsync();
	void checkIsTrialAsync();
	void checkSubscriptionAsync();
	void checkDurableAsync();
	void buyAddonAsync(winrt::hstring);
	void buyDurableAsync();
	void buyProductAsync();
	void buySubscriptionAsync();
signals:
	void appInfo(const QString&);
	void isTrial(bool);
	void isActive(bool);
	void isDurableActive(bool);
	void isSubscriptionActive(bool);
	void subInfo(const QString&);
	void productBought(bool);
public slots:

};
#endif