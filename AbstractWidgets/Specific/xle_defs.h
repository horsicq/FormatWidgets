/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#ifndef XLE_DEFS_H
#define XLE_DEFS_H

#include "../xformatwidget_def.h"

namespace X_VXD_HEADER {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_VXD_HEADER

namespace X_O32_OBJ {
enum DATA {
    o32_size = 0,
    o32_base,
    o32_flags,
    o32_pagemap,
    o32_mapsize,
    o32_reserved,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_O32_OBJ

#endif  // XLE_DEFS_H
