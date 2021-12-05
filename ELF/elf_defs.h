/* Copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#ifndef ELF_DEFS_H
#define ELF_DEFS_H

#include "../formatwidget.h"
#include "xelf.h"

namespace SELF
{
enum TYPE
{
    TYPE_HEX=0,
    TYPE_DISASM,
    TYPE_HASH,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_HEURISTICSCAN,
    TYPE_Elf_Ehdr,
    TYPE_Elf_Shdr,
    TYPE_Elf_Phdr,
    TYPE_Elf_DynamicArrayTags,
    TYPE_LIBRARIES,
    TYPE_INTERPRETER,
    TYPE_NOTES,
    TYPE_RUNPATH,
    TYPE_STRINGTABLE,
    TYPE_SYMBOLTABLE,
    TYPE_Elf_Rela,
    TYPE_Elf_Rel,
    __TYPE_size
};
}

namespace N_Elf_Ehdr
{
enum DATA
{
    ei_mag=0,
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

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_Elf_Shdr
{
enum DATA
{
    sh_name=0,
    sh_type,
    sh_flags,
    sh_addr,
    sh_offset,
    sh_size,
    sh_link,
    sh_info,
    sh_addralign,
    sh_entsize,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];

enum CB
{
    CB_TYPE=0,
    CB_FLAGS,
    __CB_size
};

enum INV
{
    INV_VirtualAddress,
    INV_PointerToRawData,
    __INV_size
};
}

namespace N_Elf_Phdr32
{
enum DATA
{
    p_type=0,
    p_offset,
    p_vaddr,
    p_paddr,
    p_filesz,
    p_memsz,
    p_flags,
    p_align,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_TYPE=0,
    CB_FLAGS,
    __CB_size
};

enum INV
{
    INV_VirtualAddress,
    INV_PointerToRawData,
    __INV_size
};
}

namespace N_Elf_Phdr64
{
enum DATA
{
    p_type=0,
    p_flags,
    p_offset,
    p_vaddr,
    p_paddr,
    p_filesz,
    p_memsz,
    p_align,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_TYPE=0,
    CB_FLAGS,
    __CB_size
};
}

namespace N_Elf_DynamicArrayTags
{
enum DATA
{
    d_tag=0,
    d_value,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];

enum CB
{
    CB_TAG=0,
    __CB_size
};

enum INV
{
    INV_VirtualAddress,
    INV_PointerToRawData,
    __INV_size
};
}

namespace N_ELF_LIBRARIES
{
enum DATA
{
    library_name=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_ELF_INTERPRETER
{
enum DATA
{
    interpreter=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_ELF_NOTES
{
enum DATA
{
    type=0,
    name,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_ELF_RUNPATH
{
enum DATA
{
    runpath=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_Elf32_Sym
{
enum DATA
{
    st_name=0,
    st_value,
    st_size,
    st_info,
    st_other,
    st_shndx,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    __CB_size
};

enum INV
{
    __INV_size
};
}

namespace N_Elf64_Sym
{
enum DATA
{
    st_name,
    st_info,
    st_other,
    st_shndx,
    st_value,
    st_size,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_Elf_Rela
{
enum DATA
{
    r_offset,
    r_info,
    r_addend,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];

enum CB
{
    __CB_size
};

enum INV
{
    __INV_size
};
}

namespace N_Elf_Rel
{
enum DATA
{
    r_offset,
    r_info,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];

enum CB
{
    __CB_size
};

enum INV
{
    __INV_size
};
}

#endif // ELF_DEFS_H
