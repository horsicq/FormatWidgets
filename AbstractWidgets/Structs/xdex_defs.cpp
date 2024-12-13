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
#include "xdex_defs.h"

namespace XTYPE_DEX {
namespace X_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {magic, "magic", offsetof(XDEX_DEF::HEADER, magic), 4, "uint32 LE", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {version, "version", offsetof(XDEX_DEF::HEADER, version), 4, "uint32 LE", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {checksum, "checksum", offsetof(XDEX_DEF::HEADER, checksum), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {signature, "signature", offsetof(XDEX_DEF::HEADER, signature), 20, "array", XFW_DEF::VAL_TYPE_DATA_ARRAY, -1},
    {file_size, "file_size", offsetof(XDEX_DEF::HEADER, file_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {header_size, "header_size", offsetof(XDEX_DEF::HEADER, header_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {endian_tag, "endian_tag", offsetof(XDEX_DEF::HEADER, endian_tag), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {link_size, "link_size", offsetof(XDEX_DEF::HEADER, link_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, link_off},
    {link_off, "link_off", offsetof(XDEX_DEF::HEADER, link_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {map_off, "map_off", offsetof(XDEX_DEF::HEADER, map_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {string_ids_size, "string_ids_size", offsetof(XDEX_DEF::HEADER, string_ids_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, string_ids_off},
    {string_ids_off, "string_ids_off", offsetof(XDEX_DEF::HEADER, string_ids_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {type_ids_size, "type_ids_size", offsetof(XDEX_DEF::HEADER, type_ids_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, type_ids_off},
    {type_ids_off, "type_ids_off", offsetof(XDEX_DEF::HEADER, type_ids_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {proto_ids_size, "proto_ids_size", offsetof(XDEX_DEF::HEADER, proto_ids_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, proto_ids_off},
    {proto_ids_off, "proto_ids_off", offsetof(XDEX_DEF::HEADER, proto_ids_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {field_ids_size, "field_ids_size", offsetof(XDEX_DEF::HEADER, field_ids_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, field_ids_off},
    {field_ids_off, "field_ids_off", offsetof(XDEX_DEF::HEADER, field_ids_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {method_ids_size, "method_ids_size", offsetof(XDEX_DEF::HEADER, method_ids_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, method_ids_off},
    {method_ids_off, "method_ids_off", offsetof(XDEX_DEF::HEADER, method_ids_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {class_defs_size, "class_defs_size", offsetof(XDEX_DEF::HEADER, class_defs_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, class_defs_off},
    {class_defs_off, "class_defs_off", offsetof(XDEX_DEF::HEADER, class_defs_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {data_size, "data_size", offsetof(XDEX_DEF::HEADER, data_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, data_off},
    {data_off, "data_off", offsetof(XDEX_DEF::HEADER, data_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_DEX_MAP_ITEM {
const XFW_DEF::HEADER_RECORD records[] = {
    {type, QObject::tr("Type"), offsetof(XDEX_DEF::MAP_ITEM, nType), 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {count, QObject::tr("Count"), offsetof(XDEX_DEF::MAP_ITEM, nCount), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {offset, QObject::tr("Offset"), offsetof(XDEX_DEF::MAP_ITEM, nOffset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_DEX_STRING {
const XFW_DEF::HEADER_RECORD records[] = {
    {string, QObject::tr("String"), 0, 2, "string", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_DEX_STRING_ID {
const XFW_DEF::HEADER_RECORD records[] = {
    {string_data_off, "string_data_off", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_DEX_TYPE_ID {
const XFW_DEF::HEADER_RECORD records[] = {
    {descriptor_idx, "descriptor_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_DEX_PROTO_ID {
const XFW_DEF::HEADER_RECORD records[] = {
    {shorty_idx, "shorty_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {return_type_idx, "return_type_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {parameters_off, "parameters_off", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_DEX_FIELD_ID {
const XFW_DEF::HEADER_RECORD records[] = {
    {class_idx, "class_idx", 0, 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {type_idx, "type_idx", 0, 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {name_idx, "name_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_DEX_METHOD_ID {
const XFW_DEF::HEADER_RECORD records[] = {
    {class_idx, "class_idx", 0, 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {proto_idx, "proto_idx", 0, 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {name_idx, "name_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_DEX_CLASS_ITEM_DEF {
const XFW_DEF::HEADER_RECORD records[] = {
    {class_idx, "class_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {access_flags, "access_flags", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {superclass_idx, "superclass_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {interfaces_off, "interfaces_off", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {source_file_idx, "source_file_idx", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {annotations_off, "annotations_off", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {class_data_off, "class_data_off", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {static_values_off, "static_values_off", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}
}  // namespace XTYPE_DEX
