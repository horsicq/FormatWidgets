/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#ifndef XSTRUCTWIDGET_H
#define XSTRUCTWIDGET_H

#include <QTreeWidget>
#include "dialogdemangle.h"
#include "dialogwidget.h"
#include "xoptions.h"
#include "xshortcutsdialog.h"
#include "xtableview.h"
#include "xgenericheaderwidget.h"
#include "xgenerictablewidget.h"
#include "xhexview.h"

namespace Ui {
class XStructWidget;
}

class XStructWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        bool bIsImage;
        XADDR nImageBase;
        bool bGlobalHexEnable;
        XBinary::FT fileType;
    };

    explicit XStructWidget(QWidget *pParent = nullptr);
    ~XStructWidget();

    void setData(QIODevice *pDevice, XInfoDB *pInfoDB = nullptr, const OPTIONS &options = OPTIONS());

    QIODevice *getDevice() const;
    XInfoDB *getInfoDB() const;
    OPTIONS *getOptions() const;

    void setGlobalHexEnable(bool bState);

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

    virtual void setReadonly(bool bState);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);
    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

    virtual XShortcutsWidget *createWidget(const QString &sGUID);
    QTreeWidget *getTreeWidgetNavi();
    XShortcutsWidget *getCurrentWidget();
    XHexView *getGlobalHexView();
    bool isGlobalHexSyncEnabled();
    XOptions::ICONTYPE getIconType(XBinary::FT fileType, quint64 nID);

    XBinary::DATA_HEADER searchDataHeaderByGuid(const QString &sGUID);

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
    void showCwWidgetSlot(const QString &sInitString, bool bNewWindow);
    void on_comboBoxType_currentIndexChanged(int nIndex);
    void showDemangleSlot(const QString &sString);
    void findValue(quint64 nValue, XBinary::ENDIAN endian);
    void followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType);

private:
    Ui::XStructWidget *ui;
    QIODevice *m_pDevice;
    XInfoDB *g_pInfoDB;
    OPTIONS g_options;
    XBinary::_MEMORY_MAP g_memoryMap;
    QList<XBinary::DATA_HEADER> g_ListDataHeaders;
    QSet<QString> g_stWidgets;
    QMap<QString, QTreeWidgetItem *> g_mapItems;
};

#endif  // XSTRUCTWIDGET_H
