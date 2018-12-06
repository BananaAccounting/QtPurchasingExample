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
	enum mType { getAppInfo, getAddons, getAddonsPurchasable, getUserCollection, checkIsTrial, buySubscription };
	void setOperation(mType operation) { m_operation = operation; }
	void run() override;
	void setContext(const winrt::Windows::Services::Store::StoreContext&);
	
private:
	winrt::Windows::Services::Store::StoreContext context = nullptr;

	winrt::hstring subscriptionID = L"9P2QZFC6NH0M";
	winrt::hstring durableID = L"9P27DZCTDFDR";

	void getStoreContext();
	mType m_operation = getAppInfo;
	void getAppInfoAsync();
	void getAddonsAsync();
	void getUserCollectionAsync();
	void checkIsTrialAsync();
	void buySubscriptionAsync();
signals:
	void appInfo(const QString&);
	void appAddons(const QString&);
	void appInfoUserCollection(const QString&);
	void isTrial(bool);
	void isActive(bool);
	void isDurablePurchased(bool);
	void isSubscriptionActive(bool);
	void subInfo(const QString&);
	void subscriptionBought(bool);
public slots:

};
#endif