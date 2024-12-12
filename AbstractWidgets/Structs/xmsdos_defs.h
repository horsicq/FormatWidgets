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
#ifndef XMSDOS_DEFS_H
#define XMSDOS_DEFS_H

#include "../xformatwidget_def.h"
namespace XTYPE_MSDOS {

namespace X_Exe_file {
enum DATA {
    exe_signature = 0,
    exe_len_mod_512,
    exe_pages,
    exe_rle_count,
    exe_par_dir,
    exe_min_BSS,
    exe_max_BSS,
    exe_SS,
    exe_SP,
    exe_chksum,
    exe_IP,
    exe_CS,
    exe_rle_table,
    exe_iov,
    exe_sym_tab,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_Exe_file

namespace X_IMAGE_DOS_HEADER {
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
    e_res,
    e_oemid,
    e_oeminfo,
    e_res2,
    e_lfanew,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_DOS_HEADER

} // namespace XTYPE_MSDOS

#endif  // XMSDOS_DEFS_H
