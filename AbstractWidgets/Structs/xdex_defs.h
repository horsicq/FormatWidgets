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
#ifndef XDEX_DEFS_H
#define XDEX_DEFS_H

#include "../xformatwidget_def.h"
namespace XTYPE_DEX {
namespace X_HEADER {
enum DATA {
    magic = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_HEADER

namespace X_DEX_MAP_ITEM {
enum DATA {
    type = 0,
    count,
    offset,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_MAP_ITEM

namespace X_DEX_STRING {
enum DATA {
    string = 0,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_STRING

namespace X_DEX_STRING_ID {
enum DATA {
    string_data_off = 0,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_STRING_ID

namespace X_DEX_TYPE_ID {
enum DATA {
    descriptor_idx = 0,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_TYPE_ID

namespace X_DEX_PROTO_ID {
enum DATA {
    shorty_idx = 0,
    return_type_idx,
    parameters_off,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_PROTO_ID

namespace X_DEX_FIELD_ID {
enum DATA {
    class_idx = 0,
    type_idx,
    name_idx,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_FIELD_ID

namespace X_DEX_METHOD_ID {
enum DATA {
    class_idx = 0,
    proto_idx,
    name_idx,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_METHOD_ID

namespace X_DEX_CLASS_ITEM_DEF {
enum DATA {
    class_idx = 0,
    access_flags,
    superclass_idx,
    interfaces_off,
    source_file_idx,
    annotations_off,
    class_data_off,
    static_values_off,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_DEX_CLASS_ITEM_DEF
}  // namespace XTYPE_DEX
#endif  // XDEX_DEFS_H
