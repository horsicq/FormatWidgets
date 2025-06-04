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
#include "xstructwidget.h"
#include "ui_xstructwidget.h"

XStructWidget::XStructWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XStructWidget)
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

    ui->widgetGlobalHex->setProperty("TYPE", "GLOBALHEX");
    ui->widgetGlobalHex->hide();
    ui->widgetGlobalHex->setEnabled(false);

    ui->toolButtonGlobalHex->hide();
    ui->checkBoxHexSync->setChecked(true);
    ui->checkBoxHexSync->hide();
}

XStructWidget::~XStructWidget()
{
    delete ui;
}

void XStructWidget::setData(QIODevice *pDevice, XInfoDB *pInfoDB, const OPTIONS &options)
{
    g_pDevice = pDevice;
    g_pInfoDB = pInfoDB;
    g_options = options;

    setGlobalHexEnable(g_options.bGlobalHexEnable);

    g_options.fileType = XFormats::setFileTypeComboBox(options.fileType, pDevice, ui->comboBoxType);

    reload();
}

QIODevice *XStructWidget::getDevice() const
{
    return g_pDevice;
}

XInfoDB *XStructWidget::getInfoDB() const
{
    return g_pInfoDB;
}

XStructWidget::OPTIONS *XStructWidget::getOptions() const
{
    return const_cast<OPTIONS *>(&g_options);
}

void XStructWidget::setGlobalHexEnable(bool bState)
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

void XStructWidget::clear()
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
    g_mapItems.clear();

    // XFormatWidget::reset();
}

void XStructWidget::cleanup()
{
    XStructWidget::clear();
}

void XStructWidget::reload()
{
    XStructWidget::clear();

    ui->checkBoxReadonly->setEnabled(g_pDevice->isWritable());

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    g_memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, g_pDevice, g_options.bIsImage, g_options.nImageBase);

    XBinary::DATA_HEADERS_OPTIONS dataHeadersOptions = {};
    dataHeadersOptions.pMemoryMap = &g_memoryMap;
    dataHeadersOptions.locType = XBinary::LT_OFFSET;
    dataHeadersOptions.nLocation = 0;
    dataHeadersOptions.nID = 0;
    dataHeadersOptions.bChildren = true;

    g_ListDataHeaders = XFormats::getDataHeaders(fileType, g_pDevice, dataHeadersOptions);  // TODO in Progress Dialog

    qint32 nNumberOfHeaders = g_ListDataHeaders.count();

    g_mapItems.clear();

    for (qint32 i = 0; i < nNumberOfHeaders; i++) {
        XBinary::DATA_HEADER dataHeader = g_ListDataHeaders.at(i);

        QTreeWidgetItem *pParentItem = nullptr;

        if (g_mapItems.contains(dataHeader.dsID_parent.sGUID)) {
            pParentItem = g_mapItems.value(dataHeader.dsID_parent.sGUID);
        } else {
            pParentItem = ui->treeWidgetNavi->invisibleRootItem();
        }

        QTreeWidgetItem *pItem = new QTreeWidgetItem;
        pItem->setText(0, dataHeader.sName);
        pItem->setData(0, Qt::UserRole, dataHeader.dsID.sGUID);

        XOptions::ICONTYPE iconType = getIconType(dataHeader.dsID.fileType, dataHeader.dsID.nID);

        XOptions::adjustTreeWidgetItem(pItem, iconType);

        pParentItem->addChild(pItem);

        g_mapItems.insert(dataHeader.dsID.sGUID, pItem);
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
        XStructWidget::reloadData(false);
    }

    // setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
}

void XStructWidget::setReadonly(bool bState)
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

void XStructWidget::adjustView()
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

void XStructWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    QString sGUID = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole).toString();

    if (!g_stWidgets.contains(sGUID)) {
        XShortcutsWidget *pWidget = createWidget(sGUID);
        if (pWidget) {
            pWidget->setGlobal(getShortcuts(), getGlobalOptions());
            pWidget->setReadonly(isReadonly());
            connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
            connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
            connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));

            //         if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) ||
            //             (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE_HEX) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEX) ||
            //             (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DISASM)) {
            //             XFormatWidget *_pXFormatWidget = dynamic_cast<XFormatWidget *>(pWidget);

            //             _pXFormatWidget->setXInfoDB(cwOptions.pXInfoDB);

            //             if (_pXFormatWidget) {
            //                 connect(_pXFormatWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
            //             }
            //         }

            pWidget->reloadData(false);
            pWidget->setProperty("GUID", sGUID);
            qint32 nPosition = ui->stackedWidgetMain->addWidget(pWidget);
            ui->stackedWidgetMain->setCurrentIndex(nPosition);
            g_stWidgets.insert(sGUID);
        }
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
    // this = this;
    // dataHeader.dsID_parent.fileType = getFileType();
    // cwOptions.memoryMap = getMemoryMap();
    // cwOptions._type = (XFW_DEF::TYPE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_TYPE).toInt());
    // cwOptions.widgetMode = (XFW_DEF::WIDGETMODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_WIDGETMODE).toInt());
    // g_pDevice = getDevice();
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

void XStructWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);

    ui->widgetGlobalHex->setGlobal(pShortcuts, pXOptions);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setGlobal(pShortcuts, pXOptions);
        }
    }
}

XShortcutsWidget *XStructWidget::createWidget(const QString &sGUID)
{
    XShortcutsWidget *pResult = nullptr;

    XBinary::DATA_HEADER dataHeader = searchDataHeaderByGuid(sGUID);

    if (dataHeader.dsID.sGUID == sGUID) {
        if (dataHeader.dhMode == XBinary::DHMODE_HEADER) {
            XGenericHeaderWidget *_pWidget = new XGenericHeaderWidget(this);

            XBinary::DATA_RECORDS_OPTIONS dataRecordsOptions = {};
            dataRecordsOptions.pMemoryMap = &g_memoryMap;
            dataRecordsOptions.dataHeader = dataHeader;

            _pWidget->setData(g_pDevice, g_pInfoDB, dataRecordsOptions);
            pResult = _pWidget;
        } else if (dataHeader.dhMode == XBinary::DHMODE_TABLE) {
            XGenericTableWidget *_pWidget = new XGenericTableWidget(this);

            XBinary::DATA_RECORDS_OPTIONS dataRecordsOptions = {};
            dataRecordsOptions.pMemoryMap = &g_memoryMap;
            dataRecordsOptions.dataHeader = dataHeader;

            _pWidget->setData(g_pDevice, g_pInfoDB, dataRecordsOptions);
            pResult = _pWidget;
        } else {
#ifdef QT_DEBUG
            qDebug("XStructWidget::createWidget: Unknown ID=%d", dataHeader.dsID.nID);
#endif
        }
    }

    return pResult;
}

// XShortcutsWidget *XStructWidget::createWidget(const XFW_DEF::CWOPTIONS &cwOptions)
// {
//     XShortcutsWidget *pResult = nullptr;

//     if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DIALOG_HEADER)) {
//         XGenericHeaderWidget *_pWidget = new XGenericHeaderWidget(this);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) {
//         XGenericTableWidget *_pWidget = new XGenericTableWidget(this);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE_HEX) {
//         XGenericTableHexWidget *_pWidget = new XGenericTableHexWidget(this);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEX) {
//         XGenericHexWidget *_pWidget = new XGenericHexWidget(this);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DISASM) {
//         XGenericDisasmWidget *_pWidget = new XGenericDisasmWidget(this);
//         _pWidget->setCwOptions(cwOptions, false);
//         pResult = _pWidget;
//     }

//     return pResult;
// }

QTreeWidget *XStructWidget::getTreeWidgetNavi()
{
    return ui->treeWidgetNavi;
}

