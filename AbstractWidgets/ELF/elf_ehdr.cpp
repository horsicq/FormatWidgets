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
#include "elf_ehdr.h"
#include "ui_elf_ehdr.h"

namespace _elf_ehdrWidget {
enum DATA {
    ei_mag = 0,
    ei_class,
    ei_data,
    ei_version,
    ei_osabi,
    ei_abiversion,
    ei_pad_0,
    ei_pad_1,
    ei_pad_2,
    ei_pad_3,
    ei_pad_4,
    ei_pad_5,
    ei_pad_6,
    e_type,
    e_machine,
    e_version,
    e_entry,
    e_phoff,
    e_shoff,
    e_flags,
    e_ehsize,
    e_phentsize,
    e_phnum,
    e_shentsize,
    e_shnum,
    e_shstrndx,
    __data_size
};

const FW_DEF::HEADER_RECORD records32[] = {
    {ei_mag, "ei_mag", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 0, 4, "uint32 LE", FW_DEF::VAL_TYPE_DATA},
    {ei_class, "ei_class", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 4, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_data, "ei_data", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 5, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_version, "ei_version", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 6, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_osabi, "ei_osabi", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 7, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_abiversion, "ei_abiversion", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 8, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_0, "ei_pad_0", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 9, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_1, "ei_pad_1", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 10, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_2, "ei_pad_2", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 11, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_3, "ei_pad_3", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 12, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_4, "ei_pad_4", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 13, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_5, "ei_pad_5", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 14, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_6, "ei_pad_6", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 15, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {e_type, "e_type", offsetof(XELF_DEF::Elf32_Ehdr, e_type), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_machine, "e_machine", offsetof(XELF_DEF::Elf32_Ehdr, e_machine), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_version, "e_version", offsetof(XELF_DEF::Elf32_Ehdr, e_version), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {e_entry, "e_entry", offsetof(XELF_DEF::Elf32_Ehdr, e_entry), 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {e_phoff, "e_phoff", offsetof(XELF_DEF::Elf32_Ehdr, e_phoff), 4, "Elf32_Off", FW_DEF::VAL_TYPE_OFFSET},
    {e_shoff, "e_shoff", offsetof(XELF_DEF::Elf32_Ehdr, e_shoff), 4, "Elf32_Off", FW_DEF::VAL_TYPE_OFFSET},
    {e_flags, "e_flags", offsetof(XELF_DEF::Elf32_Ehdr, e_flags), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {e_ehsize, "e_ehsize", offsetof(XELF_DEF::Elf32_Ehdr, e_ehsize), 2, "Elf32_Half", FW_DEF::VAL_TYPE_SIZE},
    {e_phentsize, "e_phentsize", offsetof(XELF_DEF::Elf32_Ehdr, e_phentsize), 2, "Elf32_Half", FW_DEF::VAL_TYPE_SIZE},
    {e_phnum, "e_phnum", offsetof(XELF_DEF::Elf32_Ehdr, e_phnum), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shentsize, "e_shentsize", offsetof(XELF_DEF::Elf32_Ehdr, e_shentsize), 2, "Elf32_Half", FW_DEF::VAL_TYPE_SIZE},
    {e_shnum, "e_shnum", offsetof(XELF_DEF::Elf32_Ehdr, e_shnum), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shstrndx, "e_shstrndx", offsetof(XELF_DEF::Elf32_Ehdr, e_shstrndx), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    };
const FW_DEF::HEADER_RECORD records64[] = {
    {ei_mag, "ei_mag", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 0, 4, "uint32 LE", FW_DEF::VAL_TYPE_DATA},
    {ei_class, "ei_class", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 4, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_data, "ei_data", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 5, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_version, "ei_version", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 6, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_osabi, "ei_osabi", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 7, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_abiversion, "ei_abiversion", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 8, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_0, "ei_pad_0", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 9, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_1, "ei_pad_1", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 10, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_2, "ei_pad_2", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 11, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_3, "ei_pad_3", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 12, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_4, "ei_pad_4", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 13, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_5, "ei_pad_5", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 14, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_6, "ei_pad_6", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 15, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {e_type, "e_type", offsetof(XELF_DEF::Elf64_Ehdr, e_type), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_machine, "e_machine", offsetof(XELF_DEF::Elf64_Ehdr, e_machine), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_version, "e_version", offsetof(XELF_DEF::Elf64_Ehdr, e_version), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {e_entry, "e_entry", offsetof(XELF_DEF::Elf64_Ehdr, e_entry), 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {e_phoff, "e_phoff", offsetof(XELF_DEF::Elf64_Ehdr, e_phoff), 8, "Elf64_Off", FW_DEF::VAL_TYPE_OFFSET},
    {e_shoff, "e_shoff", offsetof(XELF_DEF::Elf64_Ehdr, e_shoff), 8, "Elf64_Off", FW_DEF::VAL_TYPE_OFFSET},
    {e_flags, "e_flags", offsetof(XELF_DEF::Elf64_Ehdr, e_flags), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {e_ehsize, "e_ehsize", offsetof(XELF_DEF::Elf64_Ehdr, e_ehsize), 2, "Elf64_Half", FW_DEF::VAL_TYPE_SIZE},
    {e_phentsize, "e_phentsize", offsetof(XELF_DEF::Elf64_Ehdr, e_phentsize), 2, "Elf64_Half", FW_DEF::VAL_TYPE_SIZE},
    {e_phnum, "e_phnum", offsetof(XELF_DEF::Elf64_Ehdr, e_phnum), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shentsize, "e_shentsize", offsetof(XELF_DEF::Elf64_Ehdr, e_shentsize), 2, "Elf64_Half", FW_DEF::VAL_TYPE_SIZE},
    {e_shnum, "e_shnum", offsetof(XELF_DEF::Elf64_Ehdr, e_shnum), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shstrndx, "e_shstrndx", offsetof(XELF_DEF::Elf64_Ehdr, e_shstrndx), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    };
}

elf_ehdrWidget::elf_ehdrWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::elf_ehdrWidget)
{
    ui->setupUi(this);
}

elf_ehdrWidget::~elf_ehdrWidget()
{
    delete ui;
}

FormatWidget::SV elf_ehdrWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void elf_ehdrWidget::reloadData(bool bSaveSelection)
{
    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    cleanup();

    XELF elf(getCwOptions()->pDevice, getCwOptions()->bIsImage, getCwOptions()->nImageBase);

    if (elf.isValid()) {
        if (getCwOptions()->mode==XBinary::MODE_64) {
            createHeaderTable(ui->tableWidgetMain, _elf_ehdrWidget::records64, getListRecWidgets(), _elf_ehdrWidget::__data_size,
                              getCwOptions()->nDataOffset, getCwOptions()->endian);
        } else if (getCwOptions()->mode==XBinary::MODE_32){
            createHeaderTable(ui->tableWidgetMain, _elf_ehdrWidget::records32, getListRecWidgets(),
                              _elf_ehdrWidget::__data_size, getCwOptions()->nDataOffset, getCwOptions()->endian);
        }

        // adjustComboBox(getListRecWidgets(), XMACH::getHeaderMagicsS(), _mach_headerWidget::magic, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XMACH::getHeaderCpuTypesS(), _mach_headerWidget::cputype, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XMACH::getHeaderCpuSubTypesS(mach.getHeader_cputype()), _mach_headerWidget::cpusubtype, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XMACH::getHeaderFileTypesS(), _mach_headerWidget::filetype, XComboBoxEx::CBTYPE_LIST, 0);
        // adjustComboBox(getListRecWidgets(), XMACH::getHeaderFlagsS(), _mach_headerWidget::flags, XComboBoxEx::CBTYPE_FLAGS, 0);

        updateRecWidgets(getCwOptions()->pDevice, getListRecWidgets());
    }

    ui->tableWidgetMain->setCurrentCell(nCurrentRow, 0);
}

void elf_ehdrWidget::on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->tableWidgetMain);
}

