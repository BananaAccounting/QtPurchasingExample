#include "pch.h"
#include "shopmanager.h"

ShopManager::ShopManager(QWindow * mainWindow, QObject * parent) :QObject(parent)
{
	auto factory = winrt::get_activation_factory<winrt::Windows::Services::Store::StoreContext, winrt::Windows::Services::Store::IStoreContextStatics>();
	winrt::Windows::Services::Store::StoreContext context = factory.GetDefault();
	winrt::com_ptr<IInitializeWithWindow> initWindow;
	winrt::Windows::Foundation::IUnknown * unknown = reinterpret_cast<winrt::Windows::Foundation::IUnknown*>(&context);
	unknown->as(initWindow);
	initWindow->Initialize((HWND)mainWindow);

	m_myStore = new QInAppStore(this);
	setupConnections();
}

QString ShopManager::productIdentifier(Products product)
{
	switch (product)
	{
	case banana_once_product:
		return  QStringLiteral("banana_once_product");
	case banana_subscription:
		return  QStringLiteral("banana_subscription");
	case banana_product:
		return  QStringLiteral("9NDW9G6P5G6X");

	}
}

void ShopManager::setupConnections()
{
   connect(m_myStore, &QInAppStore::productRegistered, this, &ShopManager::handleCorrectProduct);
   connect(m_myStore, &QInAppStore::productUnknown, this, &ShopManager::handleUnknownProduct);
   connect(m_myStore, &QInAppStore::transactionReady, this, &ShopManager::handleTransactions);
}

void ShopManager::doPurchase(Products product)
{
   QInAppProduct *inAppProduct = m_myStore->registeredProduct(productIdentifier(product));   
   MyLogger::instance().writeLog("Purchasing: "+inAppProduct->title());
   inAppProduct->purchase();
}

void ShopManager::checkSubscription()
{
	//m_myStore->getSubscriptionInfo();
}

void ShopManager::checkDurable()
{
	//m_myStore->getDurableInfo();
}

void ShopManager::initShop()
{
	m_myStore->registerProduct(QInAppProduct::Unlockable, productIdentifier(Products::banana_once_product));
	m_myStore->registerProduct(QInAppProduct::Unlockable, productIdentifier(Products::banana_subscription));
	m_myStore->registerProduct(QInAppProduct::Unlockable, productIdentifier(Products::banana_product));

	m_myStore->restorePurchases();

	/*m_myStore->checkIsTrial();
	m_myStore->getAppInfo();
	m_myStore->getAddonsInfo();
	m_myStore->getCollectionInfo();*/
}

bool ShopManager::event(QEvent* e)
{
	return QObject::event(e);
}


void ShopManager::restorePurchases()
{
	MyLogger::instance().writeLog("Restoring purchases");
}

void ShopManager::handleErrorGracefully(QInAppProduct* p)
{
   emit error("Error regarding " + p->identifier());
}

void ShopManager::handleStringResponse(const QString& result) {
	MyLogger::instance().writeLog(result);
}

void ShopManager::handleCorrectProduct(QInAppProduct *)
{
}

void ShopManager::handleUnknownProduct(QInAppProduct::ProductType, const QString &)
{
}


void ShopManager::handleTransactions(QInAppTransaction* transaction)
{
	switch (transaction->status()) {
		case QInAppTransaction::PurchaseFailed:	
			break;
		case QInAppTransaction::PurchaseApproved:
		case QInAppTransaction::PurchaseRestored:
			if (transaction->product()->identifier() == productIdentifier(Products::banana_once_product))
				emit isDurableActive(true);
			else if (transaction->product()->identifier() == productIdentifier(Products::banana_subscription)) {
				emit isSubscriptionActive(true);
				MyLogger::instance().writeLog("Subscription expiration date: "+transaction->platformProperty("expiration"));
			}
				
			else if (transaction->product()->identifier() == productIdentifier(Products::banana_product))
				emit isActive(true);
			break;
		default:
			break;
	}
}