/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#include "elfwidget.h"

#include "ui_elfwidget.h"

ELFWidget::ELFWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::ELFWidget)
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

ELFWidget::ELFWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) : ELFWidget(pParent)
{
    ELFWidget::setData(pDevice, options, 0, 0, 0);
    ELFWidget::reload();
}

ELFWidget::~ELFWidget()
{
    delete ui;
}

void ELFWidget::clear()
{
    ELFWidget::reset();

    memset(g_lineEdit_Elf_Ehdr, 0, sizeof g_lineEdit_Elf_Ehdr);
    memset(g_lineEdit_Elf_Interpreter, 0, sizeof g_lineEdit_Elf_Interpreter);
    memset(g_lineEdit_Elf_RunPath, 0, sizeof g_lineEdit_Elf_RunPath);
    memset(g_comboBox, 0, sizeof g_comboBox);
    memset(g_invWidget, 0, sizeof g_invWidget);
    memset(g_tvModel, 0, sizeof g_tvModel);

    _deleteSubdevices(g_subDevice, (sizeof g_subDevice) / (sizeof(SubDevice *)));

    resetWidget();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void ELFWidget::cleanup()
{
    ELFWidget::clear();
}

void ELFWidget::reload()
{
    ELFWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (elf.isValid()) {
        setFileType(elf.getFileType());

        QTreeWidgetItem *pItemInfo = createNewItem(SELF::TYPE_INFO, tr("Info"), XOptions::ICONTYPE_INFO);
        ui->treeWidgetNavi->addTopLevelItem(pItemInfo);
        pItemInfo->addChild(createNewItem(SELF::TYPE_NFDSCAN, "Nauz File Detector (NFD)", XOptions::ICONTYPE_NFD));
        pItemInfo->addChild(createNewItem(SELF::TYPE_DIESCAN, "Detect It Easy (DiE)", XOptions::ICONTYPE_DIE));
#ifdef USE_YARA
        pItemInfo->addChild(createNewItem(SELF::TYPE_YARASCAN, "Yara rules", XOptions::ICONTYPE_YARA));
#endif
        pItemInfo->addChild(createNewItem(SELF::TYPE_VIRUSTOTAL, "VirusTotal", XOptions::ICONTYPE_VIRUSTOTAL));

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_VISUALIZATION, tr("Visualization"), XOptions::ICONTYPE_VISUALIZATION));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HEX, tr("Hex"), XOptions::ICONTYPE_HEX));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_DISASM, tr("Disasm"), XOptions::ICONTYPE_DISASM));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HASH, tr("Hash"), XOptions::ICONTYPE_HASH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_STRINGS, tr("Strings"), XOptions::ICONTYPE_STRING));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_SIGNATURES, tr("Signatures"), XOptions::ICONTYPE_SIGNATURE));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_MEMORYMAP, tr("Memory map"), XOptions::ICONTYPE_MEMORYMAP));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_ENTROPY, tr("Entropy"), XOptions::ICONTYPE_ENTROPY));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_EXTRACTOR, tr("Extractor"), XOptions::ICONTYPE_EXTRACTOR));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_SEARCH, tr("Search"), XOptions::ICONTYPE_SEARCH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Ehdr, "Elf_Ehdr", XOptions::ICONTYPE_HEADER));

        QList<XELF_DEF::Elf_Shdr> listSectionHeaders = elf.getElf_ShdrList(-1);

        if (listSectionHeaders.count()) {
            QTreeWidgetItem *pItemSections = createNewItem(SELF::TYPE_Elf_Shdr, tr("Sections"), XOptions::ICONTYPE_SECTION);

            ui->treeWidgetNavi->addTopLevelItem(pItemSections);

            QList<XBinary::DATASET> listDataSets = elf.getDatasetsFromSections(&listSectionHeaders);

            addDatasets(&elf, pItemSections, &listDataSets);
        }

        QList<XELF_DEF::Elf_Phdr> listProgramHeaders = elf.getElf_PhdrList(-1);

        if (listProgramHeaders.count()) {
            QTreeWidgetItem *pItemPrograms = createNewItem(SELF::TYPE_Elf_Phdr, "Programs", XOptions::ICONTYPE_SEGMENT);  // mn TODO tr

            ui->treeWidgetNavi->addTopLevelItem(pItemPrograms);

            QList<XBinary::DATASET> listDataSets = elf.getDatasetsFromPrograms(&listProgramHeaders);

            addDatasets(&elf, pItemPrograms, &listDataSets);
        }

        ui->treeWidgetNavi->expandAll();

        setDisasmInitAddress(elf.getEntryPointAddress());

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
    }
}

