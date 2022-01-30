// Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "ne_defs.h"

namespace N_NE_DOS_HEADER
{
const FW_DEF::HEADER_RECORD records[]=
{
    {e_magic,               "e_magic",          offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_magic),       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cblp,                "e_cblp",           offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cblp),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cp,                  "e_cp",             offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cp),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_crlc,                "e_crlc",           offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_crlc),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cparhdr,             "e_cparhdr",        offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cparhdr),     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_minalloc,            "e_minalloc",       offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_minalloc),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_maxalloc,            "e_maxalloc",       offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_maxalloc),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_ss,                  "e_ss",             offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_ss),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_sp,                  "e_sp",             offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_sp),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_csum,                "e_csum",           offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_csum),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_ip,                  "e_ip",             offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_ip),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cs,                  "e_cs",             offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cs),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_lfarlc,              "e_lfarlc",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_lfarlc),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_ovno,                "e_ovno",           offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_ovno),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res_0,               "e_res_0",          offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res)+0,       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res_1,               "e_res_1",          offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res)+2,       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res_2,               "e_res_2",          offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res)+4,       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res_3,               "e_res_3",          offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res)+6,       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_oemid,               "e_oemid",          offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_oemid),       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_oeminfo,             "e_oeminfo",        offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_oeminfo),     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_0,              "e_res2_0",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+0,      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_1,              "e_res2_1",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+2,      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_2,              "e_res2_2",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+4,      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_3,              "e_res2_3",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+6,      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_4,              "e_res2_4",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+8,      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_5,              "e_res2_5",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+10,     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_6,              "e_res2_6",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+12,     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_7,              "e_res2_7",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+14,     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_8,              "e_res2_8",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+16,     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_res2_9,              "e_res2_9",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_res2)+18,     2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_lfanew,              "e_lfanew",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_lfanew),      4,      "LONG",         FW_DEF::VAL_TYPE_OFFSET}
};
}

namespace N_OS2_HEADER
{
const FW_DEF::HEADER_RECORD records[]=
{
    {ne_magic,              "ne_magic",         offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_magic),           2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_ver,                "ne_ver",           offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_ver),             1,      "CHAR",         FW_DEF::VAL_TYPE_DATA},
    {ne_rev,                "ne_rev",           offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_rev),             1,      "CHAR",         FW_DEF::VAL_TYPE_DATA},
    {ne_enttab,             "ne_enttab",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_enttab),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_cbenttab,           "ne_cbenttab",      offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_cbenttab),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_crc,                "ne_crc",           offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_crc),             4,      "LONG",         FW_DEF::VAL_TYPE_DATA},
    {ne_flags,              "ne_flags",         offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_flags),           2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_autodata,           "ne_autodata",      offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_autodata),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_heap,               "ne_heap",          offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_heap),            2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_stack,              "ne_stack",         offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_stack),           2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_csip,               "ne_csip",          offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_csip),            4,      "LONG",         FW_DEF::VAL_TYPE_DATA},
    {ne_sssp,               "ne_sssp",          offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_sssp),            4,      "LONG",         FW_DEF::VAL_TYPE_DATA},
    {ne_cseg,               "ne_cseg",          offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_cseg),            2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_cmod,               "ne_cmod",          offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_cmod),            2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_cbnrestab,          "ne_cbnrestab",     offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_cbnrestab),       2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_segtab,             "ne_segtab",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_segtab),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_rsrctab,            "ne_rsrctab",       offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_rsrctab),         2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_restab,             "ne_restab",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_restab),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_modtab,             "ne_modtab",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_modtab),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_imptab,             "ne_imptab",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_imptab),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_nrestab,            "ne_nrestab",       offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_nrestab),         4,      "LONG",         FW_DEF::VAL_TYPE_DATA},
    {ne_cmovent,            "ne_cmovent",       offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_cmovent),         2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_align,              "ne_align",         offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_align),           2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_cres,               "ne_cres",          offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_cres),            2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_exetyp,             "ne_exetyp",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_exetyp),          1,      "BYTE",         FW_DEF::VAL_TYPE_DATA},
    {ne_flagsothers,        "ne_flagsothers",   offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_flagsothers),     1,      "BYTE",         FW_DEF::VAL_TYPE_DATA},
    {ne_pretthunks,         "ne_pretthunks",    offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_pretthunks),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_psegrefbytes,       "ne_psegrefbytes",  offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_psegrefbytes),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_swaparea,           "ne_swaparea",      offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_swaparea),        2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {ne_expver,             "ne_expver",        offsetof(XNE_DEF::IMAGE_OS2_HEADER,ne_expver),          2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_NE_SEGMENT
{
const FW_DEF::HEADER_RECORD records[]=
{
    {dwFileOffset,          "dwFileOffset",         offsetof(XNE_DEF::NE_SEGMENT,dwFileOffset),         2,      "WORD",         FW_DEF::VAL_TYPE_OFFSET},
    {dwFileSize,            "dwFileSize",           offsetof(XNE_DEF::NE_SEGMENT,dwFileSize),           2,      "WORD",         FW_DEF::VAL_TYPE_SIZE},
    {dwFlags,               "dwFlags",              offsetof(XNE_DEF::NE_SEGMENT,dwFlags),              2,      "WORD",         FW_DEF::VAL_TYPE_FLAGS},
    {dwMinAllocSize,        "dwMinAllocSize",       offsetof(XNE_DEF::NE_SEGMENT,dwMinAllocSize),       2,      "WORD",         FW_DEF::VAL_TYPE_SIZE},
};
}

