#ifndef MICROSOFTSHOP_H
#define MICROSOFTSHOP_H

#include <QObject>

#include <windows.h>
#include <Windows.Services.Store.h>
#include <wrl.h>
#include <functional>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Services::Store;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

class QWindow;

class QInAppProduct : public QObject
{
   Q_OBJECT
public:
   QInAppProduct(QObject *parent = nullptr) : QObject(parent) {}
   enum ProductType { Consumable, Unlockable };
   QString identifier() const { return QString(); }
   void purchase() {}
};
class QInAppTransaction : public QObject
{
   Q_OBJECT
public:
   enum TransactionStatus { Unknown, PurchaseApproved, PurchaseFailed, PurchaseRestored };
   QInAppTransaction(QObject *parent = nullptr) : QObject(parent) {}
   TransactionStatus status() { return Unknown; }
   void finalize() {}
   QInAppProduct* product() { return nullptr; }
};

class QInAppStore : public QObject
{
   Q_OBJECT

   void emitIsTrial(bool bActiveLicense);
public:
   QInAppStore(QWindow* mainWindow, QObject *parent = nullptr);
   void registerProduct(QInAppProduct::ProductType, const QString& id) {}
   QInAppProduct* registeredProduct(const QString& id) { return nullptr; }

   void checkIsTrial();
   void Purchase10Coins();
signals:
   void isTrial(bool);
};



#endif // MICROSOFTSHOP_H
