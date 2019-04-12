// copyright (c) 2017-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "elf_defs.h"

namespace N_Elf_Ehdr
{
    const FormatWidget::HEADER_RECORD records32[]=
    {
        {ei_mag_0,              "ei_mag_0",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_mag_1,              "ei_mag_1",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_mag_2,              "ei_mag_2",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_mag_3,              "ei_mag_3",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_class,              "ei_class",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_data,               "ei_data",              1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_version,            "ei_version",           1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_osabi,              "ei_osabi",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_abiversion,         "ei_abiversion",        1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_0,              "ei_pad_0",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_1,              "ei_pad_1",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_2,              "ei_pad_2",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_3,              "ei_pad_3",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_4,              "ei_pad_4",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_5,              "ei_pad_5",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_6,              "ei_pad_6",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_7,              "ei_pad_7",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_8,              "ei_pad_8",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {e_type,                "e_type",               2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_machine,             "e_machine",            2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_version,             "e_version",            4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
        {e_entry,               "e_entry",              4,      "Elf32_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
        {e_phoff,               "e_phoff",              4,      "Elf32_Off",            FormatWidget::VAL_TYPE_RELADDRESS},
        {e_shoff,               "e_shoff",              4,      "Elf32_Off",            FormatWidget::VAL_TYPE_RELADDRESS},
        {e_flags,               "e_flags",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
        {e_ehsize,              "e_ehsize",             2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_phentsize,           "e_phentsize",          2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_phnum,               "e_phnum",              2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_shentsize,           "e_shentsize",          2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_shnum,               "e_shnum",              2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_shstrndx,            "e_shstrndx",           2,      "Elf32_Half",           FormatWidget::VAL_TYPE_DATA},
    };
    const FormatWidget::HEADER_RECORD records64[]=
    {
        {ei_mag_0,              "ei_mag_0",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_mag_1,              "ei_mag_1",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_mag_2,              "ei_mag_2",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_mag_3,              "ei_mag_3",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_class,              "ei_class",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_data,               "ei_data",              1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_version,            "ei_version",           1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_osabi,              "ei_osabi",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_abiversion,         "ei_abiversion",        1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_0,              "ei_pad_0",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_1,              "ei_pad_1",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_2,              "ei_pad_2",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_3,              "ei_pad_3",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_4,              "ei_pad_4",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_5,              "ei_pad_5",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_6,              "ei_pad_6",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_7,              "ei_pad_7",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {ei_pad_8,              "ei_pad_8",             1,      "unsigned char",        FormatWidget::VAL_TYPE_DATA},
        {e_type,                "e_type",               2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_machine,             "e_machine",            2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_version,             "e_version",            4,      "Elf64_Word",           FormatWidget::VAL_TYPE_DATA},
        {e_entry,               "e_entry",              8,      "Elf64_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
        {e_phoff,               "e_phoff",              8,      "Elf64_Off",            FormatWidget::VAL_TYPE_RELADDRESS},
        {e_shoff,               "e_shoff",              8,      "Elf64_Off",            FormatWidget::VAL_TYPE_RELADDRESS},
        {e_flags,               "e_flags",              4,      "Elf64_Word",           FormatWidget::VAL_TYPE_DATA},
        {e_ehsize,              "e_ehsize",             2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_phentsize,           "e_phentsize",          2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_phnum,               "e_phnum",              2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_shentsize,           "e_shentsize",          2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_shnum,               "e_shnum",              2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
        {e_shstrndx,            "e_shstrndx",           2,      "Elf64_Half",           FormatWidget::VAL_TYPE_DATA},
    };
}
