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
#include "xmainwidget.h"
#include "ui_xmainwidget.h"

XMainWidget::XMainWidget(QWidget *pParent) : XFormatWidget(pParent), ui(new Ui::XMainWidget)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonNext, XOptions::ICONTYPE_FORWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonPrev, XOptions::ICONTYPE_BACKWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonGlobalHex, XOptions::ICONTYPE_HEX);

    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonNext->setToolTip(tr("Next visited"));
    ui->toolButtonPrev->setToolTip(tr("Previous visited"));
    ui->toolButtonGlobalHex->setToolTip(tr("Hex"));
    ui->checkBoxReadonly->setToolTip(tr("Readonly"));

    ui->widgetGlobalHex->setProperty("TYPE", XFW_DEF::TYPE_GLOBALHEX);
    ui->widgetGlobalHex->hide();

    connect(ui->widgetGlobalHex, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
    connect(ui->widgetGlobalHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
}

XMainWidget::XMainWidget(QIODevice *pDevice, XFW_DEF::OPTIONS options, QWidget *pParent)
{
    XMainWidget::setData(pDevice, options, 0, 0, 0);
    XMainWidget::reload();
}

XMainWidget::~XMainWidget()
{
    delete ui;
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

    XFormatWidget::reset();
}

void XMainWidget::cleanup()
{
    XMainWidget::clear();
}

void XMainWidget::reload()
{
    XMainWidget::clear();

    ui->checkBoxReadonly->setEnabled(getDevice()->isWritable());

    XBinary::FT fileType = getOptions().fileType;

    if (getOptions().fileType == XBinary::FT_UNKNOWN) {
        fileType = XBinary::getPrefFileType(getDevice());
    }

    setFileType(fileType);

    XBinary::_MEMORY_MAP _memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    setMode(_memoryMap.mode);
    setEndian(_memoryMap.endian);

    XHexView::OPTIONS options = {};
    options.memoryMapRegion = _memoryMap;
    ui->widgetGlobalHex->setXInfoDB(getXInfoDB());
    ui->widgetGlobalHex->setData(getDevice(), options, true);
    ui->widgetGlobalHex->setBytesProLine(8);

    _addBaseItems(ui->treeWidgetNavi, fileType);
    _addSpecItems(ui->treeWidgetNavi, getDevice(), fileType, getOptions().bIsImage, getOptions().nImageBase);

    ui->treeWidgetNavi->expandAll();

    ui->treeWidgetNavi->setCurrentIndex(ui->treeWidgetNavi->model()->index(0, 0));

    if (ui->treeWidgetNavi->currentItem()) {
        XMainWidget::reloadData(false);
    }

    // setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
}

XFormatWidget::SV XMainWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void XMainWidget::setReadonly(bool bState)
{
    XFormatWidget::setReadonly(bState);

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
    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->adjustView();
        }
    }

    ui->widgetGlobalHex->adjustView();
}

void XMainWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    XFW_DEF::CWOPTIONS cwOptions = {};
    cwOptions.pParent = this;
    cwOptions.fileType = getFileType();
    cwOptions._type = (XFW_DEF::TYPE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_TYPE).toInt());
    cwOptions.widgetMode = (XFW_DEF::WIDGETMODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_WIDGETMODE).toInt());
    cwOptions.pDevice = getDevice();
    cwOptions.bIsImage = getOptions().bIsImage;
    cwOptions.nImageBase = getOptions().nImageBase;
    cwOptions.pXInfoDB = getXInfoDB();
    cwOptions.endian = (XBinary::ENDIAN)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_ENDIAN).toLongLong());
    cwOptions.mode = (XBinary::MODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_MODE).toLongLong());
    cwOptions.nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_OFFSET).toLongLong();
    cwOptions.nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_SIZE).toLongLong();
    cwOptions.var1 = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR1);
    cwOptions.var2 = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR2);

    QString sInitString = getInitStringFromCwOptions(&cwOptions);

    if (isInitPresent(sInitString)) {
        qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QWidget *pWidget = ui->stackedWidgetMain->widget(i);

            // qDebug("pWidget->property(\"INITSTRING\").toString()=%s", pWidget->property("INITSTRING").toString().toLatin1().data());

            if (pWidget->property("INITSTRING").toString() == sInitString) {
                ui->stackedWidgetMain->setCurrentIndex(i);
                break;
            }
        }
    } else {
        XShortcutsWidget *pWidget = createWidget(cwOptions);

        if (pWidget) {
            pWidget->setGlobal(getShortcuts(), getGlobalOptions());
            pWidget->setReadonly(isReadonly());
            connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
            connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));

            if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE)) {
                XFormatWidget *_pXFormatWidget = dynamic_cast<XFormatWidget *>(pWidget);

                if (_pXFormatWidget) {
                    connect(_pXFormatWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
                }
            }

            pWidget->setProperty("INITSTRING", sInitString);
            pWidget->setProperty("TYPE", cwOptions._type);
            pWidget->setProperty("WIDGETMODE", cwOptions.widgetMode);
            pWidget->reloadData(false);
            qint32 nPosition = ui->stackedWidgetMain->addWidget(pWidget);
            ui->stackedWidgetMain->setCurrentIndex(nPosition);
            addInit(sInitString);
        }
    }
}

void XMainWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XFormatWidget::setGlobal(pShortcuts, pXOptions);

    ui->widgetGlobalHex->setGlobal(pShortcuts, pXOptions);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setGlobal(pShortcuts, pXOptions);
        }
    }
}

