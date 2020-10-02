// copyright (c) 2019 hors<horsicq@gmail.com>
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
#ifndef NE_DEFS_H
#define NE_DEFS_H

#include "../formatwidget.h"
#include "xne.h"

namespace SNE
{
enum TYPE
{
    TYPE_HEX=0,
    TYPE_STRINGS,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_HEURISTICSCAN,
    TYPE_DOS_HEADER,
    TYPE_OS2_HEADER,
    TYPE_SEGMENTS,
    TYPE_OVERLAY,
    __TYPE_size
};
}

namespace N_NE_DOS_HEADER
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
    e_res_0,
    e_res_1,
    e_res_2,
    e_res_3,
    e_oemid,
    e_oeminfo,
    e_res2_0,
    e_res2_1,
    e_res2_2,
    e_res2_3,
    e_res2_4,
    e_res2_5,
    e_res2_6,
    e_res2_7,
    e_res2_8,
    e_res2_9,
    e_lfanew,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_OS2_HEADER
{
enum DATA
{
    ne_magic=0,
    ne_ver,
    ne_rev,
    ne_enttab,
    ne_cbenttab,
    ne_crc,
    ne_flags,
    ne_autodata,
    ne_heap,
    ne_stack,
    ne_csip,
    ne_sssp,
    ne_cseg,
    ne_cmod,
    ne_cbnrestab,
    ne_segtab,
    ne_rsrctab,
    ne_restab,
    ne_modtab,
    ne_imptab,
    ne_nrestab,
    ne_cmovent,
    ne_align,
    ne_cres,
    ne_exetyp,
    ne_flagsothers,
    ne_pretthunks,
    ne_psegrefbytes,
    ne_swaparea,
    ne_expver,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

#endif // NE_DEFS_H
