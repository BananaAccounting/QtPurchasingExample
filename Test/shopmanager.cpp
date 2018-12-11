#include "pch.h"
#include "shopmanager.h"

ShopManager::ShopManager(QWindow * mainWindow, QObject * parent) :QObject(parent)
{
	m_myStore = new QInAppStore(this);
	setupConnections();
}


void ShopManager::setupConnections()
{
   connect(m_myStore, &QInAppStore::productRegistered, this, &ShopManager::handleCorrectProduct);
   connect(m_myStore, &QInAppStore::productUnknown, this, &ShopManager::handleUnknownProduct);
   connect(m_myStore, &QInAppStore::transactionReady, this, &ShopManager::handleTransactions);
}



void ShopManager::doPurchase(Products product)
{
  /* QInAppProduct *inAppProduct;
   switch (product)
   {
   case banana_once_product:
      inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_once_product"));
	  connect(inAppProduct, &QInAppProduct::isProductBought, this, &ShopManager::isDurableActive);
	  break;
   case banana_subscription:
	   inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_subscription"));
	   connect(inAppProduct, &QInAppProduct::isProductBought, this, &ShopManager::isSubscriptionActive);
	   break;
   case banana_product:
	   inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_product"));
	   connect(inAppProduct, &QInAppProduct::isProductBought, this, &ShopManager::isActive);
	   break;
   case banana_consumable:
	   inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_consumable"));
	   
	   break;
   }
   
   MyLogger::instance().writeLog("Purchasing: "+inAppProduct->title());
   inAppProduct->purchase();*/
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
}