#include "pch.h"
#include "window.h"
Window::Window(QWidget *parent) : QWidget(parent)
{
#ifdef Q_OS_WIN
    setFixedSize(800, 600);
#endif
    setupUi(this);
}

Window::~Window()
{
    delete m_shopManager;
}

void Window::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    if (m_shopManager == nullptr) {
        m_shopManager = new ShopManager(this->windowHandle(), this);
        QApplication::setActiveWindow(this);
        connect(btnSubscribe, &QPushButton::clicked, this, &Window::purchaseSubscription);
        connect(btnBuyProduct, &QPushButton::clicked, this, &Window::purchaseProduct);
        connect(btnBuyDurable, &QPushButton::clicked, this, &Window::purchaseDurable);
        connect(btnBuyConsumable, &QPushButton::clicked, this, &Window::purchaseConsumable);
        connect(btnUseConsumable, &QPushButton::clicked, this, &Window::useConsumable);
        connect(&MyLogger::instance(), &MyLogger::writeLog, this, &Window::writeLog);
        connect(m_shopManager, &ShopManager::isTrial, this, &Window::handleTrial);
        connect(m_shopManager, &ShopManager::isActive, this, &Window::handleActive);
        connect(m_shopManager, &ShopManager::isDurableActive, this, &Window::handleDurableActive);
        connect(m_shopManager, &ShopManager::isSubscriptionActive, this, &Window::handleSubscriptionActive);
        connect(m_shopManager, &ShopManager::updateConsumable, this, &Window::handleConsumable);
        connect(m_shopManager, &ShopManager::error, this, &Window::handleError);
        m_shopManager->initShop();
    }
}

void Window::handleError(const QString &errorMessage)
{
    QMessageBox msgBox;
    msgBox.setText(errorMessage);
    msgBox.exec();
    MyLogger::instance().writeLog(errorMessage);
}
void Window::handleTrial(bool isTrial)
{
    if (isTrial)
        markAsProductTrial();
}
void Window::handleActive(bool isActive)
{
    if (isActive)
        markAsProductPurchased();
}
void Window::handleDurableActive(bool isPurchased)
{
    if (isPurchased)
        markAsDurablePurchased();
}
void Window::handleSubscriptionActive(bool isActive)
{
    if (isActive)
        markAsSubscribed();
}
void Window::handleConsumable(const QString &str)
{
    markConsumbale(str);
}
void Window::purchaseDurable()
{
    m_shopManager->doPurchase(ShopManager::banana_once_product);
}

void Window::purchaseSubscription()
{
    m_shopManager->doPurchase(ShopManager::banana_subscription);
}

void Window::purchaseProduct()
{
    m_shopManager->doPurchase(ShopManager::banana_product);
}

void Window::purchaseConsumable()
{
    m_shopManager->doPurchase(ShopManager::banana_consumable);
}

void Window::useConsumable()
{
    m_shopManager->useProduct(ShopManager::banana_consumable, 1);
}

void Window::markAsSubscribed()
{
    lblSubscription->setText("Subscribed");
    btnSubscribe->setEnabled(false);
}


void Window::markAsDurablePurchased()
{
    lblDurable->setText("Durable purchased");
    btnBuyDurable->setEnabled(false);

}

void Window::markAsProductPurchased()
{
    lblTrial->setText("is purchased");
    btnBuyProduct->setEnabled(false);
    lblIsTrial->setStyleSheet("QLabel { color : green; }");
}

void Window::markAsProductTrial()
{
    lblTrial->setText("is trial");
    btnBuyProduct->setEnabled(true);
    lblIsTrial->setStyleSheet("QLabel { color : red; }");
}

void Window::markConsumbale(const QString &s)
{
    lblConsumable->setText("Nr. of Consumable: " + s);
}

void Window::writeLog(QString log)
{
    txtLog->append(log);
}
