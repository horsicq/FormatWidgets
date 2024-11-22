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

XMainWidget::XMainWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::XMainWidget)
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

    g_fileType = XBinary::FT_UNKNOWN;

    ui->widgetHex->setProperty("TYPE", FW_DEF::TYPE_GLOBALHEX);
    ui->widgetHex->hide();

    connect(ui->widgetHex, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
    connect(ui->widgetHex, SIGNAL(currentLocationChanged(quint64,qint32,qint64)), this, SLOT(currentLocationChangedSlot(quint64,qint32,qint64)));
}

XMainWidget::XMainWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent)
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
}

void XMainWidget::cleanup()
{
    XMainWidget::clear();
}

void XMainWidget::reload()
{
    XMainWidget::clear();

    ui->checkBoxReadonly->setEnabled(getDevice()->isWritable());

    g_fileType = getOptions().fileType;

    if (getOptions().fileType == XBinary::FT_UNKNOWN) {
        g_fileType = XBinary::getPrefFileType(getDevice());
    }

    setFileType(g_fileType);

    XHexView::OPTIONS options = {};
    options.memoryMapRegion = XFormats::getMemoryMap(g_fileType, XBinary::MAPMODE_UNKNOWN, getDevice(), getOptions().bIsImage, getOptions().nImageBase);
    ui->widgetHex->setXInfoDB(getXInfoDB());
    ui->widgetHex->setData(getDevice(), options, true);
    ui->widgetHex->setBytesProLine(8);

    _addBaseItems(ui->treeWidgetNavi, g_fileType);
    _addSpecItems(ui->treeWidgetNavi, getDevice(), g_fileType, getOptions().bIsImage, getOptions().nImageBase);

    ui->treeWidgetNavi->expandAll();

    setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
}

FormatWidget::SV XMainWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void XMainWidget::setReadonly(bool bState)
{
    XShortcutsWidget::setReadonly(bState);

    ui->checkBoxReadonly->setChecked(bState);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setReadonly(bState);
        }
    }

    ui->widgetHex->setReadonly(bState);
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

    ui->widgetHex->adjustView();
}

void XMainWidget::reloadData(bool bSaveSelection)
{
    FW_DEF::CWOPTIONS cwOptions = {};
    cwOptions.pParent = this;
    cwOptions.fileType = g_fileType;
    cwOptions._type = (FW_DEF::TYPE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::WIDGET_DATA_TYPE).toInt());
    cwOptions.pDevice = getDevice();
    cwOptions.bIsImage = getOptions().bIsImage;
    cwOptions.nImageBase = getOptions().nImageBase;
    cwOptions.pXInfoDB = getXInfoDB();
    cwOptions.endian = (XBinary::ENDIAN)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::WIDGET_DATA_ENDIAN).toLongLong());
    cwOptions.mode = (XBinary::MODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::WIDGET_DATA_MODE).toLongLong());
    cwOptions.nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::WIDGET_DATA_OFFSET).toLongLong();
    cwOptions.nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::WIDGET_DATA_SIZE).toLongLong();

    QString sUID = getInitString(ui->treeWidgetNavi->currentItem());

    if (isInitPresent(sUID)) {
        qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QWidget *pWidget = ui->stackedWidgetMain->widget(i);

            if (pWidget->property("uid").toString() == sUID) {
                ui->stackedWidgetMain->setCurrentIndex(i);
                break;
            }
        }
    } else {
        XShortcutsWidget *pWidget = createWidget(cwOptions);

        if (pWidget) {
            pWidget->setGlobal(getShortcuts(), getGlobalOptions());
            pWidget->setReadonly(isReadonly());
            connect(pWidget, SIGNAL(currentLocationChanged(quint64,qint32,qint64)), this, SLOT(currentLocationChangedSlot(quint64,qint32,qint64)));
            connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
            pWidget->setProperty("uid", sUID);
            pWidget->setProperty("TYPE", cwOptions._type);
            qint32 nPosition = ui->stackedWidgetMain->addWidget(pWidget);
            ui->stackedWidgetMain->setCurrentIndex(nPosition);
            addInit(sUID);
        }
    }
}

