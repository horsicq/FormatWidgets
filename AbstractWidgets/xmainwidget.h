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

#include <QTreeWidget>

#include "xgenericheaderwidget.h"
#include "xgenerictablewidget.h"
#include "xgenerictablehexwidget.h"
#include "xgenerichexwidget.h"
#include "xgenericdisasmwidget.h"
#include "xformatwidget.h"
#include "dialogdemangle.h"
#include "dialogwidget.h"
#include "xoptions.h"
#include "xshortcutsdialog.h"
#include "xtableview.h"
#include "xfileinfowidget.h"
#include "xhexviewwidget.h"

namespace Ui {
class XMainWidget;
}

class XMainWidget : public XFormatWidget {
    Q_OBJECT

public:
    explicit XMainWidget(QWidget *pParent = nullptr);
    ~XMainWidget();

    void setGlobalHexEnable(bool bState);

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

    virtual void setReadonly(bool bState);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);
    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

    virtual QTreeWidgetItem *_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType);
    virtual XShortcutsWidget *createWidget(const XFW_DEF::CWOPTIONS &cwOptions);
    QTreeWidget *getTreeWidgetNavi();
    XShortcutsWidget *getCurrentWidget();
    XHexView *getGlobalHexView();
    bool isGlobalHexSyncEnabled();

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
    void showCwWidgetSlot(QString sInitString, bool bNewWindow);

private:
    Ui::XMainWidget *ui;
    bool g_bGlobalHexEnable;
};

#endif  // XMAINWIDGET_H
