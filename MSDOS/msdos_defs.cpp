// Copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#include "msdos_defs.h"

namespace N_DOS_HEADER
{
const FW_DEF::HEADER_RECORD records[]=
{
    {e_magic,       "e_magic",      offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_magic),   2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cblp,        "e_cblp",       offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cblp),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cp,          "e_cp",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cp),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_crlc,        "e_crlc",       offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_crlc),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cparhdr,     "e_cparhdr",    offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cparhdr), 2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_minalloc,    "e_minalloc",   offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_minalloc),2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_maxalloc,    "e_maxalloc",   offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_maxalloc),2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_ss,          "e_ss",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_ss),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_sp,          "e_sp",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_sp),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_csum,        "e_csum",       offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_csum),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_ip,          "e_ip",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_ip),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_cs,          "e_cs",         offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_cs),      2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_lfarlc,      "e_lfarlc",     offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_lfarlc),  2,      "WORD",         FW_DEF::VAL_TYPE_DATA},
    {e_ovno,        "e_ovno",       offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX,e_ovno),    2,      "WORD",         FW_DEF::VAL_TYPE_DATA}
};
}
