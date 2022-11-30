/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
    setTreeItem(ui->treeWidgetNavi, 0);

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
}

void ELFWidget::reload()
{
    ELFWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (elf.isValid()) {
        setFileType(elf.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_INFO, tr("Info")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_VIRUSTOTAL, "VirusTotal"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HEX, tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_DISASM, tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HASH, tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_STRINGS, tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_SIGNATURES, tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_MEMORYMAP, tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_ENTROPY, tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HEURISTICSCAN, tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Ehdr, "Elf_Ehdr"));

        QList<XELF_DEF::Elf_Shdr> listSectionHeaders = elf.getElf_ShdrList();

        if (listSectionHeaders.count()) {
            QTreeWidgetItem *pItemSections = createNewItem(SELF::TYPE_Elf_Shdr, tr("Sections"));

            ui->treeWidgetNavi->addTopLevelItem(pItemSections);

            QList<XBinary::DATASET> listDataSets = elf.getDatasetsFromSections(&listSectionHeaders);

            addDatasets(&elf, pItemSections, &listDataSets);
        }

        QList<XELF_DEF::Elf_Phdr> listProgramHeaders = elf.getElf_PhdrList();

        if (listProgramHeaders.count()) {
            QTreeWidgetItem *pItemPrograms = createNewItem(SELF::TYPE_Elf_Phdr, "Programs");  // mn TODO tr

            ui->treeWidgetNavi->addTopLevelItem(pItemPrograms);

            QList<XBinary::DATASET> listDataSets = elf.getDatasetsFromPrograms(&listProgramHeaders);

            addDatasets(&elf, pItemPrograms, &listDataSets);
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);

        setDisasmInitAddress(elf.getEntryPointAddress());
    }
}

