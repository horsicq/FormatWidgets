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
#include "generictablewidget.h"
#include "ui_generictablewidget.h"

GenericTableWidget::GenericTableWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::GenericTableWidget)
{
    ui->setupUi(this);
}

GenericTableWidget::~GenericTableWidget()
{
    delete ui;
}

FormatWidget::SV GenericTableWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void GenericTableWidget::reloadData(bool bSaveSelection)
{
    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    cleanup();

    const FW_DEF::HEADER_RECORD *pRecords = 0;
    qint32 nNumberOfRecords = 0;

    if (getCwOptions()->_type == FW_DEF::TYPE_MACH_mach_header) {
        if (getCwOptions()->mode==XBinary::MODE_64) {
            pRecords = N_mach_header::records64;
            nNumberOfRecords = N_mach_header::__data_size;
        } else if (getCwOptions()->mode==XBinary::MODE_32){
            pRecords = N_mach_header::records32;
            nNumberOfRecords = N_mach_header::__data_size - 1;
        }
    } else if (getCwOptions()->_type == FW_DEF::TYPE_ELF_elf_ehdr) {
        if (getCwOptions()->mode==XBinary::MODE_64) {
            pRecords = N_Elf_Ehdr::records64;
            nNumberOfRecords = N_Elf_Ehdr::__data_size;
        } else if (getCwOptions()->mode==XBinary::MODE_32){
            pRecords = N_Elf_Ehdr::records32;
            nNumberOfRecords = N_Elf_Ehdr::__data_size;
        }
    }

    createHeaderTable(ui->tableWidgetMain, pRecords, getListRecWidgets(),
                      nNumberOfRecords, getCwOptions()->nDataOffset, getCwOptions()->endian);

    if (getCwOptions()->_type == FW_DEF::TYPE_MACH_mach_header) {
        XBinary binary(getDevice());
        quint32 _cputype = binary.read_int32(getCwOptions()->nDataOffset + offsetof(XMACH_DEF::mach_header, cputype), (getCwOptions()->endian == XBinary::ENDIAN_BIG));

        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderMagicsS(), N_mach_header::magic, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderCpuTypesS(), N_mach_header::cputype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderCpuSubTypesS(_cputype), N_mach_header::cpusubtype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderFileTypesS(), N_mach_header::filetype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(ui->tableWidgetMain, getListRecWidgets(), XMACH::getHeaderFlagsS(), N_mach_header::flags, XComboBoxEx::CBTYPE_FLAGS, 0);
    } else if (getCwOptions()->_type == FW_DEF::TYPE_ELF_elf_ehdr) {
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

void GenericTableWidget::on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->tableWidgetMain);
}

void GenericTableWidget::on_tableWidgetMain_customContextMenuRequested(const QPoint &pos)
{
    contextMenuTableHeader(pos, ui->tableWidgetMain, getListRecWidgets(), getCwOptions());
}

