#include "pch.h"
#include "microsoftshop.h"

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
class QInAppProductPrivate1
{
public:
	QInAppProductPrivate1(const QString &price, const QString &title, const QString &description, QInAppProduct1::ProductType type, const QString &id)
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
	QInAppProduct1::ProductType productType;
	QString identifier;
};

QInAppProduct1::QInAppProduct1(const QString &price, const QString &title, const QString &description, ProductType productType, const QString &identifier, QObject *parent)
	: QObject(parent)
{
	d = QSharedPointer<QInAppProductPrivate1>(new QInAppProductPrivate1(price, title, description, productType, identifier));
}

QString QInAppProduct1::identifier() const
{
	return d->identifier;
}

QString QInAppProduct1::description() const
{
	return d->localDescription;
}

QString QInAppProduct1::title() const
{
	return d->localTitle;
}

QString QInAppProduct1::price() const
{
	return d->localPrice;
}

QInAppProduct1::ProductType QInAppProduct1::productType() const
{
	return d->productType;
}

void QInAppProduct1::purchase()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	if(title() == "banana_subscription")
		asyncStore->setOperation(AsyncStore::mType::buySubscription);
	else 
		asyncStore->setOperation(AsyncStore::mType::buyDurable);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppProduct1::handleStringResponse);
	connect(asyncStore, &AsyncStore::productBought, this, &QInAppProduct1::isProductBought);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}


/* QInAppTransaction */
class QInAppTransactionPrivate1
{
public:
	QInAppTransactionPrivate1(QInAppTransaction1::TransactionStatus s,
		QInAppProduct1 *p)
		: status(s)
		, product(p)
	{
	}

	QInAppTransaction1::TransactionStatus status;
	QInAppProduct1 *product;
};

QInAppTransaction1::QInAppTransaction1(TransactionStatus status, QInAppProduct1 *product, QObject *parent) : QObject(parent)
{
	d = QSharedPointer<QInAppTransactionPrivate1>(new QInAppTransactionPrivate1(status, product));
}

QInAppTransaction1::~QInAppTransaction1()
{
}

QInAppProduct1 *QInAppTransaction1::product() const
{
	return d->product;
}

QInAppTransaction1::TransactionStatus QInAppTransaction1::status() const
{
	return d->status;
}

QInAppTransaction1::FailureReason QInAppTransaction1::failureReason() const
{
	return NoFailure;
}

QString QInAppTransaction1::errorString() const
{
	return QString();
}

QDateTime QInAppTransaction1::timestamp() const
{
	return QDateTime();
}

QString QInAppTransaction1::orderId() const
{
	return QString();
}

QString QInAppTransaction1::platformProperty(const QString &propertyName) const
{
	Q_UNUSED(propertyName);
	return QString();
}

/* QInAppStore */
QInAppProduct1 * QInAppStore1::registeredProduct(const QString & id)
{
	QInAppProduct1* product;

	if (id == "banana_product")
		product = new QInAppProduct1("0", id, id, QInAppProduct1::ProductType::Unlockable, "9NDW9G6P5G6X");
	else if (id == "banana_subscription")
		product = new QInAppProduct1("0", id, id, QInAppProduct1::ProductType::Subscription, "9P2QZFC6NH0M");
	else
		product = new QInAppProduct1("0", id, id, QInAppProduct1::ProductType::Unlockable, "9P27DZCTDFDR");

	connect(product, &QInAppProduct1::handleStringResponse, this, &QInAppStore1::handleStringResponse);
	return product;
}


QInAppStore1::QInAppStore1(QWindow* mainWindow, QObject *parent)
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
	*/


	winrt::com_ptr<IInitializeWithWindow> initWindow;
	winrt::Windows::Foundation::IUnknown * unknown = reinterpret_cast<winrt::Windows::Foundation::IUnknown*>(&context);
	unknown->as(initWindow);
	initWindow->Initialize((HWND)mainWindow);
	

}

void QInAppStore1::checkIsTrial()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::checkIsTrial);
	connect(asyncStore, &AsyncStore::isTrial, this, &QInAppStore1::isTrial);
	connect(asyncStore, &AsyncStore::isActive, this, &QInAppStore1::isActive);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore1::handleStringResponse);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}


void QInAppStore1::getAppInfo()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::getAppInfo);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore1::handleStringResponse);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}

void QInAppStore1::getAddonsInfo()
{
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::getAddons);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore1::handleStringResponse);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}

void QInAppStore1::getCollectionInfo()
{
	if (context == nullptr) {
		setContext();
	}
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::getUserCollection);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore1::handleStringResponse);
	connect(asyncStore, &AsyncStore::isDurableActive, this, &QInAppStore1::isDurableActive);
	connect(asyncStore, &AsyncStore::isSubscriptionActive, this, &QInAppStore1::isSubscriptionActive);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}

void QInAppStore1::getSubscriptionInfo()
{
	if (context == nullptr) {
		setContext();
	}
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::checkSubscription);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore1::handleStringResponse);
	connect(asyncStore, &AsyncStore::isSubscriptionActive, this, &QInAppStore1::isSubscriptionActive);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}

void QInAppStore1::getDurableInfo()
{
	if (context == nullptr) {
		setContext();
	}
	AsyncStore* asyncStore = new AsyncStore();
	asyncStore->setAutoDelete(true);
	asyncStore->setContext(context);
	asyncStore->setOperation(AsyncStore::mType::checkDurable);
	connect(asyncStore, &AsyncStore::appInfo, this, &QInAppStore1::handleStringResponse);
	connect(asyncStore, &AsyncStore::isDurableActive, this, &QInAppStore1::isDurableActive);
	QThreadPool::globalInstance()->tryStart(asyncStore);
}
