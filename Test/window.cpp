#include "window.h"
#include "storemanager.h"


class QInAppProduct {
public:
	QInAppProduct(QObject*) {}
	QInAppProduct() {}
	QString identifier() { return ""; }
	void purchase() {}
	enum MyEnum
	{
		Consumable, Unlockable
	};

};
class QInAppTransaction {
public:
	QInAppTransaction(QObject*) {}
	enum States
	{
		PurchaseRestored, PurchaseApproved, PurchaseFailed
	};
	States status() { return States::PurchaseFailed; }
	void finalize() {}
	QInAppProduct* product() { return nullptr; }
};
class QInAppStore {
public:
	QInAppStore(QObject*) {}
	void registerProduct(QInAppProduct::MyEnum x, const QString& qs) {}
	void restorePurchases() {}
	QInAppProduct* registeredProduct(QString x) { return nullptr; }
	
};
Window::Window(QWidget *parent) : QWidget(parent)
{

    #ifdef Q_OS_WIN 
    setFixedSize(1920, 1080);
    #endif
    
    m_myStore = new QInAppStore(this);
    setupConnections();

    m_myStore->registerProduct(QInAppProduct::Consumable,
                               QStringLiteral("banana_multiple_product"));

    m_myStore->registerProduct(QInAppProduct::Unlockable,
                               QStringLiteral("banana_once_product"));

	setupOpenButton();
    setupMultipleButton();

    setupLimitedButton();

    setupOnceButton();

    setupRestoreButton();

}


void Window::setupOpenButton()
{

	but_open_UWP = new QPushButton("Open UWP Window", this);
	but_open_UWP->setGeometry(100, 50, 800, 50);
	but_open_UWP->setEnabled(true);
	label_open = new QLabel("0", this);
	label_open->setTextInteractionFlags(Qt::NoTextInteraction);
	label_open->setAlignment(Qt::AlignCenter);
	label_open->setGeometry(100, 100, 800, 60);
	connect(but_open_UWP, SIGNAL(clicked()), this, SLOT(openUWPWindow()));
}

void Window::setupMultipleButton()
{

    but_purchase_multiple = new QPushButton("Purchase multiple times", this);
    but_purchase_multiple->setGeometry(100, 150, 800, 50);

    but_purchase_multiple->setEnabled(false);

    label_multiple = new QLabel("0", this);
    label_multiple->setTextInteractionFlags(Qt::NoTextInteraction);
    label_multiple->setAlignment(Qt::AlignCenter);
    label_multiple->setGeometry(100, 200, 800, 50);

    connect(but_purchase_multiple, SIGNAL(clicked()), this, SLOT(purchaseMultiple()));
}

void Window::setupLimitedButton()
{
    but_purchase_limited = new QPushButton("Purchase for limited time", this);
    but_purchase_limited->setGeometry(100, 250, 800, 50);

    but_purchase_limited->setEnabled(false);

    label_limited = new QLabel("Not purchased", this);
    label_limited->setTextInteractionFlags(Qt::NoTextInteraction);
    label_limited->setAlignment(Qt::AlignCenter);
    label_limited->setGeometry(100, 300, 800, 50);

    connect(but_purchase_limited, SIGNAL(clicked()), this, SLOT(purchaseLimited()));
}

void Window::setupOnceButton()
{
    but_purchase_once = new QPushButton("Purchase once forever", this);
    but_purchase_once->setGeometry(100, 450, 800, 50);

    but_purchase_once->setEnabled(false);

    label_once = new QLabel("Not purchased",this);
    label_once->setTextInteractionFlags(Qt::NoTextInteraction);
    label_once->setAlignment(Qt::AlignCenter);
    label_once->setGeometry(100, 500, 800, 50);

    connect(but_purchase_once, SIGNAL(clicked()), this, SLOT(purchaseOnce()));
}

void Window::setupRestoreButton()
{
    but_restore_once = new QPushButton("Restore unique product if already bought", this);
    but_restore_once->setGeometry(100, 550, 800, 50);

    but_restore_once->setEnabled(true);

    connect(but_restore_once, SIGNAL(clicked()), this, SLOT(restoreOnce()));
}


void Window::restoreOnce()
{
    m_myStore->restorePurchases();
}

void Window::setupConnections()
{
    /*connect(m_myStore, SIGNAL(productRegistered(QInAppProduct*)),
            this, SLOT(markProductAvailable(QInAppProduct*)));
    connect(m_myStore, SIGNAL(productUnknown(QInAppProduct*)),
            this, SLOT(handleErrorGracefully(QInAppProduct*)));

    connect(m_myStore, SIGNAL(transactionReady(QInAppTransaction*)),
            this, SLOT(handleTransaction(QInAppTransaction*)));*/
}

void Window::markProductAvailable(QInAppProduct* p)
{
    if (p->identifier() == QStringLiteral("banana_multiple_product"))
    {
        but_purchase_multiple->setEnabled(true);
    }
    else if (p->identifier() == QStringLiteral("banana_once_product"))
    {
        but_purchase_once->setEnabled(true);
    }
}

void Window::handleErrorGracefully(QInAppProduct* p)
{
    QMessageBox msgBox;
    msgBox.setText("Error regarding " + p->identifier());
    msgBox.exec();
}

void Window::handleTransaction(QInAppTransaction* transaction)
{

    if (transaction->status() == QInAppTransaction::PurchaseRestored && transaction->product()->identifier() == QStringLiteral("banana_once_product"))
    {
        markAsPurchasedOnce();
        transaction->finalize();
    }
    else if (transaction->status() == QInAppTransaction::PurchaseApproved)
    {
        if (transaction->product()->identifier() == QStringLiteral("banana_multiple_product"))
        {
            incrementLabelMultiple();
        }
        else if (transaction->product()->identifier() == QStringLiteral("banana_once_product"))
        {
            markAsPurchasedOnce();
        }
        transaction->finalize();
    }
    else if (transaction->status() == QInAppTransaction::PurchaseFailed)
    {
        QMessageBox msgBox;
        msgBox.setText("The purchase has not been completed or an error was found");
        msgBox.exec();
        transaction->finalize();
    }

}


void Window::purchaseMultiple()
{
    QInAppProduct *product = m_myStore->registeredProduct(QStringLiteral("banana_multiple_product"));

    Q_ASSERT(product != 0);

    product->purchase();
}

void Window::incrementLabelMultiple()
{
    int lab = label_multiple->text().toInt();
    lab++;
    label_multiple->setNum(lab);
    this->hide();
    this->show();
}

void Window::purchaseLimited()
{
//    QInAppProduct *product = m_myStore->registeredProduct(QStringLiteral("banana_multiple_product"));

//    Q_ASSERT(product != 0);

//    product->purchase();
    markAsPurchasedLimited();
}

void Window::markAsPurchasedLimited()
{
    label_limited->setText("Purchased");
    but_purchase_limited->setEnabled(false);
    this->hide();
    this->show();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(restoreLimited()));
    timer->start(5000);

}

void Window::restoreLimited()
{
    label_limited->setText("Purchase limited time");
    but_purchase_limited->setEnabled(true);
    this->hide();
    this->show();
}

void Window::purchaseOnce()
{
    QInAppProduct *product = m_myStore->registeredProduct(QStringLiteral("banana_once_product"));

    Q_ASSERT(product != 0);

    product->purchase();
}

void Window::markAsPurchasedOnce()
{
    label_once->setText("Purchased");
    but_purchase_once->setEnabled(false);
    this->hide();
    this->show();

}

void Window::openUWPWindow()
{
	StoreManager sm;
	sm.openApp();
}
