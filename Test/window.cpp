#include "window.h"

Window::Window(QWidget *parent) : QWidget(parent)
{
    #ifdef Q_OS_WIN 
    setFixedSize(800, 600);
    #endif
    setupUi(this);
	lblIsTrial->setStyleSheet("QLabel { color : red; }");
}

Window::~Window()
{
   delete m_shopManager;
}

void Window::showEvent(QShowEvent *ev)
{
   QWidget::showEvent(ev);
   if (m_shopManager == nullptr)
   {
      m_shopManager = new ShopManager(this->windowHandle(), this);

      connect(btnSubscribe, &QPushButton::clicked, this, &Window::purchaseSubscription);
      connect(btnBuyProduct, &QPushButton::clicked, this, &Window::purchaseProduct);
      connect(btnBuyDurable, &QPushButton::clicked, this, &Window::purchaseDurable);
   }
}

void Window::handleError(const QString& errorMessage)
{
   QMessageBox msgBox;
   msgBox.setText(errorMessage);
   msgBox.exec();
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


void Window::markAsSubscribed()
{
    lblSubscription->setText("Subscribed");
	btnSubscribe->setEnabled(false);
    this->hide();
    this->show();
}


void Window::markAsDurablePurchased()
{
    lblDurable->setText("Durable purchased");
	btnBuyDurable->setEnabled(false);
    this->hide();
    this->show();
}

void Window::markAsProductPurchased()
{
	lblTrial->setText("is purchased");
	btnBuyProduct->setEnabled(false);
	this->hide();
	this->show();
	lblIsTrial->setStyleSheet("QLabel { color : green; }");
}
