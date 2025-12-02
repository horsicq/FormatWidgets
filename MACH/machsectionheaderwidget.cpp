/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#include "machsectionheaderwidget.h"

#include "ui_machsectionheaderwidget.h"

MACHSectionHeaderWidget::MACHSectionHeaderWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::MACHSectionHeaderWidget)
{
    ui->setupUi(this);
}

MACHSectionHeaderWidget::MACHSectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent)
    : MACHSectionHeaderWidget(pParent)
{
    MACHSectionHeaderWidget::setData(pDevice, options, nNumber, nOffset, nType);

    XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    bool bIs64 = mach.is64();

    m_ppLinedEdit = 0;
    m_nLineEditSize = 0;
    m_ppComboBox = 0;
    m_nComboBoxSize = 0;
    m_ppInvWidget = 0;
    m_nInvWidgetSize = 0;

    if (nType == SMACH::TYPE_mach_commands) {
        m_nLineEditSize = N_mach_commands::__data_size;
        m_nComboBoxSize = N_mach_commands::__CB_size;
        m_nInvWidgetSize = N_mach_commands::__INV_size;
    }
    if ((nType == SMACH::TYPE_mach_libraries) || (nType == SMACH::TYPE_mach_weak_libraries) || (nType == SMACH::TYPE_mach_id_library)) {
        m_nLineEditSize = N_mach_library::__data_size;
        m_nComboBoxSize = N_mach_library::__CB_size;
        m_nInvWidgetSize = N_mach_library::__INV_size;
    }
    if ((nType == SMACH::TYPE_mach_LOADFVMLIB) || (nType == SMACH::TYPE_mach_IDFVMLIB)) {
        m_nLineEditSize = N_mach_fmv_library::__data_size;
        m_nComboBoxSize = N_mach_fmv_library::__CB_size;
        m_nInvWidgetSize = N_mach_fmv_library::__INV_size;
    } else if (nType == SMACH::TYPE_mach_segments) {
        m_nLineEditSize = N_mach_segments::__data_size;
        m_nComboBoxSize = N_mach_segments::__CB_size;
        m_nInvWidgetSize = N_mach_segments::__INV_size;
    } else if (nType == SMACH::TYPE_mach_sections) {
        if (bIs64) {
            m_nLineEditSize = N_mach_sections64_E::__data_size;
            m_nComboBoxSize = N_mach_sections64_E::__CB_size;
            m_nInvWidgetSize = N_mach_sections64_E::__INV_size;
        } else {
            m_nLineEditSize = N_mach_sections32_E::__data_size;
            m_nComboBoxSize = N_mach_sections32_E::__CB_size;
            m_nInvWidgetSize = N_mach_sections32_E::__INV_size;
        }
    } else if (nType == SMACH::TYPE_SYMBOLTABLE) {
        m_nLineEditSize = N_mach_nlist::__data_size;
        m_nComboBoxSize = N_mach_nlist::__CB_size;
        m_nInvWidgetSize = N_mach_nlist::__INV_size;
    } else if (nType == SMACH::TYPE_DICE) {
        m_nLineEditSize = N_mach_data_in_code_entry::__data_size;
        m_nComboBoxSize = N_mach_data_in_code_entry::__CB_size;
        m_nInvWidgetSize = N_mach_data_in_code_entry::__INV_size;
    } else if (nType == SMACH::TYPE_DYSYMTAB_modtab) {
        if (bIs64) {
            m_nLineEditSize = N_mach_modtab64::__data_size;
            m_nComboBoxSize = N_mach_modtab64::__CB_size;
            m_nInvWidgetSize = N_mach_modtab64::__INV_size;
        } else {
            m_nLineEditSize = N_mach_modtab32::__data_size;
            m_nComboBoxSize = N_mach_modtab32::__CB_size;
            m_nInvWidgetSize = N_mach_modtab32::__INV_size;
        }
    } else if (nType == SMACH::TYPE_DYSYMTAB_toc) {
        m_nLineEditSize = N_mach_table_of_contents::__data_size;
        m_nComboBoxSize = N_mach_table_of_contents::__CB_size;
        m_nInvWidgetSize = N_mach_table_of_contents::__INV_size;
    } else if (nType == SMACH::TYPE_DYSYMTAB_extrel) {
        m_nLineEditSize = N_mach_relocs::__data_size;
        m_nComboBoxSize = N_mach_relocs::__CB_size;
        m_nInvWidgetSize = N_mach_relocs::__INV_size;
    } else if (nType == SMACH::TYPE_DYSYMTAB_locrel) {
        m_nLineEditSize = N_mach_relocs::__data_size;
        m_nComboBoxSize = N_mach_relocs::__CB_size;
        m_nInvWidgetSize = N_mach_relocs::__INV_size;
    } else if (nType == SMACH::TYPE_DYSYMTAB_indirectsyms) {
        m_nLineEditSize = N_mach_value::__data_size;
        m_nComboBoxSize = N_mach_value::__CB_size;
        m_nInvWidgetSize = N_mach_value::__INV_size;
    } else if (nType == SMACH::TYPE_DYSYMTAB_extrefsyms) {
        m_nLineEditSize = N_mach_value::__data_size;
        m_nComboBoxSize = N_mach_value::__CB_size;
        m_nInvWidgetSize = N_mach_value::__INV_size;
    }

    if (m_nLineEditSize) {
        m_ppLinedEdit = new PXLineEditHEX[m_nLineEditSize];
    }

    if (m_nComboBoxSize) {
        m_ppComboBox = new PXComboBoxEx[m_nComboBoxSize];
    }

    if (m_nInvWidgetSize) {
        m_ppInvWidget = new PInvWidget[m_nInvWidgetSize];
    }
}

MACHSectionHeaderWidget::~MACHSectionHeaderWidget()
{
    if (m_ppLinedEdit) {
        delete[] m_ppLinedEdit;
    }

    if (m_ppComboBox) {
        delete[] m_ppComboBox;
    }

    if (m_ppInvWidget) {
        delete[] m_ppInvWidget;
    }

    delete ui;
}

