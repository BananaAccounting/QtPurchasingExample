#include "window.h"

Window::Window(QWidget *parent) : QWidget(parent)
{

    #ifdef Q_OS_WIN 
    setFixedSize(1920, 1080);
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
   if (m_shopManager == nullptr)
   {
      m_shopManager = new ShopManager(this->windowHandle(), this);

      connect(btnMultple, &QPushButton::clicked, this, &Window::purchaseMultiple);
      connect(btnPurchaseForever, &QPushButton::clicked, this, &Window::purchaseLimited);
      connect(btnPurchaseOnce, &QPushButton::clicked, this, &Window::purchaseOnce);
      connect(btnRestorePurchases, &QPushButton::clicked, m_shopManager, &ShopManager::restorePurchases);
      connect(btnCheckTrial, &QPushButton::clicked, m_shopManager, &ShopManager::checkIsTrial);

      connect(m_shopManager, &ShopManager::productPurchased, this, &Window::handlePurchase);
   }
}

void Window::handleError(const QString& errorMessage)
{
   QMessageBox msgBox;
   msgBox.setText(errorMessage);
   msgBox.exec();
}

void Window::handlePurchase(ShopManager::Products productId)
{
    switch (productId)
    {
    case ShopManager::banana_multiple_product:
       incrementLabelMultiple();
       break;
    case ShopManager::banana_once_product:
       markAsPurchasedOnce();
       break;
    }
}

void Window::purchaseOnce()
{
    m_shopManager->doPurchase(ShopManager::banana_once_product);
}

void Window::purchaseMultiple()
{
    m_shopManager->doPurchase(ShopManager::banana_multiple_product);
}

void Window::incrementLabelMultiple()
{
    int lab = lblAmount->text().toInt();
    lab++;
    lblAmount->setNum(lab);
    this->hide();
    this->show();
}

void Window::purchaseLimited()
{
//    QInAppProduct *product = m_shopManager->registeredProduct(QStringLiteral("banana_multiple_product"));

//    Q_ASSERT(product != 0);

//    product->purchase();
    markAsPurchasedLimited();
}

void Window::markAsPurchasedLimited()
{
    lblOnceForever->setText("Purchased");
    btnPurchaseForever->setEnabled(false);
    this->hide();
    this->show();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(restoreLimited()));
    timer->start(5000);

}

void Window::restoreLimited()
{
    lblOnceForever->setText("Purchase limited time");
    btnPurchaseForever->setEnabled(true);
    this->hide();
    this->show();
}


void Window::markAsPurchasedOnce()
{
    lblOnce->setText("Purchased");
    btnPurchaseOnce->setEnabled(false);
    this->hide();
    this->show();
}
