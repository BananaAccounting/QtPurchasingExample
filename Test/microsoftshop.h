#ifndef MICROSOFTSHOP_H
#define MICROSOFTSHOP_H

#include <QObject>
#include <QSharedPointer>
#include <QDebug>
#include <QDateTime>

#include "mylogger.h"
#include "asyncstore.h"

class QWindow;
class QInAppProductPrivate1;
class QInAppProduct1 : public QObject
{
	Q_OBJECT
	Q_ENUMS(ProductType)
	Q_PROPERTY(QString identifier READ identifier CONSTANT)
	Q_PROPERTY(ProductType productType READ productType CONSTANT)
	Q_PROPERTY(QString price READ price CONSTANT)
	Q_PROPERTY(QString title READ title CONSTANT)
	Q_PROPERTY(QString description READ description CONSTANT)
signals:
	void isProductBought(bool);
	void handleStringResponse(const QString&);
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
	explicit QInAppProduct1(const QString &price, const QString &title, const QString &description, ProductType productType, const QString &identifier, QObject *parent = nullptr);

private :
	friend class QInAppStore1;
	Q_DISABLE_COPY(QInAppProduct1)
	QSharedPointer<QInAppProductPrivate1> d;
};

class QInAppTransactionPrivate1;
class QInAppTransaction1 : public QObject
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

	~QInAppTransaction1();
	void finalize();
	TransactionStatus status() const;
	QInAppProduct1 *product() const;
	QString orderId() const;
	QString platformProperty(const QString &propertyName) const;
	FailureReason failureReason() const;
	QString errorString() const;
	QDateTime timestamp() const;

protected:
	explicit QInAppTransaction1(TransactionStatus status, QInAppProduct1 *product, QObject *parent = nullptr);
private:
	Q_DISABLE_COPY(QInAppTransaction1)
	QSharedPointer<QInAppTransactionPrivate1> d;
};

class QInAppStore1 : public QObject
{
   Q_OBJECT

public:
   QInAppStore1(QWindow* mainWindow, QObject *parent = nullptr);
   explicit QInAppStore(QObject *parent = nullptr);
   ~QInAppStore1();

   void registerProduct(QInAppProduct1::ProductType, const QString& id) {}
   QInAppProduct1* registeredProduct(const QString& id);

   void checkIsTrial();
   void getAppInfo();
   void getAddonsInfo();
   void getCollectionInfo();
   void getSubscriptionInfo();
   void getDurableInfo();
private:
	
signals:
   void isTrial(bool);
   void isActive(bool);
   void isDurableActive(bool);
   void isSubscriptionActive(bool);
   void handleStringResponse(const QString&);

   void productRegistered(QInAppProduct1 *product);
   void productUnknown(QInAppProduct1::ProductType productType, const QString &identifier);
   void transactionReady(QInAppTransaction1 *transaction);

public slots:
	

};


MIDL_INTERFACE("3E68D4BD-7135-4D10-8018-9FB6D9F33FA1")
IInitializeWithWindow: public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Initialize(
		/* [in] */ __RPC__in HWND hwnd) = 0;

};
//struct __declspec(uuid("3E68D4BD-7135-4D10-8018-9FB6D9F33FA1")) IInitializeWithWindow;
#endif // MICROSOFTSHOP_H