void XMainWidget::_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType)
{
    QTreeWidgetItem *pItemInfo = createNewItem(FW_DEF::TYPE_INFO, tr("Info"), XOptions::ICONTYPE_INFO, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN);
    pTreeWidget->addTopLevelItem(pItemInfo);
    pItemInfo->addChild(createNewItem(FW_DEF::TYPE_NFDSCAN, "Nauz File Detector (NFD)", XOptions::ICONTYPE_NFD, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pItemInfo->addChild(createNewItem(FW_DEF::TYPE_DIESCAN, "Detect It Easy (DiE)", XOptions::ICONTYPE_DIE, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
#ifdef USE_YARA
    pItemInfo->addChild(createNewItem(FW_DEF::TYPE_YARASCAN, "Yara rules", XOptions::ICONTYPE_YARA, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
#endif
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_VIRUSTOTAL, "VirusTotal", XOptions::ICONTYPE_VIRUSTOTAL, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_VISUALIZATION, tr("Visualization"), XOptions::ICONTYPE_VISUALIZATION, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_HEX, tr("Hex"), XOptions::ICONTYPE_HEX, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));

    if (fileType != XBinary::FT_DEX) {
        pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_DISASM, tr("Disasm"), XOptions::ICONTYPE_DISASM, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    }

    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_HASH, tr("Hash"), XOptions::ICONTYPE_HASH, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_STRINGS, tr("Strings"), XOptions::ICONTYPE_STRING, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_SIGNATURES, tr("Signatures"), XOptions::ICONTYPE_SIGNATURE, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_MEMORYMAP, tr("Memory map"), XOptions::ICONTYPE_MEMORYMAP, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_ENTROPY, tr("Entropy"), XOptions::ICONTYPE_ENTROPY, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_EXTRACTOR, tr("Extractor"), XOptions::ICONTYPE_EXTRACTOR, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
    pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_SEARCH, tr("Search"), XOptions::ICONTYPE_SEARCH, 0, -1, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN));
}

void XMainWidget::_addSpecItems(QTreeWidget *pTreeWidget, QIODevice *pDevice, XBinary::FT fileType, bool bIsImage, XADDR nImageBase)
{
    if ((fileType == XBinary::FT_ELF) || (fileType == XBinary::FT_ELF32) || (fileType == XBinary::FT_ELF64)) {
        XELF elf(pDevice, bIsImage, nImageBase);

        if (elf.isValid()) {
            XBinary::ENDIAN endian=elf.getEndian();
            XBinary::MODE mode=elf.getMode();
            bool bIs64 = elf.is64();

            {
                QString sTitle;
                qint64 nDataSize = 0;

                if (bIs64) {
                    sTitle = "Elf64_Ehdr";
                    nDataSize = sizeof(XELF_DEF::Elf64_Ehdr);
                } else {
                    sTitle = "Elf32_Ehdr";
                    nDataSize = sizeof(XELF_DEF::Elf32_Ehdr);
                }

                pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_ELF_elf_ehdr, sTitle, XOptions::ICONTYPE_HEADER, 0, nDataSize, 0, 0, mode, endian));
            }
        }
    } else if ((fileType == XBinary::FT_MACHO) || (fileType == XBinary::FT_MACHO32) || (fileType == XBinary::FT_MACHO64)) {
        XMACH mach(pDevice, bIsImage, nImageBase);

        if (mach.isValid()) {
            XBinary::ENDIAN endian=mach.getEndian();
            XBinary::MODE mode=mach.getMode();
            bool bIs64 = mach.is64();

            {
                QString sTitle;
                qint64 nDataSize = 0;

                if (bIs64) {
                    sTitle = "mach_header_64";
                    nDataSize = sizeof(XMACH_DEF::mach_header_64);
                } else {
                    sTitle = "mach_header";
                    nDataSize = sizeof(XMACH_DEF::mach_header);
                }

                pTreeWidget->addTopLevelItem(createNewItem(FW_DEF::TYPE_MACH_mach_header, sTitle, XOptions::ICONTYPE_HEADER, 0, nDataSize, 0, 0, mode, endian));
            }
        }
    }
}

XShortcutsWidget *XMainWidget::createWidget(const FW_DEF::CWOPTIONS &cwOptions)
{
    XShortcutsWidget *pResult = nullptr;

    if (cwOptions._type == FW_DEF::TYPE_INFO) {
        XFileInfoWidget *_pWidget = new XFileInfoWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, "Info", true);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_NFDSCAN) {
        NFDWidgetAdvanced *_pWidget = new NFDWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, true, cwOptions.fileType);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_DIESCAN) {
        DIEWidgetAdvanced *_pWidget = new DIEWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, true, cwOptions.fileType);
        pResult = _pWidget;
    }