FormatWidget::SV ELFWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
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
                        case N_Elf_Ehdr::ei_class:
                            g_comboBox[CB_Elf_Ehdr_iclass]->setValue(nValue);
                            break;
                        case N_Elf_Ehdr::ei_data:
                            g_comboBox[CB_Elf_Ehdr_idata]->setValue(nValue);
                            break;
                        case N_Elf_Ehdr::ei_version:
                            g_comboBox[CB_Elf_Ehdr_iversion]->setValue(nValue);
                            break;
                        case N_Elf_Ehdr::ei_osabi:
                            g_comboBox[CB_Elf_Ehdr_iosabi]->setValue(nValue);
                            break;
                        case N_Elf_Ehdr::e_type:
                            g_comboBox[CB_Elf_Ehdr_type]->setValue(nValue);
                            break;
                        case N_Elf_Ehdr::e_machine:
                            g_comboBox[CB_Elf_Ehdr_machine]->setValue(nValue);
                            break;
                        case N_Elf_Ehdr::e_entry:
                            g_invWidget[INV_Elf_e_entry]->setAddressAndSize(&elf, elf.is64() ? ((quint64)nValue) : ((quint32)nValue), 0);
                            break;
                        case N_Elf_Ehdr::e_phoff:
                            g_invWidget[INV_Elf_e_phoff]->setOffsetAndSize(&elf, elf.is64() ? ((quint64)nValue) : ((quint32)nValue), 0);
                            break;
                        case N_Elf_Ehdr::e_shoff:
                            g_invWidget[INV_Elf_e_shoff]->setOffsetAndSize(&elf, elf.is64() ? ((quint64)nValue) : ((quint32)nValue), 0);
                            break;
                    }
                    break;
            }

            switch (nStype) {
                case SELF::TYPE_Elf_Ehdr:
                    switch (nNdata) {
                        case N_Elf_Ehdr::ei_mag:
                            elf.setIdent_mag((quint32)nValue, 0);
                            break;
                        case N_Elf_Ehdr::ei_class:
                            elf.setIdent_class((quint8)nValue);
                            break;
                        case N_Elf_Ehdr::ei_data:
                            elf.setIdent_data((quint8)nValue);
                            break;
                        case N_Elf_Ehdr::ei_version:
                            elf.setIdent_version((quint8)nValue);
                            break;
                        case N_Elf_Ehdr::ei_osabi:
                            elf.setIdent_osabi((quint8)nValue);
                            break;
                        case N_Elf_Ehdr::ei_abiversion:
                            elf.setIdent_abiversion((quint8)nValue);
                            break;
                        case N_Elf_Ehdr::ei_pad_0:
                            elf.setIdent_pad((quint8)nValue, 0);
                            break;
                        case N_Elf_Ehdr::ei_pad_1:
                            elf.setIdent_pad((quint8)nValue, 1);
                            break;
                        case N_Elf_Ehdr::ei_pad_2:
                            elf.setIdent_pad((quint8)nValue, 2);
                            break;
                        case N_Elf_Ehdr::ei_pad_3:
                            elf.setIdent_pad((quint8)nValue, 3);
                            break;
                        case N_Elf_Ehdr::ei_pad_4:
                            elf.setIdent_pad((quint8)nValue, 4);
                            break;
                        case N_Elf_Ehdr::ei_pad_5:
                            elf.setIdent_pad((quint8)nValue, 5);
                            break;
                        case N_Elf_Ehdr::ei_pad_6:
                            elf.setIdent_pad((quint8)nValue, 6);
                            break;
                        case N_Elf_Ehdr::e_type:
                            (bIs64 ? (elf.setHdr64_type((quint16)nValue)) : (elf.setHdr32_type((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_machine:
                            (bIs64 ? (elf.setHdr64_machine((quint16)nValue)) : (elf.setHdr32_machine((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_version:
                            (bIs64 ? (elf.setHdr64_version((quint32)nValue)) : (elf.setHdr32_version((quint32)nValue)));
                            break;
                        case N_Elf_Ehdr::e_entry:
                            (bIs64 ? (elf.setHdr64_entry((quint64)nValue)) : (elf.setHdr32_entry((quint32)nValue)));
                            break;
                        case N_Elf_Ehdr::e_phoff:
                            (bIs64 ? (elf.setHdr64_phoff((quint64)nValue)) : (elf.setHdr32_phoff((quint32)nValue)));
                            break;
                        case N_Elf_Ehdr::e_shoff:
                            (bIs64 ? (elf.setHdr64_shoff((quint64)nValue)) : (elf.setHdr32_shoff((quint32)nValue)));
                            break;
                        case N_Elf_Ehdr::e_flags:
                            (bIs64 ? (elf.setHdr64_flags((quint32)nValue)) : (elf.setHdr32_flags((quint32)nValue)));
                            break;
                        case N_Elf_Ehdr::e_ehsize:
                            (bIs64 ? (elf.setHdr64_ehsize((quint16)nValue)) : (elf.setHdr32_ehsize((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_phentsize:
                            (bIs64 ? (elf.setHdr64_phentsize((quint16)nValue)) : (elf.setHdr32_phentsize((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_phnum:
                            (bIs64 ? (elf.setHdr64_phnum((quint16)nValue)) : (elf.setHdr32_phnum((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_shentsize:
                            (bIs64 ? (elf.setHdr64_shentsize((quint16)nValue)) : (elf.setHdr32_shentsize((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_shnum:
                            (bIs64 ? (elf.setHdr64_shnum((quint16)nValue)) : (elf.setHdr32_shnum((quint16)nValue)));
                            break;
                        case N_Elf_Ehdr::e_shstrndx:
                            (bIs64 ? (elf.setHdr64_shstrndx((quint16)nValue)) : (elf.setHdr32_shstrndx((quint16)nValue)));
                            break;
                    }

                    ui->widgetHex_Elf_Ehdr->reload();

                    break;

                case SELF::TYPE_INTERPRETER:
                    switch (nNdata) {
                        case N_ELF_INTERPRETER::interpreter:
                            elf.write_ansiString(nOffset, sValue);
                            break;
                    }

                    break;

                case SELF::TYPE_RUNPATH:
                    switch (nNdata) {
                        case N_ELF_RUNPATH::runpath:
                            elf.write_ansiString(nOffset, sValue);
                            break;
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

void ELFWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    XBinary::MODE mode = XELF::getMode(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET, getColumnWidth(this, CW_UINT16, mode));
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE, getColumnWidth(this, CW_TYPE, mode));

    switch (nType) {
        case SELF::TYPE_Elf_Ehdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;
    }
}

void ELFWidget::reloadData()
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
        } else if (nType == SELF::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SELF::TYPE_HEX) {
            if (!isInitPresent(sInit)) {
                XHexView::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;

                ui->widgetHex->setData(getDevice(), options);
                ui->widgetHex->setBackupDevice(getBackupDevice());
                //                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                //                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SELF::TYPE_DISASM) {
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = elf.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;

                ui->widgetDisasm->setData(getDevice(), options, getXInfoDB());
                ui->widgetDisasm->setBackupDevice(getBackupDevice());

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
                stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bCStrings = false;

                ui->widgetStrings->setData(getDevice(), stringsOptions, true);
            }
        } else if (nType == SELF::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;

                ui->widgetSignatures->setData(getDevice(), elf.getFileType(), signaturesOptions, false);
            }
        } else if (nType == SELF::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                ui->widgetMemoryMap->setData(getDevice(), elf.getFileType());
            }
        } else if (nType == SELF::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), elf.getFileType(), true);
            }
        } else if (nType == SELF::TYPE_HEURISTICSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, elf.getFileType());
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
                g_comboBox[CB_Elf_Ehdr_type] = createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getTypesS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_type, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Elf_Ehdr_machine] =
                    createComboBox(ui->tableWidget_Elf_Ehdr, XELF::getMachinesS(), SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_machine, XComboBoxEx::CBTYPE_LIST);

                g_invWidget[INV_Elf_e_entry] = createInvWidget(ui->tableWidget_Elf_Ehdr, SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_entry, InvWidget::TYPE_HEX);
                g_invWidget[INV_Elf_e_phoff] = createInvWidget(ui->tableWidget_Elf_Ehdr, SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_phoff, InvWidget::TYPE_HEX);
                g_invWidget[INV_Elf_e_shoff] = createInvWidget(ui->tableWidget_Elf_Ehdr, SELF::TYPE_Elf_Ehdr, N_Elf_Ehdr::e_shoff, InvWidget::TYPE_HEX);

                blockSignals(true);

                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_mag]->setValue(elf.getIdent_mag_LE());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue(elf.getIdent_class());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue(elf.getIdent_data());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue(elf.getIdent_version());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue(elf.getIdent_osabi());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_abiversion]->setValue(elf.getIdent_abiversion());
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_0]->setValue(elf.getIdent_pad(0));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_1]->setValue(elf.getIdent_pad(1));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_2]->setValue(elf.getIdent_pad(2));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_3]->setValue(elf.getIdent_pad(3));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_4]->setValue(elf.getIdent_pad(4));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_5]->setValue(elf.getIdent_pad(5));
                g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_6]->setValue(elf.getIdent_pad(6));

                if (elf.is64()) {
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue(elf.getHdr64_type());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue(elf.getHdr64_machine());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_version]->setValue(elf.getHdr64_version());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_entry]->setValue(elf.getHdr64_entry());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phoff]->setValue(elf.getHdr64_phoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shoff]->setValue(elf.getHdr64_shoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_flags]->setValue(elf.getHdr64_flags());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_ehsize]->setValue(elf.getHdr64_ehsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phentsize]->setValue(elf.getHdr64_phentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phnum]->setValue(elf.getHdr64_phnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shentsize]->setValue(elf.getHdr64_shentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shnum]->setValue(elf.getHdr64_shnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shstrndx]->setValue(elf.getHdr64_shstrndx());

                    g_comboBox[CB_Elf_Ehdr_type]->setValue(elf.getHdr64_type());
                    g_comboBox[CB_Elf_Ehdr_machine]->setValue(elf.getHdr64_machine());

                    g_invWidget[INV_Elf_e_entry]->setAddressAndSize(&elf, elf.getHdr64_entry(), 0);
                    g_invWidget[INV_Elf_e_phoff]->setOffsetAndSize(&elf, elf.getHdr64_phoff(), 0);  // TODO Size
                    g_invWidget[INV_Elf_e_shoff]->setOffsetAndSize(&elf, elf.getHdr64_shoff(), 0);  // TODO Size
                } else {
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue(elf.getHdr32_type());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue(elf.getHdr32_machine());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_version]->setValue(elf.getHdr32_version());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_entry]->setValue(elf.getHdr32_entry());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phoff]->setValue(elf.getHdr32_phoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shoff]->setValue(elf.getHdr32_shoff());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_flags]->setValue(elf.getHdr32_flags());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_ehsize]->setValue(elf.getHdr32_ehsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phentsize]->setValue(elf.getHdr32_phentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phnum]->setValue(elf.getHdr32_phnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shentsize]->setValue(elf.getHdr32_shentsize());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shnum]->setValue(elf.getHdr32_shnum());
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shstrndx]->setValue(elf.getHdr32_shstrndx());

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

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_Elf_Shdr], ui->tableView_Elf_Shdr, nullptr, false);

                connect(ui->tableView_Elf_Shdr->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Elf_Shdr_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_Elf_Shdr]->rowCount()) {
                    ui->tableView_Elf_Shdr->setCurrentIndex(ui->tableView_Elf_Shdr->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_Elf_Phdr) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Phdr, &g_tvModel[SELF::TYPE_Elf_Phdr], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_Elf_Phdr], ui->tableView_Elf_Phdr, nullptr, false);

                connect(ui->tableView_Elf_Phdr->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Elf_Phdr_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_Elf_Phdr]->rowCount()) {
                    ui->tableView_Elf_Phdr->setCurrentIndex(ui->tableView_Elf_Phdr->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_Elf_DynamicArrayTags) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_DynamicArrayTags, &g_tvModel[SELF::TYPE_Elf_DynamicArrayTags], &elf, nDataOffset, nDataSize, nDataExtraOffset,
                                              nDataExtraSize);

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_Elf_DynamicArrayTags], ui->tableView_DynamicArrayTags);

                connect(ui->tableView_DynamicArrayTags->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DynamicArrayTags_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_Elf_DynamicArrayTags]->rowCount()) {
                    ui->tableView_DynamicArrayTags->setCurrentIndex(ui->tableView_DynamicArrayTags->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_LIBRARIES) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_LIBRARIES, &g_tvModel[SELF::TYPE_LIBRARIES], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_LIBRARIES], ui->tableView_Libraries);

                connect(ui->tableView_Libraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Libraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SELF::TYPE_LIBRARIES]->rowCount()) {
                    ui->tableView_Libraries->setCurrentIndex(ui->tableView_Libraries->model()->index(0, 0));
                }
            }
        } else if (nType == SELF::TYPE_INTERPRETER) {
            if (!isInitPresent(sInit)) {
                createListTable(SELF::TYPE_INTERPRETER, ui->tableWidget_Interpreter, N_ELF_INTERPRETER::records, g_lineEdit_Elf_Interpreter, N_ELF_INTERPRETER::__data_size);

                blockSignals(true);

                XBinary::OS_STRING osAnsiString = elf.getOsAnsiString(nDataOffset, nDataSize);

                setLineEdit(g_lineEdit_Elf_Interpreter[N_ELF_INTERPRETER::interpreter], osAnsiString.nSize, osAnsiString.sString, osAnsiString.nOffset);

                blockSignals(false);
            }
        } else if (nType == SELF::TYPE_NOTES) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_NOTES, &g_tvModel[SELF::TYPE_NOTES], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_NOTES], ui->tableView_Notes);

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

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_SYMBOLTABLE], ui->tableView_SymbolTable, nullptr, false);
            }
        } else if (nType == SELF::TYPE_Elf_Rela) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Rela, &g_tvModel[SELF::TYPE_Elf_Rela], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_Elf_Rela], ui->tableView_Rela, nullptr, false);
            }
        } else if (nType == SELF::TYPE_Elf_Rel) {
            if (!isInitPresent(sInit)) {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Rel, &g_tvModel[SELF::TYPE_Elf_Rel], &elf, nDataOffset, nDataSize, nDataExtraOffset, nDataExtraSize);

                ajustTableView(&elfProcessData, &g_tvModel[SELF::TYPE_Elf_Rel], ui->tableView_Rel, nullptr, false);
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
            pParent->addChild(createNewItem(SELF::TYPE_INTERPRETER, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_LIBRARIES) {
            pParent->addChild(createNewItem(SELF::TYPE_LIBRARIES, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_STRINGTABLE) {
            pParent->addChild(createNewItem(SELF::TYPE_STRINGTABLE, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_SYMBOLTABLE) {
            pParent->addChild(createNewItem(SELF::TYPE_SYMBOLTABLE, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_RUNPATH) {
            pParent->addChild(createNewItem(SELF::TYPE_RUNPATH, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_NOTES) {
            pParent->addChild(createNewItem(SELF::TYPE_NOTES, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_RELA) {
            pParent->addChild(createNewItem(SELF::TYPE_Elf_Rela, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_REL) {
            pParent->addChild(createNewItem(SELF::TYPE_Elf_Rel, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize,
                                            pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize));
        } else if (pListDataSets->at(i).nType == XELF::DS_DYNAMICTAGS) {
            QTreeWidgetItem *pDynamicTags = createNewItem(SELF::TYPE_Elf_DynamicArrayTags, pListDataSets->at(i).sName, pListDataSets->at(i).nOffset,
                                                          pListDataSets->at(i).nSize, pListDataSets->at(i).nStringTableOffset, pListDataSets->at(i).nStringTableSize);

            pParent->addChild(pDynamicTags);

            QList<XELF::TAG_STRUCT> listTagStructs = pElf->_getTagStructs(pListDataSets->at(i).nOffset, pListDataSets->at(i).nSize, pElf->is64(), pElf->isBigEndian());

            XBinary::_MEMORY_MAP memoryMap = pElf->getMemoryMap();

            QList<XBinary::DATASET> listDatasets = pElf->getDatasetsFromTagStructs(&memoryMap, &listTagStructs);

            addDatasets(pElf, pDynamicTags, &listDatasets);
        }
    }
}

void ELFWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SELF::TYPE_Elf_Ehdr:
            switch (nNdata) {
                case N_Elf_Ehdr::ei_class:
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_data:
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_version:
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_osabi:
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue((quint8)nValue);
                    break;
                case N_Elf_Ehdr::e_type:
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue((quint16)nValue);
                    break;
                case N_Elf_Ehdr::e_machine:
                    g_lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue((quint16)nValue);
                    break;
            }

            break;
    }
}

void ELFWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData();
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void ELFWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void ELFWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void ELFWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void ELFWidget::loadShdr(int nRow)
{
    if (nRow != -1) {
        QModelIndex index = ui->tableView_Elf_Shdr->model()->index(nRow, 0);

        qint64 nOffset = ui->tableView_Elf_Shdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->tableView_Elf_Shdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress = ui->tableView_Elf_Shdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SELF::TYPE_Elf_Shdr], ui->widgetHex_Elf_Shdr);
    }
}

void ELFWidget::loadPhdr(int nRow)
{
    if (nRow != -1) {
        QModelIndex index = ui->tableView_Elf_Phdr->model()->index(nRow, 0);

        qint64 nOffset = ui->tableView_Elf_Phdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->tableView_Elf_Phdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress = ui->tableView_Elf_Phdr->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SELF::TYPE_Elf_Phdr], ui->widgetHex_Elf_Phdr);
    }
}

void ELFWidget::loadNote(int nRow)
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

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSymbolHeader()));

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(symbolDemangle()));

        contextMenu.addAction(&actionEdit);
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_SymbolTable->viewport()->mapToGlobal(pos));
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

void ELFWidget::showSectionHeader(int nType, QTableView *pTableView)
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
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(nType));
        dsh.setGlobal(getShortcuts(), getGlobalOptions());

        connect(&dsh, SIGNAL(changed()), this, SLOT(setEdited()));

        dsh.exec();

        reloadData();

        pTableView->setCurrentIndex(pTableView->model()->index(nRow, 0));
    }
}