FormatWidget::SV ELFWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    SV result = SV_NONE;

    blockSignals(true);

    quint64 nValue = vValue.toULongLong();
    QString sValue = vValue.toString();

    if (getDevice()->isWritable()) {
        XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (elf.isValid()) {
            bool bIs64 = elf.is64();

            switch (nStype) {
                case SELF::TYPE_Elf_Ehdr:
                    switch (nNdata) {
                        case N_Elf_Ehdr::ei_class: g_comboBox[CB_Elf_Ehdr_iclass]->setValue(nValue); break;
                        case N_Elf_Ehdr::ei_data: g_comboBox[CB_Elf_Ehdr_idata]->setValue(nValue); break;
                        case N_Elf_Ehdr::ei_version: g_comboBox[CB_Elf_Ehdr_iversion]->setValue(nValue); break;
                        case N_Elf_Ehdr::ei_osabi: g_comboBox[CB_Elf_Ehdr_iosabi]->setValue(nValue); break;
                        case N_Elf_Ehdr::e_type: g_comboBox[CB_Elf_Ehdr_type]->setValue(nValue); break;
                        case N_Elf_Ehdr::e_machine: g_comboBox[CB_Elf_Ehdr_machine]->setValue(nValue); break;
                        case N_Elf_Ehdr::e_entry: g_invWidget[INV_Elf_e_entry]->setAddressAndSize(&elf, elf.is64() ? ((quint64)nValue) : ((quint32)nValue), 0); break;
                        case N_Elf_Ehdr::e_phoff: g_invWidget[INV_Elf_e_phoff]->setOffsetAndSize(&elf, elf.is64() ? ((quint64)nValue) : ((quint32)nValue), 0); break;
                        case N_Elf_Ehdr::e_shoff: g_invWidget[INV_Elf_e_shoff]->setOffsetAndSize(&elf, elf.is64() ? ((quint64)nValue) : ((quint32)nValue), 0); break;
                    }
                    break;
            }

            switch (nStype) {
                case SELF::TYPE_Elf_Ehdr:
                    switch (nNdata) {
                        case N_Elf_Ehdr::ei_mag: elf.setIdent_mag((quint32)nValue, 0); break;
                        case N_Elf_Ehdr::ei_class: elf.setIdent_class((quint8)nValue); break;
                        case N_Elf_Ehdr::ei_data: elf.setIdent_data((quint8)nValue); break;
                        case N_Elf_Ehdr::ei_version: elf.setIdent_version((quint8)nValue); break;
                        case N_Elf_Ehdr::ei_osabi: elf.setIdent_osabi((quint8)nValue); break;
                        case N_Elf_Ehdr::ei_abiversion: elf.setIdent_abiversion((quint8)nValue); break;
                        case N_Elf_Ehdr::ei_pad_0: elf.setIdent_pad((quint8)nValue, 0); break;
                        case N_Elf_Ehdr::ei_pad_1: elf.setIdent_pad((quint8)nValue, 1); break;
                        case N_Elf_Ehdr::ei_pad_2: elf.setIdent_pad((quint8)nValue, 2); break;
                        case N_Elf_Ehdr::ei_pad_3: elf.setIdent_pad((quint8)nValue, 3); break;
                        case N_Elf_Ehdr::ei_pad_4: elf.setIdent_pad((quint8)nValue, 4); break;
                        case N_Elf_Ehdr::ei_pad_5: elf.setIdent_pad((quint8)nValue, 5); break;
                        case N_Elf_Ehdr::ei_pad_6: elf.setIdent_pad((quint8)nValue, 6); break;
                        case N_Elf_Ehdr::e_type: (bIs64 ? (elf.setHdr64_type((quint16)nValue)) : (elf.setHdr32_type((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_machine: (bIs64 ? (elf.setHdr64_machine((quint16)nValue)) : (elf.setHdr32_machine((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_version: (bIs64 ? (elf.setHdr64_version((quint32)nValue)) : (elf.setHdr32_version((quint32)nValue))); break;
                        case N_Elf_Ehdr::e_entry: (bIs64 ? (elf.setHdr64_entry((quint64)nValue)) : (elf.setHdr32_entry((quint32)nValue))); break;
                        case N_Elf_Ehdr::e_phoff: (bIs64 ? (elf.setHdr64_phoff((quint64)nValue)) : (elf.setHdr32_phoff((quint32)nValue))); break;
                        case N_Elf_Ehdr::e_shoff: (bIs64 ? (elf.setHdr64_shoff((quint64)nValue)) : (elf.setHdr32_shoff((quint32)nValue))); break;
                        case N_Elf_Ehdr::e_flags: (bIs64 ? (elf.setHdr64_flags((quint32)nValue)) : (elf.setHdr32_flags((quint32)nValue))); break;
                        case N_Elf_Ehdr::e_ehsize: (bIs64 ? (elf.setHdr64_ehsize((quint16)nValue)) : (elf.setHdr32_ehsize((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_phentsize: (bIs64 ? (elf.setHdr64_phentsize((quint16)nValue)) : (elf.setHdr32_phentsize((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_phnum: (bIs64 ? (elf.setHdr64_phnum((quint16)nValue)) : (elf.setHdr32_phnum((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_shentsize: (bIs64 ? (elf.setHdr64_shentsize((quint16)nValue)) : (elf.setHdr32_shentsize((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_shnum: (bIs64 ? (elf.setHdr64_shnum((quint16)nValue)) : (elf.setHdr32_shnum((quint16)nValue))); break;
                        case N_Elf_Ehdr::e_shstrndx: (bIs64 ? (elf.setHdr64_shstrndx((quint16)nValue)) : (elf.setHdr32_shstrndx((quint16)nValue))); break;
                    }

                    ui->widgetHex_Elf_Ehdr->reload();

                    break;

                case SELF::TYPE_INTERPRETER:
                    switch (nNdata) {
                        case N_ELF_INTERPRETER::interpreter: elf.write_ansiString(nOffset, sValue); break;
                    }

                    break;

                case SELF::TYPE_RUNPATH:
                    switch (nNdata) {
                        case N_ELF_RUNPATH::runpath: elf.write_ansiString(nOffset, sValue); break;
                    }

                    break;
            }

            result = SV_EDITED;
        }
    }

    blockSignals(false);

    return result;
}

void ELFWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(g_lineEdit_Elf_Ehdr, N_Elf_Ehdr::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_Elf_Interpreter, N_ELF_INTERPRETER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_Elf_RunPath, N_ELF_RUNPATH::__data_size, bState);

    setComboBoxesReadOnly(g_comboBox, __CB_size, bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetDisasm->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
    ui->widgetHex_Elf_Ehdr->setReadonly(bState);
    ui->widgetHex_Elf_Phdr->setReadonly(bState);
    ui->widgetHex_Elf_Shdr->setReadonly(bState);
    ui->widgetHex_Notes->setReadonly(bState);
    ui->widgetHex_StringTable->setReadonly(bState);
}

void ELFWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_Elf_Ehdr, N_Elf_Ehdr::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_Elf_Interpreter, N_ELF_INTERPRETER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_Elf_RunPath, N_ELF_RUNPATH::__data_size, bState);

    _blockSignals((QObject **)g_comboBox, __CB_size, bState);
}

void ELFWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    FormatWidget::adjustHeaderTable(nType, pTableWidget);
}

void ELFWidget::reloadData(bool bSaveSelection)
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nDataExtraOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_EXTRAOFFSET).toLongLong();
    qint64 nDataExtraSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_EXTRASIZE).toLongLong();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (elf.isValid()) {
        if (nType == SELF::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), elf.getFileType(), "Info", true);
            }
        } else if (nType == SELF::TYPE_VISUALIZATION) {
            if (!isInitPresent(sInit)) {
                ui->widgetVisualization->setData(getDevice(), elf.getFileType(), true);
            }
        } else if (nType == SELF::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SELF::TYPE_HEX) {
            ui->widgetHex->setWidgetFocus();
            if (!isInitPresent(sInit)) {
                XHexViewWidget::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;
                options.fileType = elf.getFileType();

                if (bSaveSelection) {
                    options.nStartSelectionOffset = -1;
                }

                ui->widgetHex->setXInfoDB(getXInfoDB(), getXInfoProfile());
                ui->widgetHex->setData(getDevice(), options);
                //                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                //                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SELF::TYPE_DISASM) {
            ui->widgetDisasm->setWidgetFocus();
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = elf.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;
                ui->widgetDisasm->setXInfoDB(getXInfoDB(), getXInfoProfile());
                ui->widgetDisasm->setData(getDevice(), options);

                setDisasmInitAddress(-1);
            }
        } else if (nType == SELF::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), elf.getFileType(), 0, -1, true);
            }
        } else if (nType == SELF::TYPE_STRINGS) {
            if (!isInitPresent(sInit)) {
                SearchStringsWidget::OPTIONS stringsOptions = {};
                stringsOptions.bMenu_Hex = true;
                stringsOptions.bMenu_Demangle = true;
                stringsOptions.bAnsi = true;
                // stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bNullTerminated = false;

                ui->widgetStrings->setData(getDevice(), elf.getFileType(), stringsOptions, true);
            }
        } else if (nType == SELF::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;
                signaturesOptions.fileType = elf.getFileType();

                ui->widgetSignatures->setData(getDevice(), signaturesOptions, false);
            }
        } else if (nType == SELF::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                XMemoryMapWidget::OPTIONS options = {};
                options.fileType = elf.getFileType();
                options.bIsSearchEnable = true;

                ui->widgetMemoryMap->setData(getDevice(), options, getXInfoDB(), getXInfoProfile());
            }
        } else if (nType == SELF::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), elf.getFileType(), true);
            }
        } else if (nType == SELF::TYPE_NFDSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, elf.getFileType());
            }
        } else if (nType == SELF::TYPE_DIESCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetDIEScan->setData(getDevice(), true, elf.getFileType());
            }
#ifdef USE_YARA
        } else if (nType == SELF::TYPE_YARASCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetYARAScan->setData(XBinary::getDeviceFileName(getDevice()), true);
            }
#endif
        } else if (nType == SELF::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.fileType = elf.getFileType();
                extractorOptions.bMenu_Hex = true;

                ui->widgetExtractor->setData(getDevice(), extractorOptions, true);
            }
        } else if (nType == SELF::TYPE_SEARCH) {
            if (!isInitPresent(sInit)) {
                SearchValuesWidget::OPTIONS options = {};
                options.fileType = elf.getFileType();
                options.bMenu_Hex = true;
                options.bMenu_Disasm = true;

                ui->widgetSearch->setData(getDevice(), options);
            }
        } else if (nType == SELF::TYPE_Elf_Ehdr) {
            if (!isInitPresent(sInit)) {
                if (elf.is64()) {
                    createHeaderTable(SELF::TYPE_Elf_Ehdr, ui->tableWidget_Elf_Ehdr, N_Elf_Ehdr::records64, g_lineEdit_Elf_Ehdr, N_Elf_Ehdr::__data_size, 0);
                } else {
                    createHeaderTable(SELF::TYPE_Elf_Ehdr, ui->tableWidget_Elf_Ehdr, N_Elf_Ehdr::records32, g_lineEdit_Elf_Ehdr, N_Elf_Ehdr::__data_size, 0);
                }

                g_comboBox[CB_Elf_Ehdr_mag] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getIndentMagS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::ei_mag, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_iclass] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getIndentClassesS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::ei_class, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_idata] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getIndentDatasS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::ei_data, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_iversion] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getIndentVersionsS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::ei_version, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_iosabi] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getIndentOsabisS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::ei_osabi, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_type] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getTypesS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_type, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_machine] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getMachinesS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_machine, XComboBoxEx::CBTYPE_LIST);

                g_invWidget[INV_Elf_e_entry] = createInvWidget(ui->tableWidget_Elf_Ehdr, SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_entry, InvWidget::TYPE_HEX);
                g_invWidget[INV_Elf_e_phoff] = createInvWidget(ui->tableWidget_Elf_Ehdr, SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_phoff, InvWidget::TYPE_HEX);
                g_invWidget[INV_Elf_e_shoff] = createInvWidget(ui->tableWidget_Elf_Ehdr, SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_shoff, InvWidget::TYPE_HEX);

                blockSignals(true);

                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_mag]->setValue_uint32(elf.getIdent_mag_LE());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue_uint8(elf.getIdent_class());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue_uint8(elf.getIdent_data());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue_uint8(elf.getIdent_version());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue_uint8(elf.getIdent_osabi());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_abiversion]->setValue_uint8(elf.getIdent_abiversion());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_0]->setValue_uint8(elf.getIdent_pad(0));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_1]->setValue_uint8(elf.getIdent_pad(1));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_2]->setValue_uint8(elf.getIdent_pad(2));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_3]->setValue_uint8(elf.getIdent_pad(3));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_4]->setValue_uint8(elf.getIdent_pad(4));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_5]->setValue_uint8(elf.getIdent_pad(5));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_6]->setValue_uint8(elf.getIdent_pad(6));

                if (elf.is64()) {
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue_uint16(elf.getHdr64_type());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue_uint16(elf.getHdr64_machine());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_version]->setValue_uint32(elf.getHdr64_version());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_entry]->setValue_uint64(elf.getHdr64_entry());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phoff]->setValue_uint64(elf.getHdr64_phoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shoff]->setValue_uint64(elf.getHdr64_shoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_flags]->setValue_uint32(elf.getHdr64_flags());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_ehsize]->setValue_uint16(elf.getHdr64_ehsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phentsize]->setValue_uint16(elf.getHdr64_phentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phnum]->setValue_uint16(elf.getHdr64_phnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shentsize]->setValue_uint16(elf.getHdr64_shentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shnum]->setValue_uint16(elf.getHdr64_shnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shstrndx]->setValue_uint16(elf.getHdr64_shstrndx());

                    g_comboBox[CB_Elf_Ehdr_type]->setValue(elf.getHdr64_type());
                    g_comboBox[CB_Elf_Ehdr_machine]->setValue(elf.getHdr64_machine());

                    g_invWidget[INV_Elf_e_entry]->setAddressAndSize(&elf, elf.getHdr64_entry(), 0);
                    g_invWidget[INV_Elf_e_phoff]->setOffsetAndSize(&elf, elf.getHdr64_phoff(), 0);  // TODO Size
                    g_invWidget[INV_Elf_e_shoff]->setOffsetAndSize(&elf, elf.getHdr64_shoff(), 0);  // TODO Size
                } else {
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue_uint16(elf.getHdr32_type());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue_uint16(elf.getHdr32_machine());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_version]->setValue_uint32(elf.getHdr32_version());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_entry]->setValue_uint32(elf.getHdr32_entry());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phoff]->setValue_uint32(elf.getHdr32_phoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shoff]->setValue_uint32(elf.getHdr32_shoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_flags]->setValue_uint32(elf.getHdr32_flags());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_ehsize]->setValue_uint16(elf.getHdr32_ehsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phentsize]->setValue_uint16(elf.getHdr32_phentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phnum]->setValue_uint16(elf.getHdr32_phnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shentsize]->setValue_uint16(elf.getHdr32_shentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shnum]->setValue_uint16(elf.getHdr32_shnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shstrndx]->setValue_uint16(elf.getHdr32_shstrndx());

                    g_comboBox[CB_Elf_Ehdr_type]->setValue(elf.getHdr32_type());
                    g_comboBox[CB_Elf_Ehdr_machine]->setValue(elf.getHdr32_machine());

                    g_invWidget[INV_Elf_e_entry]->setAddressAndSize(&elf, elf.getHdr32_entry(), 0);
                    g_invWidget[INV_Elf_e_phoff]->setOffsetAndSize(&elf, elf.getHdr32_phoff(), 0);  // TODO Size
                    g_invWidget[INV_Elf_e_shoff]->setOffsetAndSize(&elf, elf.getHdr32_shoff(), 0);  // TODO Size
                }

                g_comboBox[CB_Elf_Ehdr_mag]->setValue(elf.getIdent_mag_LE());
                g_comboBox[CB_Elf_Ehdr_iclass]->setValue(elf.getIdent_class());
                g_comboBox[CB_Elf_Ehdr_idata]->setValue(elf.getIdent_data());
                g_comboBox[CB_Elf_Ehdr_iversion]->setValue(elf.getIdent_version());
                g_comboBox[CB_Elf_Ehdr_iosabi]->setValue(elf.getIdent_osabi());

                qint64 nOffset = elf.getEhdrOffset();
                qint64 nSize = 0;

                if (elf.is64()) {
                    nSize = elf.getEhdr64Size();
                } else {
                    nSize = elf.getEhdr32Size();
                }

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SELF::TYPE_Elf_Ehdr], ui->widgetHex_Elf_Ehdr);

                blockSignals(false);
            }
        } else if (nType == SELF::TYPE_Elf_Shdr) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Shdr, &g_tvModel[SELF::TYPE_Elf_Shdr], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_Elf_Shdr], ui->tableView_Elf_Shdr, false);

                connect(ui->tableView_Elf_Shdr->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Elf_Shdr_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_Elf_Shdr]->rowCount()) {
                    ui->tableView_Elf_Shdr->setCurrentIndex(ui->tableView_Elf_Shdr->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_Elf_Phdr) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Phdr, &g_tvModel[SELF::TYPE_Elf_Phdr], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_Elf_Phdr], ui->tableView_Elf_Phdr, false);

                connect(ui->tableView_Elf_Phdr->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Elf_Phdr_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_Elf_Phdr]->rowCount()) {
                    ui->tableView_Elf_Phdr->setCurrentIndex(ui->tableView_Elf_Phdr->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_Elf_DynamicArrayTags) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_DynamicArrayTags, &g_tvModel[SELF::TYPE_Elf_DynamicArrayTags], &elf, nDataOffset, nDataSize,
                                              nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_Elf_DynamicArrayTags], ui->tableView_DynamicArrayTags);

                connect(ui->tableView_DynamicArrayTags->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DynamicArrayTags_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_Elf_DynamicArrayTags]->rowCount()) {
                    ui->tableView_DynamicArrayTags->setCurrentIndex(ui->tableView_DynamicArrayTags->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_LIBRARIES) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_LIBRARIES, &g_tvModel[SELF::TYPE_LIBRARIES], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_LIBRARIES], ui->tableView_Libraries);

                connect(ui->tableView_Libraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Libraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_LIBRARIES]->rowCount()) {
                    ui->tableView_Libraries->setCurrentIndex(ui->tableView_Libraries->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_INTERPRETER) {
            if (!isInitPresent(sInit)) {
                createListTable(SELF::TYPE_INTERPRETER, ui->tableWidget_Interpreter, N_ELF_INTERPRETER::records, g_lineEdit_Elf_Interpreter,
                                N_ELF_INTERPRETER::__data_size);

                blockSignals(true);

                XBinary::OS_STRING osAnsiString = elf.getOsAnsiString(nDataOffset, nDataSize);

                setLineEdit(g_lineEdit_Elf_Interpreter[N_ELF_INTERPRETER::interpreter], osAnsiString.nSize, osAnsiString.sString, osAnsiString.nOffset);

                blockSignals(false);
            }
        } else if (nType == SELF::TYPE_NOTES) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_NOTES, &g_tvModel[SELF::TYPE_NOTES], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_NOTES], ui->tableView_Notes);

                connect(ui->tableView_Notes->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Notes_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_NOTES]->rowCount()) {
                    ui->tableView_Notes->setCurrentIndex(ui->tableView_Notes->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_RUNPATH) {
            if (!isInitPresent(sInit)) {
                createListTable(SELF::TYPE_RUNPATH, ui->tableWidget_RunPath, N_ELF_RUNPATH::records, g_lineEdit_Elf_RunPath, N_ELF_RUNPATH::__data_size);

                blockSignals(true);

                XBinary::OS_STRING osAnsiString = elf.getOsAnsiString(nDataOffset, nDataSize);

                setLineEdit(g_lineEdit_Elf_RunPath[N_ELF_RUNPATH::runpath], osAnsiString.nSize, osAnsiString.sString, osAnsiString.nOffset);

                blockSignals(false);
            }
        } else if (nType == SELF::TYPE_STRINGTABLE) {
            if (!isInitPresent(sInit)) {
                loadHexSubdevice(nDataOffset, nDataSize, 0, &g_subDevice[SELF::TYPE_STRINGTABLE], ui->widgetHex_StringTable);
            }
        } else if (nType == SELF::TYPE_SYMBOLTABLE) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_SYMBOLTABLE, &g_tvModel[SELF::TYPE_SYMBOLTABLE], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_SYMBOLTABLE], ui->tableView_SymbolTable, false);
            }
        } else if (nType == SELF::TYPE_Elf_Rela) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Rela, &g_tvModel[SELF::TYPE_Elf_Rela], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_Elf_Rela], ui->tableView_Rela, false);
            }
        } else if (nType == SELF::TYPE_Elf_Rel) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Rel, &g_tvModel[SELF::TYPE_Elf_Rel], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(nType, &elfProcessData, &g_tvModel[SELF::TYPE_Elf_Rel], ui->tableView_Rel, false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void ELFWidget::addDatasets(XELF *pElf, QTreeWidgetItem *pParent, QList<XBinary::DATASET> *pListDataSets)
{
    qint32 nNumberOfRecords = pListDataSets->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (pListDataSets->at(i).nType == XELF::DS_INTERPRETER) {
            pParent->addChild(createNewItem(SELF::TYPE_INTERPRETER, pListDataSets->at(i).sName, XOptions::ICONTYPE_PATH, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_LIBRARIES) {
            pParent->addChild(createNewItem(SELF::TYPE_LIBRARIES, pListDataSets->at(i).sName, XOptions::ICONTYPE_LIBRARY, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_STRINGTABLE) {
            pParent->addChild(createNewItem(SELF::TYPE_STRINGTABLE, pListDataSets->at(i).sName, XOptions::ICONTYPE_STRING, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_SYMBOLTABLE) {
            pParent->addChild(createNewItem(SELF::TYPE_SYMBOLTABLE, pListDataSets->at(i).sName, XOptions::ICONTYPE_SYMBOL, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_RUNPATH) {
            pParent->addChild(createNewItem(SELF::TYPE_RUNPATH, pListDataSets->at(i).sName, XOptions::ICONTYPE_PATH, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_NOTES) {
            pParent->addChild(createNewItem(SELF::TYPE_NOTES, pListDataSets->at(i).sName, XOptions::ICONTYPE_NOTE, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_RELA) {
            pParent->addChild(createNewItem(SELF::TYPE_Elf_Rela, pListDataSets->at(i).sName, XOptions::ICONTYPE_RELOC, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_REL) {
            pParent->addChild(createNewItem(SELF::TYPE_Elf_Rel, pListDataSets->at(i).sName, XOptions::ICONTYPE_RELOC, pListDataSets->at(i).nOffset,
                                            pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_DYNAMICTAGS) {
            QTreeWidgetItem *pDynamicTags =
                createNewItem(SELF::TYPE_Elf_DynamicArrayTags, pListDataSets->at(i).sName, XOptions::ICONTYPE_GENERIC, pListDataSets->at(i).nOffset,
                              pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize);

            pParent->addChild(pDynamicTags);

            QList<XELF::TAG_STRUCT> listTagStructs = pElf->_getTagStructs(pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize, pElf->is64(), pElf->isBigEndian());

            XBinary::_MEMORY_MAP memoryMap = pElf->getMemoryMap();

            QList<XBinary::DATASET> listDatasets = pElf->getDatasetsFromTagStructs(&memoryMap, &listTagStructs);

            addDatasets(pElf, pDynamicTags, &listDatasets);
        }
    }
}

void ELFWidget::_widgetValueChanged(QVariant vValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    quint64 nValue = vValue.toULongLong();

    switch (nStype) {
        case SELF::TYPE_Elf_Ehdr:
            switch (nNdata) {
                case N_Elf_Ehdr::ei_class: g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue_uint8((quint8)nValue); break;
                case N_Elf_Ehdr::ei_data: g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue_uint8((quint8)nValue); break;
                case N_Elf_Ehdr::ei_version: g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue_uint8((quint8)nValue); break;
                case N_Elf_Ehdr::ei_osabi: g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue_uint8((quint8)nValue); break;
                case N_Elf_Ehdr::e_type: g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue_uint16((quint16)nValue); break;
                case N_Elf_Ehdr::e_machine: g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue_uint16((quint16)nValue); break;
            }

            break;
    }
}

void ELFWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void ELFWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void ELFWidget::on_toolButtonReload_clicked()
{
    ui->toolButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void ELFWidget::enableButton()
{
    ui->toolButtonReload->setEnabled(true);
}

void ELFWidget::loadShdr(qint32 nRow)
{
    if (nRow != -1) {
        QModelIndex index = ui->tableView_Elf_Shdr->model()->index(nRow, 0);

        qint64 nOffset = ui->tableView_Elf_Shdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->tableView_Elf_Shdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress = ui->tableView_Elf_Shdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SELF::TYPE_Elf_Shdr], ui->widgetHex_Elf_Shdr);
    }
}

void ELFWidget::loadPhdr(qint32 nRow)
{
    if (nRow != -1) {
        QModelIndex index = ui->tableView_Elf_Phdr->model()->index(nRow, 0);

        qint64 nOffset = ui->tableView_Elf_Phdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->tableView_Elf_Phdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress = ui->tableView_Elf_Phdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SELF::TYPE_Elf_Phdr], ui->widgetHex_Elf_Phdr);
    }
}

void ELFWidget::loadNote(qint32 nRow)
{
    if (nRow != -1) {
        QModelIndex index = ui->tableView_Notes->model()->index(nRow, 0);

        qint64 nOffset = ui->tableView_Notes->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->tableView_Notes->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress = ui->tableView_Notes->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SELF::TYPE_NOTES], ui->widgetHex_Notes);
    }
}

void ELFWidget::on_tableWidget_Elf_Ehdr_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_Elf_Ehdr, ui->tableWidget_Elf_Ehdr);
}

void ELFWidget::editSectionHeader()
{
    showSectionHeader(SELF::TYPE_Elf_Shdr, ui->tableView_Elf_Shdr);
}

void ELFWidget::sectionHex()
{
    showSectionHex(ui->tableView_Elf_Shdr);
}

void ELFWidget::sectionDisasm()
{
    showSectionDisasm(ui->tableView_Elf_Shdr);
}

void ELFWidget::sectionEntropy()
{
    showSectionEntropy(ui->tableView_Elf_Shdr);
}

void ELFWidget::sectionDump()
{
    dumpSection(ui->tableView_Elf_Shdr);
}

void ELFWidget::editProgramHeader()
{
    showSectionHeader(SELF::TYPE_Elf_Phdr, ui->tableView_Elf_Phdr);
}

void ELFWidget::programHex()
{
    showSectionHex(ui->tableView_Elf_Phdr);
}

void ELFWidget::programDisasm()
{
    showSectionDisasm(ui->tableView_Elf_Phdr);
}

void ELFWidget::programEntropy()
{
    showSectionEntropy(ui->tableView_Elf_Phdr);
}

void ELFWidget::programDump()
{
    dumpSection(ui->tableView_Elf_Phdr);
}

void ELFWidget::editDynamicArrayTag()
{
    showSectionHeader(SELF::TYPE_Elf_DynamicArrayTags, ui->tableView_DynamicArrayTags);
}

void ELFWidget::on_tableView_SymbolTable_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_SymbolTable->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QList<XShortcuts::MENUITEM> listMenuItems;

        getShortcuts()->_addMenuItem(&listMenuItems, X_ID_TABLE_EDIT, this, SLOT(editSymbolHeader()), XShortcuts::GROUPID_NONE);
        getShortcuts()->_addMenuItem(&listMenuItems, X_ID_TABLE_DEMANGLE, this, SLOT(symbolDemangle()), XShortcuts::GROUPID_NONE);
        getShortcuts()->_addMenuItem_CopyRow(&listMenuItems, ui->tableView_SymbolTable);

        QList<QObject *> listObjects = getShortcuts()->adjustContextMenu(&contextMenu, &listMenuItems);

        contextMenu.exec(ui->tableView_SymbolTable->viewport()->mapToGlobal(pos));

        XOptions::deleteQObjectList(&listObjects);
    }
}

void ELFWidget::editSymbolHeader()
{
    showSectionHeader(SELF::TYPE_SYMBOLTABLE, ui->tableView_SymbolTable);
}

void ELFWidget::symbolDemangle()
{
    showTableViewDemangle(ui->tableView_SymbolTable, N_Elf64_Sym::st_size + 2);
}

void ELFWidget::showSectionHeader(qint32 nType, QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET).toLongLong();

        qint64 nStringTableOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET).toLongLong();
        qint64 nStringTableSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE).toLongLong();

        ELFSectionHeaderWidget *pSectionHeaderWidget = new ELFSectionHeaderWidget(getDevice(), getOptions(), (quint32)nRow, nOffset, nType, this);

        pSectionHeaderWidget->setStringTable(nStringTableOffset, nStringTableSize);

        DialogSectionHeader dsh(this);
        dsh.setGlobal(getShortcuts(), getGlobalOptions());
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(nType));

        connect(&dsh, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));

        dsh.exec();

        reloadData(true);

        pTableView->setCurrentIndex(pTableView->model()->index(nRow, 0));
    }
}

QString ELFWidget::typeIdToString(qint32 nType)
{
    QString sResult = tr("Unknown");

    switch (nType) {
        case SELF::TYPE_Elf_Shdr: sResult = QString("Section %1").arg(tr("Header")); break;
        case SELF::TYPE_Elf_Phdr: sResult = QString("Program %1").arg(tr("Header")); break;
        case SELF::TYPE_SYMBOLTABLE: sResult = QString("Symbol %1").arg(tr("Header")); break;
        case SELF::TYPE_Elf_DynamicArrayTags: sResult = QString("Tag"); break;
        case SELF::TYPE_Elf_Rela: sResult = QString("Relocation A"); break;
        case SELF::TYPE_Elf_Rel: sResult = QString("Relocation"); break;
    }

    return sResult;
}

void ELFWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_DISASM);
    ui->widgetDisasm->setLocation(nAddress, XBinary::LT_ADDRESS, 0);
}

void ELFWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_DISASM);
    ui->widgetDisasm->setLocation(nOffset, XBinary::LT_OFFSET, 0);
}

void ELFWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void ELFWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset, nSize);
}

void ELFWidget::_findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_SEARCH);
    ui->widgetSearch->findValue(nValue, endian);
}

void ELFWidget::on_tableView_Elf_Shdr_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSectionHeader();
}

void ELFWidget::on_tableView_Elf_Shdr_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Elf_Shdr->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(this);
        QAction actionHex(this);
        QAction actionDisasm(this);
        QAction actionEntropy(this);
        QAction actionDump(this);

        getShortcuts()->adjustAction(&contextMenu, &actionEdit, X_ID_TABLE_EDIT, this, SLOT(editSectionHeader()));
        getShortcuts()->adjustAction(&contextMenu, &actionHex, X_ID_TABLE_SELECTION_HEX, this, SLOT(sectionHex()));
        getShortcuts()->adjustAction(&contextMenu, &actionDisasm, X_ID_TABLE_SELECTION_DISASM, this, SLOT(sectionDisasm()));
        getShortcuts()->adjustAction(&contextMenu, &actionEntropy, X_ID_TABLE_SELECTION_ENTROPY, this, SLOT(sectionEntropy()));
        getShortcuts()->adjustAction(&contextMenu, &actionDump, X_ID_TABLE_SELECTION_DUMPTOFILE, this, SLOT(sectionDump()));

        if (!getTableViewItemSize(ui->tableView_Elf_Shdr)) {
            actionHex.setEnabled(false);
            actionDisasm.setEnabled(false);
            actionEntropy.setEnabled(false);
            actionDump.setEnabled(false);
        }

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Elf_Shdr);

        contextMenu.exec(ui->tableView_Elf_Shdr->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_tableView_SymbolTable_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSymbolHeader();
}

