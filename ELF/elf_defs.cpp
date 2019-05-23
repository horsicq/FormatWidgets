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

namespace N_Elf_Shdr
{
const FormatWidget::HEADER_RECORD records32[]=
{
    {sh_name,               "sh_name",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_type,               "sh_type",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_flags,              "sh_flags",             4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_addr,               "sh_addr",              4,      "Elf32_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
    {sh_offset,             "sh_offset",            4,      "Elf32_Off",            FormatWidget::VAL_TYPE_OFFSET},
    {sh_size,               "sh_size",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_SIZE},
    {sh_link,               "sh_link",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_info,               "sh_info",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_addralign,          "sh_addralign",         4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_entsize,            "sh_entsize",           4,      "Elf32_Word",           FormatWidget::VAL_TYPE_SIZE},
};
const FormatWidget::HEADER_RECORD records64[]=
{
    {sh_name,               "sh_name",              4,      "Elf64_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_type,               "sh_type",              4,      "Elf64_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_flags,              "sh_flags",             8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_DATA},
    {sh_addr,               "sh_addr",              8,      "Elf64_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
    {sh_offset,             "sh_offset",            8,      "Elf64_Off",            FormatWidget::VAL_TYPE_OFFSET},
    {sh_size,               "sh_size",              8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_SIZE},
    {sh_link,               "sh_link",              4,      "Elf64_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_info,               "sh_info",              4,      "Elf64_Word",           FormatWidget::VAL_TYPE_DATA},
    {sh_addralign,          "sh_addralign",         8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_DATA},
    {sh_entsize,            "sh_entsize",           8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_SIZE},
};
}

namespace N_Elf_Phdr32
{
const FormatWidget::HEADER_RECORD records[]=
{
    {p_type,                "p_type",               4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {p_offset,              "p_offset",             4,      "Elf32_Off",            FormatWidget::VAL_TYPE_OFFSET},
    {p_vaddr,               "p_vaddr",              4,      "Elf32_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
    {p_paddr,               "p_paddr",              4,      "Elf32_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
    {p_filesz,              "p_filesz",             4,      "Elf32_Word",           FormatWidget::VAL_TYPE_SIZE},
    {p_memsz,               "p_memsz",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_SIZE},
    {p_flags,               "p_flags",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {p_align,               "p_align",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
};
}

namespace N_Elf_Phdr64
{
const FormatWidget::HEADER_RECORD records[]=
{
    {p_type,                "p_type",               4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {p_flags,               "p_flags",              4,      "Elf32_Word",           FormatWidget::VAL_TYPE_DATA},
    {p_offset,              "p_offset",             8,      "Elf64_Off",            FormatWidget::VAL_TYPE_OFFSET},
    {p_vaddr,               "p_vaddr",              8,      "Elf64_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
    {p_paddr,               "p_paddr",              8,      "Elf64_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
    {p_filesz,              "p_filesz",             8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_SIZE},
    {p_memsz,               "p_memsz",              8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_SIZE},
    {p_align,               "p_align",              8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_DATA},
};
}

namespace N_Elf_DynamicArrayTags
{
const FormatWidget::HEADER_RECORD records32[]=
{
    {d_tag,                 "d_tag",                4,      "Elf32_SWord",          FormatWidget::VAL_TYPE_DATA},
    {d_value,               "d_value",              4,      "Elf32_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
};
const FormatWidget::HEADER_RECORD records64[]=
{
    {d_tag,                 "d_tag",                8,      "Elf64_Xword",          FormatWidget::VAL_TYPE_DATA},
    {d_value,               "d_value",              8,      "Elf64_Addr",           FormatWidget::VAL_TYPE_ADDRESS},
};
}

