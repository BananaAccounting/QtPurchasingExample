
#include "qwinstorebridge.h"

#include <QDebug>

using namespace winrt::Windows::Services::Store;
inline QString hStringToQString(const winrt::hstring &h)
{
    unsigned int length;
    const wchar_t *raw = h.c_str();
    return QString::fromStdWString(raw);
}

WinStoreBridge::WinStoreBridge(QWindow *mainWindow, QObject *parent)
    : QObject(parent)
{

}

WinStoreBridge::WinStoreBridge()
{

}

void WinStoreBridge::setContext(const StoreContext &ctx)
{
    context = ctx;
}

void WinStoreBridge::getStoreContext()
{
    auto factory = winrt::get_activation_factory<StoreContext, IStoreContextStatics>();
    context = factory.GetDefault();
}

void WinStoreBridge::getAppInfoAsync()
{
    if (context == nullptr) {
        getStoreContext();
    }
    StoreProductResult storeProductResult = context.GetStoreProductForCurrentAppAsync().get();
    if (storeProductResult == nullptr) {
        winrt::check_hresult(storeProductResult.ExtendedError());
        QString res = QString::fromStdString("Something went wrong, and the product was not returned.");
    }

    auto p = storeProductResult.Product();
    emit storeProduct(hStringToQString(p.ExtendedJsonData()));
    getAddonsAsync();
}

void WinStoreBridge::getAddonsAsync()
{
    if (context == nullptr) {
        getStoreContext();
    }

    auto filter{ winrt::single_threaded_vector<winrt::hstring>({ L"Durable", L"Consumable"}) };
    StoreProductQueryResult  storeProductQueryRes = context.GetAssociatedStoreProductsAsync(
                                                        filter).get();

    if (storeProductQueryRes == nullptr) {
        winrt::check_hresult(storeProductQueryRes.ExtendedError());
        QString res = QString::fromStdString("Something went wrong, and the product was not returned.");
    }
    for (auto const &product : storeProductQueryRes.Products()) {
        auto p = product.Value();
        emit storeProduct(hStringToQString(p.ExtendedJsonData()));
    }
    emit requestDone();
}

void WinStoreBridge::getAddonsFilteredAsync()
{
    if (context == nullptr) {
        getStoreContext();
    }
    ;
    StoreProductQueryResult  storeProductQueryRes = context.GetStoreProductsAsync(filterKinds,
                                                                                  filterIds).get();

    if (storeProductQueryRes == nullptr) {
        winrt::check_hresult(storeProductQueryRes.ExtendedError());
        QString res = QString::fromStdString("Something went wrong, and the product was not returned.");
    }
    QString res = "Available addOns:";
    emit appInfo(res);
    for (auto const &product : storeProductQueryRes.Products()) {
        auto p = product.Value();
        auto data = p.ExtendedJsonData();
        res = QString::fromStdWString(data.c_str());
        emit appInfo(res);
    }
}

void WinStoreBridge::checkIsTrialAsync()
{
    if (context == nullptr) {
        getStoreContext();
    }
    StoreAppLicense  appLicense = context.GetAppLicenseAsync().get();
    if (appLicense == nullptr) {
        emit appInfo(QString::fromStdString("An error occurred while retrieving the license."));
    }
    if (appLicense.IsActive()) {
        auto appData = appLicense.ExtendedJsonData();
        emit isAppActive(QString::fromStdWString(appData.c_str()));
    }
    for (auto license : appLicense.AddOnLicenses()) {
        StoreLicense addOnLicense = license.Value();
        if (addOnLicense.IsActive()) {
            auto data = addOnLicense.ExtendedJsonData();
            emit isAddonActive(QString::fromStdWString(data.c_str()));
        }
    }
}

void WinStoreBridge::checkSubscriptionAsync()
{
    if (context == nullptr) {
        getStoreContext();
    }
    StoreAppLicense  appLicense = context.GetAppLicenseAsync().get();
    if (appLicense == nullptr) {
        emit appInfo(QString::fromStdString("An error occurred while retrieving the license."));
    }
    for (auto license : appLicense.AddOnLicenses()) {
        StoreLicense addOnLicense = license.Value();
        if (addOnLicense.IsActive() && addOnLicense.InAppOfferToken() == L"banana_subscription") {
            auto data = addOnLicense.ExtendedJsonData();
            emit appInfo(QString::fromStdWString(data.c_str()));
            emit isSubscriptionActive(true);
        }
    }
}

