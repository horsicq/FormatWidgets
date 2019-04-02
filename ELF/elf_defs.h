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
#ifndef ELF_DEFS_H
#define ELF_DEFS_H

#include "../formatwidget.h"
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

    extern const FormatWidget::HEADER_RECORD records32[__data_size];
    extern const FormatWidget::HEADER_RECORD records64[__data_size];
}

#endif // ELF_DEFS_H
