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
#include "xmainwidget.h"
#include "ui_xmainwidget.h"

XMainWidget::XMainWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XMainWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;
    g_pInfoDB = nullptr;
    g_options = {};
    g_memoryMap = {};

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonNext, XOptions::ICONTYPE_FORWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonPrev, XOptions::ICONTYPE_BACKWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonGlobalHex, XOptions::ICONTYPE_HEX);

    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonNext->setToolTip(tr("Next visited"));
    ui->toolButtonPrev->setToolTip(tr("Previous visited"));
    ui->toolButtonGlobalHex->setToolTip(tr("Hex"));
    ui->checkBoxReadonly->setToolTip(tr("Readonly"));

    // ui->widgetGlobalHex->setProperty("TYPE", XFW_DEF::TYPE_GLOBALHEX);
    ui->widgetGlobalHex->hide();
    ui->widgetGlobalHex->setEnabled(false);

    ui->toolButtonGlobalHex->hide();
    ui->checkBoxHexSync->setChecked(true);
    ui->checkBoxHexSync->hide();
}

XMainWidget::~XMainWidget()
{
    delete ui;
}

void XMainWidget::setData(QIODevice *pDevice, XInfoDB *pInfoDB, const OPTIONS &options)
{
    g_pDevice = pDevice;
    g_pInfoDB = pInfoDB;
    g_options = options;

    setGlobalHexEnable(g_options.bGlobalHexEnable);

    XFormats::setFileTypeComboBox(options.fileType, pDevice, ui->comboBoxType);

    reload();
}

void XMainWidget::setGlobalHexEnable(bool bState)
{
    if (bState) {
        ui->toolButtonGlobalHex->show();
        connect(ui->widgetGlobalHex, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
        connect(ui->widgetGlobalHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
    } else {
        ui->toolButtonGlobalHex->hide();
        disconnect(ui->widgetGlobalHex, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
        disconnect(ui->widgetGlobalHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
    }
}

void XMainWidget::clear()
{
    ui->checkBoxReadonly->setChecked(true);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        QWidget *pWidget = ui->stackedWidgetMain->widget(i);

        if (pWidget) {
            delete pWidget;
        }
    }

    ui->treeWidgetNavi->clear();

    g_stWidgets.clear();

    // XFormatWidget::reset();
}

void XMainWidget::cleanup()
{
    XMainWidget::clear();
}

void XMainWidget::reload()
{
    XMainWidget::clear();

    ui->checkBoxReadonly->setEnabled(g_pDevice->isWritable());

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    g_memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, g_pDevice, g_options.bIsImage, g_options.nImageBase);

    XBinary::DATA_HEADERS_OPTIONS dataHeadersOptions = {};
    dataHeadersOptions.pMemoryMap = &g_memoryMap;
    dataHeadersOptions.locType = XBinary::LT_OFFSET;
    dataHeadersOptions.nLocation = 0;
    dataHeadersOptions.nID = 0;
    dataHeadersOptions.bChildren = true;

    g_ListDataHeaders = XFormats::getDataHeaders(fileType, g_pDevice, dataHeadersOptions, g_options.bIsImage, g_options.nImageBase, nullptr); // TODO in Progress Dialog

    qint32 nNumberOfHeaders = g_ListDataHeaders.count();

    QTreeWidgetItem *pRootItem = ui->treeWidgetNavi->invisibleRootItem();

    for (qint32 i = 0; i < nNumberOfHeaders; i++) {
        XBinary::DATA_HEADER dataHeader = g_ListDataHeaders.at(i);

        QTreeWidgetItem *pItem = new QTreeWidgetItem(ui->treeWidgetNavi);
        pItem->setText(0, dataHeader.sName);
        pItem->setData(0,Qt::UserRole, dataHeader.dsID.sGUID);

        pRootItem->addChild(pItem);
    }


    if (g_options.bGlobalHexEnable) {
        XHexView::OPTIONS options = {};
        // options.memoryMapRegion = _memoryMap;
        ui->widgetGlobalHex->setXInfoDB(g_pInfoDB);
        ui->widgetGlobalHex->setData(g_pDevice, options, true);
        ui->widgetGlobalHex->setBytesProLine(8);
    }

    // if (getOptions().vmode == XFW_DEF::VMODE_FILETYPE) {
    //     QTreeWidgetItem *pItem = _addBaseItems(ui->treeWidgetNavi, fileType);
    //     _addFileType(pItem, getDevice(), getOptions().nOffset, getDevice()->size(), fileType, getOptions().bIsImage, getOptions().nImageBase);
    // } else {
    //     QTreeWidgetItem *pItem = ui->treeWidgetNavi->invisibleRootItem();

    //     SPSTRUCT spStruct = {};
    //     spStruct.pTreeWidgetItem = pItem;
    //     spStruct.pDevice = getDevice();
    //     spStruct.nOffset = getOptions().nOffset;
    //     spStruct.nSize = getDevice()->size();
    //     spStruct.fileType = getOptions().fileType;
    //     spStruct.bIsImage = getOptions().bIsImage;
    //     spStruct.nImageBase = getOptions().nImageBase;
    //     spStruct.endian = getOptions().endian;
    //     spStruct.mode = getOptions().mode;
    //     spStruct.nStructOffset = getOptions().nStructOffset;
    //     spStruct.nStructSize = getOptions().nStructSize;
    //     spStruct.nStructCount = getOptions().nStructCount;
    //     spStruct.widgetMode = getOptions().widgetMode;
    //     spStruct.type = getOptions().type;

    //     _addStruct(spStruct);
    // }

    {
        QList<qint32> listSizes = ui->splitterNavi->sizes();

        // qint32 nWidgetSize = listSizes[0] + listSizes[1];
        listSizes[0] = 250;
        listSizes[1] = 250;
        ui->splitterNavi->setSizes(listSizes);
        ui->splitterNavi->setStretchFactor(0, 0);
        ui->splitterNavi->setStretchFactor(1, 1);
    }

    ui->treeWidgetNavi->expandAll();

    ui->treeWidgetNavi->setCurrentIndex(ui->treeWidgetNavi->model()->index(0, 0));

    if (ui->treeWidgetNavi->currentItem()) {
        XMainWidget::reloadData(false);
    }

    // setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
}

void XMainWidget::setReadonly(bool bState)
{
    // XFormatWidget::setReadonly(bState);

    ui->checkBoxReadonly->setChecked(bState);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setReadonly(bState);
        }
    }

    ui->widgetGlobalHex->setReadonly(bState);
}

void XMainWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustTreeWidget(ui->treeWidgetNavi, XOptions::ID_VIEW_FONT_TREEVIEWS);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->adjustView();
        }
    }

    if (g_options.bGlobalHexEnable) {
        ui->widgetGlobalHex->adjustView();
    }
}

void XMainWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    QString sGUID = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole).toString();

    if (!g_stWidgets.contains(sGUID)) {
        // XShortcutsWidget *pWidget = createWidget(sGUID);
    } else {
        qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QWidget *pWidget = ui->stackedWidgetMain->widget(i);

            if (pWidget->property("GUID").toString() == sGUID) {
                ui->stackedWidgetMain->setCurrentIndex(i);
                break;
            }
        }
    }

    // XFW_DEF::CWOPTIONS cwOptions = {};
    // cwOptions.pParent = this;
    // cwOptions.fileType = getFileType();
    // cwOptions.memoryMap = getMemoryMap();
    // cwOptions._type = (XFW_DEF::TYPE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_TYPE).toInt());
    // cwOptions.widgetMode = (XFW_DEF::WIDGETMODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_WIDGETMODE).toInt());
    // cwOptions.pDevice = getDevice();
    // cwOptions.bIsImage = getOptions().bIsImage;
    // cwOptions.nImageBase = getOptions().nImageBase;
    // cwOptions.pXInfoDB = getXInfoDB();
    // cwOptions.endian = (XBinary::ENDIAN)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_ENDIAN).toLongLong());
    // cwOptions.mode = (XBinary::MODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_MODE).toLongLong());
    // cwOptions.demangleMode = getDemangleMode();
    // cwOptions.nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_OFFSET).toLongLong();
    // cwOptions.nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_SIZE).toLongLong();
    // cwOptions.nDataCount = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_COUNT).toLongLong();
    // cwOptions.var1 = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR1);
    // cwOptions.var2 = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR2);

    // QString sInitString = getInitStringFromCwOptions(&cwOptions);

    // if (isInitPresent(sInitString)) {
    //     qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    //     for (qint32 i = 0; i < nNumberOfWidgets; i++) {
    //         QWidget *pWidget = ui->stackedWidgetMain->widget(i);

    //         // qDebug("pWidget->property(\"INITSTRING\").toString()=%s", pWidget->property("INITSTRING").toString().toLatin1().data());

    //         if (pWidget->property("INITSTRING").toString() == sInitString) {
    //             ui->stackedWidgetMain->setCurrentIndex(i);
    //             break;
    //         }
    //     }
    // } else {
    //     XShortcutsWidget *pWidget = createWidget(cwOptions);

    //     if (pWidget) {
    //         pWidget->setGlobal(getShortcuts(), getGlobalOptions());
    //         pWidget->setReadonly(isReadonly());
    //         connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
    //         connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
    //         connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));

    //         if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) ||
    //             (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE_HEX) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEX) ||
    //             (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DISASM)) {
    //             XFormatWidget *_pXFormatWidget = dynamic_cast<XFormatWidget *>(pWidget);

    //             _pXFormatWidget->setXInfoDB(cwOptions.pXInfoDB);

    //             if (_pXFormatWidget) {
    //                 connect(_pXFormatWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
    //             }
    //         }

    //         pWidget->setProperty("INITSTRING", sInitString);
    //         pWidget->setProperty("TYPE", cwOptions._type);
    //         pWidget->setProperty("WIDGETMODE", cwOptions.widgetMode);
    //         pWidget->reloadData(false);
    //         qint32 nPosition = ui->stackedWidgetMain->addWidget(pWidget);
    //         ui->stackedWidgetMain->setCurrentIndex(nPosition);
    //         addInit(sInitString);
    //     }
    // }
}

void XMainWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    // XFormatWidget::setGlobal(pShortcuts, pXOptions);

    ui->widgetGlobalHex->setGlobal(pShortcuts, pXOptions);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setGlobal(pShortcuts, pXOptions);
        }
    }
}

QTreeWidgetItem *XMainWidget::_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType)
{
    Q_UNUSED(fileType)

    QTreeWidgetItem *pResult = nullptr;
    // pResult = createNewItem(XFW_DEF::TYPE_INFO, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_INFO, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN,
    // "", "");

    pTreeWidget->addTopLevelItem(pResult);

    return pResult;
}

// XShortcutsWidget *XMainWidget::createWidget(const XFW_DEF::CWOPTIONS &cwOptions)
// {
//     XShortcutsWidget *pResult = nullptr;

//     if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DIALOG_HEADER)) {
//         XGenericHeaderWidget *_pWidget = new XGenericHeaderWidget(cwOptions.pParent);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) {
//         XGenericTableWidget *_pWidget = new XGenericTableWidget(cwOptions.pParent);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE_HEX) {
//         XGenericTableHexWidget *_pWidget = new XGenericTableHexWidget(cwOptions.pParent);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEX) {
//         XGenericHexWidget *_pWidget = new XGenericHexWidget(cwOptions.pParent);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DISASM) {
//         XGenericDisasmWidget *_pWidget = new XGenericDisasmWidget(cwOptions.pParent);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     }

//     return pResult;
// }

QTreeWidget *XMainWidget::getTreeWidgetNavi()
{
    return ui->treeWidgetNavi;
}

XShortcutsWidget *XMainWidget::getCurrentWidget()
{
    return dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->currentWidget());
}

XHexView *XMainWidget::getGlobalHexView()
{
    return ui->widgetGlobalHex;
}

bool XMainWidget::isGlobalHexSyncEnabled()
{
    return (ui->widgetGlobalHex->isEnabled()) && (ui->toolButtonGlobalHex->isChecked()) && (ui->checkBoxHexSync->isChecked());
}

void XMainWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        // addPage(pItemCurrent);
        // ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        // ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void XMainWidget::on_toolButtonReload_clicked()
{
    ui->toolButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void XMainWidget::enableButton()
{
    ui->toolButtonReload->setEnabled(true);
}

void XMainWidget::on_toolButtonPrev_clicked()
{
    // setAddPageEnabled(false);
    // ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    // setAddPageEnabled(true);
}

void XMainWidget::on_toolButtonNext_clicked()
{
    // setAddPageEnabled(false);
    // ui->treeWidgetNavi->setCurrentItem(getNextPage());
    // setAddPageEnabled(true);
}

void XMainWidget::on_toolButtonGlobalHex_toggled(bool bChecked)
{
    if (bChecked) {
        ui->widgetGlobalHex->setEnabled(true);
        ui->widgetGlobalHex->show();
        QList<qint32> listSizes = ui->splitterHex->sizes();

        qint32 nWidgetSize = 0;
        nWidgetSize += ui->widgetGlobalHex->getColumnWidth(0);
        nWidgetSize += ui->widgetGlobalHex->getColumnWidth(1);
        nWidgetSize += ui->widgetGlobalHex->getColumnWidth(2);
        nWidgetSize += ui->widgetGlobalHex->getMapWidth();
        nWidgetSize += 20;
        listSizes[0] -= nWidgetSize;
        listSizes[1] += nWidgetSize;
        ui->splitterHex->setSizes(listSizes);
        ui->splitterHex->setStretchFactor(0, 1);
        ui->splitterHex->setStretchFactor(1, 0);

        ui->checkBoxHexSync->show();
    } else {
        ui->widgetGlobalHex->setEnabled(false);
        ui->widgetGlobalHex->hide();

        ui->checkBoxHexSync->hide();
    }
}

void XMainWidget::on_checkBoxReadonly_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    setReadonly(ui->checkBoxReadonly->isChecked());
}

