/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
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
#include "lewidget.h"

#include "ui_lewidget.h"

LEWidget::LEWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::LEWidget)
{
    ui->setupUi(this);

    memset(g_subDevice, 0, sizeof g_subDevice);

    initWidget();
}

LEWidget::LEWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) : LEWidget(pParent)
{
    LEWidget::setData(pDevice, options, 0, 0, 0);
    LEWidget::reload();
}

LEWidget::~LEWidget()
{
    delete ui;
}

void LEWidget::clear()
{
    LEWidget::reset();

    memset(g_lineEdit_DOS_HEADER, 0, sizeof g_lineEdit_DOS_HEADER);
    memset(g_lineEdit_VXD_HEADER, 0, sizeof g_lineEdit_VXD_HEADER);
    memset(g_comboBox, 0, sizeof g_comboBox);
    memset(g_invWidget, 0, sizeof g_invWidget);

    _deleteSubdevices(g_subDevice, (sizeof g_subDevice) / (sizeof(SubDevice *)));

    resetWidget();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void LEWidget::cleanup()
{
    LEWidget::clear();
}

void LEWidget::reload()
{
    LEWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XLE le(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (le.isValid()) {
        setFileType(le.getFileType());

        QTreeWidgetItem *pItemInfo = createNewItem(SLE::TYPE_INFO, tr("Info"));
        ui->treeWidgetNavi->addTopLevelItem(pItemInfo);
        pItemInfo->addChild(createNewItem(SLE::TYPE_NFDSCAN, "Nauz File Detector(NFD)"));
        pItemInfo->addChild(createNewItem(SLE::TYPE_DIESCAN, "Detect It Easy(DiE)"));
#ifdef USE_YARA
        pItemInfo->addChild(createNewItem(SLE::TYPE_YARASCAN, "YARA"));
#endif
        pItemInfo->addChild(createNewItem(SLE::TYPE_VIRUSTOTAL, "VirusTotal"));

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_VISUALIZATION, tr("Visualization")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_HEX, tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_DISASM, tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_HASH, tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_STRINGS, tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_SIGNATURES, tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_MEMORYMAP, tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_ENTROPY, tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_EXTRACTOR, tr("Extractor")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_SEARCH, tr("Search")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_DOS_HEADER, "DOS_HEADER"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_VXD_HEADER, "VXD_HEADER"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_OBJECTS, "Objects"));

        if (le.isOverlayPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_OVERLAY, tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();

        setDisasmInitAddress(le.getEntryPointAddress());  // Optimize

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
    }
}

FormatWidget::SV LEWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result = SV_NONE;

    blockSignals(true);

    quint64 nValue = vValue.toULongLong();

    if (getDevice()->isWritable()) {
        XLE le(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (le.isValid()) {
            switch (nStype) {
                case SLE::TYPE_DOS_HEADER:
                    switch (nNdata) {
                        case N_LE_DOS_HEADER::e_magic: g_comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue); break;
                        case N_LE_DOS_HEADER::e_lfanew: g_invWidget[INV_DOS_HEADER_e_lfanew]->setOffsetAndSize(&le, (quint32)nValue, 0); break;
                    }
                    break;
                case SLE::TYPE_VXD_HEADER:
                    switch (nNdata) {
                        case N_VXD_HEADER::e32_magic: g_comboBox[CB_VXD_HEADER_e32_magic]->setValue(nValue); break;
                        case N_VXD_HEADER::e32_cpu: g_comboBox[CB_VXD_HEADER_e32_cpu]->setValue(nValue); break;
                        case N_VXD_HEADER::e32_os: g_comboBox[CB_VXD_HEADER_e32_os]->setValue(nValue); break;
                    }
                    break;
            }

            switch (nStype) {
                case SLE::TYPE_DOS_HEADER:
                    switch (nNdata) {
                        case N_LE_DOS_HEADER::e_magic: le.set_e_magic((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_cblp: le.set_e_cblp((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_cp: le.set_e_cp((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_crlc: le.set_e_crlc((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_cparhdr: le.set_e_cparhdr((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_minalloc: le.set_e_minalloc((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_maxalloc: le.set_e_maxalloc((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_ss: le.set_e_ss((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_sp: le.set_e_sp((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_csum: le.set_e_csum((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_ip: le.set_e_ip((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_cs: le.set_e_cs((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_lfarlc: le.set_e_lfarlc((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_ovno: le.set_e_ovno((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res_0: le.set_e_res(0, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res_1: le.set_e_res(1, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res_2: le.set_e_res(2, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res_3: le.set_e_res(3, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_oemid: le.set_e_oemid((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_oeminfo: le.set_e_oeminfo((quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_0: le.set_e_res2(0, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_1: le.set_e_res2(1, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_2: le.set_e_res2(2, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_3: le.set_e_res2(3, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_4: le.set_e_res2(4, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_5: le.set_e_res2(5, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_6: le.set_e_res2(6, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_7: le.set_e_res2(7, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_8: le.set_e_res2(8, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_res2_9: le.set_e_res2(9, (quint16)nValue); break;
                        case N_LE_DOS_HEADER::e_lfanew: le.set_e_lfanew((quint32)nValue); break;
                    }

                    ui->widgetHex_DOS_HEADER->reload();

                    break;

                case SLE::TYPE_VXD_HEADER:
                    switch (nNdata) {
                        case N_VXD_HEADER::e32_magic: le.setImageVxdHeader_magic((quint16)nValue); break;
                        case N_VXD_HEADER::e32_border: le.setImageVxdHeader_border((quint8)nValue); break;
                        case N_VXD_HEADER::e32_worder: le.setImageVxdHeader_worder((quint8)nValue); break;
                        case N_VXD_HEADER::e32_level: le.setImageVxdHeader_level((quint32)nValue); break;
                        case N_VXD_HEADER::e32_cpu: le.setImageVxdHeader_cpu((quint16)nValue); break;
                        case N_VXD_HEADER::e32_os: le.setImageVxdHeader_os((quint16)nValue); break;
                        case N_VXD_HEADER::e32_ver: le.setImageVxdHeader_ver((quint32)nValue); break;
                        case N_VXD_HEADER::e32_mflags: le.setImageVxdHeader_mflags((quint32)nValue); break;
                        case N_VXD_HEADER::e32_mpages: le.setImageVxdHeader_mpages((quint32)nValue); break;
                        case N_VXD_HEADER::e32_startobj: le.setImageVxdHeader_startobj((quint32)nValue); break;
                        case N_VXD_HEADER::e32_eip: le.setImageVxdHeader_eip((quint32)nValue); break;
                        case N_VXD_HEADER::e32_stackobj: le.setImageVxdHeader_stackobj((quint32)nValue); break;
                        case N_VXD_HEADER::e32_esp: le.setImageVxdHeader_esp((quint32)nValue); break;
                        case N_VXD_HEADER::e32_pagesize: le.setImageVxdHeader_pagesize((quint32)nValue); break;
                        case N_VXD_HEADER::e32_lastpagesize: le.setImageVxdHeader_lastpagesize((quint32)nValue); break;
                        case N_VXD_HEADER::e32_fixupsize: le.setImageVxdHeader_fixupsize((quint32)nValue); break;
                        case N_VXD_HEADER::e32_fixupsum: le.setImageVxdHeader_fixupsum((quint32)nValue); break;
                        case N_VXD_HEADER::e32_ldrsize: le.setImageVxdHeader_ldrsize((quint32)nValue); break;
                        case N_VXD_HEADER::e32_ldrsum: le.setImageVxdHeader_ldrsum((quint32)nValue); break;
                        case N_VXD_HEADER::e32_objtab: le.setImageVxdHeader_objtab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_objcnt: le.setImageVxdHeader_objcnt((quint32)nValue); break;
                        case N_VXD_HEADER::e32_objmap: le.setImageVxdHeader_objmap((quint32)nValue); break;
                        case N_VXD_HEADER::e32_itermap: le.setImageVxdHeader_itermap((quint32)nValue); break;
                        case N_VXD_HEADER::e32_rsrctab: le.setImageVxdHeader_rsrctab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_rsrccnt: le.setImageVxdHeader_rsrccnt((quint32)nValue); break;
                        case N_VXD_HEADER::e32_restab: le.setImageVxdHeader_restab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_enttab: le.setImageVxdHeader_enttab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_dirtab: le.setImageVxdHeader_dirtab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_dircnt: le.setImageVxdHeader_dircnt((quint32)nValue); break;
                        case N_VXD_HEADER::e32_fpagetab: le.setImageVxdHeader_fpagetab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_frectab: le.setImageVxdHeader_frectab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_impmod: le.setImageVxdHeader_impmod((quint32)nValue); break;
                        case N_VXD_HEADER::e32_impmodcnt: le.setImageVxdHeader_impmodcnt((quint32)nValue); break;
                        case N_VXD_HEADER::e32_impproc: le.setImageVxdHeader_impproc((quint32)nValue); break;
                        case N_VXD_HEADER::e32_pagesum: le.setImageVxdHeader_pagesum((quint32)nValue); break;
                        case N_VXD_HEADER::e32_datapage: le.setImageVxdHeader_datapage((quint32)nValue); break;
                        case N_VXD_HEADER::e32_preload: le.setImageVxdHeader_preload((quint32)nValue); break;
                        case N_VXD_HEADER::e32_nrestab: le.setImageVxdHeader_nrestab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_cbnrestab: le.setImageVxdHeader_cbnrestab((quint32)nValue); break;
                        case N_VXD_HEADER::e32_nressum: le.setImageVxdHeader_nressum((quint32)nValue); break;
                        case N_VXD_HEADER::e32_autodata: le.setImageVxdHeader_autodata((quint32)nValue); break;
                        case N_VXD_HEADER::e32_debuginfo: le.setImageVxdHeader_debuginfo((quint32)nValue); break;
                        case N_VXD_HEADER::e32_debuglen: le.setImageVxdHeader_debuglen((quint32)nValue); break;
                        case N_VXD_HEADER::e32_instpreload: le.setImageVxdHeader_instpreload((quint32)nValue); break;
                        case N_VXD_HEADER::e32_instdemand: le.setImageVxdHeader_instdemand((quint32)nValue); break;
                        case N_VXD_HEADER::e32_heapsize: le.setImageVxdHeader_heapsize((quint32)nValue); break;
                    }

                    ui->widgetHex_VXD_HEADER->reload();

                    break;
            }

            result = SV_EDITED;
        }
    }

    blockSignals(false);

    return result;
}

void LEWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_lineEdit_DOS_HEADER, N_LE_DOS_HEADER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_VXD_HEADER, N_VXD_HEADER::__data_size, bState);

    setComboBoxesReadOnly(g_comboBox, __CB_size, bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetDisasm->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
}

void LEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_DOS_HEADER, N_LE_DOS_HEADER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_VXD_HEADER, N_VXD_HEADER::__data_size, bState);

    _blockSignals((QObject **)g_comboBox, __CB_size, bState);
}

void LEWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    // TODO like MACH !!!
    Q_UNUSED(nType)

    qint32 nSymbolWidth = XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE, QHeaderView::ResizeToContents);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, nSymbolWidth * 12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, nSymbolWidth * 15);
}

QString LEWidget::typeIdToString(qint32 nType)
{
    Q_UNUSED(nType)

    QString sResult;

    // TODO

    return sResult;
}

void LEWidget::_showInDisasmWindowAddress(qint64 nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_DISASM);
    ui->widgetDisasm->goToAddress(nAddress);
}

void LEWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_DISASM);
    ui->widgetDisasm->goToOffset(nOffset);
}

void LEWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void LEWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset, nSize);
}

void LEWidget::_findValue(quint64 nValue, bool bIsBigEndian)
{
    Q_UNUSED(nValue)
    Q_UNUSED(bIsBigEndian)
//    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_SEARCH);
//    ui->widgetSearch->findValue(nValue, bIsBigEndian);
#ifdef QT_DEBUG
    qDebug("TODO: _findValue");
#endif
}

void LEWidget::reloadData()
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XLE le(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (le.isValid()) {
        if (nType == SLE::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), le.getFileType(), "Info", true);
            }
        } else if (nType == SLE::TYPE_VISUALIZATION) {
            if (!isInitPresent(sInit)) {
                ui->widgetVisualization->setData(getDevice(), le.getFileType(), true);
            }
        } else if (nType == SLE::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SLE::TYPE_HEX) {
            if (!isInitPresent(sInit)) {
                XHexView::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;
                ui->widgetHex->setXInfoDB(getXInfoDB());
                ui->widgetHex->setData(getDevice(), options);
                ui->widgetHex->setBackupDevice(getBackupDevice());
                //                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                //                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SLE::TYPE_DISASM) {
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = le.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;
                ui->widgetDisasm->setXInfoDB(getXInfoDB());
                ui->widgetDisasm->setData(getDevice(), options);
                ui->widgetDisasm->setBackupDevice(getBackupDevice());

                setDisasmInitAddress(-1);
            }
        } else if (nType == SLE::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), le.getFileType(), 0, -1, true);
            }
        } else if (nType == SLE::TYPE_STRINGS) {
            if (!isInitPresent(sInit)) {
                SearchStringsWidget::OPTIONS stringsOptions = {};
                stringsOptions.bMenu_Hex = true;
                stringsOptions.bMenu_Demangle = true;
                stringsOptions.bAnsi = true;
                stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bCStrings = false;

                ui->widgetStrings->setData(getDevice(), le.getFileType(), stringsOptions, true);
            }
        } else if (nType == SLE::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;

                ui->widgetSignatures->setData(getDevice(), le.getFileType(), signaturesOptions, false);
            }
        } else if (nType == SLE::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                XMemoryMapWidget::OPTIONS options = {};
                options.fileType = le.getFileType();
                options.bIsSearchEnable = true;

                ui->widgetMemoryMap->setData(getDevice(), options, getXInfoDB());
            }
        } else if (nType == SLE::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), le.getFileType(), true);
            }
        } else if (nType == SLE::TYPE_NFDSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, le.getFileType());
            }
        } else if (nType == SLE::TYPE_DIESCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetDIEScan->setData(getDevice(), true, le.getFileType());
            }
#ifdef USE_YARA
        } else if (nType == SLE::TYPE_YARASCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetYARAScan->setData(XBinary::getDeviceFileName(getDevice()), true);
            }
#endif
        } else if (nType == SLE::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.fileType = le.getFileType();
                extractorOptions.bMenu_Hex = true;

                ui->widgetExtractor->setData(getDevice(), extractorOptions, true);
            }
        } else if (nType == SLE::TYPE_SEARCH) {
            if (!isInitPresent(sInit)) {
                SearchValuesWidget::OPTIONS options = {};
                options.fileType = le.getFileType();
                options.bMenu_Hex = true;
                options.bMenu_Disasm = true;

                ui->widgetSearch->setData(getDevice(), options);
            }
        } else if (nType == SLE::TYPE_DOS_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SLE::TYPE_DOS_HEADER, ui->tableWidget_DOS_HEADER, N_LE_DOS_HEADER::records, g_lineEdit_DOS_HEADER, N_LE_DOS_HEADER::__data_size, 0);
                g_comboBox[CB_DOS_HEADER_e_magic] =
                    createComboBox(ui->tableWidget_DOS_HEADER, XMSDOS::getImageMagicsS(), SLE::TYPE_DOS_HEADER, N_LE_DOS_HEADER::e_magic, XComboBoxEx::CBTYPE_LIST);
                g_invWidget[INV_DOS_HEADER_e_lfanew] = createInvWidget(ui->tableWidget_DOS_HEADER, SLE::TYPE_DOS_HEADER, N_LE_DOS_HEADER::e_lfanew, InvWidget::TYPE_HEX);
                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex = le.getDosHeaderEx();

                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_magic]->setValue(msdosheaderex.e_magic);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cblp]->setValue(msdosheaderex.e_cblp);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cp]->setValue(msdosheaderex.e_cp);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_crlc]->setValue(msdosheaderex.e_crlc);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cparhdr]->setValue(msdosheaderex.e_cparhdr);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_minalloc]->setValue(msdosheaderex.e_minalloc);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_maxalloc]->setValue(msdosheaderex.e_maxalloc);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_ss]->setValue(msdosheaderex.e_ss);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_sp]->setValue(msdosheaderex.e_sp);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_csum]->setValue(msdosheaderex.e_csum);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_ip]->setValue(msdosheaderex.e_ip);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cs]->setValue(msdosheaderex.e_cs);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_lfarlc]->setValue(msdosheaderex.e_lfarlc);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_ovno]->setValue(msdosheaderex.e_ovno);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_0]->setValue(msdosheaderex.e_res[0]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_1]->setValue(msdosheaderex.e_res[1]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_2]->setValue(msdosheaderex.e_res[2]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_3]->setValue(msdosheaderex.e_res[3]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_oemid]->setValue(msdosheaderex.e_oemid);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_oeminfo]->setValue(msdosheaderex.e_oeminfo);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_0]->setValue(msdosheaderex.e_res2[0]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_1]->setValue(msdosheaderex.e_res2[1]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_2]->setValue(msdosheaderex.e_res2[2]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_3]->setValue(msdosheaderex.e_res2[3]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_4]->setValue(msdosheaderex.e_res2[4]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_5]->setValue(msdosheaderex.e_res2[5]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_6]->setValue(msdosheaderex.e_res2[6]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_7]->setValue(msdosheaderex.e_res2[7]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_8]->setValue(msdosheaderex.e_res2[8]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_9]->setValue(msdosheaderex.e_res2[9]);
                g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_lfanew]->setValue(msdosheaderex.e_lfanew);

                g_comboBox[CB_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

                g_invWidget[INV_DOS_HEADER_e_lfanew]->setOffsetAndSize(&le, msdosheaderex.e_lfanew, 0);

                qint64 nOffset = le.getDosHeaderExOffset();
                qint64 nSize = le.getDosHeaderExSize();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SLE::TYPE_DOS_HEADER], ui->widgetHex_DOS_HEADER);

                blockSignals(false);
            }
        } else if (nType == SLE::TYPE_VXD_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SLE::TYPE_VXD_HEADER, ui->tableWidget_VXD_HEADER, N_VXD_HEADER::records, g_lineEdit_VXD_HEADER, N_VXD_HEADER::__data_size, 0);
                g_comboBox[CB_VXD_HEADER_e32_magic] =
                    createComboBox(ui->tableWidget_VXD_HEADER, XLE::getImageLEMagicsS(), SLE::TYPE_VXD_HEADER, N_VXD_HEADER::e32_magic, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_VXD_HEADER_e32_cpu] =
                    createComboBox(ui->tableWidget_VXD_HEADER, XLE::getImageLECpusS(), SLE::TYPE_VXD_HEADER, N_VXD_HEADER::e32_cpu, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_VXD_HEADER_e32_os] =
                    createComboBox(ui->tableWidget_VXD_HEADER, XLE::getImageLEOssS(), SLE::TYPE_VXD_HEADER, N_VXD_HEADER::e32_os, XComboBoxEx::CBTYPE_LIST);

                blockSignals(true);

                XLE_DEF::IMAGE_VXD_HEADER vxdheader = le.getImageVxdHeader();

                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_magic]->setValue(vxdheader.e32_magic);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_border]->setValue(vxdheader.e32_border);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_worder]->setValue(vxdheader.e32_worder);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_level]->setValue(vxdheader.e32_level);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_cpu]->setValue(vxdheader.e32_cpu);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_os]->setValue(vxdheader.e32_os);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_ver]->setValue(vxdheader.e32_ver);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_mflags]->setValue(vxdheader.e32_mflags);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_mpages]->setValue(vxdheader.e32_mpages);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_startobj]->setValue(vxdheader.e32_startobj);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_eip]->setValue(vxdheader.e32_eip);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_stackobj]->setValue(vxdheader.e32_stackobj);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_esp]->setValue(vxdheader.e32_esp);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_pagesize]->setValue(vxdheader.e32_pagesize);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_lastpagesize]->setValue(vxdheader.e32_lastpagesize);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_fixupsize]->setValue(vxdheader.e32_fixupsize);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_fixupsum]->setValue(vxdheader.e32_fixupsum);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_ldrsize]->setValue(vxdheader.e32_ldrsize);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_ldrsum]->setValue(vxdheader.e32_ldrsum);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_objtab]->setValue(vxdheader.e32_objtab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_objcnt]->setValue(vxdheader.e32_objcnt);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_objmap]->setValue(vxdheader.e32_objmap);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_itermap]->setValue(vxdheader.e32_itermap);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_rsrctab]->setValue(vxdheader.e32_rsrctab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_rsrccnt]->setValue(vxdheader.e32_rsrccnt);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_restab]->setValue(vxdheader.e32_restab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_enttab]->setValue(vxdheader.e32_enttab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_dirtab]->setValue(vxdheader.e32_dirtab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_dircnt]->setValue(vxdheader.e32_dircnt);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_fpagetab]->setValue(vxdheader.e32_fpagetab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_frectab]->setValue(vxdheader.e32_frectab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_impmod]->setValue(vxdheader.e32_impmod);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_impmodcnt]->setValue(vxdheader.e32_impmodcnt);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_impproc]->setValue(vxdheader.e32_impproc);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_pagesum]->setValue(vxdheader.e32_pagesum);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_datapage]->setValue(vxdheader.e32_datapage);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_preload]->setValue(vxdheader.e32_preload);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_nrestab]->setValue(vxdheader.e32_nrestab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_cbnrestab]->setValue(vxdheader.e32_cbnrestab);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_nressum]->setValue(vxdheader.e32_nressum);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_autodata]->setValue(vxdheader.e32_autodata);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_debuginfo]->setValue(vxdheader.e32_debuginfo);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_debuglen]->setValue(vxdheader.e32_debuglen);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_instpreload]->setValue(vxdheader.e32_instpreload);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_instdemand]->setValue(vxdheader.e32_instdemand);
                g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_heapsize]->setValue(vxdheader.e32_heapsize);

                g_comboBox[CB_VXD_HEADER_e32_magic]->setValue(vxdheader.e32_magic);
                g_comboBox[CB_VXD_HEADER_e32_cpu]->setValue(vxdheader.e32_cpu);
                g_comboBox[CB_VXD_HEADER_e32_os]->setValue(vxdheader.e32_os);

                qint64 nOffset = le.getImageVxdHeaderOffset();
                qint64 nSize = le.getImageVxdHeaderSize();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SLE::TYPE_VXD_HEADER], ui->widgetHex_VXD_HEADER);

                blockSignals(false);
            }
        } else if (nType == SLE::TYPE_OBJECTS) {
            if (!isInitPresent(sInit)) {
                LEProcessData leProcessData(SLE::TYPE_OBJECTS, &g_tvModel[SLE::TYPE_OBJECTS], &le, nDataOffset, nDataSize);

                ajustTableView(&leProcessData, &g_tvModel[SLE::TYPE_OBJECTS], ui->tableView_Objects, nullptr, false);

                connect(ui->tableView_Objects->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Objects_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SLE::TYPE_OBJECTS]->rowCount()) {
                    ui->tableView_Objects->setCurrentIndex(ui->tableView_Objects->model()->index(0, 0));
                }
            }
        } else if (nType == SLE::TYPE_OVERLAY) {
            qint64 nOverLayOffset = le.getOverlayOffset();
            qint64 nOverlaySize = le.getOverlaySize();

            loadHexSubdevice(nOverLayOffset, nOverlaySize, nOverLayOffset, &g_subDevice[SLE::TYPE_OVERLAY], ui->widgetHex_OVERLAY);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void LEWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    int nStype = pWidget->property("STYPE").toInt();
    int nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SLE::TYPE_DOS_HEADER:
            switch (nNdata) {
                case N_LE_DOS_HEADER::e_magic: g_lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_magic]->setValue((quint16)nValue); break;
            }
            break;
        case SLE::TYPE_VXD_HEADER:
            switch (nNdata) {
                case N_VXD_HEADER::e32_magic: g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_magic]->setValue((quint16)nValue); break;
                case N_VXD_HEADER::e32_cpu: g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_cpu]->setValue((quint16)nValue); break;
                case N_VXD_HEADER::e32_os: g_lineEdit_VXD_HEADER[N_VXD_HEADER::e32_os]->setValue((quint16)nValue); break;
            }
            break;
    }
}

void LEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if (pCurrent) {
        reloadData();
        addPage(pCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void LEWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void LEWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));  // TODO const
}

void LEWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void LEWidget::on_tableWidget_DOS_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_DOS_HEADER, ui->tableWidget_DOS_HEADER);
}

void LEWidget::on_tableWidget_VXD_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_VXD_HEADER, ui->tableWidget_VXD_HEADER);
}

void LEWidget::on_tableView_Objects_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Objects->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_Objects);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        //        connect(&actionEdit,SIGNAL(triggered()),this,SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(tr("Hex"), this);
        //        connect(&actionHex,SIGNAL(triggered()),this,SLOT(sectionHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionEntropy(tr("Entropy"), this);
        //        connect(&actionEntropy,SIGNAL(triggered()),this,SLOT(sectionEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        contextMenu.addMenu(getShortcuts()->getRowCopyMenu(this, ui->tableView_Objects));

        contextMenu.exec(ui->tableView_Objects->viewport()->mapToGlobal(pos));
    }
}

void LEWidget::on_tableView_Objects_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    //    editSectionHeader();
}

void LEWidget::onTableView_Objects_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SLE::TYPE_OBJECTS, ui->widgetHex_Object, ui->tableView_Objects, &g_subDevice[SLE::TYPE_OBJECTS]);
}

void LEWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void LEWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void LEWidget::on_pushButtonHex_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_HEX);
}

void LEWidget::on_pushButtonDisasm_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_DISASM);
}

void LEWidget::on_pushButtonStrings_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_STRINGS);
}

void LEWidget::on_pushButtonMemoryMap_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_MEMORYMAP);
}

void LEWidget::on_pushButtonEntropy_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_ENTROPY);
}

void LEWidget::on_pushButtonHeuristicScan_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SLE::TYPE_NFDSCAN);
}
