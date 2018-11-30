#include "microsoftshop.h"
#include "vector.h"
#include "winrt/base.h"
#include <QDebug>


#define CheckHr(hr) do { if (FAILED(hr)) __debugbreak(); } while (false)
const wchar_t kItemFriendlyName[] = L"10 coins";
const wchar_t kItemStoreId[] = L"ten_coins";

ComPtr<IStoreContext> storeContext;

void writeLog(QString log) {
	qDebug() << log;
	MyLogger::instance().writeLog(log);
}
void OnPurchaseOperationDone(IAsyncOperation<StorePurchaseResult*>* operation, AsyncStatus status)
{
	if (status != AsyncStatus::Completed)
	{
		// It failed for some reason. Find out why.
		ComPtr<IAsyncInfo> asyncInfo;
		auto hr = operation->QueryInterface(__uuidof(asyncInfo), &asyncInfo);
		CheckHr(hr);

		HRESULT errorCode;
		hr = asyncInfo->get_ErrorCode(&errorCode);
		CheckHr(hr);

		// Do something with the errorCode
		emit writeLog(QString(errorCode));

		// Return once error is handled
		return;
	}

	ComPtr<IStorePurchaseResult> purchaseResult;
	auto hr = operation->GetResults(&purchaseResult);
	CheckHr(hr);

	StorePurchaseStatus purchaseStatus;
	hr = purchaseResult->get_Status(&purchaseStatus);
	CheckHr(hr);

	switch (purchaseStatus)
	{
	case StorePurchaseStatus_Succeeded:
	case StorePurchaseStatus_AlreadyPurchased:
		// Success. Product was purchased

		writeLog(QString("Success. Product was purchased"));
		break;

	case StorePurchaseStatus_NotPurchased:
		// User canceled the purchase
		writeLog(QString("User canceled the purchase"));
		break;

	case StorePurchaseStatus_NetworkError:
		// The device could not reach windows store
		writeLog(QString(" The device could not reach windows store"));
		break;

	case StorePurchaseStatus_ServerError:
		// Something broke on the server
		writeLog(QString(" Something broke on the server"));
		break;
	}
}

/* QInAppProduct */
class QInAppProductPrivate
{
public:
	QInAppProductPrivate(const QString &price, const QString &title, const QString &description, QInAppProduct::ProductType type, const QString &id)
		: localPrice(price)
		, localTitle(title)
		, localDescription(description)
		, productType(type)
		, identifier(id)
	{
	}

	QString localPrice;
	QString localTitle;
	QString localDescription;
	QInAppProduct::ProductType productType;
	QString identifier;
};

QInAppProduct::QInAppProduct(const QString &price, const QString &title, const QString &description, ProductType productType, const QString &identifier, QObject *parent)
	: QObject(parent)
{
	d = QSharedPointer<QInAppProductPrivate>(new QInAppProductPrivate(price, title, description, productType, identifier));
}

QString QInAppProduct::identifier() const
{
	return d->identifier;
}

QString QInAppProduct::description() const
{
	return d->localDescription;
}

QString QInAppProduct::title() const
{
	return d->localTitle;
}

QString QInAppProduct::price() const
{
	return d->localPrice;
}

QInAppProduct::ProductType QInAppProduct::productType() const
{
	return d->productType;
}

void QInAppProduct::purchase()
{
	ComPtr<IStoreContextStatics> storeContextStatics;
	auto hr = RoGetActivationFactory(HStringReference(L"Windows.Services.Store.StoreContext").Get(), __uuidof(storeContextStatics), &storeContextStatics);
	CheckHr(hr);

	ComPtr<IAsyncOperation<StoreAppLicense*>> getLicenseOperation;
	hr = storeContext->GetAppLicenseAsync(&getLicenseOperation);
	CheckHr(hr);
	QString idString = identifier();
	const wchar_t* id = idString.toStdWString().c_str();
	ComPtr<IAsyncOperation<StorePurchaseResult*>> purchaseOperation;
	hr = storeContext->RequestPurchaseAsync(HStringReference(id).Get(), &purchaseOperation);
	CheckHr(hr);

	auto onCompletedCallback = Callback<Implements<RuntimeClassFlags<ClassicCom>, IAsyncOperationCompletedHandler<StorePurchaseResult*>, FtmBase>>(
		[this](IAsyncOperation<StorePurchaseResult*>* operation, AsyncStatus status)
	{
		OnPurchaseOperationDone(operation, status);
		return S_OK;
	});



	hr = purchaseOperation->put_Completed(onCompletedCallback.Get());
	CheckHr(hr);
}





