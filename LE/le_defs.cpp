/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "le_defs.h"

namespace N_LE_DOS_HEADER {
const FW_DEF::HEADER_RECORD records[] = {{e_magic, "e_magic", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_magic), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cblp, "e_cblp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cblp), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cp, "e_cp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cp), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_crlc, "e_crlc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_crlc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cparhdr, "e_cparhdr", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cparhdr), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_minalloc, "e_minalloc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_minalloc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_maxalloc, "e_maxalloc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_maxalloc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_ss, "e_ss", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ss), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_sp, "e_sp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_sp), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_csum, "e_csum", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_csum), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_ip, "e_ip", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ip), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cs, "e_cs", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cs), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_lfarlc, "e_lfarlc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_lfarlc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_ovno, "e_ovno", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ovno), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_0, "e_res_0", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_1, "e_res_1", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 2, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_2, "e_res_2", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 4, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_3, "e_res_3", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 6, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_oemid, "e_oemid", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_oemid), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_oeminfo, "e_oeminfo", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_oeminfo), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_0, "e_res2_0", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_1, "e_res2_1", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 2, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_2, "e_res2_2", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 4, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_3, "e_res2_3", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 6, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_4, "e_res2_4", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 8, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_5, "e_res2_5", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 10, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_6, "e_res2_6", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 12, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_7, "e_res2_7", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 14, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_8, "e_res2_8", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 16, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_9, "e_res2_9", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 18, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_lfanew, "e_lfanew", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_lfanew), 4, "LONG", FW_DEF::VAL_TYPE_OFFSET}};
}

namespace N_VXD_HEADER {
const FW_DEF::HEADER_RECORD records[] = {
    {e32_magic, "e32_magic", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_magic), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {e32_border, "e32_border", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_border), 1, "BYTE", FW_DEF::VAL_TYPE_DATA},
    {e32_worder, "e32_worder", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_worder), 1, "BYTE", FW_DEF::VAL_TYPE_DATA},
    {e32_level, "e32_level", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_level), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_cpu, "e32_cpu", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_cpu), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {e32_os, "e32_os", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_os), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {e32_ver, "e32_ver", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_ver), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_mflags, "e32_mflags", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_mflags), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_mpages, "e32_mpages", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_mpages), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_startobj, "e32_startobj", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_startobj), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_eip, "e32_eip", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_eip), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_stackobj, "e32_stackobj", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_stackobj), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_esp, "e32_esp", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_esp), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_pagesize, "e32_pagesize", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_pagesize), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_lastpagesize, "e32_lastpagesize", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_lastpagesize), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_fixupsize, "e32_fixupsize", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_fixupsize), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_fixupsum, "e32_fixupsum", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_fixupsum), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_ldrsize, "e32_ldrsize", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_ldrsize), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_ldrsum, "e32_ldrsum", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_ldrsum), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_objtab, "e32_objtab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_objtab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_objcnt, "e32_objcnt", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_objcnt), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_objmap, "e32_objmap", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_objmap), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_itermap, "e32_itermap", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_itermap), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_rsrctab, "e32_rsrctab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_rsrctab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_rsrccnt, "e32_rsrccnt", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_rsrccnt), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_restab, "e32_restab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_restab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_enttab, "e32_enttab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_enttab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_dirtab, "e32_dirtab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_dirtab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_dircnt, "e32_dircnt", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_dircnt), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_fpagetab, "e32_fpagetab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_fpagetab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_frectab, "e32_frectab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_frectab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_impmod, "e32_impmod", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_impmod), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_impmodcnt, "e32_impmodcnt", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_impmodcnt), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_impproc, "e32_impproc", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_impproc), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_pagesum, "e32_pagesum", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_pagesum), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_datapage, "e32_datapage", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_datapage), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_preload, "e32_preload", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_preload), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_nrestab, "e32_nrestab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_nrestab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_cbnrestab, "e32_cbnrestab", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_cbnrestab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_nressum, "e32_nressum", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_nressum), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_autodata, "e32_autodata", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_autodata), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_debuginfo, "e32_debuginfo", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_debuginfo), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_debuglen, "e32_debuglen", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_debuglen), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_instpreload, "e32_instpreload", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_instpreload), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_instdemand, "e32_instdemand", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_instdemand), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {e32_heapsize, "e32_heapsize", offsetof(XLE_DEF::IMAGE_VXD_HEADER, e32_heapsize), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_O32_OBJ {
const FW_DEF::HEADER_RECORD records[] = {
    {o32_size, "o32_size", offsetof(XLE_DEF::o32_obj, o32_size), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {o32_base, "o32_base", offsetof(XLE_DEF::o32_obj, o32_base), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {o32_flags, "o32_flags", offsetof(XLE_DEF::o32_obj, o32_flags), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS},
    {o32_pagemap, "o32_pagemap", offsetof(XLE_DEF::o32_obj, o32_pagemap), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {o32_mapsize, "o32_mapsize", offsetof(XLE_DEF::o32_obj, o32_mapsize), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {o32_reserved, "o32_reserved", offsetof(XLE_DEF::o32_obj, o32_reserved), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}  // namespace N_O32_OBJ
