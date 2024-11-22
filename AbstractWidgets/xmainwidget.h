/* Copyright (c) 2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XMAINWIDGET_H
#define XMAINWIDGET_H

#include "formatwidget.h"
#include "genericheaderwidget.h"
#include "generictablewidget.h"

namespace Ui {
class XMainWidget;
}

class XMainWidget : public FormatWidget {
    Q_OBJECT

public:
    explicit XMainWidget(QWidget *pParent = nullptr);
    XMainWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent = nullptr);
    ~XMainWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

    virtual SV _setValue(QVariant vValue, qint32 nPosition);
    virtual void setReadonly(bool bState);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

    static void _addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType);
    static void _addSpecItems(QTreeWidget *pTreeWidget, QIODevice *pDevice, XBinary::FT fileType, bool bIsImage, XADDR nImageBase);
    static XShortcutsWidget *createWidget(const FW_DEF::CWOPTIONS &cwOptions);

private slots:
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious);
    void on_toolButtonReload_clicked();
    void enableButton();
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void on_toolButtonGlobalHex_toggled(bool bChecked);
    void on_checkBoxReadonly_stateChanged(int nArg);
    void dataChangedSlot(qint64 nOffset, qint64 nSize);
    void currentLocationChangedSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize);

private:
    Ui::XMainWidget *ui;
    XBinary::FT g_fileType;
};

#endif  // XMAINWIDGET_H
