/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#ifndef XNE_DEFS_H
#define XNE_DEFS_H

#include "../xformatwidget_def.h"

namespace X_OS2_HEADER {
enum DATA {
    ne_magic = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_OS2_HEADER

namespace X_NE_SEGMENT {
enum DATA {
    dwFileOffset = 0,
    dwFileSize,
    dwFlags,
    dwMinAllocSize,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_NE_SEGMENT

#endif  // XNE_DEFS_H
