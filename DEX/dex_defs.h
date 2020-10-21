// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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
#ifndef DEX_DEFS_H
#define DEX_DEFS_H

#include "../formatwidget.h"
#include "xdex.h"

namespace SDEX
{
enum TYPE
{
    TYPE_HEX=0,
    TYPE_STRINGS,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_HEURISTICSCAN,
    TYPE_HEADER,
    TYPE_MAPITEMS,
    TYPE_STRING_ID_ITEM,
    TYPE_TYPE_ID_ITEM,
    TYPE_PROTO_ID_ITEM,
    TYPE_FIELD_ID_ITEM,
    TYPE_METHOD_ID_ITEM,
    TYPE_CLASS_DEF_ITEM,
    __TYPE_size
};
}

namespace N_DEX_HEADER
{
enum DATA
{
    magic=0,
    version,
    checksum,
    signature,
    file_size,
    header_size,
    endian_tag,
    link_size,
    link_off,
    map_off,
    string_ids_size,
    string_ids_off,
    type_ids_size,
    type_ids_off,
    proto_ids_size,
    proto_ids_off,
    field_ids_size,
    field_ids_off,
    method_ids_size,
    method_ids_off,
    class_defs_size,
    class_defs_off,
    data_size,
    data_off,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_MAP_ITEM
{
enum DATA
{
    type=0,
    count,
    offset,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_STRING
{
enum DATA
{
    string=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_STRING_ID
{
enum DATA
{
    string_data_off=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_TYPE_ID
{
enum DATA
{
    descriptor_idx=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_PROTO_ID
{
enum DATA
{
    shorty_idx=0,
    return_type_idx,
    parameters_off,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_FIELD_ID
{
enum DATA
{
    class_idx=0,
    type_idx,
    name_idx,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_METHOD_ID
{
enum DATA
{
    class_idx=0,
    proto_idx,
    name_idx,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_DEX_CLASS_ITEM_DEF
{
enum DATA
{
    class_idx=0,
    access_flags,
    superclass_idx,
    interfaces_off,
    source_file_idx,
    annotations_off,
    class_data_off,
    static_values_off,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

#endif // DEX_DEFS_H
