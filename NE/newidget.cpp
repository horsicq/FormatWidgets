/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "newidget.h"

#include "ui_newidget.h"

NEWidget::NEWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::NEWidget)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonNext, XOptions::ICONTYPE_FORWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonPrev, XOptions::ICONTYPE_BACKWARD, Qt::ToolButtonIconOnly);

    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonNext->setToolTip(tr("Next visited"));
    ui->toolButtonPrev->setToolTip(tr("Previous visited"));
    ui->checkBoxReadonly->setToolTip(tr("Readonly"));

    memset(g_subDevice, 0, sizeof g_subDevice);

    initWidget();
}

NEWidget::NEWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) : NEWidget(pParent)
{
    NEWidget::setData(pDevice, options, 0, 0, 0);
    NEWidget::reload();
}

NEWidget::~NEWidget()
{
    delete ui;
}

void NEWidget::clear()
{
    NEWidget::reset();

    memset(g_lineEdit_DOS_HEADER, 0, sizeof g_lineEdit_DOS_HEADER);
    memset(g_lineEdit_OS2_HEADER, 0, sizeof g_lineEdit_OS2_HEADER);
    memset(g_comboBox, 0, sizeof g_comboBox);
    memset(g_invWidget, 0, sizeof g_invWidget);

    memset(g_tvModel, 0, sizeof g_tvModel);

    _deleteSubdevices(g_subDevice, (sizeof g_subDevice) / (sizeof(SubDevice *)));

    resetWidget();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void NEWidget::cleanup()
{
    NEWidget::clear();
}

void NEWidget::reload()
{
    NEWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XNE ne(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (ne.isValid()) {
        setFileType(ne.getFileType());

        QTreeWidgetItem *pItemInfo = createNewItem(SNE::TYPE_INFO, tr("Info"), XOptions::ICONTYPE_INFO);
        ui->treeWidgetNavi->addTopLevelItem(pItemInfo);
        pItemInfo->addChild(createNewItem(SNE::TYPE_NFDSCAN, "Nauz File Detector (NFD)", XOptions::ICONTYPE_NFD));
        pItemInfo->addChild(createNewItem(SNE::TYPE_DIESCAN, "Detect It Easy (DiE)", XOptions::ICONTYPE_DIE));
#ifdef USE_YARA
        pItemInfo->addChild(createNewItem(SNE::TYPE_YARASCAN, "Yara rules", XOptions::ICONTYPE_YARA));
#endif
        pItemInfo->addChild(createNewItem(SNE::TYPE_VIRUSTOTAL, "VirusTotal", XOptions::ICONTYPE_VIRUSTOTAL));

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_VISUALIZATION, tr("Visualization"), XOptions::ICONTYPE_VISUALIZATION));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_HEX, tr("Hex"), XOptions::ICONTYPE_HEX));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_DISASM, tr("Disasm"), XOptions::ICONTYPE_DISASM));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_HASH, tr("Hash"), XOptions::ICONTYPE_HASH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_STRINGS, tr("Strings"), XOptions::ICONTYPE_STRING));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_SIGNATURES, tr("Signatures"), XOptions::ICONTYPE_SIGNATURE));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_MEMORYMAP, tr("Memory map"), XOptions::ICONTYPE_MEMORYMAP));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_ENTROPY, tr("Entropy"), XOptions::ICONTYPE_ENTROPY));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_EXTRACTOR, tr("Extractor"), XOptions::ICONTYPE_EXTRACTOR));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_SEARCH, tr("Search"), XOptions::ICONTYPE_SEARCH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_DOS_HEADER, "DOS_HEADER", XOptions::ICONTYPE_HEADER));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_OS2_HEADER, "OS2_HEADER", XOptions::ICONTYPE_HEADER));

        QList<XNE_DEF::NE_SEGMENT> listSegments = ne.getSegmentList();

        if (listSegments.count()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_SEGMENTS, tr("Segments"), XOptions::ICONTYPE_SEGMENT));
        }

        if (ne.isOverlayPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_OVERLAY, tr("Overlay"), XOptions::ICONTYPE_OVERLAY));
        }

        ui->treeWidgetNavi->expandAll();

        setDisasmInitAddress(ne.getEntryPointAddress());  // Optimize

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
    }
}

