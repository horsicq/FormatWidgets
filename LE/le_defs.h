/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef LE_DEFS_H
#define LE_DEFS_H

#include "../formatwidget.h"
#include "xle.h"

namespace SLE {
enum TYPE {
    TYPE_INFO = 0,
    TYPE_VISUALIZATION,
    TYPE_VIRUSTOTAL,
    TYPE_HEX,
    TYPE_DISASM,
    TYPE_HASH,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_NFDSCAN,
    TYPE_EXTRACTOR,
    TYPE_SEARCH,
    TYPE_DIESCAN,
    TYPE_YARASCAN,
    TYPE_DOS_HEADER,
    TYPE_VXD_HEADER,
    TYPE_OBJECTS,
    TYPE_OVERLAY,
    __TYPE_size
};
}  // namespace SLE

namespace N_LE_DOS_HEADER {
enum DATA {
    e_magic = 0,
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
}  // namespace N_LE_DOS_HEADER

namespace N_VXD_HEADER {
enum DATA {
    e32_magic = 0,
    e32_border,
    e32_worder,
    e32_level,
    e32_cpu,
    e32_os,
    e32_ver,
    e32_mflags,
    e32_mpages,
    e32_startobj,
    e32_eip,
    e32_stackobj,
    e32_esp,
    e32_pagesize,
    e32_lastpagesize,
    e32_fixupsize,
    e32_fixupsum,
    e32_ldrsize,
    e32_ldrsum,
    e32_objtab,
    e32_objcnt,
    e32_objmap,
    e32_itermap,
    e32_rsrctab,
    e32_rsrccnt,
    e32_restab,
    e32_enttab,
    e32_dirtab,
    e32_dircnt,
    e32_fpagetab,
    e32_frectab,
    e32_impmod,
    e32_impmodcnt,
    e32_impproc,
    e32_pagesum,
    e32_datapage,
    e32_preload,
    e32_nrestab,
    e32_cbnrestab,
    e32_nressum,
    e32_autodata,
    e32_debuginfo,
    e32_debuglen,
    e32_instpreload,
    e32_instdemand,
    e32_heapsize,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_VXD_HEADER

namespace N_O32_OBJ {
enum DATA {
    o32_size = 0,
    o32_base,
    o32_flags,
    o32_pagemap,
    o32_mapsize,
    o32_reserved,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_O32_OBJ

#endif  // LE_DEFS_H
