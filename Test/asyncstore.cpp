#include "pch.h"
#include "asyncstore.h"
#include <QDebug>

using namespace winrt::Windows::Services::Store;

AsyncStore::AsyncStore(QWindow* mainWindow, QObject *parent)
	: QObject(parent)
{

}
AsyncStore::AsyncStore() {

}


void AsyncStore::setContext(const StoreContext & ctx)
{
	context = ctx;
}

void AsyncStore::getStoreContext() {
	auto factory = winrt::get_activation_factory<StoreContext, IStoreContextStatics>();
	context = factory.GetDefault();
}


void AsyncStore::getAppInfoAsync()
{
	if (context == nullptr) {
		getStoreContext();
	}
	StoreProductResult storeProduct = context.GetStoreProductForCurrentAppAsync().get();
	if (storeProduct == nullptr) {
		winrt::check_hresult(storeProduct.ExtendedError());
		QString res = QString::fromStdString("Something went wrong, and the product was not returned.");
	}

	winrt::hstring price = storeProduct.Product().Price().FormattedBasePrice();
	QString res = "The price of this app is : " + QString::fromStdWString(price.c_str());
	emit appInfo(res);
}

void AsyncStore::getAddonsAsync()
{
	if (context == nullptr) {
		getStoreContext();
	}

	auto filter{ winrt::single_threaded_vector<winrt::hstring>({ L"Durable", L"Consumable", L"UnmanagedConsumable", L"Application" }) };
	StoreProductQueryResult  storeProductQueryRes = context.GetAssociatedStoreProductsAsync(filter).get();

	if (storeProductQueryRes == nullptr) {
		winrt::check_hresult(storeProductQueryRes.ExtendedError());
		QString res = QString::fromStdString("Something went wrong, and the product was not returned.");
	}
	QString res = "Products: \n";
	for (auto const& product : storeProductQueryRes.Products()) {
		auto p = product.Value();
		res += QString::fromStdWString(product.Key().c_str()) + ":" + QString::fromStdWString(p.Title().c_str())
			+ " (" + QString::fromStdWString(p.ProductKind().c_str()) + ") Price: " + QString::fromStdWString(p.Price().FormattedBasePrice().c_str()) + "\n";
	}

	emit appAddons(res);
}


void AsyncStore::getUserCollectionAsync()
{
	if (context == nullptr) {
		getStoreContext();
	}

	auto filter{ winrt::single_threaded_vector<winrt::hstring>({ L"Durable", L"Consumable", L"UnmanagedConsumable", L"Application" }) };
	StoreProductQueryResult  storeProductQueryRes = context.GetUserCollectionAsync(filter).get();

	if (storeProductQueryRes == nullptr) {
		winrt::check_hresult(storeProductQueryRes.ExtendedError());
		QString res = QString::fromStdString("Something went wrong, and the product was not returned.");
	}
	QString res = "User products: \n";
	for (auto const& product : storeProductQueryRes.Products()) {
		qDebug() << "Checking subscription " << product.Key().c_str();
		StoreProduct p = product.Value();
		res += QString::fromStdWString(product.Key().c_str()) + ":" + QString::fromStdWString(p.Title().c_str())
			+ " (" + QString::fromStdWString(p.ProductKind().c_str()) + ") Price: " + QString::fromStdWString(p.Price().FormattedBasePrice().c_str()) + "\n";
		if (product.Key() == durableID) {
			emit isDurablePurchased(true);
		}

	}

	emit appInfoUserCollection(res);
}

void AsyncStore::checkIsTrialAsync()
{
	if (context == nullptr) {
		getStoreContext();
	}
	StoreAppLicense  appLicense = context.GetAppLicenseAsync().get();
	if (appLicense == nullptr) {
		emit appInfo(QString::fromStdString("An error occurred while retrieving the license."));
	}
	if (appLicense.IsActive())
	{
		if (appLicense.IsTrial())
		{
			auto formatter = winrt::Windows::Globalization::DateTimeFormatting::DateTimeFormatter(L"hour:minute day/month/year");
			qDebug() << "Is trial";
			emit appInfo("This is the trial version. Expiration date: " + QString::fromStdWString(formatter.Format(appLicense.ExpirationDate()).c_str()));
			emit isTrial(true);
		}
		else
		{
			qDebug() << "Is active";
			emit appInfo("This is active ");
			emit isActive(true);
		}
	}
	for (auto const& licenseMap : appLicense.AddOnLicenses())
	{
		auto license = licenseMap.Value();
		if (license.SkuStoreId() == subscriptionID)
		{
			if (license.IsActive())
			{

				emit isSubscriptionActive(true);
				auto formatter = winrt::Windows::Globalization::DateTimeFormatting::DateTimeFormatter(L"hour:minute day/month/year");
				emit appInfo("This subscription expires on " + QString::fromStdWString(formatter.Format(license.ExpirationDate()).c_str()));
				;

			}
		}
	}

}

void AsyncStore::buySubscriptionAsync()
{
	if (context == nullptr) {
		getStoreContext();
	}
	StorePurchaseResult result = context.RequestPurchaseAsync(subscriptionID).get();

	winrt::hresult extendedError;
	if (result == nullptr)
	{
		winrt::check_hresult(result.ExtendedError());
		extendedError = result.ExtendedError();
		return;
	}

	switch (result.Status())
	{
	case StorePurchaseStatus::AlreadyPurchased:
		//textBlock.Text = "The user has already purchased the product.";3
		emit appInfo("The user has already purchased the product.");
		emit subscriptionBought(true);
		break;

	case StorePurchaseStatus::Succeeded:
		//textBlock.Text = "The purchase was successful.";
		emit appInfo("The purchase was successful.");
		emit subscriptionBought(true);
		break;

	case StorePurchaseStatus::NotPurchased:
		//textBlock.Text = "The purchase did not complete. " + "The user may have cancelled the purchase. ExtendedError: " + extendedError;
		emit appInfo("The purchase did not complete. The user may have cancelled the purchase. ExtendedError: " + QString::number(extendedError));
		break;

	case StorePurchaseStatus::NetworkError:
		//textBlock.Text = "The purchase was unsuccessful due to a network error. " + "ExtendedError: " + extendedError;
		emit appInfo("The purchase was unsuccessful due to a network error. ExtendedError: " + QString::number(extendedError));
		break;

	case StorePurchaseStatus::ServerError:
		//textBlock.Text = "The purchase was unsuccessful due to a server error. " +"ExtendedError: " + extendedError;
		emit appInfo("The purchase was unsuccessful due to a server error. ExtendedError: " + QString::number(extendedError));
		break;

	default:
		//textBlock.Text = "The purchase was unsuccessful due to an unknown error. " +		"ExtendedError: " + extendedError;
		emit appInfo("The purchase was unsuccessful due to an unknown error.  ExtendedError: " + QString::number(extendedError));
		break;
	}

}

void AsyncStore::run()
{
	switch (m_operation)
	{
	case getAppInfo:
	{
		getAppInfoAsync();
		break;
	}
	case getAddons:
	{
		getAddonsAsync();
		break;
	}
	case getUserCollection:
	{
		getUserCollectionAsync();
		break;
	}
	case checkIsTrial:
	{
		checkIsTrialAsync();
		break;
	}
	case buySubscription:
	{
		buySubscriptionAsync();
		break;
	}

	}
}