FormatWidget::SV NEWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result = SV_NONE;

    blockSignals(true);

    quint64 nValue = vValue.toULongLong();

    if (getDevice()->isWritable()) {
        XNE ne(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (ne.isValid()) {
            switch (nStype) {
                case SNE::TYPE_DOS_HEADER:
                    switch (nNdata) {
                        case N_NE_DOS_HEADER::e_magic: g_comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue); break;
                        case N_NE_DOS_HEADER::e_lfanew: g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&ne, (quint32)nValue, 0); break;
                    }
                    break;

                case SNE::TYPE_OS2_HEADER:
                    switch (nNdata) {
                        case N_OS2_HEADER::ne_magic: g_comboBox[CB_OS2_HEADER_ne_magic]->setValue(nValue); break;
                        case N_OS2_HEADER::ne_flags: g_comboBox[CB_OS2_HEADER_ne_flags]->setValue(nValue); break;
                        case N_OS2_HEADER::ne_exetyp: g_comboBox[CB_OS2_HEADER_ne_exetype]->setValue(nValue); break;
                        case N_OS2_HEADER::ne_flagsothers: g_comboBox[CB_OS2_HEADER_ne_flagsothers]->setValue(nValue); break;
                        case N_OS2_HEADER::ne_enttab: g_invWidget[CB_OS2_HEADER_ne_enttab]->setOffsetAndSize(&ne, ne.getImageOS2HeaderOffset() + nValue, 0); break;
                        case N_OS2_HEADER::ne_segtab: g_invWidget[CB_OS2_HEADER_ne_segtab]->setOffsetAndSize(&ne, ne.getImageOS2HeaderOffset() + nValue, 0); break;
                        case N_OS2_HEADER::ne_rsrctab: g_invWidget[CB_OS2_HEADER_ne_rsrctab]->setOffsetAndSize(&ne, ne.getImageOS2HeaderOffset() + nValue, 0); break;
                        case N_OS2_HEADER::ne_restab: g_invWidget[CB_OS2_HEADER_ne_restab]->setOffsetAndSize(&ne, ne.getImageOS2HeaderOffset() + nValue, 0); break;
                        case N_OS2_HEADER::ne_modtab: g_invWidget[CB_OS2_HEADER_ne_modtab]->setOffsetAndSize(&ne, ne.getImageOS2HeaderOffset() + nValue, 0); break;
                        case N_OS2_HEADER::ne_imptab: g_invWidget[CB_OS2_HEADER_ne_imptab]->setOffsetAndSize(&ne, ne.getImageOS2HeaderOffset() + nValue, 0); break;
                        case N_OS2_HEADER::ne_nrestab: g_invWidget[CB_OS2_HEADER_ne_nrestab]->setOffsetAndSize(&ne, nValue, 0); break;
                    }
                    break;
            }

            switch (nStype) {
                case SNE::TYPE_DOS_HEADER:
                    switch (nNdata) {
                        case N_NE_DOS_HEADER::e_magic: ne.set_e_magic((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_cblp: ne.set_e_cblp((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_cp: ne.set_e_cp((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_crlc: ne.set_e_crlc((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_cparhdr: ne.set_e_cparhdr((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_minalloc: ne.set_e_minalloc((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_maxalloc: ne.set_e_maxalloc((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_ss: ne.set_e_ss((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_sp: ne.set_e_sp((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_csum: ne.set_e_csum((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_ip: ne.set_e_ip((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_cs: ne.set_e_cs((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_lfarlc: ne.set_e_lfarlc((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_ovno: ne.set_e_ovno((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res_0: ne.set_e_res(0, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res_1: ne.set_e_res(1, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res_2: ne.set_e_res(2, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res_3: ne.set_e_res(3, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_oemid: ne.set_e_oemid((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_oeminfo: ne.set_e_oeminfo((quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_0: ne.set_e_res2(0, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_1: ne.set_e_res2(1, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_2: ne.set_e_res2(2, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_3: ne.set_e_res2(3, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_4: ne.set_e_res2(4, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_5: ne.set_e_res2(5, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_6: ne.set_e_res2(6, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_7: ne.set_e_res2(7, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_8: ne.set_e_res2(8, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_res2_9: ne.set_e_res2(9, (quint16)nValue); break;
                        case N_NE_DOS_HEADER::e_lfanew: ne.set_e_lfanew((quint32)nValue); break;
                    }

                    ui->widgetHex_DOS_HEADER->reload();

                    break;

                case SNE::TYPE_OS2_HEADER:
                    switch (nNdata) {
                        case N_OS2_HEADER::ne_magic: ne.setImageOS2Header_magic((quint16)nValue); break;
                        case N_OS2_HEADER::ne_ver: ne.setImageOS2Header_ver((quint8)nValue); break;
                        case N_OS2_HEADER::ne_rev: ne.setImageOS2Header_rev((quint8)nValue); break;
                        case N_OS2_HEADER::ne_enttab: ne.setImageOS2Header_enttab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_cbenttab: ne.setImageOS2Header_cbenttab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_crc: ne.setImageOS2Header_crc((quint32)nValue); break;
                        case N_OS2_HEADER::ne_flags: ne.setImageOS2Header_flags((quint16)nValue); break;
                        case N_OS2_HEADER::ne_autodata: ne.setImageOS2Header_autodata((quint16)nValue); break;
                        case N_OS2_HEADER::ne_heap: ne.setImageOS2Header_heap((quint16)nValue); break;
                        case N_OS2_HEADER::ne_stack: ne.setImageOS2Header_stack((quint16)nValue); break;
                        case N_OS2_HEADER::ne_csip: ne.setImageOS2Header_csip((quint32)nValue); break;
                        case N_OS2_HEADER::ne_sssp: ne.setImageOS2Header_sssp((quint32)nValue); break;
                        case N_OS2_HEADER::ne_cseg: ne.setImageOS2Header_cseg((quint16)nValue); break;
                        case N_OS2_HEADER::ne_cmod: ne.setImageOS2Header_cmod((quint16)nValue); break;
                        case N_OS2_HEADER::ne_cbnrestab: ne.setImageOS2Header_cbnrestab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_segtab: ne.setImageOS2Header_segtab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_rsrctab: ne.setImageOS2Header_rsrctab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_restab: ne.setImageOS2Header_restab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_modtab: ne.setImageOS2Header_modtab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_imptab: ne.setImageOS2Header_imptab((quint16)nValue); break;
                        case N_OS2_HEADER::ne_nrestab: ne.setImageOS2Header_nrestab((quint32)nValue); break;
                        case N_OS2_HEADER::ne_cmovent: ne.setImageOS2Header_cmovent((quint16)nValue); break;
                        case N_OS2_HEADER::ne_align: ne.setImageOS2Header_align((quint16)nValue); break;
                        case N_OS2_HEADER::ne_cres: ne.setImageOS2Header_cres((quint16)nValue); break;
                        case N_OS2_HEADER::ne_exetyp: ne.setImageOS2Header_exetyp((quint8)nValue); break;
                        case N_OS2_HEADER::ne_flagsothers: ne.setImageOS2Header_flagsothers((quint8)nValue); break;
                        case N_OS2_HEADER::ne_pretthunks: ne.setImageOS2Header_pretthunks((quint16)nValue); break;
                        case N_OS2_HEADER::ne_psegrefbytes: ne.setImageOS2Header_psegrefbytes((quint16)nValue); break;
                        case N_OS2_HEADER::ne_swaparea: ne.setImageOS2Header_swaparea((quint16)nValue); break;
                        case N_OS2_HEADER::ne_expver: ne.setImageOS2Header_expver((quint16)nValue); break;
                    }

                    ui->widgetHex_OS2_HEADER->reload();

                    break;
            }

            result = SV_EDITED;
        }
    }

    blockSignals(false);

    return result;
}

void NEWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(g_lineEdit_DOS_HEADER, N_NE_DOS_HEADER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_OS2_HEADER, N_OS2_HEADER::__data_size, bState);

    setComboBoxesReadOnly(g_comboBox, __CB_size, bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetDisasm->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
}

void NEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_DOS_HEADER, N_NE_DOS_HEADER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_OS2_HEADER, N_OS2_HEADER::__data_size, bState);

    _blockSignals((QObject **)g_comboBox, __CB_size, bState);
}

void NEWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    FormatWidget::adjustHeaderTable(nType, pTableWidget);
}

QString NEWidget::typeIdToString(qint32 nType)
{
    Q_UNUSED(nType)

    QString sResult;

    // TODO

    return sResult;
}

void NEWidget::_showInDisasmWindowAddress(qint64 nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SNE::TYPE_DISASM);
    ui->widgetDisasm->setLocation(nAddress, XBinary::LT_ADDRESS, 0);
}

void NEWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SNE::TYPE_DISASM);
    ui->widgetDisasm->setLocation(nOffset, XBinary::LT_OFFSET, 0);
}

void NEWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SNE::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void NEWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi, SNE::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset, nSize);
}

void NEWidget::_findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    Q_UNUSED(nValue)
    Q_UNUSED(endian)
//    setTreeItem(ui->treeWidgetNavi, SNE::TYPE_SEARCH);
//    ui->widgetSearch->findValue(nValue, bIsBigEndian);
#ifdef QT_DEBUG
    qDebug("TODO: _findValue");
#endif
}

void NEWidget::reloadData(bool bSaveSelection)
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
    //    qint64
    //    nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    //    qint64
    //    nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XNE ne(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (ne.isValid()) {
        if (nType == SNE::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), ne.getFileType(), "Info", true);
            }
        } else if (nType == SNE::TYPE_VISUALIZATION) {
            if (!isInitPresent(sInit)) {
                ui->widgetVisualization->setData(getDevice(), ne.getFileType(), true);
            }
        } else if (nType == SNE::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SNE::TYPE_HEX) {
            ui->widgetHex->setWidgetFocus();
            if (!isInitPresent(sInit)) {
                XHexViewWidget::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;
                options.fileType = ne.getFileType();

                if (bSaveSelection) {
                    options.nStartSelectionOffset = -1;
                }

                ui->widgetHex->setXInfoDB(getXInfoDB());
                ui->widgetHex->setData(getDevice(), options);
                //                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                //                ui->widgetHex->enableReadOnly(false);
                ui->widgetHex->reload();
            }
        } else if (nType == SNE::TYPE_DISASM) {
            ui->widgetDisasm->setWidgetFocus();
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = ne.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;
                ui->widgetDisasm->setXInfoDB(getXInfoDB());
                ui->widgetDisasm->setData(getDevice(), options);

                setDisasmInitAddress(-1);
            }
        } else if (nType == SNE::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), ne.getFileType(), 0, -1, true);
            }
        } else if (nType == SNE::TYPE_STRINGS) {
            if (!isInitPresent(sInit)) {
                SearchStringsWidget::OPTIONS stringsOptions = {};
                stringsOptions.bMenu_Hex = true;
                stringsOptions.bMenu_Demangle = true;
                stringsOptions.bAnsi = true;
                // stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bNullTerminated = false;

                ui->widgetStrings->setData(getDevice(), ne.getFileType(), stringsOptions, true);
            }
        } else if (nType == SNE::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;
                signaturesOptions.fileType = ne.getFileType();

                ui->widgetSignatures->setData(getDevice(), signaturesOptions, false);
            }
        } else if (nType == SNE::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                XMemoryMapWidget::OPTIONS options = {};
                options.fileType = ne.getFileType();
                options.bIsSearchEnable = true;

                ui->widgetMemoryMap->setData(getDevice(), options, getXInfoDB());
            }
        } else if (nType == SNE::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), ne.getFileType(), true);
            }
        } else if (nType == SNE::TYPE_NFDSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, ne.getFileType());
            }
        } else if (nType == SNE::TYPE_DIESCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetDIEScan->setData(getDevice(), true, ne.getFileType());
            }
#ifdef USE_YARA
        } else if (nType == SNE::TYPE_YARASCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetYARAScan->setData(XBinary::getDeviceFileName(getDevice()), true);
            }
#endif
        } else if (nType == SNE::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.fileType = ne.getFileType();
                extractorOptions.bMenu_Hex = true;

                ui->widgetExtractor->setData(getDevice(), getXInfoDB(), extractorOptions, true);
            }
        } else if (nType == SNE::TYPE_SEARCH) {
            if (!isInitPresent(sInit)) {
                SearchValuesWidget::OPTIONS options = {};
                options.fileType = ne.getFileType();
                options.bMenu_Hex = true;
                options.bMenu_Disasm = true;

                ui->widgetSearch->setData(getDevice(), options);
            }
        } else if (nType == SNE::TYPE_DOS_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SNE::TYPE_DOS_HEADER, ui->tableWidget_DOS_HEADER, N_NE_DOS_HEADER::records, g_lineEdit_DOS_HEADER, N_NE_DOS_HEADER::__data_size, 0);
                g_comboBox[CB_DOS_HEADER_e_magic] =
                    createComboBox(ui->tableWidget_DOS_HEADER, XMSDOS::getImageMagicsS(), SNE::TYPE_DOS_HEADER, N_NE_DOS_HEADER::e_magic, XComboBoxEx::CBTYPE_LIST);
                g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew] =
                    createInvWidget(ui->tableWidget_DOS_HEADER, SNE::TYPE_DOS_HEADER, N_NE_DOS_HEADER::e_lfanew, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex = ne.getDosHeaderEx();

                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_magic]->setValue_uint16(msdosheaderex.e_magic);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_cblp]->setValue_uint16(msdosheaderex.e_cblp);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_cp]->setValue_uint16(msdosheaderex.e_cp);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_crlc]->setValue_uint16(msdosheaderex.e_crlc);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_cparhdr]->setValue_uint16(msdosheaderex.e_cparhdr);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_minalloc]->setValue_uint16(msdosheaderex.e_minalloc);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_maxalloc]->setValue_uint16(msdosheaderex.e_maxalloc);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_ss]->setValue_uint16(msdosheaderex.e_ss);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_sp]->setValue_uint16(msdosheaderex.e_sp);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_csum]->setValue_uint16(msdosheaderex.e_csum);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_ip]->setValue_uint16(msdosheaderex.e_ip);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_cs]->setValue_uint16(msdosheaderex.e_cs);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_lfarlc]->setValue_uint16(msdosheaderex.e_lfarlc);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_ovno]->setValue_uint16(msdosheaderex.e_ovno);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res_0]->setValue_uint16(msdosheaderex.e_res[0]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res_1]->setValue_uint16(msdosheaderex.e_res[1]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res_2]->setValue_uint16(msdosheaderex.e_res[2]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res_3]->setValue_uint16(msdosheaderex.e_res[3]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_oemid]->setValue_uint16(msdosheaderex.e_oemid);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_oeminfo]->setValue_uint16(msdosheaderex.e_oeminfo);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_0]->setValue_uint16(msdosheaderex.e_res2[0]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_1]->setValue_uint16(msdosheaderex.e_res2[1]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_2]->setValue_uint16(msdosheaderex.e_res2[2]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_3]->setValue_uint16(msdosheaderex.e_res2[3]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_4]->setValue_uint16(msdosheaderex.e_res2[4]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_5]->setValue_uint16(msdosheaderex.e_res2[5]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_6]->setValue_uint16(msdosheaderex.e_res2[6]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_7]->setValue_uint16(msdosheaderex.e_res2[7]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_8]->setValue_uint16(msdosheaderex.e_res2[8]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_res2_9]->setValue_uint16(msdosheaderex.e_res2[9]);
                g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_lfanew]->setValue_uint32(msdosheaderex.e_lfanew);

                g_comboBox[CB_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

                g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&ne, msdosheaderex.e_lfanew, 0);

                qint64 nOffset = ne.getDosHeaderExOffset();
                qint64 nSize = ne.getDosHeaderExSize();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SNE::TYPE_DOS_HEADER], ui->widgetHex_DOS_HEADER);

                blockSignals(false);
            }
        } else if (nType == SNE::TYPE_OS2_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SNE::TYPE_OS2_HEADER, ui->tableWidget_OS2_HEADER, N_OS2_HEADER::records, g_lineEdit_OS2_HEADER, N_OS2_HEADER::__data_size, 0);
                g_comboBox[CB_OS2_HEADER_ne_magic] =
                    createComboBox(ui->tableWidget_OS2_HEADER, XNE::getImageNEMagicsS(), SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_magic, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_OS2_HEADER_ne_flags] =
                    createComboBox(ui->tableWidget_OS2_HEADER, XNE::getImageNEFlagsS(), SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_flags, XComboBoxEx::CBTYPE_FLAGS);
                g_comboBox[CB_OS2_HEADER_ne_exetype] =
                    createComboBox(ui->tableWidget_OS2_HEADER, XNE::getImageNEExetypesS(), SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_exetyp, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_OS2_HEADER_ne_flagsothers] = createComboBox(ui->tableWidget_OS2_HEADER, XNE::getImageNEFlagsothersS(), SNE::TYPE_OS2_HEADER,
                                                                          N_OS2_HEADER::ne_flagsothers, XComboBoxEx::CBTYPE_FLAGS);

                g_invWidget[CB_OS2_HEADER_ne_enttab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_enttab, InvWidget::TYPE_HEX);
                g_invWidget[CB_OS2_HEADER_ne_segtab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_segtab, InvWidget::TYPE_HEX);
                g_invWidget[CB_OS2_HEADER_ne_rsrctab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_rsrctab, InvWidget::TYPE_HEX);
                g_invWidget[CB_OS2_HEADER_ne_restab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_restab, InvWidget::TYPE_HEX);
                g_invWidget[CB_OS2_HEADER_ne_modtab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_modtab, InvWidget::TYPE_HEX);
                g_invWidget[CB_OS2_HEADER_ne_imptab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_imptab, InvWidget::TYPE_HEX);
                g_invWidget[CB_OS2_HEADER_ne_nrestab] = createInvWidget(ui->tableWidget_OS2_HEADER, SNE::TYPE_OS2_HEADER, N_OS2_HEADER::ne_nrestab, InvWidget::TYPE_HEX);

                blockSignals(true);

                XNE_DEF::IMAGE_OS2_HEADER os2header = ne.getImageOS2Header();

                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_magic]->setValue_uint16(os2header.ne_magic);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_ver]->setValue_uint8(os2header.ne_ver);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_rev]->setValue_uint8(os2header.ne_rev);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_enttab]->setValue_uint16(os2header.ne_enttab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_cbenttab]->setValue_uint16(os2header.ne_cbenttab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_crc]->setValue_uint32(os2header.ne_crc);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_flags]->setValue_uint16(os2header.ne_flags);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_autodata]->setValue_uint16(os2header.ne_autodata);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_heap]->setValue_uint16(os2header.ne_heap);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_stack]->setValue_uint16(os2header.ne_stack);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_csip]->setValue_uint32(os2header.ne_csip);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_sssp]->setValue_uint32(os2header.ne_sssp);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_cseg]->setValue_uint16(os2header.ne_cseg);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_cmod]->setValue_uint16(os2header.ne_cmod);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_cbnrestab]->setValue_uint16(os2header.ne_cbnrestab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_segtab]->setValue_uint16(os2header.ne_segtab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_rsrctab]->setValue_uint16(os2header.ne_rsrctab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_restab]->setValue_uint16(os2header.ne_restab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_modtab]->setValue_uint16(os2header.ne_modtab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_imptab]->setValue_uint16(os2header.ne_imptab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_nrestab]->setValue_uint32(os2header.ne_nrestab);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_cmovent]->setValue_uint16(os2header.ne_cmovent);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_align]->setValue_uint16(os2header.ne_align);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_cres]->setValue_uint16(os2header.ne_cres);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_exetyp]->setValue_uint8(os2header.ne_exetyp);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_flagsothers]->setValue_uint8(os2header.ne_flagsothers);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_pretthunks]->setValue_uint16(os2header.ne_pretthunks);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_psegrefbytes]->setValue_uint16(os2header.ne_psegrefbytes);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_swaparea]->setValue_uint16(os2header.ne_swaparea);
                g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_expver]->setValue_uint16(os2header.ne_expver);

                g_comboBox[CB_OS2_HEADER_ne_magic]->setValue(os2header.ne_magic);
                g_comboBox[CB_OS2_HEADER_ne_flags]->setValue(os2header.ne_flags);
                g_comboBox[CB_OS2_HEADER_ne_exetype]->setValue(os2header.ne_exetyp);
                g_comboBox[CB_OS2_HEADER_ne_flagsothers]->setValue(os2header.ne_flagsothers);

                qint64 nOffset = ne.getImageOS2HeaderOffset();

                g_invWidget[CB_OS2_HEADER_ne_enttab]->setOffsetAndSize(&ne, nOffset + os2header.ne_enttab, 0);
                g_invWidget[CB_OS2_HEADER_ne_segtab]->setOffsetAndSize(&ne, nOffset + os2header.ne_segtab, 0);
                g_invWidget[CB_OS2_HEADER_ne_rsrctab]->setOffsetAndSize(&ne, nOffset + os2header.ne_rsrctab, 0);
                g_invWidget[CB_OS2_HEADER_ne_restab]->setOffsetAndSize(&ne, nOffset + os2header.ne_restab, 0);
                g_invWidget[CB_OS2_HEADER_ne_modtab]->setOffsetAndSize(&ne, nOffset + os2header.ne_modtab, 0);
                g_invWidget[CB_OS2_HEADER_ne_imptab]->setOffsetAndSize(&ne, nOffset + os2header.ne_imptab, 0);
                g_invWidget[CB_OS2_HEADER_ne_nrestab]->setOffsetAndSize(&ne, os2header.ne_nrestab, 0);

                qint64 nSize = ne.getImageOS2HeaderSize();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SNE::TYPE_OS2_HEADER], ui->widgetHex_OS2_HEADER);

                blockSignals(false);
            }
        } else if (nType == SNE::TYPE_SEGMENTS) {
            if (!isInitPresent(sInit)) {
                NEProcessData neProcessData(SNE::TYPE_SEGMENTS, &g_tvModel[SNE::TYPE_SEGMENTS], &ne, 0, 0);

                ajustTableView(nType, &neProcessData, &g_tvModel[SNE::TYPE_SEGMENTS], ui->tableView_SEGMENTS, false);

                connect(ui->tableView_SEGMENTS->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_SEGMENTS_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SNE::TYPE_SEGMENTS]->rowCount()) {
                    ui->tableView_SEGMENTS->setCurrentIndex(ui->tableView_SEGMENTS->model()->index(0, 0));
                }
            }
        } else if (nType == SNE::TYPE_OVERLAY)  // TODO Check
        {
            qint64 nOverLayOffset = ne.getOverlayOffset();
            qint64 nOverlaySize = ne.getOverlaySize();

            loadHexSubdevice(nOverLayOffset, nOverlaySize, nOverLayOffset, &g_subDevice[SNE::TYPE_OVERLAY], ui->widgetHex_OVERLAY);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void NEWidget::_widgetValueChanged(QVariant vValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    quint64 nValue = vValue.toULongLong();

    switch (nStype) {
        case SNE::TYPE_DOS_HEADER:
            switch (nNdata) {
                case N_NE_DOS_HEADER::e_magic: g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::e_magic]->setValue_uint16((quint16)nValue); break;
            }

            break;
        case SNE::TYPE_OS2_HEADER:
            switch (nNdata) {
                case N_OS2_HEADER::ne_magic: g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_magic]->setValue_uint16((quint16)nValue); break;
                case N_OS2_HEADER::ne_flags: g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_flags]->setValue_uint16((quint16)nValue); break;
                case N_OS2_HEADER::ne_exetyp: g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_exetyp]->setValue_uint16((quint16)nValue); break;
                case N_OS2_HEADER::ne_flagsothers: g_lineEdit_OS2_HEADER[N_OS2_HEADER::ne_flagsothers]->setValue_uint16((quint16)nValue); break;
            }

            break;
    }
}

void NEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void NEWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void NEWidget::on_toolButtonReload_clicked()
{
    ui->toolButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void NEWidget::enableButton()
{
    ui->toolButtonReload->setEnabled(true);
}

void NEWidget::on_tableWidget_DOS_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_DOS_HEADER, ui->tableWidget_DOS_HEADER);
}

void NEWidget::on_tableWidget_OS2_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_OS2_HEADER, ui->tableWidget_OS2_HEADER);
}

void NEWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void NEWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void NEWidget::onTableView_SEGMENTS_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SNE::TYPE_SEGMENTS, ui->widgetHex_SEGMENTS, ui->tableView_SEGMENTS, &g_subDevice[SNE::TYPE_SEGMENTS]);
}