void ELFWidget::onTableView_Elf_Shdr_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SELF::TYPE_Elf_Shdr, ui->widgetHex_Elf_Shdr, ui->tableView_Elf_Shdr, &g_subDevice[SELF::TYPE_Elf_Shdr]);
}

void ELFWidget::onTableView_Elf_Phdr_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SELF::TYPE_Elf_Phdr, ui->widgetHex_Elf_Phdr, ui->tableView_Elf_Phdr, &g_subDevice[SELF::TYPE_Elf_Phdr]);
}

void ELFWidget::onTableView_DynamicArrayTags_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    // TODO
}

void ELFWidget::onTableView_Notes_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SELF::TYPE_NOTES, ui->widgetHex_Notes, ui->tableView_Notes, &g_subDevice[SELF::TYPE_NOTES]);
}

void ELFWidget::onTableView_Libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    // TODO
}

void ELFWidget::on_tableView_Elf_Phdr_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editProgramHeader();
}

void ELFWidget::on_tableView_Elf_Phdr_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Elf_Phdr->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(this);
        QAction actionHex(this);
        QAction actionDisasm(this);
        QAction actionEntropy(this);
        QAction actionDump(this);

        getShortcuts()->adjustAction(&contextMenu, &actionEdit, X_ID_TABLE_EDIT, this, SLOT(editProgramHeader()));
        getShortcuts()->adjustAction(&contextMenu, &actionHex, X_ID_TABLE_SELECTION_HEX, this, SLOT(programHex()));
        getShortcuts()->adjustAction(&contextMenu, &actionDisasm, X_ID_TABLE_SELECTION_DISASM, this, SLOT(programDisasm()));
        getShortcuts()->adjustAction(&contextMenu, &actionEntropy, X_ID_TABLE_SELECTION_ENTROPY, this, SLOT(programEntropy()));
        getShortcuts()->adjustAction(&contextMenu, &actionDump, X_ID_TABLE_SELECTION_DUMPTOFILE, this, SLOT(programDump()));

        if (!getTableViewItemSize(ui->tableView_Elf_Phdr)) {
            actionHex.setEnabled(false);
            actionDisasm.setEnabled(false);
            actionEntropy.setEnabled(false);
            actionDump.setEnabled(false);
        }

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Elf_Phdr);

        contextMenu.exec(ui->tableView_Elf_Phdr->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_tableView_DynamicArrayTags_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editDynamicArrayTag();
}

