#include "microsoftshop.h"

#include <QDebug>
#include "shobjidl.h"

#define CheckHr(hr) do { if (FAILED(hr)) __debugbreak(); } while (false)
const wchar_t kItemFriendlyName[] = L"10 coins";
const wchar_t kItemStoreId[] = L"ten_coins";

ComPtr<IStoreContext> storeContext;

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

void QInAppStore::emitIsTrial(bool bActiveLicense)
{
   emit isTrial(bActiveLicense);
}

void QInAppStore::checkIsTrial()
{
   //std::function<void(bool)> onCompleted(emitIsTrial);
   //CheckIsTrial(f2);

   std::function<void(bool)> onCompleted = std::bind(&QInAppStore::emitIsTrial, this, std::placeholders::_1);

//}
//}

//void CheckIsTrial(std::function<void(bool)> onCompleted)
//{
   ComPtr<IStoreContextStatics> storeContextStatics;
   auto hr = RoGetActivationFactory(HStringReference(L"Windows.Services.Store.StoreContext").Get(), __uuidof(storeContextStatics), &storeContextStatics);
   CheckHr(hr);

   ComPtr<IAsyncOperation<StoreAppLicense*>> getLicenseOperation;
   hr = storeContext->GetAppLicenseAsync(&getLicenseOperation);
   CheckHr(hr);

   hr = getLicenseOperation->put_Completed
      (Callback<Implements<RuntimeClassFlags<ClassicCom>, IAsyncOperationCompletedHandler<StoreAppLicense*>, FtmBase>>(
         [onCompleted{ std::move(onCompleted) }](IAsyncOperation<StoreAppLicense*>* operation, AsyncStatus status)
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

         // Return once error is handled
         return S_OK;
      }

      ComPtr<IStoreAppLicense> appLicense;
      auto hr = operation->GetResults(&appLicense);
      CheckHr(hr);

      boolean isActive, isTrial = false;

      hr = appLicense->get_IsActive(&isActive);
      CheckHr(hr);

      if (isActive)
      {
         hr = appLicense->get_IsTrial(&isTrial);
         CheckHr(hr);
      }

      onCompleted(static_cast<bool>(isActive));
      return S_OK;
   }).Get());
   CheckHr(hr);
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
      break;

   case StorePurchaseStatus_NotPurchased:
      // User canceled the purchase
      break;

   case StorePurchaseStatus_NetworkError:
      // The device could not reach windows store
      break;

   case StorePurchaseStatus_ServerError:
      // Something broke on the server
      break;
   }
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
      [](IAsyncOperation<StorePurchaseResult*>* operation, AsyncStatus status)
   {
      OnPurchaseOperationDone(operation, status);
      return S_OK;
   });

   hr = purchaseOperation->put_Completed(onCompletedCallback.Get());
   CheckHr(hr);
}
