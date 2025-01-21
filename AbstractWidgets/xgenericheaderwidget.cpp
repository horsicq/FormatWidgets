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

    g_nDataSize = 0;

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSize, XOptions::ICONTYPE_SIZE);
    XOptions::adjustToolButton(ui->toolButtonTableSave, XOptions::ICONTYPE_SAVE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSize->setToolTip(tr("Size"));
    ui->toolButtonTableSave->setToolTip(tr("Save"));
}

XGenericHeaderWidget::~XGenericHeaderWidget()
{
    delete ui;
}

void XGenericHeaderWidget::reloadData(bool bSaveSelection)
{
    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    cleanup();

    g_nDataSize = getCwOptions()->nDataSize;

    qint32 nLimit = -1;

    if ((getCwOptions()->_type > XFW_DEF::TYPE_MACH_START) && (getCwOptions()->_type < XFW_DEF::TYPE_MACH_END)) {
        if (getCwOptions()->_type == XFW_DEF::TYPE_MACH_CS_CodeDirectory) {
            XBinary binary(getDevice());
            quint32 nVersion = binary.read_int32(getCwOptions()->nDataOffset + offsetof(XMACH_DEF::CS_CodeDirectory, version), true);

            if (nVersion >= 0x20600) {
                nLimit = -1;
            } else if (nVersion >= 0x20500) {
                nLimit = offsetof(XMACH_DEF::CS_CodeDirectory, linkageHashType);
            } else if (nVersion >= 0x20400) {
                nLimit = offsetof(XMACH_DEF::CS_CodeDirectory, runtime);
            } else if (nVersion >= 0x20300) {
                nLimit = offsetof(XMACH_DEF::CS_CodeDirectory, execSegBase);
            } else if (nVersion >= 0x20200) {
                nLimit = offsetof(XMACH_DEF::CS_CodeDirectory, spare3);
            } else if (nVersion >= 0x20100) {
                nLimit = offsetof(XMACH_DEF::CS_CodeDirectory, scatterOffset);
            }

            if (nLimit != -1) {
                g_nDataSize = nLimit;
            }
        } else if (getCwOptions()->_type == XFW_DEF::TYPE_MACH_routines_command_64) {
            XMACH xmach(getDevice(), getCwOptions()->bIsImage, getCwOptions()->nImageBase);
            XMACH::COMMAND_RECORD cr = xmach._readLoadCommand(getCwOptions()->nDataOffset, getCwOptions()->endian == XBinary::ENDIAN_BIG);

            nLimit = cr.nSize;
            g_nDataSize = cr.nSize;
        }
    }

    QList<XFW_DEF::HEADER_RECORD> listHeaderRecords = getHeaderRecords(getCwOptions(), nLimit);

    if (listHeaderRecords.count() == 0) {
        if (getCwOptions()->_type == XFW_DEF::TYPE_PE_NET_METADATA) {
            XBinary binary(getDevice());
            quint32 nVersionStringLength = binary.read_int32(getCwOptions()->nDataOffset + 12, getCwOptions()->endian == XBinary::ENDIAN_BIG);

            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 0, "Signature", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 1, "MajorVersion", 4, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 2, "MinorVersion", 6, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 3, "Reserved", 8, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 4, "VersionStringLength", 12, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 5, "Version", 16, nVersionStringLength, "TEXT", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 6, "Flags", 16 + nVersionStringLength + 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1);
            XFormatWidget::_addHeaderRecord(&listHeaderRecords, 7, "Streams", 16 + nVersionStringLength + 2, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1);
        }
    }

    createHeaderTable(ui->tableWidgetMain, &listHeaderRecords, getListRecWidgets(), getCwOptions()->nDataOffset, getCwOptions()->endian, getCwOptions()->var1,
                      getCwOptions()->var2);

    if ((getCwOptions()->_type > XFW_DEF::TYPE_MSDOS_START) && (getCwOptions()->_type < XFW_DEF::TYPE_MSDOS_END)) {
        if (getCwOptions()->_type == XFW_DEF::TYPE_MSDOS_IMAGE_DOS_HEADER) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMSDOS::getImageMagicsS(), XTYPE_MSDOS::X_Exe_file::exe_signature, XComboBoxEx::CBTYPE_LIST, 0);
        }
    } else if ((getCwOptions()->_type > XFW_DEF::TYPE_PE_START) && (getCwOptions()->_type < XFW_DEF::TYPE_PE_END)) {
        if (getCwOptions()->_type == XFW_DEF::TYPE_PE_IMAGE_DOS_HEADER) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMSDOS::getImageMagicsS(), XTYPE_MSDOS::X_IMAGE_DOS_HEADER::e_magic, XComboBoxEx::CBTYPE_LIST, 0);
        } else if (getCwOptions()->_type == XFW_DEF::TYPE_PE_IMAGE_NT_HEADERS) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getImageNtHeadersSignaturesS(), XTYPE_PE::X_IMAGE_NT_HEADERS::Signature, XComboBoxEx::CBTYPE_LIST,
                        0);
        } else if (getCwOptions()->_type == XFW_DEF::TYPE_PE_IMAGE_FILE_HEADER) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getImageFileHeaderMachinesS(), XTYPE_PE::X_IMAGE_FILE_HEADER::Machine, XComboBoxEx::CBTYPE_LIST,
                        0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getImageFileHeaderCharacteristicsS(), XTYPE_PE::X_IMAGE_FILE_HEADER::Characteristics,
                        XComboBoxEx::CBTYPE_FLAGS, 0);
        } else if ((getCwOptions()->_type == XFW_DEF::TYPE_PE_IMAGE_OPTIONAL_HEADER32) || (getCwOptions()->_type == XFW_DEF::TYPE_PE_IMAGE_OPTIONAL_HEADER64)) {
            XBinary binary(getDevice());
            quint16 nMajorOperatingSystemVersion =
                binary.read_int16(getCwOptions()->nDataOffset + offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorOperatingSystemVersion));

            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getImageOptionalHeaderMagicS(), XTYPE_PE::X_IMAGE_OPTIONAL_HEADER::Magic, XComboBoxEx::CBTYPE_LIST,
                        0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getMajorOperatingSystemVersionS(), XTYPE_PE::X_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion,
                        XComboBoxEx::CBTYPE_LIST, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getMinorOperatingSystemVersionS(nMajorOperatingSystemVersion),
                        XTYPE_PE::X_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion, XComboBoxEx::CBTYPE_LIST, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getImageOptionalHeaderSubsystemS(), XTYPE_PE::X_IMAGE_OPTIONAL_HEADER::Subsystem,
                        XComboBoxEx::CBTYPE_LIST, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XPE::getImageOptionalHeaderDllCharacteristicsS(), XTYPE_PE::X_IMAGE_OPTIONAL_HEADER::DllCharacteristics,
                        XComboBoxEx::CBTYPE_FLAGS, 0);
        }
    } else if ((getCwOptions()->_type > XFW_DEF::TYPE_MACH_START) && (getCwOptions()->_type < XFW_DEF::TYPE_MACH_END)) {
        if ((getCwOptions()->_type == XFW_DEF::TYPE_MACH_load_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_segment_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_segment_command_64) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_dylib_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_rpath_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_sub_umbrella_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_sub_client_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_sub_library_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_symtab_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_dysymtab_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_segment_split_info_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_atom_info_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_function_starts_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_dyld_exports_trie_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_dyld_chained_fixups_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_encryption_info_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_encryption_info_command_64) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_dyld_info_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_version_min_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_uuid_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_build_version_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_main_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_fileset_entry_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_source_version_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_dylinker_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_data_in_code_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_code_signature_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_routines_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_routines_command_64) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_fvm_library_command) ||
            (getCwOptions()->_type == XFW_DEF::TYPE_MACH_unix_thread_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_twolevel_hints_command)) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getLoadCommandTypesS(), XTYPE_MACH::X_load_commands::cmd, XComboBoxEx::CBTYPE_LIST, 0);
        }

        if ((getCwOptions()->_type == XFW_DEF::TYPE_MACH_mach_header) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_mach_header_64)) {
            XBinary binary(getDevice());
            quint32 _cputype =
                binary.read_int32(getCwOptions()->nDataOffset + offsetof(XMACH_DEF::mach_header, cputype), (getCwOptions()->endian == XBinary::ENDIAN_BIG));

            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderMagicsS(), XTYPE_MACH::X_mach_header::magic, XComboBoxEx::CBTYPE_LIST, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderCpuTypesS(), XTYPE_MACH::X_mach_header::cputype, XComboBoxEx::CBTYPE_LIST, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderCpuSubTypesS(_cputype), XTYPE_MACH::X_mach_header::cpusubtype, XComboBoxEx::CBTYPE_LIST,
                        0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderFileTypesS(), XTYPE_MACH::X_mach_header::filetype, XComboBoxEx::CBTYPE_LIST, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderFlagsS(), XTYPE_MACH::X_mach_header::flags, XComboBoxEx::CBTYPE_FLAGS, 0);
        } else if ((getCwOptions()->_type == XFW_DEF::TYPE_MACH_segment_command) || (getCwOptions()->_type == XFW_DEF::TYPE_MACH_segment_command_64)) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getVMProtectionsS(), XTYPE_MACH::X_segment_command::initprot, XComboBoxEx::CBTYPE_FLAGS, 0);
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getVMProtectionsS(), XTYPE_MACH::X_segment_command::maxprot, XComboBoxEx::CBTYPE_FLAGS, 0);
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
        } else if (getCwOptions()->_type == XFW_DEF::TYPE_MACH_build_version_command) {
            addComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getPlatformS(), XTYPE_MACH::X_build_version_command::platform, XComboBoxEx::CBTYPE_LIST, 0);
        } else if (getCwOptions()->_type == XFW_DEF::TYPE_MACH_CS_CodeDirectory) {
        } else if ((getCwOptions()->_type == XFW_DEF::TYPE_Elf32_Ehdr) || (getCwOptions()->_type == XFW_DEF::TYPE_Elf64_Ehdr)) {
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderMagicsS(), _elf_ehdrWidget::ei_mag, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderClassesS(), _elf_ehdrWidget::ei_class, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderDataS(), _elf_ehdrWidget::ei_data, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderVersionsS(), _elf_ehdrWidget::ei_version, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderOsabisS(), _elf_ehdrWidget::ei_osabi, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderFileTypesS(), _elf_ehdrWidget::e_type, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderMachinesS(), _elf_ehdrWidget::e_machine, XComboBoxEx::CBTYPE_LIST, 0);
            // adjustComboBox(getListRecWidgets(), XELF::getHeaderFlagsS(), _elf_ehdrWidget::e_flags, XComboBoxEx::CBTYPE_FLAGS, 0);
        }
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

void XGenericHeaderWidget::on_toolButtonTableSize_clicked()
{
    emit followLocation(getCwOptions()->nDataOffset, XBinary::LT_OFFSET, g_nDataSize, XOptions::WIDGETTYPE_HEX);
}

void XGenericHeaderWidget::on_toolButtonTableSave_clicked()
{
    saveModel(ui->tableWidgetMain->model(), getTypeTitle(getCwOptions()));
}

void XGenericHeaderWidget::on_tableWidgetMain_cellClicked(int nRow, int nColumn)
{
    Q_UNUSED(nRow)
    Q_UNUSED(nColumn)

    setHeaderSelection(ui->tableWidgetMain);
}
