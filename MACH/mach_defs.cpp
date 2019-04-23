// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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
#include "mach_defs.h"

namespace N_mach_header
{
    const FormatWidget::HEADER_RECORD records32[]=
    {
        {magic,                 "magic",             4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {cputype,               "cputype",           4,      "cpu_type_t",          FormatWidget::VAL_TYPE_DATA},
        {cpusubtype,            "cpusubtype",        4,      "cpu_subtype_t",       FormatWidget::VAL_TYPE_DATA},
        {filetype,              "filetype",          4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {ncmds,                 "ncmds",             4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {sizeofcmds,            "sizeofcmds",        4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {flags,                 "flags",             4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
    };

    const FormatWidget::HEADER_RECORD records64[]=
    {
        {magic,                 "magic",             4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {cputype,               "cputype",           4,      "cpu_type_t",          FormatWidget::VAL_TYPE_DATA},
        {cpusubtype,            "cpusubtype",        4,      "cpu_subtype_t",       FormatWidget::VAL_TYPE_DATA},
        {filetype,              "filetype",          4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {ncmds,                 "ncmds",             4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {sizeofcmds,            "sizeofcmds",        4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {flags,                 "flags",             4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
        {reserved,              "reserved",          4,      "uint32_t",            FormatWidget::VAL_TYPE_DATA},
    };
}

namespace N_commands
{
    const FormatWidget::HEADER_RECORD records[]=
    {
        {type,                  "type",             2,      "uint32_t",             FormatWidget::VAL_TYPE_DATA},
        {size,                  "size",             4,      "uint32_t",             FormatWidget::VAL_TYPE_SIZE},
        {offset,                "offset",           8,      "",                     FormatWidget::VAL_TYPE_OFFSET}
    };
}
