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
#include "xgenericheaderwidget.h"
#include "ui_xgenericheaderwidget.h"

XGenericHeaderWidget::XGenericHeaderWidget(QWidget *pParent) : XFormatWidget(pParent), ui(new Ui::XGenericHeaderWidget)
{
    ui->setupUi(this);
}

XGenericHeaderWidget::~XGenericHeaderWidget()
{
    delete ui;
}

XFormatWidget::SV XGenericHeaderWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void XGenericHeaderWidget::reloadData(bool bSaveSelection)
{
    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    cleanup();

    QList<XFW_DEF::HEADER_RECORD> listHeaderRecords = getHeaderRecords(getCwOptions());

    createHeaderTable(ui->tableWidgetMain, &listHeaderRecords, getListRecWidgets(), getCwOptions()->nDataOffset, getCwOptions()->endian);

    if ((getCwOptions()->_type == XFW_DEF::TYPE_MACH_command) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_segment) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_dylib) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_rpath) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_sub_umbrella) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_sub_client) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_sub_library) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_symtab) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_dysymtab) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_segment_split_info) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_atom_info) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_function_starts) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_dyld_exports_trie) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_dyld_chained_fixups) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_encryption_info) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_dyld_info) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_version_min) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_uuid) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_build_version) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_main) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_fileset_entry) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_source_version) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_dylinker) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_data_in_code) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_code_signature) ||
        (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_routines)){
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getLoadCommandTypesS(), X_mach_commands::cmd, XComboBoxEx::CBTYPE_LIST, 0);
    }

    if (getCwOptions()->_type == XFW_DEF::TYPE_MACH_mach_header) {
        XBinary binary(getDevice());
        quint32 _cputype = binary.read_int32(getCwOptions()->nDataOffset + offsetof(XMACH_DEF::mach_header, cputype), (getCwOptions()->endian == XBinary::ENDIAN_BIG));

        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderMagicsS(), X_mach_header::magic, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderCpuTypesS(), X_mach_header::cputype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderCpuSubTypesS(_cputype), X_mach_header::cpusubtype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderFileTypesS(), X_mach_header::filetype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderFlagsS(), X_mach_header::flags, XComboBoxEx::CBTYPE_FLAGS, 0);
    } else if (getCwOptions()->_type == XFW_DEF::TYPE_MACH_command_segment) {
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getVMProtectionsS(), X_mach_segments::initprot, XComboBoxEx::CBTYPE_FLAGS, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getVMProtectionsS(), X_mach_segments::maxprot, XComboBoxEx::CBTYPE_FLAGS, 0);
        // } else if (getCwOptions()->_type == FW_DEF::TYPE_MACH_section) {
        //     adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getSectionTypesS(), N_mach_section::flags, XComboBoxEx::CBTYPE_FLAGS, 0);
        // } else if (getCwOptions()->_type == FW_DEF::TYPE_MACH_dyld_info_command) {
        //     adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getCommandsS(), N_mach_dyld_info_command::cmd, XComboBoxEx::CBTYPE_LIST, 0);
        // } else if (getCwOptions()->_type == FW_DEF::TYPE_MACH_uuid_command) {
        //     adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getCommandsS(), N_mach_uuid_command::cmd, XComboBoxEx::CBTYPE_LIST, 0);
        // } else if (getCwOptions()->_type == FW_DEF::TYPE_MACH_dylib_command) {
        //     adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getCommandsS(), N_mach_dylib_command::cmd, XComboBoxEx::CBTYPE_LIST, 0);
        // } else if (getCwOptions()->_type == FW_DEF::TYPE_MACH_routines_command) {
        //     adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getCommandsS(), N_mach_routines_command::cmd, XComboBoxEx::CBTYPE_LIST, 0);
        // } else if (getCwOptions()->_type == FW_DEF::TYPE_MACH_routines_command_64) {
        //     adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getCommandsS(), N_mach_routines_command_64::cmd, XComboBoxEx::CBTYPE_LIST, 0);
    } else if (getCwOptions()->_type == XFW_DEF::TYPE_ELF_elf_ehdr) {
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderMagicsS(), _elf_ehdrWidget::ei_mag, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderClassesS(), _elf_ehdrWidget::ei_class, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderDataS(), _elf_ehdrWidget::ei_data, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderVersionsS(), _elf_ehdrWidget::ei_version, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderOsabisS(), _elf_ehdrWidget::ei_osabi, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderFileTypesS(), _elf_ehdrWidget::e_type, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderMachinesS(), _elf_ehdrWidget::e_machine, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XELF::getHeaderFlagsS(), _elf_ehdrWidget::e_flags, XComboBoxEx::CBTYPE_FLAGS, 0);
    }

    updateRecWidgets(getCwOptions()->pDevice, getListRecWidgets());

    ui->tableWidgetMain->setCurrentCell(nCurrentRow, 0);
}

void XGenericHeaderWidget::adjustView()
{
    XFormatWidget::adjustView();
    getGlobalOptions()->adjustWidget(ui->tableWidgetMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericHeaderWidget::on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderSelection(ui->tableWidgetMain);
}

void XGenericHeaderWidget::on_tableWidgetMain_customContextMenuRequested(const QPoint &pos)
{
    contextMenuGenericHeaderWidget(pos, ui->tableWidgetMain, getListRecWidgets(), getCwOptions());
}

void XGenericHeaderWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericHeaderWidget::on_toolButtonTableSave_clicked()
{
}
