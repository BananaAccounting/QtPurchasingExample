#include "pch.h"
#include "shopmanager.h"
#include <QWindow>
#include <QThread>

ShopManager::ShopManager(QWindow* mainWindow, QObject *parent) : QObject(parent)
{
   m_myStore = new QInAppStore(mainWindow, this);
   //m_myStore->registerProduct(QInAppProduct::Unlockable, QStringLiteral("banana_product"));
   //m_myStore->registerProduct(QInAppProduct::Unlockable, QStringLiteral("banana_durable"));
   //m_myStore->registerProduct(QInAppProduct::Subscription, QStringLiteral("banana_subscription"));
   setupConnections();
}


void ShopManager::setupConnections()
{
   //connect(m_myStore, SIGNAL(productRegistered(QInAppProduct*)), this, SLOT(markProductAvailable(QInAppProduct*)));
   //connect(m_myStore, SIGNAL(productUnknown(QInAppProduct*)), this, SLOT(handleErrorGracefully(QInAppProduct*)));
   //connect(m_myStore, SIGNAL(transactionReady(QInAppTransaction*)), this, SLOT(handleTransaction(QInAppTransaction*)));

   connect(m_myStore, &QInAppStore::isTrial, this, &ShopManager::isTrial);
   connect(m_myStore, &QInAppStore::isActive, this, &ShopManager::isActive);
   connect(m_myStore, &QInAppStore::handleStringResponse, this, &ShopManager::handleStringResponse);
   connect(m_myStore, &QInAppStore::isDurableActive, this, &ShopManager::isDurableActive);
   connect(m_myStore, &QInAppStore::isSubscriptionActive, this, &ShopManager::isSubscriptionActive);

}

void ShopManager::doPurchase(Products product)
{
   QInAppProduct *inAppProduct;
   switch (product)
   {
   case banana_durable:
      inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_durable"));
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
   inAppProduct->purchase();
}

void ShopManager::checkSubscription()
{
	m_myStore->getSubscriptionInfo();
}

void ShopManager::checkDurable()
{
	m_myStore->getDurableInfo();
}

void ShopManager::initShop()
{
	m_myStore->checkIsTrial();
	m_myStore->getAppInfo();
	m_myStore->getAddonsInfo();
	m_myStore->getCollectionInfo();
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


void ShopManager::handleTransaction(QInAppTransaction* transaction)
{

   /*if (transaction->status() == QInAppTransaction::PurchaseRestored && transaction->product()->identifier() == QStringLiteral("banana_durable"))
   {
      emit productPurchased(banana_durable);
   }
   else if (transaction->status() == QInAppTransaction::PurchaseApproved)
   {
      if (transaction->product()->identifier() == QStringLiteral("banana_multiple_product"))
         emit productPurchased(banana_multiple_product);
      else if (transaction->product()->identifier() == QStringLiteral("banana_durable"))
         emit productPurchased(banana_durable);
   }
   else if (transaction->status() == QInAppTransaction::PurchaseFailed)
   {
      emit error("The purchase has not been completed or an error was found");
   }
   transaction->finalize();*/
}