void MACHSectionHeaderWidget::clear()
{
    reset();

    memset(m_ppLinedEdit, 0, m_nLineEditSize * sizeof(XLineEditHEX *));
    memset(m_ppComboBox, 0, m_nComboBoxSize * sizeof(XComboBoxEx *));
    memset(m_ppInvWidget, 0, m_nInvWidgetSize * sizeof(InvWidget *));

    m_pSubDevice = nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void MACHSectionHeaderWidget::cleanup()
{
    MACHSectionHeaderWidget::clear();
}

void MACHSectionHeaderWidget::reload()
{
    MACHSectionHeaderWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData(false);
}

void MACHSectionHeaderWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetHex->setGlobal(pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

FormatWidget::SV MACHSectionHeaderWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    SV result = SV_NONE;

    quint64 nValue = vValue.toULongLong();
    QString sValue = vValue.toString();

    if (getDevice()->isWritable()) {
        XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (mach.isValid()) {
            switch (nStype) {
                case SMACH::TYPE_mach_commands:
                    switch (nNdata) {
                        case N_mach_commands::cmd: m_ppComboBox[N_mach_commands::CB_CMD]->setValue(nValue); break;
                    }

                    break;

                case SMACH::TYPE_mach_segments:
                    switch (nNdata) {
                        case N_mach_segments::initprot: m_ppComboBox[N_mach_segments::CB_initprot]->setValue(nValue); break;
                        case N_mach_segments::maxprot: m_ppComboBox[N_mach_segments::CB_maxprot]->setValue(nValue); break;
                    }

                    break;

                case SMACH::TYPE_DICE:
                    switch (nNdata) {
                        case N_mach_data_in_code_entry::kind: m_ppComboBox[N_mach_data_in_code_entry::CB_kind]->setValue(nValue); break;
                    }

                    break;

                case SMACH::TYPE_SYMBOLTABLE:
                    switch (nNdata) {
                        case N_mach_nlist::n_strx:
                            XBinary::OFFSETSIZE osStringTable = mach.getStringTableOffsetSize();
                            addComment(ui->tableWidget, N_mach_nlist::n_strx, HEADER_COLUMN_COMMENT,
                                       mach.getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, nValue));
                            break;
                    }

                    break;

                case SMACH::TYPE_DYSYMTAB_indirectsyms:
                    switch (nNdata) {
                        case N_mach_value::value: addComment(ui->tableWidget, N_mach_value::value, HEADER_COLUMN_COMMENT, mach.getIndexSymbolName(nValue)); break;
                    }

                    break;

                case SMACH::TYPE_DYSYMTAB_toc:
                    switch (nNdata) {
                        case N_mach_table_of_contents::symbol_index:
                            addComment(ui->tableWidget, N_mach_table_of_contents::symbol_index, HEADER_COLUMN_COMMENT, mach.getIndexSymbolName(nValue));
                            break;
                    }

                    break;
            }

            switch (nStype) {
                case SMACH::TYPE_mach_commands:
                    switch (nNdata) {
                        case N_mach_commands::cmd: mach._setCommand_cmd(nOffset, nValue); break;
                        case N_mach_commands::cmdsize: mach._setCommand_cmdsize(nOffset, nValue); break;
                    }

                    break;

                case SMACH::TYPE_mach_libraries:
                case SMACH::TYPE_mach_weak_libraries:
                case SMACH::TYPE_mach_id_library:
                    switch (nNdata) {
                        case N_mach_library::timestamp: mach._setLibraryRecord_timestamp(nOffset, nValue); break;
                        case N_mach_library::current_version: mach._setLibraryRecord_current_version(nOffset, nValue); break;
                        case N_mach_library::compatibility_version: mach._setLibraryRecord_compatibility_version(nOffset, nValue); break;
                        case N_mach_library::name: mach._setLibraryRecord_name(nOffset, sValue); break;
                    }

                    break;

                case SMACH::TYPE_mach_LOADFVMLIB:
                case SMACH::TYPE_mach_IDFVMLIB:
                    switch (nNdata) {
                        case N_mach_fmv_library::minor_version: mach._setFvmLibraryRecord_minor_version(nOffset, nValue); break;
                        case N_mach_fmv_library::header_addr: mach._setFvmLibraryRecord_header_addr(nOffset, nValue); break;
                        case N_mach_fmv_library::name: mach._setLibraryRecord_name(nOffset, sValue); break;
                    }

                    break;

                case SMACH::TYPE_mach_segments:
                    if (mach.is64()) {
                        switch (nNdata) {
                            case N_mach_segments::segname: mach._setSegment64_segname(nOffset, sValue); break;
                            case N_mach_segments::vmaddr: mach._setSegment64_vmaddr(nOffset, nValue); break;
                            case N_mach_segments::vmsize: mach._setSegment64_vmsize(nOffset, nValue); break;
                            case N_mach_segments::fileoff: mach._setSegment64_fileoff(nOffset, nValue); break;
                            case N_mach_segments::filesize: mach._setSegment64_filesize(nOffset, nValue); break;
                            case N_mach_segments::maxprot: mach._setSegment64_maxprot(nOffset, nValue); break;
                            case N_mach_segments::initprot: mach._setSegment64_initprot(nOffset, nValue); break;
                            case N_mach_segments::nsects: mach._setSegment64_nsects(nOffset, nValue); break;
                            case N_mach_segments::flags: mach._setSegment64_flags(nOffset, nValue); break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_mach_segments::segname: mach._setSegment32_segname(nOffset, sValue); break;
                            case N_mach_segments::vmaddr: mach._setSegment32_vmaddr(nOffset, nValue); break;
                            case N_mach_segments::vmsize: mach._setSegment32_vmsize(nOffset, nValue); break;
                            case N_mach_segments::fileoff: mach._setSegment32_fileoff(nOffset, nValue); break;
                            case N_mach_segments::filesize: mach._setSegment32_filesize(nOffset, nValue); break;
                            case N_mach_segments::maxprot: mach._setSegment32_maxprot(nOffset, nValue); break;
                            case N_mach_segments::initprot: mach._setSegment32_initprot(nOffset, nValue); break;
                            case N_mach_segments::nsects: mach._setSegment32_nsects(nOffset, nValue); break;
                            case N_mach_segments::flags: mach._setSegment32_flags(nOffset, nValue); break;
                        }
                    }

                    break;

                case SMACH::TYPE_mach_sections:
                    if (mach.is64()) {
                        switch (nNdata) {
                            case N_mach_sections64_E::sectname: mach._setSection64_sectname(nOffset, sValue); break;
                            case N_mach_sections64_E::segname: mach._setSection64_segname(nOffset, sValue); break;
                            case N_mach_sections64_E::addr: mach._setSection64_addr(nOffset, nValue); break;
                            case N_mach_sections64_E::size: mach._setSection64_size(nOffset, nValue); break;
                            case N_mach_sections64_E::offset: mach._setSection64_offset(nOffset, nValue); break;
                            case N_mach_sections64_E::align: mach._setSection64_align(nOffset, nValue); break;
                            case N_mach_sections64_E::reloff: mach._setSection64_reloff(nOffset, nValue); break;
                            case N_mach_sections64_E::nreloc: mach._setSection64_nreloc(nOffset, nValue); break;
                            case N_mach_sections64_E::flags_0: mach._setSection64_flags(nOffset, nValue); break;
                            case N_mach_sections64_E::reserved1: mach._setSection64_reserved1(nOffset, nValue); break;
                            case N_mach_sections64_E::reserved2: mach._setSection64_reserved2(nOffset, nValue); break;
                            case N_mach_sections64_E::reserved3: mach._setSection64_reserved3(nOffset, nValue); break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_mach_sections32_E::sectname: mach._setSection32_sectname(nOffset, sValue); break;
                            case N_mach_sections32_E::segname: mach._setSection32_segname(nOffset, sValue); break;
                            case N_mach_sections32_E::addr: mach._setSection32_addr(nOffset, nValue); break;
                            case N_mach_sections32_E::size: mach._setSection32_size(nOffset, nValue); break;
                            case N_mach_sections32_E::offset: mach._setSection32_offset(nOffset, nValue); break;
                            case N_mach_sections32_E::align: mach._setSection32_align(nOffset, nValue); break;
                            case N_mach_sections32_E::reloff: mach._setSection32_reloff(nOffset, nValue); break;
                            case N_mach_sections32_E::nreloc: mach._setSection32_nreloc(nOffset, nValue); break;
                            case N_mach_sections32_E::flags_0: mach._setSection32_flags(nOffset, nValue); break;
                            case N_mach_sections32_E::reserved1: mach._setSection32_reserved1(nOffset, nValue); break;
                            case N_mach_sections32_E::reserved2: mach._setSection32_reserved2(nOffset, nValue); break;
                        }
                    }

                    break;

                case SMACH::TYPE_SYMBOLTABLE:
                    if (mach.is64()) {
                        switch (nNdata) {
                            case N_mach_nlist::n_strx: mach._set_nlist_64_n_strx(nOffset, nValue); break;
                            case N_mach_nlist::n_type: mach._set_nlist_64_n_type(nOffset, nValue); break;
                            case N_mach_nlist::n_sect: mach._set_nlist_64_n_sect(nOffset, nValue); break;
                            case N_mach_nlist::n_desc: mach._set_nlist_64_n_desc(nOffset, nValue); break;
                            case N_mach_nlist::n_value: mach._set_nlist_64_n_value(nOffset, nValue); break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_mach_nlist::n_strx: mach._set_nlist_n_strx(nOffset, nValue); break;
                            case N_mach_nlist::n_type: mach._set_nlist_n_type(nOffset, nValue); break;
                            case N_mach_nlist::n_sect: mach._set_nlist_n_sect(nOffset, nValue); break;
                            case N_mach_nlist::n_desc: mach._set_nlist_n_desc(nOffset, nValue); break;
                            case N_mach_nlist::n_value: mach._set_nlist_n_value(nOffset, nValue); break;
                        }
                    }

                    break;

                case SMACH::TYPE_DICE:
                    switch (nNdata) {
                        case N_mach_data_in_code_entry::offset: mach._set_data_in_code_entry_offset(nOffset, nValue); break;
                        case N_mach_data_in_code_entry::length: mach._set_data_in_code_entry_length(nOffset, nValue); break;
                        case N_mach_data_in_code_entry::kind: mach._set_data_in_code_entry_kind(nOffset, nValue); break;
                    }

                    break;

                case SMACH::TYPE_DYSYMTAB_modtab:
                    if (mach.is64()) {
                        switch (nNdata) {
                            case N_mach_modtab64::module_name: mach._set_dylib_module_64_module_name(nOffset, nValue); break;
                            case N_mach_modtab64::iextdefsym: mach._set_dylib_module_64_iextdefsym(nOffset, nValue); break;
                            case N_mach_modtab64::nextdefsym: mach._set_dylib_module_64_nextdefsym(nOffset, nValue); break;
                            case N_mach_modtab64::irefsym: mach._set_dylib_module_64_irefsym(nOffset, nValue); break;
                            case N_mach_modtab64::nrefsym: mach._set_dylib_module_64_nrefsym(nOffset, nValue); break;
                            case N_mach_modtab64::ilocalsym: mach._set_dylib_module_64_ilocalsym(nOffset, nValue); break;
                            case N_mach_modtab64::nlocalsym: mach._set_dylib_module_64_nlocalsym(nOffset, nValue); break;
                            case N_mach_modtab64::iextrel: mach._set_dylib_module_64_iextrel(nOffset, nValue); break;
                            case N_mach_modtab64::nextrel: mach._set_dylib_module_64_nextrel(nOffset, nValue); break;
                            case N_mach_modtab64::iinit_iterm: mach._set_dylib_module_64_iinit_iterm(nOffset, nValue); break;
                            case N_mach_modtab64::ninit_nterm: mach._set_dylib_module_64_ninit_nterm(nOffset, nValue); break;
                            case N_mach_modtab64::objc_module_info_size: mach._set_dylib_module_64_objc_module_info_size(nOffset, nValue); break;
                            case N_mach_modtab64::objc_module_info_addr: mach._set_dylib_module_64_objc_module_info_addr(nOffset, nValue); break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_mach_modtab32::module_name: mach._set_dylib_module_module_name(nOffset, nValue); break;
                            case N_mach_modtab32::iextdefsym: mach._set_dylib_module_iextdefsym(nOffset, nValue); break;
                            case N_mach_modtab32::nextdefsym: mach._set_dylib_module_nextdefsym(nOffset, nValue); break;
                            case N_mach_modtab32::irefsym: mach._set_dylib_module_irefsym(nOffset, nValue); break;
                            case N_mach_modtab32::nrefsym: mach._set_dylib_module_nrefsym(nOffset, nValue); break;
                            case N_mach_modtab32::ilocalsym: mach._set_dylib_module_ilocalsym(nOffset, nValue); break;
                            case N_mach_modtab32::nlocalsym: mach._set_dylib_module_nlocalsym(nOffset, nValue); break;
                            case N_mach_modtab32::iextrel: mach._set_dylib_module_iextrel(nOffset, nValue); break;
                            case N_mach_modtab32::nextrel: mach._set_dylib_module_nextrel(nOffset, nValue); break;
                            case N_mach_modtab32::iinit_iterm: mach._set_dylib_module_iinit_iterm(nOffset, nValue); break;
                            case N_mach_modtab32::ninit_nterm: mach._set_dylib_module_ninit_nterm(nOffset, nValue); break;
                            case N_mach_modtab32::objc_module_info_addr: mach._set_dylib_module_objc_module_info_addr(nOffset, nValue); break;
                            case N_mach_modtab32::objc_module_info_size: mach._set_dylib_module_objc_module_info_size(nOffset, nValue); break;
                        }
                    }

                    break;

                case SMACH::TYPE_DYSYMTAB_toc:
                    switch (nNdata) {
                        case N_mach_table_of_contents::symbol_index: mach._set_dylib_table_of_contents_symbol_index(nOffset, nValue); break;
                        case N_mach_table_of_contents::module_index: mach._set_dylib_table_of_contents_module_index(nOffset, nValue); break;
                    }

                    break;

                case SMACH::TYPE_DYSYMTAB_extrel:
                case SMACH::TYPE_DYSYMTAB_locrel:
                    switch (nNdata) {
                        case N_mach_relocs::r_address: mach._set_relocation_info_r_address(nOffset, nValue); break;
                        case N_mach_relocs::value: mach._set_relocation_info_value(nOffset, nValue); break;
                    }

                    break;

                case SMACH::TYPE_DYSYMTAB_indirectsyms:
                case SMACH::TYPE_DYSYMTAB_extrefsyms:
                    switch (nNdata) {
                        case N_mach_value::value: mach.write_uint32(nOffset, nValue, mach.isBigEndian()); break;
                    }

                    break;
            }

            ui->widgetHex->reload();

            result = SV_EDITED;
        }
    }

    return result;
}

void MACHSectionHeaderWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(m_ppLinedEdit, m_nLineEditSize, bState);
    setComboBoxesReadOnly(m_ppComboBox, m_nComboBoxSize, bState);
}

void MACHSectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)m_ppLinedEdit, m_nLineEditSize, bState);
    _blockSignals((QObject **)m_ppComboBox, m_nComboBoxSize, bState);
}

void MACHSectionHeaderWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    FormatWidget::adjustHeaderTable(nType, pTableWidget);
}

void MACHSectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void MACHSectionHeaderWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    qint32 nType = getType();

    XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (mach.isValid()) {
        bool bIs64 = mach.is64();
        bool bIsBigEndian = mach.isBigEndian();

        if (nType == SMACH::TYPE_mach_commands) {
            createHeaderTable(SMACH::TYPE_mach_commands, ui->tableWidget, N_mach_commands::records, m_ppLinedEdit, N_mach_commands::__data_size, getNumber(),
                              getOffset());
            m_ppComboBox[N_mach_commands::CB_CMD] =
                createComboBox(ui->tableWidget, XMACH::getLoadCommandTypesS(), SMACH::TYPE_mach_commands, N_mach_commands::cmd, XComboBoxEx::CBTYPE_LIST);

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            XMACH::COMMAND_RECORD cr = mach._readLoadCommand(nHeaderOffset, bIsBigEndian);

            m_ppLinedEdit[N_mach_commands::cmd]->setValue_uint32((quint32)cr.nId);
            m_ppLinedEdit[N_mach_commands::cmdsize]->setValue_uint32((quint32)cr.nSize);

            m_ppComboBox[N_mach_commands::CB_CMD]->setValue(cr.nId);

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.getCommandHeaderSize();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if ((nType == SMACH::TYPE_mach_libraries) || (nType == SMACH::TYPE_mach_weak_libraries) || (nType == SMACH::TYPE_mach_id_library)) {
            createHeaderTable(nType, ui->tableWidget, N_mach_library::records, m_ppLinedEdit, N_mach_library::__data_size, getNumber(), getOffset());

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            XMACH::LIBRARY_RECORD lr = mach._readLibraryRecord(nHeaderOffset, bIsBigEndian);
            XMACH::COMMAND_RECORD cr = mach._readLoadCommand(nHeaderOffset, bIsBigEndian);

            m_ppLinedEdit[N_mach_library::timestamp]->setValue_uint32((quint32)lr.timestamp);
            m_ppLinedEdit[N_mach_library::current_version]->setValue_uint32((quint32)lr.current_version);
            m_ppLinedEdit[N_mach_library::compatibility_version]->setValue_uint32((quint32)lr.compatibility_version);
            m_ppLinedEdit[N_mach_library::name]->setValue_String(lr.sFullName, lr.nMaxStringSize);

            if (lr.nMaxStringSize) {
                QTableWidgetItem *pItem = ui->tableWidget->item(N_mach_library::name, 0);

                if (pItem) {
                    pItem->setData(Qt::UserRole + HEADER_DATA_SIZE, lr.nMaxStringSize);
                }
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = cr.nSize;
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if ((nType == SMACH::TYPE_mach_LOADFVMLIB) || (nType == SMACH::TYPE_mach_IDFVMLIB)) {
            createHeaderTable(nType, ui->tableWidget, N_mach_fmv_library::records, m_ppLinedEdit, N_mach_fmv_library::__data_size, getNumber(), getOffset());

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            XMACH::FVM_LIBRARY_RECORD lr = mach._readFvmLibraryRecord(nHeaderOffset, bIsBigEndian);
            XMACH::COMMAND_RECORD cr = mach._readLoadCommand(nHeaderOffset, bIsBigEndian);

            m_ppLinedEdit[N_mach_fmv_library::minor_version]->setValue_uint32((quint32)lr.minor_version);
            m_ppLinedEdit[N_mach_fmv_library::header_addr]->setValue_uint32((quint32)lr.header_addr);
            m_ppLinedEdit[N_mach_fmv_library::name]->setValue_String(lr.sFullName, lr.nMaxStringSize);

            if (lr.nMaxStringSize) {
                QTableWidgetItem *pItem = ui->tableWidget->item(N_mach_fmv_library::name, 0);

                if (pItem) {
                    pItem->setData(Qt::UserRole + HEADER_DATA_SIZE, lr.nMaxStringSize);
                }
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = cr.nSize;
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if (nType == SMACH::TYPE_mach_segments) {
            createHeaderTable(SMACH::TYPE_mach_segments, ui->tableWidget, bIs64 ? (N_mach_segments::records64) : (N_mach_segments::records32), m_ppLinedEdit,
                              N_mach_segments::__data_size, getNumber(), getOffset());
            m_ppComboBox[N_mach_segments::CB_initprot] =
                createComboBox(ui->tableWidget, XMACH::getVMProtectionsS(), SMACH::TYPE_mach_segments, N_mach_segments::initprot, XComboBoxEx::CBTYPE_FLAGS);
            m_ppComboBox[N_mach_segments::CB_maxprot] =
                createComboBox(ui->tableWidget, XMACH::getVMProtectionsS(), SMACH::TYPE_mach_segments, N_mach_segments::maxprot, XComboBoxEx::CBTYPE_FLAGS);

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            if (bIs64) {
                XMACH_DEF::segment_command_64 segment = mach._read_segment_command_64(nHeaderOffset, bIsBigEndian);

                m_ppLinedEdit[N_mach_segments::segname]->setValue_String(segment.segname);
                m_ppLinedEdit[N_mach_segments::vmaddr]->setValue_uint64(segment.vmaddr);
                m_ppLinedEdit[N_mach_segments::vmsize]->setValue_uint64(segment.vmsize);
                m_ppLinedEdit[N_mach_segments::fileoff]->setValue_uint64(segment.fileoff);
                m_ppLinedEdit[N_mach_segments::filesize]->setValue_uint64(segment.filesize);
                m_ppLinedEdit[N_mach_segments::maxprot]->setValue_uint32(segment.maxprot);
                m_ppLinedEdit[N_mach_segments::initprot]->setValue_uint32(segment.initprot);
                m_ppLinedEdit[N_mach_segments::nsects]->setValue_uint32(segment.nsects);
                m_ppLinedEdit[N_mach_segments::flags]->setValue_uint32(segment.flags);

                m_ppComboBox[N_mach_segments::CB_initprot]->setValue(segment.initprot);
                m_ppComboBox[N_mach_segments::CB_maxprot]->setValue(segment.maxprot);
            } else {
                XMACH_DEF::segment_command segment = mach._read_segment_command(nHeaderOffset, bIsBigEndian);

                m_ppLinedEdit[N_mach_segments::segname]->setValue_String(segment.segname);
                m_ppLinedEdit[N_mach_segments::vmaddr]->setValue_uint32(segment.vmaddr);
                m_ppLinedEdit[N_mach_segments::vmsize]->setValue_uint32(segment.vmsize);
                m_ppLinedEdit[N_mach_segments::fileoff]->setValue_uint32(segment.fileoff);
                m_ppLinedEdit[N_mach_segments::filesize]->setValue_uint32(segment.filesize);
                m_ppLinedEdit[N_mach_segments::maxprot]->setValue_uint32(segment.maxprot);
                m_ppLinedEdit[N_mach_segments::initprot]->setValue_uint32(segment.initprot);
                m_ppLinedEdit[N_mach_segments::nsects]->setValue_uint32(segment.nsects);
                m_ppLinedEdit[N_mach_segments::flags]->setValue_uint32(segment.flags);

                m_ppComboBox[N_mach_segments::CB_initprot]->setValue(segment.initprot);
                m_ppComboBox[N_mach_segments::CB_maxprot]->setValue(segment.maxprot);
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.getSegmentHeaderSize();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if (nType == SMACH::TYPE_mach_sections) {
            if (bIs64) {
                createHeaderTable(SMACH::TYPE_mach_sections, ui->tableWidget, N_mach_sections64_E::records, m_ppLinedEdit, N_mach_sections64_E::__data_size, getNumber(),
                                  getOffset());
            } else {
                createHeaderTable(SMACH::TYPE_mach_sections, ui->tableWidget, N_mach_sections32_E::records, m_ppLinedEdit, N_mach_sections32_E::__data_size, getNumber(),
                                  getOffset());
            }

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            if (bIs64) {
                m_ppComboBox[N_mach_sections64_E::CB_flag0] = createComboBox(ui->tableWidget, XMACH::getSectionFlagsTypesS(), SMACH::TYPE_mach_sections,
                                                                             N_mach_sections64_E::flags_0, XComboBoxEx::CBTYPE_ELIST, XMACH_DEF::S_SECTION_TYPE);
                m_ppComboBox[N_mach_sections64_E::CB_flag1] =
                    createComboBox(ui->tableWidget, XMACH::getSectionAttributesSysS(), SMACH::TYPE_mach_sections, N_mach_sections64_E::flags_1, XComboBoxEx::CBTYPE_ELIST,
                                   XMACH_DEF::S_SECTION_ATTRIBUTES_SYS);
                m_ppComboBox[N_mach_sections64_E::CB_flag2] =
                    createComboBox(ui->tableWidget, XMACH::getSectionAttributesUsrS(), SMACH::TYPE_mach_sections, N_mach_sections64_E::flags_2, XComboBoxEx::CBTYPE_ELIST,
                                   XMACH_DEF::S_SECTION_ATTRIBUTES_USR);

                XMACH_DEF::section_64 section = mach._read_section_64(nHeaderOffset, bIsBigEndian);

                m_ppLinedEdit[N_mach_sections64_E::sectname]->setValue_String(section.sectname);
                m_ppLinedEdit[N_mach_sections64_E::segname]->setValue_String(section.segname);
                m_ppLinedEdit[N_mach_sections64_E::addr]->setValue_uint64(section.addr);
                m_ppLinedEdit[N_mach_sections64_E::size]->setValue_uint64(section.size);
                m_ppLinedEdit[N_mach_sections64_E::offset]->setValue_uint32(section.offset);
                m_ppLinedEdit[N_mach_sections64_E::align]->setValue_uint32(section.align);
                m_ppLinedEdit[N_mach_sections64_E::reloff]->setValue_uint32(section.reloff);
                m_ppLinedEdit[N_mach_sections64_E::nreloc]->setValue_uint32(section.nreloc);
                m_ppLinedEdit[N_mach_sections64_E::flags_0]->setValue_uint32(section.flags);
                m_ppLinedEdit[N_mach_sections64_E::reserved1]->setValue_uint32(section.reserved1);
                m_ppLinedEdit[N_mach_sections64_E::reserved2]->setValue_uint32(section.reserved2);
                m_ppLinedEdit[N_mach_sections64_E::reserved3]->setValue_uint32(section.reserved3);

                m_ppComboBox[N_mach_sections64_E::CB_flag0]->setValue(section.flags);
                m_ppComboBox[N_mach_sections64_E::CB_flag1]->setValue(section.flags);
                m_ppComboBox[N_mach_sections64_E::CB_flag2]->setValue(section.flags);
            } else {
                m_ppComboBox[N_mach_sections32_E::CB_flag0] = createComboBox(ui->tableWidget, XMACH::getSectionFlagsTypesS(), SMACH::TYPE_mach_sections,
                                                                             N_mach_sections32_E::flags_0, XComboBoxEx::CBTYPE_ELIST, XMACH_DEF::S_SECTION_TYPE);
                m_ppComboBox[N_mach_sections32_E::CB_flag1] =
                    createComboBox(ui->tableWidget, XMACH::getSectionAttributesSysS(), SMACH::TYPE_mach_sections, N_mach_sections32_E::flags_1, XComboBoxEx::CBTYPE_ELIST,
                                   XMACH_DEF::S_SECTION_ATTRIBUTES_SYS);
                m_ppComboBox[N_mach_sections32_E::CB_flag2] =
                    createComboBox(ui->tableWidget, XMACH::getSectionAttributesUsrS(), SMACH::TYPE_mach_sections, N_mach_sections32_E::flags_2, XComboBoxEx::CBTYPE_ELIST,
                                   XMACH_DEF::S_SECTION_ATTRIBUTES_USR);

                XMACH_DEF::section section = mach._read_section(nHeaderOffset, bIsBigEndian);

                m_ppLinedEdit[N_mach_sections32_E::sectname]->setValue_String(section.sectname);
                m_ppLinedEdit[N_mach_sections32_E::segname]->setValue_String(section.segname);
                m_ppLinedEdit[N_mach_sections32_E::addr]->setValue_uint32(section.addr);
                m_ppLinedEdit[N_mach_sections32_E::size]->setValue_uint32(section.size);
                m_ppLinedEdit[N_mach_sections32_E::offset]->setValue_uint32(section.offset);
                m_ppLinedEdit[N_mach_sections32_E::align]->setValue_uint32(section.align);
                m_ppLinedEdit[N_mach_sections32_E::reloff]->setValue_uint32(section.reloff);
                m_ppLinedEdit[N_mach_sections32_E::nreloc]->setValue_uint32(section.nreloc);
                m_ppLinedEdit[N_mach_sections32_E::flags_0]->setValue_uint32(section.flags);
                m_ppLinedEdit[N_mach_sections32_E::reserved1]->setValue_uint32(section.reserved1);
                m_ppLinedEdit[N_mach_sections32_E::reserved2]->setValue_uint32(section.reserved2);

                m_ppComboBox[N_mach_sections32_E::CB_flag0]->setValue(section.flags);
                m_ppComboBox[N_mach_sections32_E::CB_flag1]->setValue(section.flags);
                m_ppComboBox[N_mach_sections32_E::CB_flag2]->setValue(section.flags);
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.getSectionHeaderSize();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if (nType == SMACH::TYPE_SYMBOLTABLE) {
            createHeaderTable(SMACH::TYPE_SYMBOLTABLE, ui->tableWidget, bIs64 ? (N_mach_nlist::records64) : (N_mach_nlist::records32), m_ppLinedEdit,
                              N_mach_nlist::__data_size, getNumber(), getOffset());

            blockSignals(true);

            XBinary::OFFSETSIZE osStringTable = mach.getStringTableOffsetSize();

            qint64 nHeaderOffset = getOffset();

            if (bIs64) {
                XMACH_DEF::nlist_64 nlist = mach._read_nlist_64(nHeaderOffset);

                m_ppLinedEdit[N_mach_nlist::n_strx]->setValue_uint32(nlist.n_strx);
                m_ppLinedEdit[N_mach_nlist::n_type]->setValue_uint8(nlist.n_type);
                m_ppLinedEdit[N_mach_nlist::n_sect]->setValue_uint8(nlist.n_sect);
                m_ppLinedEdit[N_mach_nlist::n_desc]->setValue_uint16(nlist.n_desc);
                m_ppLinedEdit[N_mach_nlist::n_value]->setValue_uint64(nlist.n_value);

                addComment(ui->tableWidget, N_mach_nlist::n_strx, HEADER_COLUMN_COMMENT,
                           mach.getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, nlist.n_strx));
            } else {
                XMACH_DEF::nlist nlist = mach._read_nlist(nHeaderOffset);

                m_ppLinedEdit[N_mach_nlist::n_strx]->setValue_uint32(nlist.n_strx);
                m_ppLinedEdit[N_mach_nlist::n_type]->setValue_uint8(nlist.n_type);
                m_ppLinedEdit[N_mach_nlist::n_sect]->setValue_uint8(nlist.n_sect);
                m_ppLinedEdit[N_mach_nlist::n_desc]->setValue_uint16(nlist.n_desc);
                m_ppLinedEdit[N_mach_nlist::n_value]->setValue_uint32(nlist.n_value);

                addComment(ui->tableWidget, N_mach_nlist::n_strx, HEADER_COLUMN_COMMENT,
                           mach.getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, nlist.n_strx));
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = 0;

            if (bIs64) {
                nSize = mach.get_nlist_64_size();
            } else {
                nSize = mach.get_nlist_size();
            }

            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if (nType == SMACH::TYPE_DICE) {
            createHeaderTable(SMACH::TYPE_DICE, ui->tableWidget, N_mach_data_in_code_entry::records, m_ppLinedEdit, N_mach_data_in_code_entry::__data_size, getNumber(),
                              getOffset());
            m_ppComboBox[N_mach_data_in_code_entry::CB_kind] =
                createComboBox(ui->tableWidget, XMACH::getDICEKindsS(), SMACH::TYPE_DICE, N_mach_data_in_code_entry::kind, XComboBoxEx::CBTYPE_LIST);

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            XMACH_DEF::data_in_code_entry dice = mach._read_data_in_code_entry(nHeaderOffset);

            m_ppLinedEdit[N_mach_data_in_code_entry::offset]->setValue_uint32(dice.offset);
            m_ppLinedEdit[N_mach_data_in_code_entry::length]->setValue_uint16(dice.length);
            m_ppLinedEdit[N_mach_data_in_code_entry::kind]->setValue_uint16(dice.kind);

            m_ppComboBox[N_mach_data_in_code_entry::CB_kind]->setValue(dice.kind);

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.get_data_in_code_entry_size();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if (nType == SMACH::TYPE_DYSYMTAB_modtab) {
            if (bIs64) {
                createHeaderTable(SMACH::TYPE_DYSYMTAB_modtab, ui->tableWidget, N_mach_modtab64::records, m_ppLinedEdit, N_mach_modtab64::__data_size, getNumber(),
                                  getOffset());
            } else {
                createHeaderTable(SMACH::TYPE_DYSYMTAB_modtab, ui->tableWidget, N_mach_modtab32::records, m_ppLinedEdit, N_mach_modtab32::__data_size, getNumber(),
                                  getOffset());
            }

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            if (bIs64) {
                XMACH_DEF::dylib_module_64 record = mach._read_dylib_module_64(nHeaderOffset);

                m_ppLinedEdit[N_mach_modtab64::module_name]->setValue_uint32(record.module_name);
                m_ppLinedEdit[N_mach_modtab64::iextdefsym]->setValue_uint32(record.iextdefsym);
                m_ppLinedEdit[N_mach_modtab64::nextdefsym]->setValue_uint32(record.nextdefsym);
                m_ppLinedEdit[N_mach_modtab64::irefsym]->setValue_uint32(record.irefsym);
                m_ppLinedEdit[N_mach_modtab64::nrefsym]->setValue_uint32(record.nrefsym);
                m_ppLinedEdit[N_mach_modtab64::ilocalsym]->setValue_uint32(record.ilocalsym);
                m_ppLinedEdit[N_mach_modtab64::nlocalsym]->setValue_uint32(record.nlocalsym);
                m_ppLinedEdit[N_mach_modtab64::iextrel]->setValue_uint32(record.iextrel);
                m_ppLinedEdit[N_mach_modtab64::nextrel]->setValue_uint32(record.nextrel);
                m_ppLinedEdit[N_mach_modtab64::iinit_iterm]->setValue_uint32(record.iinit_iterm);
                m_ppLinedEdit[N_mach_modtab64::ninit_nterm]->setValue_uint32(record.ninit_nterm);
                m_ppLinedEdit[N_mach_modtab64::objc_module_info_size]->setValue_uint32(record.objc_module_info_size);
                m_ppLinedEdit[N_mach_modtab64::objc_module_info_addr]->setValue_uint64(record.objc_module_info_addr);
            } else {
                XMACH_DEF::dylib_module record = mach._read_dylib_module(nHeaderOffset);

                m_ppLinedEdit[N_mach_modtab32::module_name]->setValue_uint32(record.module_name);
                m_ppLinedEdit[N_mach_modtab32::iextdefsym]->setValue_uint32(record.iextdefsym);
                m_ppLinedEdit[N_mach_modtab32::nextdefsym]->setValue_uint32(record.nextdefsym);
                m_ppLinedEdit[N_mach_modtab32::irefsym]->setValue_uint32(record.irefsym);
                m_ppLinedEdit[N_mach_modtab32::nrefsym]->setValue_uint32(record.nrefsym);
                m_ppLinedEdit[N_mach_modtab32::ilocalsym]->setValue_uint32(record.ilocalsym);
                m_ppLinedEdit[N_mach_modtab32::nlocalsym]->setValue_uint32(record.nlocalsym);
                m_ppLinedEdit[N_mach_modtab32::iextrel]->setValue_uint32(record.iextrel);
                m_ppLinedEdit[N_mach_modtab32::nextrel]->setValue_uint32(record.nextrel);
                m_ppLinedEdit[N_mach_modtab32::iinit_iterm]->setValue_uint32(record.iinit_iterm);
                m_ppLinedEdit[N_mach_modtab32::ninit_nterm]->setValue_uint32(record.ninit_nterm);
                m_ppLinedEdit[N_mach_modtab32::objc_module_info_addr]->setValue_uint32(record.objc_module_info_addr);
                m_ppLinedEdit[N_mach_modtab32::objc_module_info_size]->setValue_uint32(record.objc_module_info_size);
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = 0;

            if (bIs64) {
                nSize = mach.get_dylib_module_64_size();
            } else {
                nSize = mach.get_dylib_module_size();
            }

            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if (nType == SMACH::TYPE_DYSYMTAB_toc) {
            createHeaderTable(SMACH::TYPE_DYSYMTAB_toc, ui->tableWidget, N_mach_table_of_contents::records, m_ppLinedEdit, N_mach_table_of_contents::__data_size,
                              getNumber(), getOffset());

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            XMACH_DEF::dylib_table_of_contents toc = mach._read_dylib_table_of_contents(nHeaderOffset);

            m_ppLinedEdit[N_mach_table_of_contents::symbol_index]->setValue_uint32(toc.symbol_index);
            m_ppLinedEdit[N_mach_table_of_contents::module_index]->setValue_uint32(toc.module_index);

            addComment(ui->tableWidget, N_mach_table_of_contents::symbol_index, HEADER_COLUMN_COMMENT, mach.getIndexSymbolName(toc.symbol_index));

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.get_dylib_table_of_contents_size();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if ((nType == SMACH::TYPE_DYSYMTAB_extrel) || (nType == SMACH::TYPE_DYSYMTAB_locrel)) {
            createHeaderTable(nType, ui->tableWidget, N_mach_relocs::records, m_ppLinedEdit, N_mach_relocs::__data_size, getNumber(), getOffset());

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            XMACH_DEF::relocation_info reloc = mach._read_relocation_info(nHeaderOffset);

            m_ppLinedEdit[N_mach_relocs::r_address]->setValue_uint32(reloc.r_address);
            m_ppLinedEdit[N_mach_relocs::value]->setValue_uint32(reloc.s.value);

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.get_relocation_info_size();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        } else if ((nType == SMACH::TYPE_DYSYMTAB_indirectsyms) || (nType == SMACH::TYPE_DYSYMTAB_extrefsyms)) {
            createHeaderTable(SMACH::TYPE_DYSYMTAB_indirectsyms, ui->tableWidget, N_mach_value::records, m_ppLinedEdit, N_mach_value::__data_size, getNumber(),
                              getOffset());

            blockSignals(true);

            qint64 nHeaderOffset = getOffset();

            quint32 nValue = mach.read_uint32(nHeaderOffset, mach.isBigEndian());

            m_ppLinedEdit[N_mach_value::value]->setValue_uint32(nValue);

            if (nType == SMACH::TYPE_DYSYMTAB_indirectsyms) {
                addComment(ui->tableWidget, N_mach_value::value, HEADER_COLUMN_COMMENT, mach.getIndexSymbolName(nValue));
            }

            qint64 nOffset = nHeaderOffset;
            qint64 nSize = mach.get_value32_size();
            qint64 nAddress = mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &m_pSubDevice, ui->widgetHex, false, false, false);

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void MACHSectionHeaderWidget::_widgetValueChanged(QVariant vValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    quint64 nValue = vValue.toULongLong();

    if (mach.isValid()) {
        switch (nStype) {
            case SMACH::TYPE_mach_commands:
                switch (nNdata) {
                    case N_mach_commands::cmd: m_ppLinedEdit[N_mach_commands::cmd]->setValue_uint32((quint32)nValue); break;
                }
                break;

            case SMACH::TYPE_mach_segments:
                switch (nNdata) {
                    case N_mach_segments::initprot: m_ppLinedEdit[N_mach_segments::initprot]->setValue_uint32((quint32)nValue); break;
                    case N_mach_segments::maxprot: m_ppLinedEdit[N_mach_segments::maxprot]->setValue_uint32((quint32)nValue); break;
                }
                break;

            case SMACH::TYPE_DICE:
                switch (nNdata) {
                    case N_mach_data_in_code_entry::kind: m_ppLinedEdit[N_mach_data_in_code_entry::kind]->setValue_uint64(nValue); break;
                }

                break;

            case SMACH::TYPE_mach_sections:

                if (mach.is64()) {
                    switch (nNdata) {
                        case N_mach_sections64_E::flags_0:
                        case N_mach_sections64_E::flags_1:
                        case N_mach_sections64_E::flags_2:
                            m_ppLinedEdit[N_mach_sections64_E::flags_0]->setValue_uint32((quint32)nValue);
                            m_ppComboBox[N_mach_sections64_E::CB_flag0]->setValue(nValue);
                            m_ppComboBox[N_mach_sections64_E::CB_flag1]->setValue(nValue);
                            m_ppComboBox[N_mach_sections64_E::CB_flag2]->setValue(nValue);
                            break;
                    }
                } else {
                    switch (nNdata) {
                        case N_mach_sections32_E::flags_0:
                        case N_mach_sections32_E::flags_1:
                        case N_mach_sections32_E::flags_2:
                            m_ppLinedEdit[N_mach_sections32_E::flags_0]->setValue_uint32((quint32)nValue);
                            m_ppComboBox[N_mach_sections32_E::CB_flag0]->setValue(nValue);
                            m_ppComboBox[N_mach_sections32_E::CB_flag1]->setValue(nValue);
                            m_ppComboBox[N_mach_sections32_E::CB_flag2]->setValue(nValue);
                            break;
                    }
                }

                break;
        }
    }
}

void MACHSectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex, ui->tableWidget);
}