#ifdef USE_YARA
    else if (cwOptions._type == FW_DEF::TYPE_YARASCAN) {
        YARAWidgetAdvanced *_pWidget = new YARAWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(XBinary::getDeviceFileName(cwOptions.pDevice), true);
        pResult = _pWidget;
#endif
    } else if (cwOptions._type == FW_DEF::TYPE_VIRUSTOTAL) {
        XVirusTotalWidget *_pWidget = new XVirusTotalWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_VISUALIZATION) {
        XVisualizationWidget *_pWidget = new XVisualizationWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, true);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_HEX) {
        XHexViewWidget *_pWidget = new XHexViewWidget(cwOptions.pParent);
        XHexViewWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bMenu_Disasm = true;
        options.bMenu_MemoryMap = true;
        _pWidget->setXInfoDB(cwOptions.pXInfoDB);
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_DISASM) {
        XMultiDisasmWidget *_pWidget = new XMultiDisasmWidget(cwOptions.pParent);
        XMultiDisasmWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.nInitAddress = XFormats::getEntryPointAddress(cwOptions.fileType, cwOptions.pDevice, cwOptions.bIsImage, cwOptions.nImageBase);
        options.bMenu_Hex = true;
        _pWidget->setXInfoDB(cwOptions.pXInfoDB);
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_HASH) {
        XHashWidget *_pWidget = new XHashWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, cwOptions.nDataOffset, cwOptions.nDataSize, true);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_STRINGS) {
        SearchStringsWidget *_pWidget = new SearchStringsWidget(cwOptions.pParent);
        SearchStringsWidget::OPTIONS stringsOptions = {};
        stringsOptions.bMenu_Hex = true;
        stringsOptions.bMenu_Demangle = true;
        stringsOptions.bAnsi = true;
        stringsOptions.bUnicode = true;
        stringsOptions.bNullTerminated = false;
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, stringsOptions, true);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_SIGNATURES) {
        SearchSignaturesWidget *_pWidget = new SearchSignaturesWidget(cwOptions.pParent);
        SearchSignaturesWidget::OPTIONS signaturesOptions = {};
        signaturesOptions.bMenu_Hex = true;
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, signaturesOptions, false);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_MEMORYMAP) {
        XMemoryMapWidget *_pWidget = new XMemoryMapWidget(cwOptions.pParent);
        XMemoryMapWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bIsSearchEnable = true;
        _pWidget->setData(cwOptions.pDevice, options, cwOptions.pXInfoDB);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_ENTROPY) {
        XEntropyWidget *_pWidget = new XEntropyWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.nDataOffset, cwOptions.nDataSize, cwOptions.fileType, true);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_EXTRACTOR) {
        XExtractorWidget *_pWidget = new XExtractorWidget(cwOptions.pParent);
        XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
        extractorOptions.fileType = cwOptions.fileType;
        extractorOptions.bMenu_Hex = true;
        _pWidget->setData(cwOptions.pDevice, extractorOptions, true);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_SEARCH) {
        SearchValuesWidget *_pWidget = new SearchValuesWidget(cwOptions.pParent);
        SearchValuesWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bMenu_Hex = true;
        options.bMenu_Disasm = true;
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_ELF_elf_ehdr) {
        elf_ehdrWidget *_pWidget = new elf_ehdrWidget(cwOptions.pParent);
        _pWidget->setCwOptions(cwOptions);
        pResult = _pWidget;
    } else if (cwOptions._type == FW_DEF::TYPE_MACH_mach_header) {
        mach_headerWidget *_pWidget = new mach_headerWidget(cwOptions.pParent);
        _pWidget->setCwOptions(cwOptions);
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
        ui->widgetHex->show();
        QList<qint32> listSizes = ui->splitterHex->sizes();

        qint32 nWidgetSize = 0;
        nWidgetSize += ui->widgetHex->getColumnWidth(0);
        nWidgetSize += ui->widgetHex->getColumnWidth(1);
        nWidgetSize += ui->widgetHex->getColumnWidth(2);
        nWidgetSize += ui->widgetHex->getMapWidth();
        nWidgetSize += 20;
        listSizes[0] -= nWidgetSize;
        listSizes[1] += nWidgetSize;
        ui->splitterHex->setSizes(listSizes);
        ui->splitterHex->setStretchFactor(0, 8);
        ui->splitterHex->setStretchFactor(0, 1);
    } else {
        ui->widgetHex->hide();
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

    FW_DEF::TYPE _type = (FW_DEF::TYPE)(sender()->property("TYPE").toInt());

    if (_type == FW_DEF::TYPE_GLOBALHEX) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->currentWidget());
        if (pWidget) {
            FW_DEF::TYPE _typeRecv = (FW_DEF::TYPE)(pWidget->property("TYPE").toInt());

            if (_typeRecv == FW_DEF::TYPE_MACH_mach_header) {
                pWidget->reloadData(true);
            }
        }
    } else {
        ui->widgetHex->reloadData(true);
    }
}

void XMainWidget::currentLocationChangedSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    FW_DEF::TYPE _type = (FW_DEF::TYPE)(sender()->property("TYPE").toInt());
    if (_type != FW_DEF::TYPE_GLOBALHEX) {
        ui->widgetHex->currentLocationChangedSlot(nLocation, nLocationType, nSize);
    }
}