void XMainWidget::dataChangedSlot(qint64 nOffset, qint64 nSize)
{
    Q_UNUSED(nOffset)
    Q_UNUSED(nSize)

    // XFW_DEF::TYPE _typeSend = (XFW_DEF::TYPE)(sender()->property("TYPE").toInt());
    // XFW_DEF::WIDGETMODE _widgetModeSend = (XFW_DEF::WIDGETMODE)(sender()->property("WIDGETMODE").toInt());
    // bool bIsWidgetReload = false;

    // if (_typeSend == XFW_DEF::TYPE_GLOBALHEX) {
    //     bIsWidgetReload = true;
    // } else {
    //     ui->widgetGlobalHex->reloadData(true);
    //     bIsWidgetReload = (_widgetModeSend == XFW_DEF::WIDGETMODE_DIALOG_HEADER);
    // }

    // if (bIsWidgetReload) {
    //     XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->currentWidget());
    //     if (pWidget) {
    //         XFW_DEF::WIDGETMODE _widgetModeRecv = (XFW_DEF::WIDGETMODE)(pWidget->property("WIDGETMODE").toInt());

    //         if (_widgetModeRecv != XFW_DEF::WIDGETMODE_UNKNOWN) {
    //             pWidget->reloadData(true);
    //         }
    //     }
    // }
}

void XMainWidget::currentLocationChangedSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    // XFW_DEF::TYPE _type = (XFW_DEF::TYPE)(sender()->property("TYPE").toInt());
    // if (_type != XFW_DEF::TYPE_GLOBALHEX) {
    //     if (g_bGlobalHexEnable) {
    //         if (isGlobalHexSyncEnabled()) {
    //             ui->widgetGlobalHex->goToLocation(nLocation, (XBinary::LT)nLocationType, true, false, false);
    //             ui->widgetGlobalHex->setLocationOffset(nLocation, (XBinary::LT)nLocationType, nSize);
    //         }
    //     }
    // }
}

void XMainWidget::showCwWidgetSlot(const QString &sInitString, bool bNewWindow)
{
    // XFW_DEF::CWOPTIONS cwOptions = {};
    // cwOptions.pParent = this;
    // cwOptions.fileType = getFileType();
    // cwOptions._type = _getTypeFromInitString(sInitString);
    // cwOptions.widgetMode = XFW_DEF::WIDGETMODE_DIALOG_HEADER;
    // cwOptions.pDevice = getDevice();
    // cwOptions.bIsImage = getOptions().bIsImage;
    // cwOptions.nImageBase = getOptions().nImageBase;
    // cwOptions.pXInfoDB = getXInfoDB();
    // cwOptions.endian = getEndian();
    // cwOptions.mode = getMode();
    // cwOptions.demangleMode = getDemangleMode();
    // cwOptions.memoryMap = getMemoryMap();
    // cwOptions.nDataOffset = _getDataOffsetFromInitString(sInitString);
    // cwOptions.nDataSize = _getDataSizeFromInitString(sInitString);
    // cwOptions.nDataCount = _getDataCountFromInitString(sInitString);

    // XShortcutsWidget *pWidget = createWidget(cwOptions);

    // if (pWidget) {
    //     pWidget->setGlobal(getShortcuts(), getGlobalOptions());
    //     pWidget->setReadonly(isReadonly());
    //     connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
    //     connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
    //     connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));

    //     if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) ||
    //         (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE_HEX) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEX) ||
    //         (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DISASM)) {
    //         XFormatWidget *_pXFormatWidget = dynamic_cast<XFormatWidget *>(pWidget);

    //         _pXFormatWidget->setXInfoDB(cwOptions.pXInfoDB);

    //         if (_pXFormatWidget) {
    //             connect(_pXFormatWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
    //         }
    //     }

    //     pWidget->setProperty("INITSTRING", sInitString);
    //     pWidget->setProperty("TYPE", cwOptions._type);
    //     pWidget->setProperty("WIDGETMODE", cwOptions.widgetMode);
    //     pWidget->reloadData(false);

    //     DialogWidget dialogWidget(this);
    //     dialogWidget.setGlobal(getShortcuts(), getGlobalOptions());
    //     dialogWidget.setModal(true);
    //     dialogWidget.addWidget(pWidget);

    //     dialogWidget.exec();
    // }
}
