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
#include "mach_headerwidget.h"
#include "ui_mach_headerwidget.h"

namespace _mach_headerWidget {
enum DATA {
    magic = 0,
    cputype,
    cpusubtype,
    filetype,
    ncmds,
    sizeofcmds,
    flags,
    reserved,
    __data_size
};

const FW_DEF::HEADER_RECORD records32[] = {
    {magic, "magic", offsetof(XMACH_DEF::mach_header, magic), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {cputype, "cputype", offsetof(XMACH_DEF::mach_header, cputype), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {cpusubtype, "cpusubtype", offsetof(XMACH_DEF::mach_header, cpusubtype), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {filetype, "filetype", offsetof(XMACH_DEF::mach_header, filetype), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {ncmds, "ncmds", offsetof(XMACH_DEF::mach_header, ncmds), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_NONE},
    {sizeofcmds, "sizeofcmds", offsetof(XMACH_DEF::mach_header, sizeofcmds), 4, "uint32", FW_DEF::VAL_TYPE_SIZE, FW_DEF::INFO_NONE},
    {flags, "flags", offsetof(XMACH_DEF::mach_header, flags), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
};

const FW_DEF::HEADER_RECORD records64[] = {
    {magic, "magic", offsetof(XMACH_DEF::mach_header_64, magic), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {cputype, "cputype", offsetof(XMACH_DEF::mach_header_64, cputype), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {cpusubtype, "cpusubtype", offsetof(XMACH_DEF::mach_header_64, cpusubtype), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {filetype, "filetype", offsetof(XMACH_DEF::mach_header_64, filetype), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {ncmds, "ncmds", offsetof(XMACH_DEF::mach_header_64, ncmds), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_NONE},
    {sizeofcmds, "sizeofcmds", offsetof(XMACH_DEF::mach_header_64, sizeofcmds), 4, "uint32", FW_DEF::VAL_TYPE_SIZE, FW_DEF::INFO_NONE},
    {flags, "flags", offsetof(XMACH_DEF::mach_header_64, flags), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_COMBOBOX},
    {reserved, "reserved", offsetof(XMACH_DEF::mach_header_64, reserved), 4, "uint32", FW_DEF::VAL_TYPE_DATA, FW_DEF::INFO_NONE},
};
}

mach_headerWidget::mach_headerWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::mach_headerWidget)
{
    ui->setupUi(this);

    ui->widgetHeader->setData(ui->tableWidgetMain, getListRecWidgets(), getCwOptions());
    connect(ui->widgetHeader, SIGNAL(reload()), this, SLOT(_reload()));
}

mach_headerWidget::~mach_headerWidget()
{
    delete ui;
}

FormatWidget::SV mach_headerWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void mach_headerWidget::reloadData(bool bSaveSelection)
{
    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    cleanup();

    XMACH mach(getCwOptions()->pDevice, getCwOptions()->bIsImage, getCwOptions()->nImageBase);

    if (mach.isValid()) {
        if (getCwOptions()->mode==XBinary::MODE_64) {
            createHeaderTable(ui->tableWidgetMain, _mach_headerWidget::records64, getListRecWidgets(), _mach_headerWidget::__data_size,
                              getCwOptions()->nDataOffset, getCwOptions()->endian);
        } else if (getCwOptions()->mode==XBinary::MODE_32){
            createHeaderTable(ui->tableWidgetMain, _mach_headerWidget::records32, getListRecWidgets(),
                              _mach_headerWidget::__data_size - 1, getCwOptions()->nDataOffset, getCwOptions()->endian);
        }

        adjustComboBox(getListRecWidgets(), XMACH::getHeaderMagicsS(), _mach_headerWidget::magic, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(getListRecWidgets(), XMACH::getHeaderCpuTypesS(), _mach_headerWidget::cputype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(getListRecWidgets(), XMACH::getHeaderCpuSubTypesS(mach.getHeader_cputype()), _mach_headerWidget::cpusubtype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(getListRecWidgets(), XMACH::getHeaderFileTypesS(), _mach_headerWidget::filetype, XComboBoxEx::CBTYPE_LIST, 0);
        adjustComboBox(getListRecWidgets(), XMACH::getHeaderFlagsS(), _mach_headerWidget::flags, XComboBoxEx::CBTYPE_FLAGS, 0);

        updateRecWidgets(getCwOptions()->pDevice, getListRecWidgets());
    }

    ui->tableWidgetMain->setCurrentCell(nCurrentRow, 0);
}

void mach_headerWidget::on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->tableWidgetMain);
}

void mach_headerWidget::on_tableWidgetMain_customContextMenuRequested(const QPoint &pos)
{
    contextMenuTableHeader(pos, ui->tableWidgetMain, getListRecWidgets(), getCwOptions());
}

