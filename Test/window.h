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
#include "mylogger.h"
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
	void markAsProductTrial();
	void writeLog(QString log);
signals:
protected:
   virtual void showEvent(QShowEvent *ev) override;
public slots:
    void handleError(const QString& errorMessage);
    void purchaseDurable();
    void purchaseSubscription();
    void purchaseProduct();
	void checkSubscription();
	void checkDurable();
	void handleTrial(bool);
	void handleActive(bool);
	void handleDurableActive(bool);
	void handleSubscriptionActive(bool);


	
};

#endif // WINDOW_H
