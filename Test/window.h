#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QString>
#include <QStringLiteral>
#include <QTimer>
#include "shopmanager.h"
#include "ui_window.h"

class Window : public QWidget, Ui_Window
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    ShopManager* m_shopManager = nullptr;
    
    void markAsSubscribed();
    void markAsDurablePurchased();
    void markAsProductPurchased();
signals:
protected:
   virtual void showEvent(QShowEvent *ev) override;
public slots:
    void handleError(const QString& errorMessage);
    void purchaseDurable();
    void purchaseSubscription();
    void purchaseProduct();
};

#endif // WINDOW_H
