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
#include "ne_defs.h"

namespace N_DOS_HEADER
{
const FormatWidget::HEADER_RECORD records[]=
{
    {e_magic,       "e_magic",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cblp,        "e_cblp",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cp,          "e_cp",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_crlc,        "e_crlc",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cparhdr,     "e_cparhdr",    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_minalloc,    "e_minalloc",   2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_maxalloc,    "e_maxalloc",   2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_ss,          "e_ss",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_sp,          "e_sp",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_csum,        "e_csum",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_ip,          "e_ip",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cs,          "e_cs",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_lfarlc,      "e_lfarlc",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_ovno,        "e_ovno",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_0,       "e_res_0",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_1,       "e_res_1",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_2,       "e_res_2",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_3,       "e_res_3",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_oemid,       "e_oemid",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_oeminfo,     "e_oeminfo",    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_0,      "e_res2_0",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_1,      "e_res2_1",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_2,      "e_res2_2",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_3,      "e_res2_3",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_4,      "e_res2_4",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_5,      "e_res2_5",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_6,      "e_res2_6",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_7,      "e_res2_7",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_8,      "e_res2_8",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_9,      "e_res2_9",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_lfanew,      "e_lfanew",     4,      "LONG",         FormatWidget::VAL_TYPE_OFFSET}
};
}

namespace N_OS2_HEADER
{
const FormatWidget::HEADER_RECORD records[]=
{
    {ne_magic,              "ne_magic",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_ver,                "ne_ver",           1,      "CHAR",         FormatWidget::VAL_TYPE_DATA},
    {ne_rev,                "ne_rev",           1,      "CHAR",         FormatWidget::VAL_TYPE_DATA},
    {ne_enttab,             "ne_enttab",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_cbenttab,           "ne_cbenttab",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_crc,                "ne_crc",           4,      "LONG",         FormatWidget::VAL_TYPE_DATA},
    {ne_flags,              "ne_flags",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_autodata,           "ne_autodata",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_heap,               "ne_heap",          2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_stack,              "ne_stack",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_csip,               "ne_csip",          4,      "LONG",         FormatWidget::VAL_TYPE_DATA},
    {ne_sssp,               "ne_sssp",          4,      "LONG",         FormatWidget::VAL_TYPE_DATA},
    {ne_cseg,               "ne_cseg",          2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_cmod,               "ne_cmod",          2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_cbnrestab,          "ne_cbnrestab",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_segtab,             "ne_segtab",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_rsrctab,            "ne_rsrctab",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_restab,             "ne_restab",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_modtab,             "ne_modtab",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_imptab,             "ne_imptab",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_nrestab,            "ne_nrestab",       4,      "LONG",         FormatWidget::VAL_TYPE_DATA},
    {ne_cmovent,            "ne_cmovent",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_align,              "ne_align",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_cres,               "ne_cres",          2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_exetyp,             "ne_exetyp",        1,      "BYTE",         FormatWidget::VAL_TYPE_DATA},
    {ne_flagsothers,        "ne_flagsothers",   1,      "BYTE",         FormatWidget::VAL_TYPE_DATA},
    {ne_pretthunks,         "ne_pretthunks",    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_psegrefbytes,       "ne_psegrefbytes",  2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_swaparea,           "ne_swaparea",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {ne_expver,             "ne_expver",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
};
}
