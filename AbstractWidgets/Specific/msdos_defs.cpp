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
#include "msdos_defs.h"

namespace N_Exe_file {
const FW_DEF::HEADER_RECORD records[] = {{exe_signature, "exe_signature", offsetof(XMSDOS_DEF::EXE_file, exe_signature), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_len_mod_512, "exe_len_mod_512", offsetof(XMSDOS_DEF::EXE_file, exe_len_mod_512), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_pages, "exe_pages", offsetof(XMSDOS_DEF::EXE_file, exe_pages), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_rle_count, "exe_rle_count", offsetof(XMSDOS_DEF::EXE_file, exe_rle_count), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_par_dir, "exe_par_dir", offsetof(XMSDOS_DEF::EXE_file, exe_par_dir), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_min_BSS, "exe_min_BSS", offsetof(XMSDOS_DEF::EXE_file, exe_min_BSS), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_max_BSS, "exe_max_BSS", offsetof(XMSDOS_DEF::EXE_file, exe_max_BSS), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_SS, "exe_SS", offsetof(XMSDOS_DEF::EXE_file, exe_SS), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_SP, "exe_SP", offsetof(XMSDOS_DEF::EXE_file, exe_SP), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_chksum, "exe_chksum", offsetof(XMSDOS_DEF::EXE_file, exe_chksum), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_IP, "exe_IP", offsetof(XMSDOS_DEF::EXE_file, exe_IP), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_CS, "exe_CS", offsetof(XMSDOS_DEF::EXE_file, exe_CS), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_rle_table, "exe_rle_table", offsetof(XMSDOS_DEF::EXE_file, exe_rle_table), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_iov, "exe_iov", offsetof(XMSDOS_DEF::EXE_file, exe_iov), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {exe_sym_tab, "exe_sym_tab", offsetof(XMSDOS_DEF::EXE_file, exe_sym_tab), 4, "DWORD", FW_DEF::VAL_TYPE_DATA}};
}
