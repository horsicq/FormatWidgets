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
#include "elfsectionheaderwidget.h"

#include "ui_elfsectionheaderwidget.h"

ELFSectionHeaderWidget::ELFSectionHeaderWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::ELFSectionHeaderWidget)
{
    ui->setupUi(this);
}

ELFSectionHeaderWidget::ELFSectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent)
    : ELFSectionHeaderWidget(pParent)
{
    setData(pDevice, options, nNumber, nOffset, nType);

    g_ppLinedEdit = 0;
    g_nLineEditSize = 0;
    g_ppComboBox = 0;
    g_nComboBoxSize = 0;
    g_ppInvWidget = 0;
    g_nInvWidgetSize = 0;

    if (nType == SELF::TYPE_Elf_Shdr) {
        g_nLineEditSize = N_Elf_Shdr::__data_size;
        g_nComboBoxSize = N_Elf_Shdr::__CB_size;
        g_nInvWidgetSize = N_Elf_Shdr::__INV_size;
    } else if (nType == SELF::TYPE_Elf_Phdr) {
        g_nLineEditSize = N_Elf_Phdr32::__data_size;
        g_nComboBoxSize = N_Elf_Phdr32::__CB_size;
        g_nInvWidgetSize = N_Elf_Phdr32::__INV_size;
    } else if (nType == SELF::TYPE_Elf_DynamicArrayTags) {
        g_nLineEditSize = N_Elf_DynamicArrayTags::__data_size;
        g_nComboBoxSize = N_Elf_DynamicArrayTags::__CB_size;
        g_nInvWidgetSize = N_Elf_DynamicArrayTags::__INV_size;
    } else if (nType == SELF::TYPE_SYMBOLTABLE) {
        g_nLineEditSize = N_Elf32_Sym::__data_size;
        g_nComboBoxSize = N_Elf32_Sym::__CB_size;
        g_nInvWidgetSize = N_Elf32_Sym::__INV_size;
    } else if (nType == SELF::TYPE_Elf_Rela) {
        g_nLineEditSize = N_Elf_Rela::__data_size;
        g_nComboBoxSize = N_Elf_Rela::__CB_size;
        g_nInvWidgetSize = N_Elf_Rela::__INV_size;
    } else if (nType == SELF::TYPE_Elf_Rel) {
        g_nLineEditSize = N_Elf_Rel::__data_size;
        g_nComboBoxSize = N_Elf_Rel::__CB_size;
    }

    if (g_nLineEditSize) {
        g_ppLinedEdit = new PXLineEditHEX[g_nLineEditSize];
    }

    if (g_nComboBoxSize) {
        g_ppComboBox = new PXComboBoxEx[g_nComboBoxSize];
    }

    if (g_nInvWidgetSize) {
        g_ppInvWidget = new PInvWidget[g_nInvWidgetSize];
    }
}

ELFSectionHeaderWidget::~ELFSectionHeaderWidget()
{
    if (g_ppLinedEdit) {
        delete[] g_ppLinedEdit;
    }

    if (g_ppComboBox) {
        delete[] g_ppComboBox;
    }

    if (g_ppInvWidget) {
        delete[] g_ppInvWidget;
    }

    delete ui;
}

