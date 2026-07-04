/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef XFWIDGETADVANCED_H
#define XFWIDGETADVANCED_H

#include <QMap>
#include <QString>
#include "xftreeview.h"
#include "xftableview.h"
#include "xformats.h"
#include "xshortcutswidget.h"

namespace Ui {
class XFWidgetAdvanced;
}

class XFWidgetAdvanced : public XShortcutsWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        qint32 nDummy;
    };

    explicit XFWidgetAdvanced(QWidget *pParent = nullptr);
    ~XFWidgetAdvanced();

    void setData(const XFormats::INDATA &inData, const OPTIONS &options);
    void clear();

    void reload();
    void reloadFileType();

    void setReadonly(bool bIsReadonly);

    virtual void adjustView();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    virtual void reloadData(bool bSaveSelection);

signals:
    void headerSelected(const XBinary::XFHEADER &xfHeader);
    void fieldSelected(qint32 nFieldIndex, QVariant value, const XBinary::XFRECORD &xfRecord);
    void fieldDoubleClicked(qint32 nFieldIndex, QVariant value, const XBinary::XFRECORD &xfRecord);

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void onHeaderSelected(const XBinary::XFHEADER &xfHeader);
    void on_toolButtonReload_clicked();
    void on_comboBoxFileType_currentIndexChanged(int nIndex);

    QWidget *getOrCreateWidget(const QString &sName, const XFormats::INDATA &inData, const XBinary::XFHEADER &xfHeader);

private:
    Ui::XFWidgetAdvanced *ui;
    XFormats::INDATA m_inData;
    OPTIONS m_options;
    QMap<QString, QWidget *> m_mapWidgets;
    QList<QString> m_lruOrder;
    bool m_bIsReadonly;
};

#endif  // XFWIDGETADVANCED_H
