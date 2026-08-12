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

#ifndef XFWIDGET_TABLE_H
#define XFWIDGET_TABLE_H

#include <QWidget>
#include "xdemangle.h"
#include "xftableview.h"
#include "xformats.h"
#include "xmodel_ximport.h"
#include "xmodel_xexport.h"
#include "xmodel_xsymbol.h"
#include "xmodel_xresource.h"

namespace Ui {
class XFWidget_Table;
}

class XShortcuts;

class XFWidget_Table : public QWidget {
    Q_OBJECT

public:
    explicit XFWidget_Table(QWidget *pParent = nullptr);
    virtual ~XFWidget_Table();

    void setData(const XBinary::INDATA &inData, const XBinary::XFHEADER &xfHeader);
    void setData(const XBinary::INDATA &inData, const QVector<XBinary::XIMPORT_STRUCT> &listImports);
    void setData(const XBinary::INDATA &inData, const QVector<XBinary::XEXPORT_STRUCT> &listExports);
    void setData(const XBinary::INDATA &inData, const QVector<XBinary::XSYMBOL_STRUCT> &listSymbols);
    void setData(const XBinary::INDATA &inData, const QVector<XBinary::XRESOURCE_STRUCT> &listResources);
    void clear();

    // Not an XShortcutsWidget; this only forwards the table font to the inner XFTableView.
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

    void setReadonly(bool bIsReadonly);

    XFTableView *getTableView();

signals:
    void fieldSelected(qint32 nFieldIndex, QVariant value, const XBinary::XFRECORD &xfRecord);
    void fieldDoubleClicked(qint32 nFieldIndex, QVariant value, const XBinary::XFRECORD &xfRecord);

private slots:
    void onSaveClicked();
    void onShowOffsetsToggled(bool bChecked);
    void onShowPresentationToggled(bool bChecked);
    void onDemangleModeChanged(int nIndex);

private:
    enum LISTTYPE {
        LISTTYPE_NONE = 0,
        LISTTYPE_IMPORT,
        LISTTYPE_EXPORT,
        LISTTYPE_SYMBOL,
        LISTTYPE_RESOURCE
    };

    void setListMode(const QString &sTag, bool bDemanglable);
    void _applyDemangle();

    Ui::XFWidget_Table *ui;
    XBinary::INDATA m_inData;
    QString m_sCurrentTag;
    bool m_bIsReadonly;
    LISTTYPE m_listType;
    XDemangle m_demangle;

    // The "*Raw" copies keep the original mangled names; the plain lists are the
    // display copies bound to the model (rewritten when the demangle mode changes).
    QVector<XBinary::XIMPORT_STRUCT> m_listImports;
    QVector<XBinary::XIMPORT_STRUCT> m_listImportsRaw;
    QVector<XBinary::XEXPORT_STRUCT> m_listExports;
    QVector<XBinary::XEXPORT_STRUCT> m_listExportsRaw;
    QVector<XBinary::XSYMBOL_STRUCT> m_listSymbols;
    QVector<XBinary::XSYMBOL_STRUCT> m_listSymbolsRaw;
    QVector<XBinary::XRESOURCE_STRUCT> m_listResources;
};

#endif  // XFWIDGET_TABLE_H
