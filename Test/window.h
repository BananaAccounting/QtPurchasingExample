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
#include <QtPurchasing>

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

private:
    QPushButton *but_purchase_multiple;
    QLabel *label_multiple;
    QPushButton *but_purchase_once;
    QLabel *label_once;
    QPushButton *but_purchase_limited;
    QLabel *label_limited;
    QPushButton *but_restore_once;
    QInAppStore *m_myStore;
    void setupMultipleButton();
    void setupLimitedButton();
    void setupOnceButton();
    void setupRestoreButton();
    void setupConnections();
    void incrementLabelMultiple();
    void markAsPurchasedLimited();
    void markAsPurchasedOnce();
signals:

public slots:
    void markProductAvailable(QInAppProduct*);
    void handleErrorGracefully(QInAppProduct*);
    void handleTransaction(QInAppTransaction*);
    void purchaseMultiple();
    void purchaseLimited();
    void restoreLimited();
    void restoreOnce();
    void purchaseOnce();
};

#endif // WINDOW_H