void ELFSectionHeaderWidget::clear()
{
    reset();

    memset(g_ppLinedEdit, 0, g_nLineEditSize * sizeof(XLineEditHEX *));
    memset(g_ppComboBox, 0, g_nComboBoxSize * sizeof(XComboBoxEx *));
    memset(g_ppInvWidget, 0, g_nInvWidgetSize * sizeof(InvWidget *));

    g_pSubDevice = nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void ELFSectionHeaderWidget::cleanup()
{
    ELFSectionHeaderWidget::clear();
}

void ELFSectionHeaderWidget::reload()
{
    ELFSectionHeaderWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

void ELFSectionHeaderWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetHex->setGlobal(pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void ELFSectionHeaderWidget::setStringTable(qint64 nStringTableOffset, qint64 nStringTableSize)
{
    this->g_nStringTableOffset = nStringTableOffset;
    this->g_nStringTableSize = nStringTableSize;
}

FormatWidget::SV ELFSectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)

    SV result = SV_NONE;

    quint64 nValue = vValue.toULongLong();

    if (getDevice()->isWritable()) {
        XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (elf.isValid()) {
            switch (nStype) {
                case SELF::TYPE_Elf_Shdr:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf_Shdr::sh_name:
                                addComment(ui->tableWidget, N_Elf_Shdr::sh_name, HEADER_COLUMN_COMMENT,
                                           elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, nValue));
                                break;
                            case N_Elf_Shdr::sh_type:
                                g_ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(nValue);
                                break;
                            case N_Elf_Shdr::sh_flags:
                                g_ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(nValue);
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf_Shdr::sh_name:
                                addComment(ui->tableWidget, N_Elf_Shdr::sh_name, HEADER_COLUMN_COMMENT,
                                           elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, nValue));
                                break;
                            case N_Elf_Shdr::sh_type:
                                g_ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(nValue);
                                break;
                            case N_Elf_Shdr::sh_flags:
                                g_ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(nValue);
                                break;
                        }
                    }

                    break;

                case SELF::TYPE_Elf_Phdr:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf_Phdr64::p_type:
                                g_ppComboBox[N_Elf_Phdr64::CB_TYPE]->setValue(nValue);
                                break;
                            case N_Elf_Phdr64::p_flags:
                                g_ppComboBox[N_Elf_Phdr64::CB_FLAGS]->setValue(nValue);
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf_Phdr32::p_type:
                                g_ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(nValue);
                                break;
                            case N_Elf_Phdr32::p_flags:
                                g_ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(nValue);
                                break;
                        }
                    }

                    break;

                case SELF::TYPE_Elf_DynamicArrayTags:
                    switch (nNdata) {
                        case N_Elf_DynamicArrayTags::d_tag:
                            g_ppComboBox[N_Elf_DynamicArrayTags::CB_TAG]->setValue(nValue);
                            break;
                    }

                    break;

                case SELF::TYPE_SYMBOLTABLE:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf64_Sym::st_name:
                                addComment(ui->tableWidget, N_Elf64_Sym::st_name, HEADER_COLUMN_COMMENT,
                                           elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, nValue));
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf32_Sym::st_name:
                                addComment(ui->tableWidget, N_Elf32_Sym::st_name, HEADER_COLUMN_COMMENT,
                                           elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, nValue));
                                break;
                        }
                    }

                    break;
            }

            switch (nStype) {
                case SELF::TYPE_Elf_Shdr:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf_Shdr::sh_name:
                                elf.setElf64_Shdr_name((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_type:
                                elf.setElf64_Shdr_type((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_flags:
                                elf.setElf64_Shdr_flags((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_addr:
                                elf.setElf64_Shdr_addr((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_offset:
                                elf.setElf64_Shdr_offset((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_size:
                                elf.setElf64_Shdr_size((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_link:
                                elf.setElf64_Shdr_link((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_info:
                                elf.setElf64_Shdr_info((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_addralign:
                                elf.setElf64_Shdr_addralign((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_entsize:
                                elf.setElf64_Shdr_entsize((quint64)nPosition, (quint32)nValue);
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf_Shdr::sh_name:
                                elf.setElf32_Shdr_name((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_type:
                                elf.setElf32_Shdr_type((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_flags:
                                elf.setElf32_Shdr_flags((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_addr:
                                elf.setElf32_Shdr_addr((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_offset:
                                elf.setElf32_Shdr_offset((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_size:
                                elf.setElf32_Shdr_size((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_link:
                                elf.setElf32_Shdr_link((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_info:
                                elf.setElf32_Shdr_info((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_addralign:
                                elf.setElf32_Shdr_addralign((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Shdr::sh_entsize:
                                elf.setElf32_Shdr_entsize((quint32)nPosition, (quint32)nValue);
                                break;
                        }
                    }

                    break;

                case SELF::TYPE_Elf_Phdr:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf_Phdr64::p_type:
                                elf.setElf64_Phdr_type((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_flags:
                                elf.setElf64_Phdr_flags((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_offset:
                                elf.setElf64_Phdr_offset((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_vaddr:
                                elf.setElf64_Phdr_vaddr((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_paddr:
                                elf.setElf64_Phdr_paddr((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_filesz:
                                elf.setElf64_Phdr_filesz((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_memsz:
                                elf.setElf64_Phdr_memsz((quint64)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr64::p_align:
                                elf.setElf64_Phdr_align((quint64)nPosition, (quint32)nValue);
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf_Phdr32::p_type:
                                elf.setElf32_Phdr_type((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_offset:
                                elf.setElf32_Phdr_offset((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_vaddr:
                                elf.setElf32_Phdr_vaddr((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_paddr:
                                elf.setElf32_Phdr_paddr((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_filesz:
                                elf.setElf32_Phdr_filesz((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_memsz:
                                elf.setElf32_Phdr_memsz((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_flags:
                                elf.setElf32_Phdr_flags((quint32)nPosition, (quint32)nValue);
                                break;
                            case N_Elf_Phdr32::p_align:
                                elf.setElf32_Phdr_align((quint32)nPosition, (quint32)nValue);
                                break;
                        }
                    }

                    break;

                case SELF::TYPE_Elf_DynamicArrayTags:
                    switch (nNdata) {
                        case N_Elf_DynamicArrayTags::d_tag:
                            elf.setDynamicArrayTag(nOffset, nValue);
                            break;
                        case N_Elf_DynamicArrayTags::d_value:
                            elf.setDynamicArrayValue(nOffset, nValue);
                            break;
                    }

                    break;

                case SELF::TYPE_SYMBOLTABLE:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf64_Sym::st_name:
                                elf.setElf64_Sym_st_name(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf64_Sym::st_info:
                                elf.setElf64_Sym_st_info(nOffset, (quint8)nValue);
                                break;
                            case N_Elf64_Sym::st_other:
                                elf.setElf64_Sym_st_other(nOffset, (quint8)nValue);
                                break;
                            case N_Elf64_Sym::st_shndx:
                                elf.setElf64_Sym_st_shndx(nOffset, (quint16)nValue, elf.isBigEndian());
                                break;
                            case N_Elf64_Sym::st_value:
                                elf.setElf64_Sym_st_value(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                            case N_Elf64_Sym::st_size:
                                elf.setElf64_Sym_st_size(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf32_Sym::st_name:
                                elf.setElf32_Sym_st_name(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf32_Sym::st_value:
                                elf.setElf32_Sym_st_value(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf32_Sym::st_size:
                                elf.setElf32_Sym_st_size(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf32_Sym::st_info:
                                elf.setElf32_Sym_st_info(nOffset, (quint8)nValue);
                                break;
                            case N_Elf32_Sym::st_other:
                                elf.setElf32_Sym_st_other(nOffset, (quint8)nValue);
                                break;
                            case N_Elf32_Sym::st_shndx:
                                elf.setElf32_Sym_st_shndx(nOffset, (quint16)nValue, elf.isBigEndian());
                                break;
                        }
                    }

                    break;

                case SELF::TYPE_Elf_Rela:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf_Rela::r_offset:
                                elf.setElf64_Rela_r_offset(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf_Rela::r_info:
                                elf.setElf64_Rela_r_info(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf_Rela::r_addend:
                                elf.setElf64_Rela_r_addend(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf_Rela::r_offset:
                                elf.setElf32_Rela_r_offset(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                            case N_Elf_Rela::r_info:
                                elf.setElf32_Rela_r_info(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                            case N_Elf_Rela::r_addend:
                                elf.setElf32_Rela_r_addend(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                        }
                    }

                    break;

                case SELF::TYPE_Elf_Rel:
                    if (elf.is64()) {
                        switch (nNdata) {
                            case N_Elf_Rel::r_offset:
                                elf.setElf64_Rel_r_offset(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                            case N_Elf_Rel::r_info:
                                elf.setElf64_Rel_r_info(nOffset, (quint32)nValue, elf.isBigEndian());
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_Elf_Rel::r_offset:
                                elf.setElf32_Rel_r_offset(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                            case N_Elf_Rel::r_info:
                                elf.setElf32_Rel_r_info(nOffset, (quint64)nValue, elf.isBigEndian());
                                break;
                        }
                    }

                    break;
            }

            ui->widgetHex->reload();

            result = SV_EDITED;
        }
    }

    return result;
}
void ELFSectionHeaderWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(g_ppLinedEdit, g_nLineEditSize, bState);
    setComboBoxesReadOnly(g_ppComboBox, g_nComboBoxSize, bState);
}

void ELFSectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_ppLinedEdit, g_nLineEditSize, bState);
    _blockSignals((QObject **)g_ppComboBox, g_nComboBoxSize, bState);
}

void ELFSectionHeaderWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    XBinary::MODE mode = XELF::getMode(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET, getColumnWidth(this, CW_UINT16, mode));
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE, getColumnWidth(this, CW_TYPE, mode));

    switch (nType) {
        case SELF::TYPE_Elf_Shdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SELF::TYPE_Elf_Phdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SELF::TYPE_Elf_DynamicArrayTags:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SELF::TYPE_SYMBOLTABLE:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SELF::TYPE_Elf_Rela:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SELF::TYPE_Elf_Rel:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;
    }
}

void ELFSectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void ELFSectionHeaderWidget::reloadData()
{
    qint32 nType = getType();

    XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (elf.isValid()) {
        bool bIs64 = elf.is64();

        if (nType == SELF::TYPE_Elf_Shdr) {
            createHeaderTable(SELF::TYPE_Elf_Shdr, ui->tableWidget, bIs64 ? (N_Elf_Shdr::records64) : (N_Elf_Shdr::records32), g_ppLinedEdit, N_Elf_Shdr::__data_size,
                              getNumber());
            g_ppComboBox[N_Elf_Shdr::CB_TYPE] = createComboBox(ui->tableWidget, XELF::getSectionTypesS(), SELF::TYPE_Elf_Shdr, N_Elf_Shdr::sh_type, XComboBoxEx::CBTYPE_LIST);
            g_ppComboBox[N_Elf_Shdr::CB_FLAGS] =
                createComboBox(ui->tableWidget, XELF::getSectionFlagsS(), SELF::TYPE_Elf_Shdr, N_Elf_Shdr::sh_flags, XComboBoxEx::CBTYPE_FLAGS);

            blockSignals(true);

            if (bIs64) {
                XELF_DEF::Elf64_Shdr shdr64 = elf.getElf64_Shdr(getNumber());

                g_ppLinedEdit[N_Elf_Shdr::sh_name]->setValue(shdr64.sh_name);
                g_ppLinedEdit[N_Elf_Shdr::sh_type]->setValue(shdr64.sh_type);
                g_ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue(shdr64.sh_flags);
                g_ppLinedEdit[N_Elf_Shdr::sh_addr]->setValue(shdr64.sh_addr);
                g_ppLinedEdit[N_Elf_Shdr::sh_offset]->setValue(shdr64.sh_offset);
                g_ppLinedEdit[N_Elf_Shdr::sh_size]->setValue(shdr64.sh_size);
                g_ppLinedEdit[N_Elf_Shdr::sh_link]->setValue(shdr64.sh_link);
                g_ppLinedEdit[N_Elf_Shdr::sh_info]->setValue(shdr64.sh_info);
                g_ppLinedEdit[N_Elf_Shdr::sh_addralign]->setValue(shdr64.sh_addralign);
                g_ppLinedEdit[N_Elf_Shdr::sh_entsize]->setValue(shdr64.sh_entsize);

                g_ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(shdr64.sh_type);
                g_ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(shdr64.sh_flags);

                addComment(ui->tableWidget, N_Elf_Shdr::sh_name, HEADER_COLUMN_COMMENT, elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, shdr64.sh_name));
            } else {
                XELF_DEF::Elf32_Shdr shdr32 = elf.getElf32_Shdr(getNumber());

                g_ppLinedEdit[N_Elf_Shdr::sh_name]->setValue(shdr32.sh_name);
                g_ppLinedEdit[N_Elf_Shdr::sh_type]->setValue(shdr32.sh_type);
                g_ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue(shdr32.sh_flags);
                g_ppLinedEdit[N_Elf_Shdr::sh_addr]->setValue(shdr32.sh_addr);
                g_ppLinedEdit[N_Elf_Shdr::sh_offset]->setValue(shdr32.sh_offset);
                g_ppLinedEdit[N_Elf_Shdr::sh_size]->setValue(shdr32.sh_size);
                g_ppLinedEdit[N_Elf_Shdr::sh_link]->setValue(shdr32.sh_link);
                g_ppLinedEdit[N_Elf_Shdr::sh_info]->setValue(shdr32.sh_info);
                g_ppLinedEdit[N_Elf_Shdr::sh_addralign]->setValue(shdr32.sh_addralign);
                g_ppLinedEdit[N_Elf_Shdr::sh_entsize]->setValue(shdr32.sh_entsize);

                g_ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(shdr32.sh_type);
                g_ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(shdr32.sh_flags);

                addComment(ui->tableWidget, N_Elf_Shdr::sh_name, HEADER_COLUMN_COMMENT, elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, shdr32.sh_name));
            }

            qint64 nOffset = elf.getShdrOffset(getNumber());
            qint64 nSize = elf.getShdrSize();
            qint64 nAddress = elf.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SELF::TYPE_Elf_Phdr) {
            if (bIs64) {
                createHeaderTable(SELF::TYPE_Elf_Phdr, ui->tableWidget, N_Elf_Phdr64::records, g_ppLinedEdit, N_Elf_Phdr64::__data_size, getNumber());
                g_ppComboBox[N_Elf_Phdr64::CB_TYPE] =
                    createComboBox(ui->tableWidget, XELF::getProgramTypesS(), SELF::TYPE_Elf_Phdr, N_Elf_Phdr64::p_type, XComboBoxEx::CBTYPE_LIST);
                g_ppComboBox[N_Elf_Phdr64::CB_FLAGS] =
                    createComboBox(ui->tableWidget, XELF::getProgramFlagsS(), SELF::TYPE_Elf_Phdr, N_Elf_Phdr64::p_flags, XComboBoxEx::CBTYPE_FLAGS);
            } else {
                createHeaderTable(SELF::TYPE_Elf_Phdr, ui->tableWidget, N_Elf_Phdr32::records, g_ppLinedEdit, N_Elf_Phdr32::__data_size, getNumber());
                g_ppComboBox[N_Elf_Phdr32::CB_TYPE] =
                    createComboBox(ui->tableWidget, XELF::getProgramTypesS(), SELF::TYPE_Elf_Phdr, N_Elf_Phdr32::p_type, XComboBoxEx::CBTYPE_LIST);
                g_ppComboBox[N_Elf_Phdr32::CB_FLAGS] =
                    createComboBox(ui->tableWidget, XELF::getProgramFlagsS(), SELF::TYPE_Elf_Phdr, N_Elf_Phdr32::p_flags, XComboBoxEx::CBTYPE_FLAGS);
            }

            blockSignals(true);

            if (bIs64) {
                XELF_DEF::Elf64_Phdr phdr64 = elf.getElf64_Phdr(getNumber());

                g_ppLinedEdit[N_Elf_Phdr64::p_type]->setValue(phdr64.p_type);
                g_ppLinedEdit[N_Elf_Phdr64::p_flags]->setValue(phdr64.p_flags);
                g_ppLinedEdit[N_Elf_Phdr64::p_offset]->setValue(phdr64.p_offset);
                g_ppLinedEdit[N_Elf_Phdr64::p_vaddr]->setValue(phdr64.p_vaddr);
                g_ppLinedEdit[N_Elf_Phdr64::p_paddr]->setValue(phdr64.p_paddr);
                g_ppLinedEdit[N_Elf_Phdr64::p_filesz]->setValue(phdr64.p_filesz);
                g_ppLinedEdit[N_Elf_Phdr64::p_memsz]->setValue(phdr64.p_memsz);
                g_ppLinedEdit[N_Elf_Phdr64::p_align]->setValue(phdr64.p_align);

                g_ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(phdr64.p_type);
                g_ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(phdr64.p_flags);
            } else {
                XELF_DEF::Elf32_Phdr phdr32 = elf.getElf32_Phdr(getNumber());

                g_ppLinedEdit[N_Elf_Phdr32::p_type]->setValue(phdr32.p_type);
                g_ppLinedEdit[N_Elf_Phdr32::p_offset]->setValue(phdr32.p_offset);
                g_ppLinedEdit[N_Elf_Phdr32::p_vaddr]->setValue(phdr32.p_vaddr);
                g_ppLinedEdit[N_Elf_Phdr32::p_paddr]->setValue(phdr32.p_paddr);
                g_ppLinedEdit[N_Elf_Phdr32::p_filesz]->setValue(phdr32.p_filesz);
                g_ppLinedEdit[N_Elf_Phdr32::p_memsz]->setValue(phdr32.p_memsz);
                g_ppLinedEdit[N_Elf_Phdr32::p_flags]->setValue(phdr32.p_flags);
                g_ppLinedEdit[N_Elf_Phdr32::p_align]->setValue(phdr32.p_align);

                g_ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(phdr32.p_type);
                g_ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(phdr32.p_flags);
            }

            qint64 nOffset = elf.getPhdrOffset(getNumber());
            qint64 nSize = elf.getPhdrSize();
            qint64 nAddress = elf.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SELF::TYPE_Elf_DynamicArrayTags) {
            createHeaderTable(SELF::TYPE_Elf_DynamicArrayTags, ui->tableWidget, bIs64 ? (N_Elf_DynamicArrayTags::records64) : (N_Elf_DynamicArrayTags::records32),
                              g_ppLinedEdit, N_Elf_DynamicArrayTags::__data_size, getNumber(), getOffset());
            g_ppComboBox[N_Elf_DynamicArrayTags::CB_TAG] =
                createComboBox(ui->tableWidget, XELF::getDynamicTagsS(), SELF::TYPE_Elf_DynamicArrayTags, N_Elf_DynamicArrayTags::d_tag, XComboBoxEx::CBTYPE_LIST);

            blockSignals(true);

            qint64 nOffset = getOffset();

            qint64 nTag = elf.getDynamicArrayTag(nOffset);
            qint64 nValue = elf.getDynamicArrayValue(nOffset);

            g_ppLinedEdit[N_Elf_DynamicArrayTags::d_tag]->setValue(bIs64 ? ((qint64)nTag) : ((qint32)nTag));
            g_ppLinedEdit[N_Elf_DynamicArrayTags::d_value]->setValue(bIs64 ? ((qint64)nValue) : ((qint32)nValue));

            g_ppComboBox[N_Elf_DynamicArrayTags::CB_TAG]->setValue(nTag);

            qint64 nSize = elf.getDynamicArraySize();
            qint64 nAddress = elf.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SELF::TYPE_SYMBOLTABLE) {
            createHeaderTable(SELF::TYPE_SYMBOLTABLE, ui->tableWidget, bIs64 ? (N_Elf64_Sym::records) : (N_Elf32_Sym::records), g_ppLinedEdit, N_Elf32_Sym::__data_size,
                              getNumber(), getOffset());

            blockSignals(true);

            qint64 nOffset = getOffset();

            bool bIsBigEndian = elf.isBigEndian();

            if (bIs64) {
                XELF_DEF::Elf64_Sym sym64 = elf._readElf64_Sym(nOffset, bIsBigEndian);

                g_ppLinedEdit[N_Elf64_Sym::st_name]->setValue(sym64.st_name);
                g_ppLinedEdit[N_Elf64_Sym::st_info]->setValue(sym64.st_info);
                g_ppLinedEdit[N_Elf64_Sym::st_other]->setValue(sym64.st_other);
                g_ppLinedEdit[N_Elf64_Sym::st_shndx]->setValue(sym64.st_shndx);
                g_ppLinedEdit[N_Elf64_Sym::st_value]->setValue(sym64.st_value);
                g_ppLinedEdit[N_Elf64_Sym::st_size]->setValue(sym64.st_size);

                addComment(ui->tableWidget, N_Elf64_Sym::st_name, HEADER_COLUMN_COMMENT, elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, sym64.st_name));
            } else {
                XELF_DEF::Elf32_Sym sym32 = elf._readElf32_Sym(nOffset, bIsBigEndian);

                g_ppLinedEdit[N_Elf32_Sym::st_name]->setValue(sym32.st_name);
                g_ppLinedEdit[N_Elf32_Sym::st_value]->setValue(sym32.st_value);
                g_ppLinedEdit[N_Elf32_Sym::st_size]->setValue(sym32.st_size);
                g_ppLinedEdit[N_Elf32_Sym::st_info]->setValue(sym32.st_info);
                g_ppLinedEdit[N_Elf32_Sym::st_other]->setValue(sym32.st_other);
                g_ppLinedEdit[N_Elf32_Sym::st_shndx]->setValue(sym32.st_shndx);

                addComment(ui->tableWidget, N_Elf32_Sym::st_name, HEADER_COLUMN_COMMENT, elf.getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, sym32.st_name));
            }

            qint64 nSize = elf.getSymSize();
            qint64 nAddress = elf.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SELF::TYPE_Elf_Rela) {
            createHeaderTable(SELF::TYPE_Elf_Rela, ui->tableWidget, bIs64 ? (N_Elf_Rela::records64) : (N_Elf_Rela::records32), g_ppLinedEdit, N_Elf_Rela::__data_size,
                              getNumber(), getOffset());

            blockSignals(true);

            qint64 nOffset = getOffset();

            bool bIsBigEndian = elf.isBigEndian();

            if (bIs64) {
                XELF_DEF::Elf64_Rela rela64 = elf._readElf64_Rela(nOffset, bIsBigEndian);

                g_ppLinedEdit[N_Elf_Rela::r_offset]->setValue(rela64.r_offset);
                g_ppLinedEdit[N_Elf_Rela::r_info]->setValue(rela64.r_info);
                g_ppLinedEdit[N_Elf_Rela::r_addend]->setValue(rela64.r_addend);
            } else {
                XELF_DEF::Elf32_Rela rela32 = elf._readElf32_Rela(nOffset, bIsBigEndian);

                g_ppLinedEdit[N_Elf_Rela::r_offset]->setValue(rela32.r_offset);
                g_ppLinedEdit[N_Elf_Rela::r_info]->setValue(rela32.r_info);
                g_ppLinedEdit[N_Elf_Rela::r_addend]->setValue(rela32.r_addend);
            }

            qint64 nSize = elf.getSymSize();
            qint64 nAddress = elf.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SELF::TYPE_Elf_Rel) {
            createHeaderTable(SELF::TYPE_Elf_Rel, ui->tableWidget, bIs64 ? (N_Elf_Rel::records64) : (N_Elf_Rel::records32), g_ppLinedEdit, N_Elf_Rel::__data_size, getNumber(),
                              getOffset());

            blockSignals(true);

            qint64 nOffset = getOffset();

            bool bIsBigEndian = elf.isBigEndian();

            if (bIs64) {
                XELF_DEF::Elf64_Rel rel64 = elf._readElf64_Rel(nOffset, bIsBigEndian);

                g_ppLinedEdit[N_Elf_Rel::r_offset]->setValue(rel64.r_offset);
                g_ppLinedEdit[N_Elf_Rel::r_info]->setValue(rel64.r_info);
            } else {
                XELF_DEF::Elf32_Rel rel32 = elf._readElf32_Rel(nOffset, bIsBigEndian);

                g_ppLinedEdit[N_Elf_Rel::r_offset]->setValue(rel32.r_offset);
                g_ppLinedEdit[N_Elf_Rel::r_info]->setValue(rel32.r_info);
            }

            qint64 nSize = elf.getSymSize();
            qint64 nAddress = elf.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void ELFSectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    XELF elf(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (elf.isValid()) {
        bool bIs64 = elf.is64();

        switch (nStype) {
            case SELF::TYPE_Elf_Shdr:
                if (bIs64) {
                    switch (nNdata) {
                        case N_Elf_Shdr::sh_type:
                            g_ppLinedEdit[N_Elf_Shdr::sh_type]->setValue((quint32)nValue);
                            break;
                        case N_Elf_Shdr::sh_flags:
                            g_ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue((quint64)nValue);
                            break;
                    }
                } else {
                    switch (nNdata) {
                        case N_Elf_Shdr::sh_type:
                            g_ppLinedEdit[N_Elf_Shdr::sh_type]->setValue((quint32)nValue);
                            break;
                        case N_Elf_Shdr::sh_flags:
                            g_ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue((quint32)nValue);
                            break;
                    }
                }
                break;

            case SELF::TYPE_Elf_Phdr:
                if (bIs64) {
                    switch (nNdata) {
                        case N_Elf_Phdr64::p_type:
                            g_ppLinedEdit[N_Elf_Phdr64::p_type]->setValue((quint32)nValue);
                            break;
                        case N_Elf_Phdr64::p_flags:
                            g_ppLinedEdit[N_Elf_Phdr64::p_flags]->setValue((quint32)nValue);
                            break;
                    }
                } else {
                    switch (nNdata) {
                        case N_Elf_Phdr32::p_type:
                            g_ppLinedEdit[N_Elf_Phdr32::p_type]->setValue((quint32)nValue);
                            break;
                        case N_Elf_Phdr32::p_flags:
                            g_ppLinedEdit[N_Elf_Phdr32::p_flags]->setValue((quint32)nValue);
                            break;
                    }
                }

                break;

            case SELF::TYPE_Elf_DynamicArrayTags:

                switch (nNdata) {
                    case N_Elf_DynamicArrayTags::d_tag:
                        g_ppLinedEdit[N_Elf_DynamicArrayTags::d_tag]->setValue(bIs64 ? ((qint64)nValue) : ((qint32)nValue));
                        break;
                }

                break;
        }
    }
}

void ELFSectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex, ui->tableWidget);
}

void ELFSectionHeaderWidget::on_pushButtonSaveHeader_clicked()
{
    saveHeaderTable(ui->tableWidget, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(tr("Header"))));
}