void XMainWidget::_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType)
{
    QTreeWidgetItem *pItemInfo =
        createNewItem(XFW_DEF::TYPE_INFO, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Info"), XOptions::ICONTYPE_INFO, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN);
    pTreeWidget->addTopLevelItem(pItemInfo);
    pItemInfo->addChild(createNewItem(XFW_DEF::TYPE_NFDSCAN, XFW_DEF::WIDGETMODE_UNKNOWN, "Nauz File Detector (NFD)", XOptions::ICONTYPE_NFD, 0, -1, 0, 0,
                                      XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pItemInfo->addChild(createNewItem(XFW_DEF::TYPE_DIESCAN, XFW_DEF::WIDGETMODE_UNKNOWN, "Detect It Easy (DiE)", XOptions::ICONTYPE_DIE, 0, -1, 0, 0,
                                      XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
#ifdef USE_YARA
    pItemInfo->addChild(createNewItem(XFW_DEF::TYPE_YARASCAN, XFW_DEF::WIDGETMODE_UNKNOWN, "Yara rules", XOptions::ICONTYPE_YARA, 0, -1, 0, 0, XBinary::MODE_UNKNOWN,
                                      XBinary::ENDIAN_UNKNOWN));
#endif
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_VIRUSTOTAL, XFW_DEF::WIDGETMODE_UNKNOWN, "VirusTotal", XOptions::ICONTYPE_VIRUSTOTAL, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_VISUALIZATION, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Visualization"), XOptions::ICONTYPE_VISUALIZATION, 0, -1, 0,
                                               0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(
        createNewItem(XFW_DEF::TYPE_HEX, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Hex"), XOptions::ICONTYPE_HEX, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));

    if (fileType != XBinary::FT_DEX) {
        pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_DISASM, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Disasm"), XOptions::ICONTYPE_DISASM, 0, -1, 0, 0,
                                                   XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    }

    pTreeWidget->addTopLevelItem(
        createNewItem(XFW_DEF::TYPE_HASH, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Hash"), XOptions::ICONTYPE_HASH, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_STRINGS, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Strings"), XOptions::ICONTYPE_STRING, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_SIGNATURES, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Signatures"), XOptions::ICONTYPE_SIGNATURE, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_MEMORYMAP, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Memory map"), XOptions::ICONTYPE_MEMORYMAP, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_ENTROPY, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Entropy"), XOptions::ICONTYPE_ENTROPY, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_EXTRACTOR, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Extractor"), XOptions::ICONTYPE_EXTRACTOR, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(XFW_DEF::TYPE_SEARCH, XFW_DEF::WIDGETMODE_UNKNOWN, tr("Search"), XOptions::ICONTYPE_SEARCH, 0, -1, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
}

XShortcutsWidget *XMainWidget::createWidget(const XFW_DEF::CWOPTIONS &cwOptions)
{
    XShortcutsWidget *pResult = nullptr;

    if (cwOptions._type == XFW_DEF::TYPE_INFO) {
        XFileInfoWidget *_pWidget = new XFileInfoWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, "Info", false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_NFDSCAN) {
        NFDWidgetAdvanced *_pWidget = new NFDWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, false, cwOptions.fileType);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_DIESCAN) {
        DIEWidgetAdvanced *_pWidget = new DIEWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, false, cwOptions.fileType);
        pResult = _pWidget;
    }
#ifdef USE_YARA
    else if (cwOptions._type == XFW_DEF::TYPE_YARASCAN) {
        YARAWidgetAdvanced *_pWidget = new YARAWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(XBinary::getDeviceFileName(cwOptions.pDevice), false);
        pResult = _pWidget;
#endif
    } else if (cwOptions._type == XFW_DEF::TYPE_VIRUSTOTAL) {
        XVirusTotalWidget *_pWidget = new XVirusTotalWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_VISUALIZATION) {
        XVisualizationWidget *_pWidget = new XVisualizationWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_HEX) {
        XHexViewWidget *_pWidget = new XHexViewWidget(cwOptions.pParent);
        XHexViewWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bMenu_Disasm = true;
        options.bMenu_MemoryMap = true;
        _pWidget->setXInfoDB(cwOptions.pXInfoDB);
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_DISASM) {
        XMultiDisasmWidget *_pWidget = new XMultiDisasmWidget(cwOptions.pParent);
        XMultiDisasmWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.nInitAddress = XFormats::getEntryPointAddress(cwOptions.fileType, cwOptions.pDevice, cwOptions.bIsImage, cwOptions.nImageBase);
        options.bMenu_Hex = true;
        _pWidget->setXInfoDB(cwOptions.pXInfoDB);
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_HASH) {
        XHashWidget *_pWidget = new XHashWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, cwOptions.nDataOffset, cwOptions.nDataSize, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_STRINGS) {
        SearchStringsWidget *_pWidget = new SearchStringsWidget(cwOptions.pParent);
        SearchStringsWidget::OPTIONS stringsOptions = {};
        stringsOptions.bMenu_Hex = true;
        stringsOptions.bMenu_Demangle = true;
        stringsOptions.bAnsi = true;
        stringsOptions.bUnicode = true;
        stringsOptions.bNullTerminated = false;
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, stringsOptions, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_SIGNATURES) {
        SearchSignaturesWidget *_pWidget = new SearchSignaturesWidget(cwOptions.pParent);
        SearchSignaturesWidget::OPTIONS signaturesOptions = {};
        signaturesOptions.bMenu_Hex = true;
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, signaturesOptions, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_MEMORYMAP) {
        XMemoryMapWidget *_pWidget = new XMemoryMapWidget(cwOptions.pParent);
        XMemoryMapWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bIsSearchEnable = true;
        _pWidget->setData(cwOptions.pDevice, options, cwOptions.pXInfoDB);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_ENTROPY) {
        XEntropyWidget *_pWidget = new XEntropyWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.nDataOffset, cwOptions.nDataSize, cwOptions.fileType, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_EXTRACTOR) {
        XExtractorWidget *_pWidget = new XExtractorWidget(cwOptions.pParent);
        XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
        extractorOptions.fileType = cwOptions.fileType;
        extractorOptions.bMenu_Hex = true;
        _pWidget->setData(cwOptions.pDevice, extractorOptions, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_SEARCH) {
        SearchValuesWidget *_pWidget = new SearchValuesWidget(cwOptions.pParent);
        SearchValuesWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bMenu_Hex = true;
        options.bMenu_Disasm = true;
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DIALOG_HEADER)) {
        XGenericHeaderWidget *_pWidget = new XGenericHeaderWidget(cwOptions.pParent);
        _pWidget->setCwOptions(cwOptions, false);
        pResult = _pWidget;
    } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) {
        XGenericTableWidget *_pWidget = new XGenericTableWidget(cwOptions.pParent);
        _pWidget->setCwOptions(cwOptions, false);
        pResult = _pWidget;
    }

    return pResult;
}

void XMainWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
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
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void XMainWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void XMainWidget::on_toolButtonGlobalHex_toggled(bool bChecked)
{
    if (bChecked) {
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
        ui->splitterHex->setStretchFactor(0, 8);
        ui->splitterHex->setStretchFactor(0, 1);
    } else {
        ui->widgetGlobalHex->hide();
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

    XFW_DEF::TYPE _typeSend = (XFW_DEF::TYPE)(sender()->property("TYPE").toInt());
    XFW_DEF::WIDGETMODE _widgetModeSend = (XFW_DEF::WIDGETMODE)(sender()->property("WIDGETMODE").toInt());
    bool bIsWidgetReload = false;

    if (_typeSend == XFW_DEF::TYPE_GLOBALHEX) {
        bIsWidgetReload = true;
    } else {
        ui->widgetGlobalHex->reloadData(true);
        bIsWidgetReload = (_widgetModeSend == XFW_DEF::WIDGETMODE_DIALOG_HEADER);
    }

    if (bIsWidgetReload) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->currentWidget());
        if (pWidget) {
            XFW_DEF::WIDGETMODE _widgetModeRecv = (XFW_DEF::WIDGETMODE)(pWidget->property("WIDGETMODE").toInt());

            if (_widgetModeRecv != XFW_DEF::WIDGETMODE_UNKNOWN) {
                pWidget->reloadData(true);
            }
        }
    }
}

void XMainWidget::currentLocationChangedSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    XFW_DEF::TYPE _type = (XFW_DEF::TYPE)(sender()->property("TYPE").toInt());
    if (_type != XFW_DEF::TYPE_GLOBALHEX) {
        ui->widgetGlobalHex->currentLocationChangedSlot(nLocation, nLocationType, nSize);
    }
}

void XMainWidget::showCwWidgetSlot(QString sInitString, bool bNewWindow)
{
    XFW_DEF::CWOPTIONS cwOptions = {};
    cwOptions.pParent = this;
    cwOptions.fileType = getFileType();
    cwOptions._type = _getTypeFromInitString(sInitString);
    cwOptions.widgetMode = XFW_DEF::WIDGETMODE_DIALOG_HEADER;
    cwOptions.pDevice = getDevice();
    cwOptions.bIsImage = getOptions().bIsImage;
    cwOptions.nImageBase = getOptions().nImageBase;
    cwOptions.pXInfoDB = getXInfoDB();
    cwOptions.endian = getEndian();
    cwOptions.mode = getMode();
    cwOptions.nDataOffset = _getDataOffsetFromInitString(sInitString);
    cwOptions.nDataSize = _getDataSizeFromInitString(sInitString);

    XShortcutsWidget *pWidget = createWidget(cwOptions);

    if (pWidget) {
        pWidget->setGlobal(getShortcuts(), getGlobalOptions());
        pWidget->setReadonly(isReadonly());
        connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
        connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));

        if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE)) {
            connect(pWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
        }

        pWidget->setProperty("INITSTRING", sInitString);
        pWidget->setProperty("TYPE", cwOptions._type);
        pWidget->setProperty("WIDGETMODE", cwOptions.widgetMode);
        pWidget->reloadData(false);

        DialogWidget dialogWidget(this);
        dialogWidget.setGlobal(getShortcuts(), getGlobalOptions());
        dialogWidget.setModal(true);
        dialogWidget.addWidget(pWidget);

        dialogWidget.exec();
    }
}
