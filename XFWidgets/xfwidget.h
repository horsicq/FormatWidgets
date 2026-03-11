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

#ifndef XFWIDGET_H
#define XFWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include "xftreeview.h"
#include "xftableview.h"
#include "xbinary.h"
#include "xfmodel.h"

class XFWidget : public QWidget {
    Q_OBJECT

public:
    explicit XFWidget(QWidget *pParent = nullptr);
    virtual ~XFWidget();

    void setData(XBinary *pXBinary, const QList<XBinary::XFHEADER> &listHeaders);
    void clear();

    void setReadonly(bool bIsReadonly);

    XFTreeView *getTreeView();
    XFTableView *getTableView();

signals:
    void headerSelected(const XBinary::XFHEADER &xfHeader);
    void fieldSelected(qint32 nFieldIndex, quint64 nValue, const XBinary::XFRECORD &xfRecord);
    void fieldDoubleClicked(qint32 nFieldIndex, quint64 nValue, const XBinary::XFRECORD &xfRecord);

private slots:
    void onHeaderSelected(const XBinary::XFHEADER &xfHeader);
    void onSaveClicked();
    void onShowOffsetsToggled(bool bChecked);
    void onShowPresentationToggled(bool bChecked);

private:
    XBinary *m_pXBinary;
    QSplitter *m_pSplitter;
    XFTreeView *m_pTreeView;
    XFTableView *m_pTableView;
    QWidget *m_pTablePanel;
    QPushButton *m_pPushButtonSave;
    QLineEdit *m_pLineEditTag;
    QWidget *m_pToolBar;
    QCheckBox *m_pCheckBoxShowOffsets;
    QCheckBox *m_pCheckBoxShowPresentation;
    QString m_sCurrentTag;
    QMap<QString, XBinary::XFHEADER> m_mapHeaders;
    bool m_bIsReadonly;
};

#endif  // XFWIDGET_H
