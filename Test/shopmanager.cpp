#include "pch.h"
#include "shopmanager.h"

ShopManager::ShopManager(QWindow *mainWindow, QObject *parent) : QObject(parent)
{
    m_myStore = new QInAppStore(this);
    m_myStore->setPlatformProperty("window", QString::number(mainWindow->winId()));
    setupConnections();
}

QString ShopManager::productIdentifier(Products product)
{
    switch (product) {
    case banana_once_product:
        return  QStringLiteral("banana_once_product");
    case banana_subscription:
        return  QStringLiteral("banana_subscription");
    case banana_product:
        return  QStringLiteral("9NDW9G6P5G6X");
    case banana_consumable:
        return  QStringLiteral("banana_consumable");
    default:
        return "";
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
    MyLogger::instance().writeLog("Purchasing: " + inAppProduct->title());
    inAppProduct->purchase();
}

void ShopManager::useProduct(Products product, int quantity)
{
    if (product == Products::banana_consumable) {
        if (myConsumables - quantity >= 0) {
            myConsumables -= quantity;
            emit updateConsumable(QString::number(myConsumables));
        } else {
            emit error("You don't have enough consumables.");

        }
    }

}

void ShopManager::initShop()
{
    m_myStore->registerProduct(QInAppProduct::Unlockable,
                               productIdentifier(Products::banana_once_product));
    m_myStore->registerProduct(QInAppProduct::Unlockable,
                               productIdentifier(Products::banana_subscription));
    m_myStore->registerProduct(QInAppProduct::Unlockable, productIdentifier(Products::banana_product));
    m_myStore->registerProduct(QInAppProduct::Consumable,
                               productIdentifier(Products::banana_consumable));

    m_myStore->restorePurchases();
}

bool ShopManager::event(QEvent *e)
{
    return QObject::event(e);
}


void ShopManager::restorePurchases()
{
    MyLogger::instance().writeLog("Restoring purchases");
}

void ShopManager::handleErrorGracefully(QInAppProduct *p)
{
    emit error("Error regarding " + p->identifier());
}

void ShopManager::handleStringResponse(const QString &result)
{
    MyLogger::instance().writeLog(result);
}

void ShopManager::handleCorrectProduct(QInAppProduct *p)
{
    MyLogger::instance().writeLog("Product: " + p->title() + " registred.");
}

void ShopManager::handleUnknownProduct(QInAppProduct::ProductType type, const QString &id)
{
    emit error("Unknown product " + id);
}


void ShopManager::handleTransactions(QInAppTransaction *transaction)
{
    switch (transaction->status()) {
    case QInAppTransaction::PurchaseFailed: {
        emit error(transaction->platformProperty("extendedError"));
        break;
    }
    case QInAppTransaction::PurchaseApproved:
    case QInAppTransaction::PurchaseRestored:
        if (transaction->product()->identifier() == productIdentifier(Products::banana_once_product))
            emit isDurableActive(true);
        else if (transaction->product()->identifier() == productIdentifier(Products::banana_subscription)) {
            emit isSubscriptionActive(true);
            MyLogger::instance().writeLog("Subscription expiration date: " +
                                          transaction->platformProperty("expiration"));
        } else if (transaction->product()->identifier() == productIdentifier(Products::banana_product))
            emit isActive(true);
        else if (transaction->product()->identifier() == productIdentifier(Products::banana_consumable)) {
            myConsumables++;
            emit updateConsumable(QString::number(myConsumables));
        }


        break;
    default:
        break;
    }
    transaction->finalize();
}