QString ELFWidget::typeIdToString(int nType)
{
    QString sResult = tr("Unknown");

    switch (nType) {
        case SELF::TYPE_Elf_Shdr:
            sResult = QString("Section %1").arg(tr("Header"));
            break;
        case SELF::TYPE_Elf_Phdr:
            sResult = QString("Program %1").arg(tr("Header"));
            break;
        case SELF::TYPE_SYMBOLTABLE:
            sResult = QString("Symbol %1").arg(tr("Header"));
            break;
        case SELF::TYPE_Elf_DynamicArrayTags:
            sResult = QString("Tag");
            break;
        case SELF::TYPE_Elf_Rela:
            sResult = QString("Relocation A");
            break;
        case SELF::TYPE_Elf_Rel:
            sResult = QString("Relocation");
            break;
    }

    return sResult;
}

void ELFWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_DISASM);
    ui->widgetDisasm->goToAddress(nAddress);
}

void ELFWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_DISASM);
    ui->widgetDisasm->goToOffset(nOffset);
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

void ELFWidget::on_tableView_Elf_Shdr_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSectionHeader();
}

void ELFWidget::on_tableView_Elf_Shdr_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Elf_Shdr->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_Elf_Shdr);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(QString("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(sectionHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(sectionDisasm()));
        actionDisasm.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDisasm);

        QAction actionEntropy(tr("Entropy"), this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(sectionEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        QAction actionDump(tr("Dump to file"), this);
        connect(&actionDump, SIGNAL(triggered()), this, SLOT(sectionDump()));
        actionDump.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDump);

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
        bool bIsEnable = getTableViewItemSize(ui->tableView_Elf_Phdr);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editProgramHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(QString("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(programHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(programDisasm()));
        actionDisasm.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDisasm);

        QAction actionEntropy(tr("Entropy"), this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(programEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        QAction actionDump(tr("Dump to file"), this);
        connect(&actionDump, SIGNAL(triggered()), this, SLOT(programDump()));
        actionDump.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDump);

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
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDynamicArrayTag()));
        contextMenu.addAction(&actionEdit);

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
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelaHeaderTag()));
        contextMenu.addAction(&actionEdit);

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
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelHeaderTag()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_Rel->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_pushButtonHex_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_HEX);
}

void ELFWidget::on_pushButtonDisasm_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_DISASM);
}

void ELFWidget::on_pushButtonStrings_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_STRINGS);
}

void ELFWidget::on_pushButtonMemoryMap_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_MEMORYMAP);
}

void ELFWidget::on_pushButtonEntropy_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_ENTROPY);
}

void ELFWidget::on_pushButtonHeuristicScan_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SELF::TYPE_HEURISTICSCAN);
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
    XShortcutsWidget::saveModel(ui->tableView_Elf_Shdr->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(tr("Sections"))));
}

void ELFWidget::on_pushButtonSavePrograms_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Elf_Phdr->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Programs"))));
}

void ELFWidget::on_pushButtonSave_Rela_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Rela->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Rela"))));
}

void ELFWidget::on_pushButtonSave_Rel_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Rel->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Rel"))));
}

void ELFWidget::on_pushButtonSave_Elf_Ehdr_clicked()
{
    saveHeaderTable(ui->tableWidget_Elf_Ehdr, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Elf_Ehdr"))));
}