/* QInAppTransaction */
class QInAppTransactionPrivate
{
public:
	QInAppTransactionPrivate(QInAppTransaction::TransactionStatus s,
		QInAppProduct *p)
		: status(s)
		, product(p)
	{
	}

	QInAppTransaction::TransactionStatus status;
	QInAppProduct *product;
};

QInAppTransaction::QInAppTransaction(TransactionStatus status, QInAppProduct *product, QObject *parent) : QObject(parent)
{
	d = QSharedPointer<QInAppTransactionPrivate>(new QInAppTransactionPrivate(status, product));
}

QInAppTransaction::~QInAppTransaction()
{
}

QInAppProduct *QInAppTransaction::product() const
{
	return d->product;
}

QInAppTransaction::TransactionStatus QInAppTransaction::status() const
{
	return d->status;
}

QInAppTransaction::FailureReason QInAppTransaction::failureReason() const
{
	return NoFailure;
}

QString QInAppTransaction::errorString() const
{
	return QString();
}

QDateTime QInAppTransaction::timestamp() const
{
	return QDateTime();
}

QString QInAppTransaction::orderId() const
{
	return QString();
}

QString QInAppTransaction::platformProperty(const QString &propertyName) const
{
	Q_UNUSED(propertyName);
	return QString();
}

/* QInAppStore */
QInAppProduct * QInAppStore::registeredProduct(const QString & id)
{
	QInAppProduct* product;

	if (id == "banana_product")
		product = new QInAppProduct("0", id, id, QInAppProduct::ProductType::Unlockable, "9NDW9G6P5G6X");
	else if (id == "banana_subscription")
		product = new QInAppProduct("0", id, id, QInAppProduct::ProductType::Subscription, "9P2QZFC6NH0M");
	else
		product = new QInAppProduct("0", id, id, QInAppProduct::ProductType::Unlockable, "9P27DZCTDFDR");

	return product;
}

QInAppStore::QInAppStore(QWindow* mainWindow, QObject *parent)
	: QObject(parent)
{
	ComPtr<IStoreContextStatics> storeContextStatics;
	auto hr = RoGetActivationFactory(HStringReference(L"Windows.Services.Store.StoreContext").Get(), __uuidof(storeContextStatics), &storeContextStatics);
	CheckHr(hr);

	hr = storeContextStatics->GetDefault(&storeContext);
	CheckHr(hr);

	ComPtr<IInitializeWithWindow> initWindow;
	hr = storeContext->QueryInterface(IID_PPV_ARGS(&initWindow));
	hr = initWindow->Initialize((HWND)(void*)mainWindow);
}

void QInAppStore::checkIsTrial()
{
	ComPtr<IAsyncOperation<StoreAppLicense*>> getLicenseOperation;
	auto hr = storeContext->GetAppLicenseAsync(&getLicenseOperation);
	CheckHr(hr);

	hr = getLicenseOperation->put_Completed
	(Callback<Implements<RuntimeClassFlags<ClassicCom>, IAsyncOperationCompletedHandler<StoreAppLicense*>, FtmBase>>(
		[this](IAsyncOperation<StoreAppLicense*>* operation, AsyncStatus status)
	{
		if (status != AsyncStatus::Completed)
		{
			// It failed for some reason. Find out why.
			ComPtr<IAsyncInfo> asyncInfo;
			auto hr = operation->QueryInterface(__uuidof(asyncInfo), &asyncInfo);
			CheckHr(hr);

			HRESULT errorCode;
			hr = asyncInfo->get_ErrorCode(&errorCode);
			CheckHr(hr);

			// Do something with the errorCode
			writeLog(QString("Error getLicenseOperation: ").append(errorCode));
			// Return once error is handled
			return S_OK;
		}

		ComPtr<IStoreAppLicense> appLicense;
		auto hr = operation->GetResults(&appLicense);
		CheckHr(hr);

		boolean isActiveb, isTrialb = false;

		hr = appLicense->get_IsActive(&isActiveb);
		CheckHr(hr);

		emit isActive(isActiveb);
		if (isActiveb)
		{
			hr = appLicense->get_IsTrial(&isTrialb);
			if (isTrialb)
			{
				writeLog(QString("Is trial!"));
			}
			CheckHr(hr);
			emit isTrial(isTrialb);
			writeLog(QString("isActive: true"));
		}
		else
			writeLog(QString("isActive: false"));
		return S_OK;
	}).Get());
	CheckHr(hr);
}

