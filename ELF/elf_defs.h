#ifndef ELF_DEFS_H
#define ELF_DEFS_H

#include "../dialogformat.h"
#include "qelf.h"

namespace SELF
{
    enum TYPE
    {
        TYPE_Elf_Ehdr=0,
        __TYPE_size
    };
}

namespace N_Elf_Ehdr
{
    enum DATA
    {
        ei_mag_0=0,
        ei_mag_1,
        ei_mag_2,
        ei_mag_3,
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
        ei_pad_7,
        ei_pad_8,
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

    extern const DialogFormat::HEADER_RECORD records32[__data_size];
    extern const DialogFormat::HEADER_RECORD records64[__data_size];
}

#endif // ELF_DEFS_H
