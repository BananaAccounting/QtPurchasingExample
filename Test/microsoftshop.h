#ifndef MICROSOFTSHOP_H
#define MICROSOFTSHOP_H

#include <QObject>
#include <QSharedPointer>

#include <windows.h>
#include <Windows.Services.Store.h>
#include <wrl.h>
#include <functional>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Services::Store;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

class QWindow;
class QInAppProductPrivate;
class QInAppProduct : public QObject
{
	Q_OBJECT
	Q_ENUMS(ProductType)
	Q_PROPERTY(QString identifier READ identifier CONSTANT)
	Q_PROPERTY(ProductType productType READ productType CONSTANT)
	Q_PROPERTY(QString price READ price CONSTANT)
	Q_PROPERTY(QString title READ title CONSTANT)
	Q_PROPERTY(QString description READ description CONSTANT)
public:
	enum ProductType
	{
		Consumable,
		Unlockable,
		Subscription
	};
	QString identifier() const;
	ProductType productType() const;

	QString price() const;
	QString title() const;
	QString description() const;
	void purchase();
protected:
	explicit QInAppProduct(const QString &price, const QString &title, const QString &description, ProductType productType, const QString &identifier, QObject *parent = nullptr);

private :
	friend class QInAppStore;
	Q_DISABLE_COPY(QInAppProduct)
	QSharedPointer<QInAppProductPrivate> d;
};

class QInAppTransactionPrivate;
class QInAppTransaction : public QObject
{
	Q_OBJECT
		Q_ENUMS(TransactionStatus FailureReason)
		Q_PROPERTY(TransactionStatus status READ status CONSTANT)
		Q_PROPERTY(QInAppProduct* product READ product CONSTANT)
		Q_PROPERTY(QString orderId READ orderId CONSTANT)
		Q_PROPERTY(FailureReason failureReason READ failureReason CONSTANT)
		Q_PROPERTY(QString errorString READ errorString CONSTANT)
		Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT)
public:
	enum TransactionStatus {
		Unknown,
		PurchaseApproved,
		PurchaseFailed,
		PurchaseRestored
	};

	enum FailureReason {
		NoFailure,
		CanceledByUser,
		ErrorOccurred
	};

	~QInAppTransaction();
	void finalize();
	TransactionStatus status() const;
	QInAppProduct *product() const;
	QString orderId() const;
	QString platformProperty(const QString &propertyName) const;
	FailureReason failureReason() const;
	QString errorString() const;
	QDateTime timestamp() const;
protected:
	explicit QInAppTransaction(TransactionStatus status, QInAppProduct *product, QObject *parent = nullptr);
private:
	Q_DISABLE_COPY(QInAppTransaction)
	QSharedPointer<QInAppTransactionPrivate> d;
};

class QInAppStore : public QObject
{
   Q_OBJECT

   void emitIsTrial(bool bActiveLicense);
public:
   QInAppStore(QWindow* mainWindow, QObject *parent = nullptr);
   void registerProduct(QInAppProduct::ProductType, const QString& id) {}
   QInAppProduct* registeredProduct(const QString& id);

   void checkIsTrial();
   void Purchase10Coins();
signals:
   void isTrial(bool);
};



#endif // MICROSOFTSHOP_H
