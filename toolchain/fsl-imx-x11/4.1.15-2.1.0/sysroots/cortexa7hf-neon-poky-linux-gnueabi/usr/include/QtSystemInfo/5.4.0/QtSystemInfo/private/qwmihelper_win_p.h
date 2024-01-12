/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WMIHELPER_H
#define WMIHELPER_H

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

#include "../qsysteminfoglobal_p.h"

#if !defined (Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
#include <QObject>
#include <QVariant>
#include <QString>
#include <wbemidl.h>

#include <QStringList>

QT_BEGIN_NAMESPACE

class Q_SYSTEMINFO_PRIVATE_EXPORT WMIHelper : public QObject
{
    Q_OBJECT

public:
    WMIHelper(QObject *parent = 0);
    ~WMIHelper();
    QVariant getWMIData();
    QVariant getWMIData(const QString &wmiNamespace,const QString &className, const QStringList &classProperties);
    QList <QVariant> wmiVariantList;
    void setWmiNamespace(const QString &wmiNamespace);
    void setClassName(const QString &className);
    void setClassProperty(const QStringList &classProperties);

    void setConditional(const QString &conditional); //see WQL SQL for WMI)
    void setupNotfication(const QString &wmiNamespace,const QString &className, const QStringList &classProperties);
    static QVariant  msVariantToQVariant(VARIANT msVariant, CIMTYPE variantType);

    static  WMIHelper *instance();
    void emitNotificationArrived();

Q_SIGNALS:
    void wminotificationArrived();

private:
    IWbemLocator *wbemLocator;
    IWbemServices *wbemServices;
    IWbemClassObject *wbemCLassObject;
    IEnumWbemClassObject *wbemEnumerator;

    QString m_className;
    QStringList m_classProperties;
    QString m_conditional;
    QString m_wmiNamespace;
    void initializeWMI(const QString &wmiNamespace);

    bool initialized;
};

class Q_SYSTEMINFO_PRIVATE_EXPORT EventSink : public IWbemObjectSink
{
    LONG m_lRef;
    bool bDone;

public:
    EventSink() { m_lRef = 0; }
    ~EventSink() { bDone = true; }

    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    virtual HRESULT
            STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

    virtual HRESULT STDMETHODCALLTYPE Indicate(
            LONG lObjectCount,
            IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray);

    virtual HRESULT STDMETHODCALLTYPE SetStatus(
            LONG lFlags,
            HRESULT hResult,
            BSTR strParam,
            IWbemClassObject __RPC_FAR *pObjParam);

};

QT_END_NAMESPACE

#endif // !defined (Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
#endif // WMIHELPER_H