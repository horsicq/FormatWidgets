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

    ui->widgetGlobalHex->setProperty("TYPE", "GLOBALHEX");
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
    g_mapItems.clear();

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
        pItem->setData(0,Qt::UserRole, dataHeader.dsID.sGUID);

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

void XMainWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
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

XShortcutsWidget *XMainWidget::createWidget(const QString &sGUID)
{
    XShortcutsWidget *pResult = nullptr;

    XBinary::DATA_HEADER dataHeader = XBinary::_searchDataHeaderByGuid(sGUID, g_ListDataHeaders);

    if (dataHeader.dsID.sGUID == sGUID) {
        if (dataHeader.dsID.nID == 0) {
            XFileInfoWidget *_pWidget = new XFileInfoWidget(this);
            _pWidget->setData(g_pDevice, dataHeader.dsID.fileType, dataHeader.sName, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_NFDSCAN)) {
            NFDWidgetAdvanced *_pWidget = new NFDWidgetAdvanced(this);
            _pWidget->setData(g_pDevice, false, dataHeader.dsID_parent.fileType);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_DIESCAN)) {
            DIEWidgetAdvanced *_pWidget = new DIEWidgetAdvanced(this);
            _pWidget->setData(g_pDevice, false, dataHeader.dsID_parent.fileType);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_YARASCAN)) {
            YARAWidgetAdvanced *_pWidget = new YARAWidgetAdvanced(this);
            _pWidget->setData(XBinary::getDeviceFileName(g_pDevice), false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_VIRUSTOTALSCAN)) {
            XVirusTotalWidget *_pWidget = new XVirusTotalWidget(this);
            _pWidget->setData(g_pDevice);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_VISUALIZATION)) {
            XVisualizationWidget *_pWidget = new XVisualizationWidget(this);
            _pWidget->setData(g_pDevice, dataHeader.dsID_parent.fileType, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_HEX)) {
            XHexViewWidget *_pWidget = new XHexViewWidget(this);
            XHexViewWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bMenu_Disasm = true;
            options.bMenu_MemoryMap = true;
            _pWidget->setXInfoDB(g_pInfoDB);
            _pWidget->setData(g_pDevice, options);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_DISASM)) {
            XMultiDisasmWidget *_pWidget = new XMultiDisasmWidget(this);
            XMultiDisasmWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.nInitAddress = XFormats::getEntryPointAddress(dataHeader.dsID_parent.fileType, g_pDevice, g_options.bIsImage, g_options.nImageBase);
            options.bMenu_Hex = true;
            _pWidget->setXInfoDB(g_pInfoDB);
            _pWidget->setData(g_pDevice, options);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_HASH)) {
            XHashWidget *_pWidget = new XHashWidget(this);
            _pWidget->setData(g_pDevice, dataHeader.dsID_parent.fileType, 0, g_pDevice->size(), false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_STRINGS)) {
            SearchStringsWidget *_pWidget = new SearchStringsWidget(this);
            SearchStringsWidget::OPTIONS stringsOptions = {};
            stringsOptions.bMenu_Hex = true;
            stringsOptions.bMenu_Disasm = true;
            stringsOptions.bMenu_Demangle = true;
            stringsOptions.bAnsi = true;
            stringsOptions.bUnicode = true;
            stringsOptions.bNullTerminated = false;
            _pWidget->setData(g_pDevice, dataHeader.dsID_parent.fileType, stringsOptions, false);

            connect(_pWidget, SIGNAL(showDemangle(QString)), this, SLOT(showDemangleSlot(QString)));

            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_SIGNATURES)) {
            SearchSignaturesWidget *_pWidget = new SearchSignaturesWidget(this);
            SearchSignaturesWidget::OPTIONS signaturesOptions = {};
            signaturesOptions.bMenu_Hex = true;
            signaturesOptions.bMenu_Disasm = true;
            signaturesOptions.fileType = dataHeader.dsID_parent.fileType;
            _pWidget->setData(g_pDevice, signaturesOptions, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_REGIONS)) {
            XRegionsWidget *_pWidget = new XRegionsWidget(this);
            XRegionsModel::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bIsImage = g_options.bIsImage;
            options.nModuleAddress = g_options.nImageBase;
            _pWidget->setData(g_pDevice, g_pInfoDB, options);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_MEMORYMAP)) {
            XMemoryMapWidget *_pWidget = new XMemoryMapWidget(this);
            XMemoryMapWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bIsSearchEnable = true;
            _pWidget->setData(g_pDevice, options, g_pInfoDB);

            connect(_pWidget, SIGNAL(findValue(quint64, XBinary::ENDIAN)), this, SLOT(findValue(quint64, XBinary::ENDIAN)));

            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_SYMBOLS)) {
            XSymbolsWidget *_pWidget = new XSymbolsWidget(this);
            XSymbolsWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.symbolMode = XInfoDB::SYMBOL_MODE_ALL;
            options.bMenu_Disasm = true;
            options.bMenu_Hex = true;
            _pWidget->setData(g_pDevice, options, g_pInfoDB, true);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_ENTROPY)) {
            XEntropyWidget *_pWidget = new XEntropyWidget(this);
            _pWidget->setData(g_pDevice, 0, g_pDevice->size(), dataHeader.dsID_parent.fileType, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_EXTRACTOR)) {
            XExtractorWidget *_pWidget = new XExtractorWidget(this);
            XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
            extractorOptions.fileType = dataHeader.dsID_parent.fileType;
            extractorOptions.bMenu_Hex = true;
            _pWidget->setData(g_pDevice, extractorOptions, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_SEARCH)) {
            SearchValuesWidget *_pWidget = new SearchValuesWidget(this);
            SearchValuesWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bMenu_Hex = true;
            options.bMenu_Disasm = true;
            _pWidget->setData(g_pDevice, options);
            pResult = _pWidget;
        } else if (dataHeader.dhMode == XBinary::DHMODE_HEADER) {
            XGenericHeaderWidget *_pWidget = new XGenericHeaderWidget(this);
            // TODO
            pResult = _pWidget;
        } else {
#ifdef QT_DEBUG
            qDebug("XMainWidget::createWidget: Unknown ID=%d", dataHeader.dsID.nID);
#endif
        }
    }

    return pResult;
}