void ELFWidget::on_tableView_DynamicArrayTags_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DynamicArrayTags->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(this);

        getShortcuts()->adjustAction(&contextMenu, &actionEdit, X_ID_TABLE_EDIT, this, SLOT(editDynamicArrayTag()));

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_DynamicArrayTags);

        contextMenu.exec(ui->tableView_DynamicArrayTags->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_tableView_Notes_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    // TODO
}

void ELFWidget::on_tableView_Rela_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editRelaHeaderTag();
}

void ELFWidget::on_tableView_Rela_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Rela->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(this);

        getShortcuts()->adjustAction(&contextMenu, &actionEdit, X_ID_TABLE_EDIT, this, SLOT(editRelaHeaderTag()));

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Rela);

        contextMenu.exec(ui->tableView_Rela->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::editRelaHeaderTag()
{
    showSectionHeader(SELF::TYPE_Elf_Rela, ui->tableView_Rela);
}

void ELFWidget::editRelHeaderTag()
{
    showSectionHeader(SELF::TYPE_Elf_Rel, ui->tableView_Rel);
}

void ELFWidget::on_tableView_Rel_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editRelHeaderTag();
}

void ELFWidget::on_tableView_Rel_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Rel->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(this);

        getShortcuts()->adjustAction(&contextMenu, &actionEdit, X_ID_TABLE_EDIT, this, SLOT(editRelHeaderTag()));

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Rel);

        contextMenu.exec(ui->tableView_Rel->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void ELFWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void ELFWidget::on_pushButtonSaveSections_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Elf_Shdr->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(tr("Sections"))));
}

void ELFWidget::on_pushButtonSavePrograms_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Elf_Phdr->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Programs"))));
}

void ELFWidget::on_pushButtonSave_Rela_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Rela->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Rela"))));
}

void ELFWidget::on_pushButtonSave_Rel_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Rel->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Rel"))));
}

void ELFWidget::on_pushButtonSave_Elf_Ehdr_clicked()
{
    saveHeaderTable(ui->tableWidget_Elf_Ehdr, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Elf_Ehdr"))));
}

void ELFWidget::on_pushButtonSave_SymbolTable_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_SymbolTable->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("SymbolTable"))));
}
