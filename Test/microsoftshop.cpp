#include "pch.h"
#include "microsoftshop.h"
#include "shobjidl.h"

void writeLog(QString log) {
	qDebug() << log;
	MyLogger::instance().writeLog(log);
}
winrt::Windows::Services::Store::StoreContext context = nullptr;
void setContext() {
	auto factory = winrt::get_activation_factory<winrt::Windows::Services::Store::StoreContext, winrt::Windows::Services::Store::IStoreContextStatics>();
	context = factory.GetDefault();
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
	
	if (context == nullptr) {
		setContext();
	}
	

	winrt::Windows::Services::Store::StorePurchaseResult result = context.RequestPurchaseAsync(identifier().toStdWString()).get();

	winrt::hresult extendedError;
	if (result == nullptr)
	{
		winrt::check_hresult(result.ExtendedError());
		extendedError = result.ExtendedError();
		return;
	}

	switch (result.Status())
	{
	case winrt::Windows::Services::Store::StorePurchaseStatus::AlreadyPurchased:
		//textBlock.Text = "The user has already purchased the product.";3
		emit handleStringResponse("The user has already purchased the product.");
		emit isSubscriptionActive(true);
		break;

	case winrt::Windows::Services::Store::StorePurchaseStatus::Succeeded:
		//textBlock.Text = "The purchase was successful.";
		emit handleStringResponse("The purchase was successful.");
		emit isSubscriptionActive(true);
		break;

	case winrt::Windows::Services::Store::StorePurchaseStatus::NotPurchased:
		//textBlock.Text = "The purchase did not complete. " + "The user may have cancelled the purchase. ExtendedError: " + extendedError;
		emit handleStringResponse("The purchase did not complete. The user may have cancelled the purchase. ExtendedError: " + QString::number(extendedError));
		break;

	case winrt::Windows::Services::Store::StorePurchaseStatus::NetworkError:
		//textBlock.Text = "The purchase was unsuccessful due to a network error. " + "ExtendedError: " + extendedError;
		emit handleStringResponse("The purchase was unsuccessful due to a network error. ExtendedError: " + QString::number(extendedError));
		break;

	case winrt::Windows::Services::Store::StorePurchaseStatus::ServerError:
		//textBlock.Text = "The purchase was unsuccessful due to a server error. " +"ExtendedError: " + extendedError;
		emit handleStringResponse("The purchase was unsuccessful due to a server error. ExtendedError: " + QString::number(extendedError));
		break;

	default:
		//textBlock.Text = "The purchase was unsuccessful due to an unknown error. " +		"ExtendedError: " + extendedError;
		emit handleStringResponse("The purchase was unsuccessful due to an unknown error.  ExtendedError: " + QString::number(extendedError));
		break;
	}

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

	connect(product, &QInAppProduct::handleStringResponse, this, &QInAppStore::handleStringResponse);
	connect(product, &QInAppProduct::isSubscriptionActive, this, &QInAppStore::isSubscriptionActive);
	return product;
}


QInAppStore::QInAppStore(QWindow* mainWindow, QObject *parent)
	: QObject(parent)
{
	setContext();
	
	/*StoreContext context = StoreContext.GetDefault();
	IInitializeWithWindow initWindow = (IInitializeWithWindow)(object)context;
	initWindow.Initialize(System.Diagnostics.Process.GetCurrentProcess().MainWindowHandle);	

	ComPtr<IInitializeWithWindow> initWindow;
	hr = storeContext->QueryInterface(IID_PPV_ARGS(&initWindow));
	hr = initWindow->Initialize((HWND)(void*)mainWindow);
	
	IInitializeWithWindow* initWindow = (IInitializeWithWindow*)(IUnknown*) &context;
	initWindow->Initialize(mainWindow);
	
	IInitializeWithWindow* pIInitWithWindow;
	IInspectable* iInspect = reinterpret_cast<IInspectable*>(&context);
	iInspect->QueryInterface(__uiidd(&pIInitWithWindow), &pIInitWithWindow);
	pIInitWithWindow->Initialize((HWND)(void *)mainWindow);
	pIInitWithWindow->Release();

	winrt::com_ptr<IInitializeWithWindow>* initWindow;
	initWindow = (winrt::com_ptr<IInitializeWithWindow>*)(winrt::com_ptr<IUnknown>*)(&context);
	initWindow->get()->QueryInterface(__uuidof(initWindow), initWindow->put_void());
	initWindow->get()->Initialize((HWND)(void*)mainWindow);
	
	IInitializeWithWindow initWindow = (IInitializeWithWindow) context;
	initWindow->Initialize((HWND)(void*)mainWindow);*/

}

void QInAppStore::checkIsTrial()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::checkIsTrial);
	connect(asyncStore, &AsyncStore::isTrial, this, &QInAppStore::isTrial);
	connect(asyncStore, &AsyncStore::isActive, this, &QInAppStore::isActive);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore::handleStringResponse);
	connect(asyncStore, &AsyncStore::isSubscriptionActive, this, &QInAppStore::isSubscriptionActive);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}


void QInAppStore::getAppInfo()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::getAppInfo);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore::handleStringResponse);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}

void QInAppStore::getAddonsInfo()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::getAddons);
	connect(asyncStore, &AsyncStore::appAddons, this, &QInAppStore::handleStringResponse);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}

void QInAppStore::getCollectionInfo()
{
	if (context == nullptr) {
		setContext();
	}
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::getUserCollection);
	connect(asyncStore, &AsyncStore::appInfoUserCollection, this, &QInAppStore::handleStringResponse);
	connect(asyncStore, &AsyncStore::isDurablePurchased, this, &QInAppStore::isDurablePurchased);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}
