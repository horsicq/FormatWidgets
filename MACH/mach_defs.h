// copyright (c) 2019 hors<horsicq@gmail.com>
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
#ifndef MACH_DEFS_H
#define MACH_DEFS_H

#include "../formatwidget.h"
#include "xmach.h"

namespace SMACH
{
    enum TYPE
    {
        TYPE_HEX=0,
        TYPE_mach_header,
        TYPE_commands,
        __TYPE_size
    };
}

namespace N_mach_header
{
    enum DATA
    {
        magic=0,
        cputype,
        cpusubtype,
        filetype,
        ncmds,
        sizeofcmds,
        flags,
        reserved,
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records32[__data_size-1];
    extern const FormatWidget::HEADER_RECORD records64[__data_size];
}

namespace N_commands
{
    enum DATA
    {
        type=0,
        size,
        offset,
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records[__data_size];
}

#endif // MACH_DEFS_H
