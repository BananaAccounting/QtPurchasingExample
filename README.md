# In-App PlayStore purchases using Qt Purchasing

Implementing In-App purchases for PlayStore using [Qt Purchasing](http://doc.qt.io/qt-5/qtpurchasing-index.html) is pretty simple and straightforward.

We will assume that the application is ready to be deployed on an android device (if that's not the case, see [here](http://doc.qt.io/qt-5/androidgs.html))

## Getting started 
First off we want to add the Qt Purchasing library to the .pro file of the project, by adding this line to the file

	QT += purchasing

Then we want to include the library in the classes that will use it

	#include <QtPurchasing>
	
We want to create a global instance of [QInAppStore](https://doc.qt.io/Qt-5/qinappstore.html), which we will use to purchase or register items

	QInAppStore inAppStore = new QInAppStore(this);
	
## Connections
We need to setup the connections needed to make in-app purchases work, so we will mark down slots for when a correct product is registered, when an unknown product is registered and when transactions are handled

	connect(myStore, SIGNAL(productRegistered(QInAppProduct*)),
            this, SLOT(handleCorrectProduct(QInAppProduct*)));
            
    connect(myStore, SIGNAL(productUnknown(QInAppProduct*)),
            this, SLOT(handleUnknownProduct(QInAppProduct*)));

    connect(myStore, SIGNAL(transactionReady(QInAppTransaction*)),
            this, SLOT(handleTransactions(QInAppTransaction*)));

## Registering Products  
After the connections are in place, we need to finally register the in-app products

There are two types of in-app purchases

* Consumables: objects that can be purchased multiple times
* Unlockables: objects that can be purchased only once

Knowing this we will register the products accordingly

	myStore->registerProduct(QInAppProduct::Consumable, QStringLiteral("consumable_product"));
	
or

	myStore->registerProduct(QInAppProduct::Unlockable, QStringLiteral("unlockable_product"));
	
or both if we need to have different in-app purchases.

## Purchase a product

When we want to start the purchase of an in-app product we might want to implement a similar code

	QInAppProduct *product = myStore->registeredProduct(QStringLiteral("consumable_product"));

or 

	QInAppProduct *product = myStore->registeredProduct(QStringLiteral("unlockable_product"));

Then check that the product is in fact registered

	// Should not get here if product is not registered
	Q_ASSERT(product != 0);
	
And proceed with the purchase
	
	product->purchase();

Once the purchase is concluded (approved or failed) a **transactionReady(QInAppTransaction*****)**

## Handling Transactions

Transactions occur when the signal **transactionReady(QInAppTransaction*****)** is emitted. In this slot we will need to handle a correct purchase or a failed purchase.
It's importante to finalize transactions.

	void handleTransactions(QInAppTransaction* transaction)
	{
	
	    if (transaction->status() == QInAppTransaction::PurchaseApproved)
	    {
	        if (transaction->product()->identifier() == QStringLiteral("consumable_product"))
	        {
	            purchaseConsumableProduct();
	        }
	        else if (transaction->product()->identifier() == QStringLiteral("unlockable_product"))
	        {
	            purchaseUnlockableProduct();
	        }
	        transaction->finalize();
	    }
	    else if (transaction->status() == QInAppTransaction::PurchaseFailed)
	    {
	        QMessageBox msgBox;
	        msgBox.setText("The purchase has not been completed or an error occurred");
	        msgBox.exec();
	        transaction->finalize();
	    }
	}
	
##Restoring purchases

We want to provide a way to restore previously purchased unlockable products
So we will have to invoke

	myStore->restorePurchases();
	
when we want to proceed to that action.
Then we will have to add this block of code in the handleTransactions function

	if (transaction->status() == QInAppTransaction::PurchaseRestored
		&& transaction->product()->identifier() == QStringLiteral("unlockable_product"))
    {
        restoreUnlockableProduct();
        transaction->finalize();
    }

Allowing the user to restore previously purchased unlockable content.    


# Register Products on the Play Store

This part will be about register in-app products on the Play Store
If the app is ready to be deployed, then you just need to follow this steps. Otherwhise, you shold check [this](http://doc.qt.io/qt-5/qtpurchasing-googleplay.html).

Head over to the Google Play Console, and log in. Access to the application you want to monetize and go to *Store Presence* - *In-app products*

Qt Purchasing can only work with managed products, so we will work with that.
Click on **Create Managed Product**

On Product ID give the same name you used to identify the product on your code (in our example, it would be either *consumable_product* or *unlockable_product*)

Then proceed to fill out informations about the product (Title, description and price)
In the end activate its status by clicking on **Active**

# QtPurchasing and the Windows Store

Currently QtPurchasing doesn't support Windows Store InApp purchases

![alt text](https://i.imgur.com/H7FRLY1.png)

## Our implementation

Import the project using QT VS Tools.

### Configuration Properties
Windows SDK version: minimum `Windows 10, version 1607`  or later

*not needed if already imported in the dll*
Debugging -> Environment -> add to PATH `C:\Program Files\WindowsApps\Microsoft.VCLibs.140.00.Debug_14.0.27023.1_x86__8wekyb3d8bbwe;
C:\Program Files\WindowsApps\Microsoft.VCLibs.140.00.Debug_14.0.27023.1_x86__8wekyb3d8bbwe;`

(fixes missing store dlls.)

C/C++ -> Additional #using Directories -> add `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\vcpackages;C:\Program Files (x86)\Windows Kits\10\UnionMetadata`

(fixes winmd missing files)

C/C++ -> Consume Windows Runtime Extension -> set `Yes (/ZW)`

C/C++ -> Language -> C++ Language Standard -> set `ISO C++17 Standard (/std:c++17)`

Linker -> Input -> Additional Dependencies -> add `WindowsApp.lib`