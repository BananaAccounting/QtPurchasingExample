#include "shopmanager.h"

#include <QWindow>


ShopManager::ShopManager(QWindow* mainWindow, QObject *parent) : QObject(parent)
{
   m_myStore = new QInAppStore(mainWindow, this);
   //m_myStore->registerProduct(QInAppProduct::Unlockable, QStringLiteral("banana_product"));
   //m_myStore->registerProduct(QInAppProduct::Unlockable, QStringLiteral("banana_once_product"));
   //m_myStore->registerProduct(QInAppProduct::Subscription, QStringLiteral("banana_subscription"));
   setupConnections();
}


void ShopManager::setupConnections()
{
   //connect(m_myStore, SIGNAL(productRegistered(QInAppProduct*)), this, SLOT(markProductAvailable(QInAppProduct*)));
   //connect(m_myStore, SIGNAL(productUnknown(QInAppProduct*)), this, SLOT(handleErrorGracefully(QInAppProduct*)));
   //connect(m_myStore, SIGNAL(transactionReady(QInAppTransaction*)), this, SLOT(handleTransaction(QInAppTransaction*)));

   connect(m_myStore, &QInAppStore::isTrial, this, &ShopManager::isTrial);
}

void ShopManager::doPurchase(Products product)
{
   QInAppProduct *inAppProduct;
   switch (product)
   {
   case banana_once_product:
      inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_once_product"));
   case banana_subscription:
	   inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_subscription"));
   case banana_product:
	   inAppProduct = m_myStore->registeredProduct(QStringLiteral("banana_product"));
   }
   emit MyLogger::instance().writeLog("Purchasing: "+inAppProduct->title());
   inAppProduct->purchase();
}

void ShopManager::checkIsTrial()
{
   emit  MyLogger::instance().writeLog("Check is trial");
   m_myStore->checkIsTrial();
}

void ShopManager::checkAddon()
{
	emit  MyLogger::instance().writeLog("Check durable Addon");
	m_myStore->checkDurable();
}

void ShopManager::checkSubscription()
{
	emit  MyLogger::instance().writeLog("Check Subscription");
	m_myStore->checkSubscription();
}


void ShopManager::restorePurchases()
{
	emit  MyLogger::instance().writeLog("Restoring purchases");
}

void ShopManager::handleErrorGracefully(QInAppProduct* p)
{
   emit error("Error regarding " + p->identifier());
}

void ShopManager::handleTransaction(QInAppTransaction* transaction)
{

   /*if (transaction->status() == QInAppTransaction::PurchaseRestored && transaction->product()->identifier() == QStringLiteral("banana_once_product"))
   {
      emit productPurchased(banana_once_product);
   }
   else if (transaction->status() == QInAppTransaction::PurchaseApproved)
   {
      if (transaction->product()->identifier() == QStringLiteral("banana_multiple_product"))
         emit productPurchased(banana_multiple_product);
      else if (transaction->product()->identifier() == QStringLiteral("banana_once_product"))
         emit productPurchased(banana_once_product);
   }
   else if (transaction->status() == QInAppTransaction::PurchaseFailed)
   {
      emit error("The purchase has not been completed or an error was found");
   }
   transaction->finalize();*/
}