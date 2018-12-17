/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-COMM$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWININAPPPURCHASEBACKEND_P_H
#define QWININAPPPURCHASEBACKEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
#include "qinapppurchasebackend_p.h"
#include "qinappproduct.h"

#include <QObject>

QT_BEGIN_NAMESPACE

class QWinInAppProduct;
class QWinInAppPurchaseBackendPrivate;
class QWinInAppTransaction;
class QInAppStore;
class QWinInAppPurchaseBackend : public QInAppPurchaseBackend
{
	Q_OBJECT
public:
	explicit QWinInAppPurchaseBackend(QObject *parent = 0);

	void initialize() override;
	bool isReady() const override;

	void queryProducts(const QList<Product> &products) override;
	void queryProduct(QInAppProduct::ProductType productType, const QString &identifier) override;
	void restorePurchases() override;

	void setPlatformProperty(const QString &propertyName, const QString &value) override;

	void purchaseProduct(QWinInAppProduct *product);

	void fulfillConsumable(QWinInAppTransaction *transaction);
	void setContext();
public slots:
	void isAppActive(const QString&);
	void isAddonActive(const QString&);
	void purchaseFailed(const QString&, const QString&);
	void purchaseSuccess(const QString&, const QString&);
	void purchaseCanceled(const QString&, const QString&);
signals:

private:
	QScopedPointer<QWinInAppPurchaseBackendPrivate> d_ptr;
	
	Q_DECLARE_PRIVATE(QWinInAppPurchaseBackend);
};
QT_END_NAMESPACE

#endif // QWININAPPPURCHASEBACKEND_P_H
