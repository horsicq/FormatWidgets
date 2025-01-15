/* Copyright (c) 2022-2025 hors<horsicq@gmail.com>
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
#include "xsevenzip_defs.h"

namespace XTYPE_7ZIP {
namespace X_SIGNATUREHEADER {
const XFW_DEF::HEADER_RECORD records[__data_size] = {
    {kSignature, "kSignature", offsetof(XSevenZip::SIGNATUREHEADER, kSignature), 6, "array", XFW_DEF::VAL_TYPE_DATA_ARRAY, -1},
    {Major, "Major", offsetof(XSevenZip::SIGNATUREHEADER, Major), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {Minor, "Minor", offsetof(XSevenZip::SIGNATUREHEADER, Minor), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {StartHeaderCRC, "StartHeaderCRC", offsetof(XSevenZip::SIGNATUREHEADER, StartHeaderCRC), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {NextHeaderOffset, "NextHeaderOffset", offsetof(XSevenZip::SIGNATUREHEADER, NextHeaderOffset), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADEREND, -1},
    {NextHeaderSize, "NextHeaderSize", offsetof(XSevenZip::SIGNATUREHEADER, NextHeaderSize), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, NextHeaderOffset},
    {NextHeaderCRC, "NextHeaderCRC", offsetof(XSevenZip::SIGNATUREHEADER, NextHeaderCRC), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_SIGNATUREHEADER
}  // namespace XTYPE_7ZIP