void WinStoreBridge::checkDurableAsync()
{
    if (context == nullptr) {
        getStoreContext();
    }
    StoreAppLicense  appLicense = context.GetAppLicenseAsync().get();
    if (appLicense == nullptr) {
        emit appInfo(QString::fromStdString("An error occurred while retrieving the license."));
    }
    for (auto license : appLicense.AddOnLicenses()) {
        StoreLicense addOnLicense = license.Value();
        if (addOnLicense.IsActive() && addOnLicense.InAppOfferToken() == L"banana_once_product") {
            auto data = addOnLicense.ExtendedJsonData();
            emit appInfo(QString::fromStdWString(data.c_str()));
            emit isDurableActive(true);
        }
    }
}

/* Buy Addons */

void WinStoreBridge::buyProductAsync()
{
    winrt::hstring id = (m_product->storeID).toStdWString().c_str();
    if (context == nullptr) {
        getStoreContext();
    }
    StorePurchaseResult result = context.RequestPurchaseAsync(id).get();

    winrt::hresult extendedError;
    if (result == nullptr) {
        winrt::check_hresult(result.ExtendedError());
        extendedError = result.ExtendedError();
        return;
    }
    QWinInAppTransaction *transaction;
    switch (result.Status()) {
    case StorePurchaseStatus::AlreadyPurchased:
        emit purchaseSuccess(m_product->identifier(), "The user has already purchased the product.");
        break;

    case StorePurchaseStatus::Succeeded:
        emit purchaseSuccess(m_product->identifier(), "The purchase was successful.");
        break;

    case StorePurchaseStatus::NotPurchased:
        emit purchaseCanceled(m_product->identifier(),
                              "The purchase did not complete. The user may have cancelled the purchase.");
        break;

    case StorePurchaseStatus::NetworkError:
        emit purchaseFailed(m_product->identifier(),
                            "The purchase was unsuccessful due to a network error");
        break;

    case StorePurchaseStatus::ServerError:
        emit purchaseFailed(m_product->identifier(), "The purchase was unsuccessful due to a server error");
        break;

    default:
        emit purchaseFailed(m_product->identifier(),
                            "The purchase was unsuccessful due to an unknown error.");
        break;
    }
}

void WinStoreBridge::fulfillConsumableAsync()
{
    winrt::hstring id = (m_product->storeID).toStdWString().c_str();
    if (context == nullptr) {
        getStoreContext();
    }
    winrt::guid trackingId = winrt::guid();

    StoreConsumableResult result = context.ReportConsumableFulfillmentAsync(id, m_quantity,
                                                                            trackingId).get();

    winrt::hresult extendedError;
    if (result == nullptr) {
        winrt::check_hresult(result.ExtendedError());
        extendedError = result.ExtendedError();
        return;
    }
    QWinInAppTransaction *transaction;
    switch (result.Status()) {
    case StoreConsumableStatus::Succeeded:
        emit appInfo("The fulfillment was successful.");
        emit fulfilled(true);
        break;

    case StoreConsumableStatus::InsufficentQuantity:
        emit appInfo("The fulfillment was unsuccessful because the remaining balance is insufficient.");
        emit fulfilled(false);
        break;

    case StoreConsumableStatus::NetworkError:
        emit appInfo("The fulfillment was unsuccessful due to a network error. ");
        emit fulfilled(false);
        break;

    case StoreConsumableStatus::ServerError:
        appInfo("The fulfillment was unsuccessful due to a server error.");
        emit fulfilled(false);
        break;

    default:
        appInfo("The fulfillment was unsuccessful due to an unknown error. ");
        emit fulfilled(false);
        break;
    }
}

void WinStoreBridge::addFilterKind(const winrt::hstring &kind)
{
    filterKinds.Append(kind);
}

void WinStoreBridge::addFilterId(const winrt::hstring &id)
{
    filterIds.Append(id);
}

void WinStoreBridge::run()
{
    switch (m_operation) {
    case getAppInfo: {
        getAppInfoAsync();
        break;
    }
    case getProducts: {
        getAddonsAsync();
        break;
    }
    case getAddonsFiltered: {
        getAddonsFilteredAsync();
        break;
    }
    case checkIsTrial: {
        checkIsTrialAsync();
        break;
    }
    case buyProduct: {
        buyProductAsync();
        break;
    }
    case checkSubscription: {
        checkSubscriptionAsync();
        break;
    }
    case checkDurable: {
        checkDurableAsync();
        break;
    }
    case fulfillConsumable: {
        break;
    }

    }
}