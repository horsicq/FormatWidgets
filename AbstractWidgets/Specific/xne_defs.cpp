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
#include "xne_defs.h"

namespace X_OS2_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {ne_magic, "ne_magic", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_magic), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_ver, "ne_ver", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_ver), 1, "CHAR", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_rev, "ne_rev", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_rev), 1, "CHAR", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_enttab, "ne_enttab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_enttab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_cbenttab, "ne_cbenttab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_cbenttab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_crc, "ne_crc", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_crc), 4, "LONG", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_flags, "ne_flags", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_flags), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_autodata, "ne_autodata", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_autodata), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_heap, "ne_heap", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_heap), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_stack, "ne_stack", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_stack), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_csip, "ne_csip", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_csip), 4, "LONG", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_sssp, "ne_sssp", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_sssp), 4, "LONG", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_cseg, "ne_cseg", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_cseg), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_cmod, "ne_cmod", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_cmod), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_cbnrestab, "ne_cbnrestab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_cbnrestab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_segtab, "ne_segtab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_segtab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_rsrctab, "ne_rsrctab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_rsrctab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_restab, "ne_restab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_restab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_modtab, "ne_modtab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_modtab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_imptab, "ne_imptab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_imptab), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_nrestab, "ne_nrestab", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_nrestab), 4, "LONG", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_cmovent, "ne_cmovent", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_cmovent), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_align, "ne_align", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_align), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_cres, "ne_cres", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_cres), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_exetyp, "ne_exetyp", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_exetyp), 1, "BYTE", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_flagsothers, "ne_flagsothers", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_flagsothers), 1, "BYTE", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_pretthunks, "ne_pretthunks", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_pretthunks), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_psegrefbytes, "ne_psegrefbytes", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_psegrefbytes), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_swaparea, "ne_swaparea", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_swaparea), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {ne_expver, "ne_expver", offsetof(XNE_DEF::IMAGE_OS2_HEADER, ne_expver), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
};
}

namespace X_NE_SEGMENT {
const XFW_DEF::HEADER_RECORD records[] = {
    {dwFileOffset, "dwFileOffset", offsetof(XNE_DEF::NE_SEGMENT, dwFileOffset), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {dwFileSize, "dwFileSize", offsetof(XNE_DEF::NE_SEGMENT, dwFileSize), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {dwFlags, "dwFlags", offsetof(XNE_DEF::NE_SEGMENT, dwFlags), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT},
    {dwMinAllocSize, "dwMinAllocSize", offsetof(XNE_DEF::NE_SEGMENT, dwMinAllocSize), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
};
}