// XShortcutsWidget *XMainWidget::createWidget(const XFW_DEF::CWOPTIONS &cwOptions)
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

XOptions::ICONTYPE XMainWidget::getIconType(XBinary::FT fileType, quint64 nID)
{
    XOptions::ICONTYPE iconType = XOptions::ICONTYPE_NONE;

    if (nID == 0) {
        iconType = XOptions::ICONTYPE_INFO;
    } else if (fileType == XBinary::FT_BINARY) {
        if (nID == XBinary::STRUCTID_MEMORYMAP) {
            iconType = XOptions::ICONTYPE_MEMORYMAP;
        }
    }

    return iconType;
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

void XMainWidget::showCwWidgetSlot(const QString &sInitString, bool bNewWindow)
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

void XMainWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reload();
}

void XMainWidget::showDemangleSlot(const QString &sString)
{
    DialogDemangle dialogDemangle(this, sString);
    dialogDemangle.setGlobal(getShortcuts(), getGlobalOptions());

    dialogDemangle.exec();
}

void XMainWidget::findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    getTreeWidgetNavi()->setCurrentItem(g_mapItems.value(XBinary::_searchDataHeaderById(XBinary::FT_BINARY, XBinary::STRUCTID_SEARCH, g_ListDataHeaders).dsID.sGUID));
    SearchValuesWidget *pWidget = dynamic_cast<SearchValuesWidget *>(getCurrentWidget());

    if (pWidget) {
        pWidget->findValue(nValue, endian);
    }
}

void XMainWidget::followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    if (nWidgetType == XOptions::WIDGETTYPE_HEX) {
        if (isGlobalHexSyncEnabled()) {
            getGlobalHexView()->setLocation(nLocation, nLocationType, nSize);
        } else {
            getTreeWidgetNavi()->setCurrentItem(g_mapItems.value(XBinary::_searchDataHeaderById(XBinary::FT_BINARY, XBinary::STRUCTID_HEX, g_ListDataHeaders).dsID.sGUID));
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
        qDebug("XMainWidget::followLocationSlot: Unknown widgetType=%d", nWidgetType);
#endif
    }
}