XShortcutsWidget *XStructWidget::getCurrentWidget()
{
    return dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->currentWidget());
}

XHexView *XStructWidget::getGlobalHexView()
{
    return ui->widgetGlobalHex;
}

bool XStructWidget::isGlobalHexSyncEnabled()
{
    return (ui->widgetGlobalHex->isEnabled()) && (ui->toolButtonGlobalHex->isChecked()) && (ui->checkBoxHexSync->isChecked());
}

XOptions::ICONTYPE XStructWidget::getIconType(XBinary::FT fileType, quint64 nID)
{
    XOptions::ICONTYPE iconType = XOptions::ICONTYPE_NONE;

    if (nID == 0) {
        iconType = XOptions::ICONTYPE_INFO;
    } else if (fileType == XBinary::FT_BINARY) {
        if (nID == XBinary::STRUCTID_NFDSCAN) {
            iconType = XOptions::ICONTYPE_NFD;
        } else if (nID == XBinary::STRUCTID_DIESCAN) {
            iconType = XOptions::ICONTYPE_DIE;
        } else if (nID == XBinary::STRUCTID_YARASCAN) {
            iconType = XOptions::ICONTYPE_YARA;
        } else if (nID == XBinary::STRUCTID_VIRUSTOTALSCAN) {
            iconType = XOptions::ICONTYPE_VIRUSTOTAL;
        } else if (nID == XBinary::STRUCTID_VISUALIZATION) {
            iconType = XOptions::ICONTYPE_VISUALIZATION;
        } else if (nID == XBinary::STRUCTID_MEMORYMAP) {
            iconType = XOptions::ICONTYPE_MEMORYMAP;
        } else if (nID == XBinary::STRUCTID_ENTROPY) {
            iconType = XOptions::ICONTYPE_ENTROPY;
        } else if (nID == XBinary::STRUCTID_DISASM) {
            iconType = XOptions::ICONTYPE_DISASM;
        } else if (nID == XBinary::STRUCTID_HEX) {
            iconType = XOptions::ICONTYPE_HEX;
        } else if (nID == XBinary::STRUCTID_HASH) {
            iconType = XOptions::ICONTYPE_HASH;
        } else if (nID == XBinary::STRUCTID_STRINGS) {
            iconType = XOptions::ICONTYPE_STRING;
        } else if (nID == XBinary::STRUCTID_SIGNATURES) {
            iconType = XOptions::ICONTYPE_SIGNATURE;
        } else if (nID == XBinary::STRUCTID_REGIONS) {
            iconType = XOptions::ICONTYPE_SEGMENT;
        } else if (nID == XBinary::STRUCTID_SYMBOLS) {
            iconType = XOptions::ICONTYPE_SYMBOL;
        } else if (nID == XBinary::STRUCTID_EXTRACTOR) {
            iconType = XOptions::ICONTYPE_EXTRACTOR;
        } else if (nID == XBinary::STRUCTID_SEARCH) {
            iconType = XOptions::ICONTYPE_SEARCH;
        } else if (nID == XBinary::STRUCTID_ENTROPY) {
            iconType = XOptions::ICONTYPE_ENTROPY;
        } else if (nID == XBinary::STRUCTID_UNKNOWN) {
        }
    }

    return iconType;
}

XBinary::DATA_HEADER XStructWidget::searchDataHeaderByGuid(const QString &sGUID)
{
    XBinary::DATA_HEADER dataHeader = {};

    if (g_ListDataHeaders.count() > 0) {
        dataHeader = XBinary::_searchDataHeaderByGuid(sGUID, g_ListDataHeaders);
    }

    return dataHeader;
}

void XStructWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        // addPage(pItemCurrent);
        // ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        // ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void XStructWidget::on_toolButtonReload_clicked()
{
    ui->toolButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void XStructWidget::enableButton()
{
    ui->toolButtonReload->setEnabled(true);
}

void XStructWidget::on_toolButtonPrev_clicked()
{
    // setAddPageEnabled(false);
    // ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    // setAddPageEnabled(true);
}

void XStructWidget::on_toolButtonNext_clicked()
{
    // setAddPageEnabled(false);
    // ui->treeWidgetNavi->setCurrentItem(getNextPage());
    // setAddPageEnabled(true);
}

void XStructWidget::on_toolButtonGlobalHex_toggled(bool bChecked)
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

void XStructWidget::on_checkBoxReadonly_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    setReadonly(ui->checkBoxReadonly->isChecked());
}

void XStructWidget::dataChangedSlot(qint64 nOffset, qint64 nSize)
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

void XStructWidget::currentLocationChangedSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    QString sType = sender()->property("TYPE").toString();
    if (sType != "GLOBALHEX") {
        if (g_options.bGlobalHexEnable) {
            if (isGlobalHexSyncEnabled()) {
                ui->widgetGlobalHex->goToLocation(nLocation, (XBinary::LT)nLocationType, true, false, false);
                ui->widgetGlobalHex->setLocationOffset(nLocation, (XBinary::LT)nLocationType, nSize);
            }
        }
    }
}

void XStructWidget::showCwWidgetSlot(const QString &sInitString, bool bNewWindow)
{
    // XFW_DEF::CWOPTIONS cwOptions = {};
    // this = this;
    // dataHeader.dsID_parent.fileType = getFileType();
    // cwOptions._type = _getTypeFromInitString(sInitString);
    // cwOptions.widgetMode = XFW_DEF::WIDGETMODE_DIALOG_HEADER;
    // g_pDevice = getDevice();
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

void XStructWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reload();
}

void XStructWidget::showDemangleSlot(const QString &sString)
{
    DialogDemangle dialogDemangle(this, sString);
    dialogDemangle.setGlobal(getShortcuts(), getGlobalOptions());

    dialogDemangle.exec();
}

void XStructWidget::findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    getTreeWidgetNavi()->setCurrentItem(g_mapItems.value(XBinary::_searchDataHeaderById(XBinary::FT_BINARY, XBinary::STRUCTID_SEARCH, g_ListDataHeaders).dsID.sGUID));
    SearchValuesWidget *pWidget = dynamic_cast<SearchValuesWidget *>(getCurrentWidget());

    if (pWidget) {
        pWidget->findValue(nValue, endian);
    }
}

void XStructWidget::followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    if (nWidgetType == XOptions::WIDGETTYPE_HEX) {
        if (isGlobalHexSyncEnabled()) {
            getGlobalHexView()->setLocation(nLocation, nLocationType, nSize);
        } else {
            getTreeWidgetNavi()->setCurrentItem(
                g_mapItems.value(XBinary::_searchDataHeaderById(XBinary::FT_BINARY, XBinary::STRUCTID_HEX, g_ListDataHeaders).dsID.sGUID));
            getCurrentWidget()->setLocation(nLocation, nLocationType, nSize);
        }
    } else if (nWidgetType == XOptions::WIDGETTYPE_DISASM) {
        getTreeWidgetNavi()->setCurrentItem(g_mapItems.value(XBinary::_searchDataHeaderById(XBinary::FT_BINARY, XBinary::STRUCTID_DISASM, g_ListDataHeaders).dsID.sGUID));
        getCurrentWidget()->setLocation(nLocation, nLocationType, nSize);
    } else if (nWidgetType == XOptions::WIDGETTYPE_MEMORYMAP) {
        getTreeWidgetNavi()->setCurrentItem(g_mapItems.value(XBinary::_searchDataHeaderById(XBinary::FT_BINARY, XBinary::STRUCTID_SEARCH, g_ListDataHeaders).dsID.sGUID));
        getCurrentWidget()->setLocation(nLocation, nLocationType, nSize);
    } else {
#ifdef QT_DEBUG
        qDebug("XStructWidget::followLocationSlot: Unknown widgetType=%d", nWidgetType);
#endif
    }
}
