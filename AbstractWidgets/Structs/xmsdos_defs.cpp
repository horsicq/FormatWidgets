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
#include "xmsdos_defs.h"

namespace XTYPE_MSDOS {

namespace X_Exe_file {
const XFW_DEF::HEADER_RECORD records[] = {{exe_signature, "exe_signature", offsetof(XMSDOS_DEF::EXE_file, exe_signature), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_len_mod_512, "exe_len_mod_512", offsetof(XMSDOS_DEF::EXE_file, exe_len_mod_512), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_pages, "exe_pages", offsetof(XMSDOS_DEF::EXE_file, exe_pages), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_rle_count, "exe_rle_count", offsetof(XMSDOS_DEF::EXE_file, exe_rle_count), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_par_dir, "exe_par_dir", offsetof(XMSDOS_DEF::EXE_file, exe_par_dir), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_min_BSS, "exe_min_BSS", offsetof(XMSDOS_DEF::EXE_file, exe_min_BSS), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_max_BSS, "exe_max_BSS", offsetof(XMSDOS_DEF::EXE_file, exe_max_BSS), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_SS, "exe_SS", offsetof(XMSDOS_DEF::EXE_file, exe_SS), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_SP, "exe_SP", offsetof(XMSDOS_DEF::EXE_file, exe_SP), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_chksum, "exe_chksum", offsetof(XMSDOS_DEF::EXE_file, exe_chksum), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_IP, "exe_IP", offsetof(XMSDOS_DEF::EXE_file, exe_IP), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_CS, "exe_CS", offsetof(XMSDOS_DEF::EXE_file, exe_CS), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_rle_table, "exe_rle_table", offsetof(XMSDOS_DEF::EXE_file, exe_rle_table), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_iov, "exe_iov", offsetof(XMSDOS_DEF::EXE_file, exe_iov), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
                                          {exe_sym_tab, "exe_sym_tab", offsetof(XMSDOS_DEF::EXE_file, exe_sym_tab), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT}};
}

namespace X_IMAGE_DOS_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {e_magic, "e_magic", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_magic), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_cblp, "e_cblp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cblp), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_cp, "e_cp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cp), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_crlc, "e_crlc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_crlc), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_cparhdr, "e_cparhdr", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cparhdr), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_minalloc, "e_minalloc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_minalloc), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_maxalloc, "e_maxalloc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_maxalloc), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_ss, "e_ss", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ss), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_sp, "e_sp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_sp), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_csum, "e_csum", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_csum), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_ip, "e_ip", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ip), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_cs, "e_cs", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cs), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_lfarlc, "e_lfarlc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_lfarlc), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_ovno, "e_ovno", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ovno), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_res, "e_res", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res), 8, "WORD[4]", XFW_DEF::VAL_TYPE_DATA_ARRAY},
    {e_oemid, "e_oemid", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_oemid), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_oeminfo, "e_oeminfo", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_oeminfo), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_res2, "e_res2", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2), 20, "WORD[10]", XFW_DEF::VAL_TYPE_DATA_ARRAY},
    {e_lfanew, "e_lfanew", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_lfanew), 4, "LONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET}};
}

} // namespace XTYPE_MSDOS
