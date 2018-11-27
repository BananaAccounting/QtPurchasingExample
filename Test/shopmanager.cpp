#include "shopmanager.h"

#include <QWindow>


ShopManager::ShopManager(QWindow* mainWindow, QObject *parent) : QObject(parent)
{
   m_myStore = new QInAppStore(mainWindow, this);


   m_myStore->registerProduct(QInAppProduct::Unlockable,
      QStringLiteral("banana_once_product"));

   m_myStore->registerProduct(QInAppProduct::Subscription,
      QStringLiteral("banana_subscription"));
}


void ShopManager::setupConnections()
{
   connect(m_myStore, SIGNAL(productRegistered(QInAppProduct*)),
      this, SLOT(markProductAvailable(QInAppProduct*)));
   connect(m_myStore, SIGNAL(productUnknown(QInAppProduct*)),
      this, SLOT(handleErrorGracefully(QInAppProduct*)));
   connect(m_myStore, SIGNAL(transactionReady(QInAppTransaction*)),
      this, SLOT(handleTransaction(QInAppTransaction*)));
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
   inAppProduct->purchase();
}

void ShopManager::checkIsTrial()
{
   m_myStore->checkIsTrial();
}

void ShopManager::restorePurchases()
{

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