void QInAppStore::checkDurable()
{
	//Vector<HSTRING>^ filterList = ref new Vector<HSTRING>();
	//filterList->Append(HSTRING("Durable"));
	//IVectorView<HSTRING> view{ filterList->GetView() };
	ComPtr<Vector<HSTRING>> filters = Make<Vector<HSTRING>>();
	filters->Append(HString::MakeReference(L"Durable").Get());
	ComPtr<IIterable<HSTRING>> iterable;
	iterable = reinterpret_cast<IIterable<HSTRING>*>(filters.Get());
	
	ComPtr<IAsyncOperation<StoreProductQueryResult*>> products;
	IIterable<HSTRING>* v = iterable.Get();
	auto hr = storeContext->GetUserCollectionAsync(v, &products);
	CheckHr(hr);

	hr = products->put_Completed
	(Callback<Implements<RuntimeClassFlags<ClassicCom>, IAsyncOperationCompletedHandler<StoreProductQueryResult *>, FtmBase>>(
		[this](IAsyncOperation<StoreProductQueryResult *>* operation, AsyncStatus status)
	{
		if (status != AsyncStatus::Completed)
		{
			// It failed for some reason. Find out why.
			ComPtr<IAsyncInfo> asyncInfo;
			auto hr = operation->QueryInterface(__uuidof(asyncInfo), &asyncInfo);
			CheckHr(hr);

			HRESULT errorCode;
			hr = asyncInfo->get_ErrorCode(&errorCode);
			CheckHr(hr);

			// Do something with the errorCode
			writeLog(QString("Error getLicenseOperation: ").append(errorCode));
			// Return once error is handled
			return S_OK;
		}

		//ComPtr<IStoreAppLicense> appLicense;
		//auto hr = operation->GetResults(&appLicense);
		//CheckHr(hr);

		//ComPtr<IIterable> addonlicense;
		//hr = appLicense->get_AddOnLicenses(&addonlicense);


		//CheckHr(hr);
		/*
		if (addonlicense.SkuStoreId.StartsWith(subscriptionStoreId))
		{
			if (license.IsActive)
			{
				// The expiration date is available in the license.ExpirationDate property.
				return true;
			}
		}*/

		return S_OK;
	}).Get());
	CheckHr(hr);
}

void QInAppStore::checkSubscription()
{

}

void QInAppStore::Purchase10Coins()
{
	ComPtr<IStoreContextStatics> storeContextStatics;
	auto hr = RoGetActivationFactory(HStringReference(L"Windows.Services.Store.StoreContext").Get(), __uuidof(storeContextStatics), &storeContextStatics);
	CheckHr(hr);

	ComPtr<IStoreContext> storeContext;
	hr = storeContextStatics->GetDefault(&storeContext);
	CheckHr(hr);

	ComPtr<IStorePurchasePropertiesFactory> purchasePropertiesFactory;
	hr = RoGetActivationFactory(HStringReference(L"Windows.Services.Store.StorePurchaseProperties").Get(), __uuidof(purchasePropertiesFactory), &purchasePropertiesFactory);
	CheckHr(hr);

	ComPtr<IStorePurchaseProperties> purchaseProperties;
	hr = purchasePropertiesFactory->Create(HStringReference(kItemFriendlyName).Get(), &purchaseProperties);
	CheckHr(hr);

	ComPtr<IAsyncOperation<StorePurchaseResult*>> purchaseOperation;
	hr = storeContext->RequestPurchaseWithPurchasePropertiesAsync(HStringReference(kItemStoreId).Get(), purchaseProperties.Get(), &purchaseOperation);
	CheckHr(hr);

	// Change the following line to call Callback<IAsyncOperationCompletedHandler<StorePurchaseResult*>> if you want the callback to happen back on the UI thread
	// Implementing FtmBase allows it to fire on the thread the operation finished
	auto onCompletedCallback = Callback<Implements<RuntimeClassFlags<ClassicCom>, IAsyncOperationCompletedHandler<StorePurchaseResult*>, FtmBase>>(
		[this](IAsyncOperation<StorePurchaseResult*>* operation, AsyncStatus status)
	{
		OnPurchaseOperationDone(operation, status);
		return S_OK;
	});

	hr = purchaseOperation->put_Completed(onCompletedCallback.Get());
	CheckHr(hr);
}

