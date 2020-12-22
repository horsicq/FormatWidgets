// copyright (c) 2020 hors<horsicq@gmail.com>
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
#ifndef MSDOS_DEFS_H
#define MSDOS_DEFS_H

#include "../formatwidget.h"
#include "xmsdos.h"

namespace SMSDOS
{
enum TYPE
{
    TYPE_HEX=0,
    TYPE_DISASM,
    TYPE_STRINGS,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_HEURISTICSCAN,
    TYPE_DOS_HEADER,
    TYPE_OVERLAY,
    __TYPE_size
};
}

namespace N_DOS_HEADER
{
enum DATA
{
    e_magic=0,
    e_cblp,
    e_cp,
    e_crlc,
    e_cparhdr,
    e_minalloc,
    e_maxalloc,
    e_ss,
    e_sp,
    e_csum,
    e_ip,
    e_cs,
    e_lfarlc,
    e_ovno,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

#endif // MSDOS_DEFS_H
