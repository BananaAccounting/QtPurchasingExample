/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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

#include "qwininapptransaction_p.h"
#include "qwininapppurchasebackend_p.h"
#include "qinappproduct.h"

#include <QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcPurchasingTransaction, "qt.purchasing.transaction")

QWinInAppTransaction::QWinInAppTransaction(TransactionStatus status,
	QInAppProduct *product, FailureReason reason, const QString &expiration,QObject *parent)
	: QInAppTransaction(status, product, parent)
	, m_failureReason(reason), m_expiration(expiration)
{
	qCDebug(lcPurchasingTransaction) << __FUNCTION__;
	m_backend = qobject_cast<QWinInAppPurchaseBackend *>(parent);
}

void QWinInAppTransaction::finalize()
{
	/*qCDebug(lcPurchasingTransaction) << __FUNCTION__;
	if (product()->productType() == QInAppProduct::Consumable &&
		(status() == QInAppTransaction::PurchaseApproved ||
			status() == QInAppTransaction::PurchaseRestored)) {
		m_backend->fulfillConsumable(this);
	}*/
	deleteLater();
}


QString QWinInAppTransaction::platformProperty(const QString &propertyName) const
{
	if (propertyName == QLatin1String("expiration"))
		return m_expiration;
	if (propertyName == QLatin1String("extendedError"))
		return m_extendedError;
	return QString();
}

QT_END_NAMESPACE
