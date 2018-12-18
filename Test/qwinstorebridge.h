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

#ifndef QWINSTOREBRIDGE_P_H
#define QWINSTOREBRIDGE_P_H

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

#include <unknwn.h>
#include <shobjidl.h>
#include <winrt/Windows.Services.Store.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Globalization.DateTimeFormatting.h>
#include <QRunnable>
#include <QThreadPool>
#include <QObject>
#include <QWindow>
#include "qwininappproduct_p.h"
#include "qwininapptransaction_p.h"

class WinStoreBridge : public QObject, public QRunnable
{
    Q_OBJECT

public:
    WinStoreBridge(QWindow *mainWindow, QObject *parent = nullptr);
    WinStoreBridge();
    enum mType {
        getAppInfo, getProducts, getAddonsFiltered, getAddonsPurchasable, getUserCollection, checkIsTrial, checkSubscription, checkDurable, buyProduct, fulfillConsumable
    };
    void setOperation(mType operation)
    {
        m_operation = operation;
    }
    void setProduct(QWinInAppProduct *product)
    {
        m_product = product;
    }
    void addFilterKind(const winrt::hstring &);
    void addFilterId(const winrt::hstring &);
    void run() override;
    void setContext(const winrt::Windows::Services::Store::StoreContext &);

private:
    winrt::Windows::Services::Store::StoreContext context = nullptr;
    mType m_operation = getAppInfo;
    int m_quantity = 1;
    QWinInAppProduct *m_product;
    winrt::Windows::Foundation::Collections::IVector<winrt::hstring> filterKinds =
        winrt::single_threaded_vector<winrt::hstring>();
    winrt::Windows::Foundation::Collections::IVector<winrt::hstring> filterIds =
        winrt::single_threaded_vector<winrt::hstring>();
    void getStoreContext();

    void getAppInfoAsync();
    void getAddonsAsync();
    void getAddonsFilteredAsync();
    void checkIsTrialAsync();
    void checkSubscriptionAsync();
    void checkDurableAsync();
    void buyProductAsync();
    void fulfillConsumableAsync();
signals:
    void appInfo(const QString &);
    void isTrial(bool);
    void isAppActive(const QString &);
    void isAddonActive(const QString &);
    void isDurableActive(bool);
    void isSubscriptionActive(bool);
    void subInfo(const QString &);
    void fulfilled(bool);
    void storeProduct(const QString &);
    void requestDone();

    void purchaseSuccess(const QString &, const QString &);
    void purchaseFailed(const QString &, const QString &);
    void purchaseCanceled(const QString &, const QString &);
public slots:

};
#endif // QWINSTOREBRIDGE_P_H