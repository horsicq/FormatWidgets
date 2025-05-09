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
#include "dex_defs.h"

namespace N_DEX_HEADER {
const FW_DEF::HEADER_RECORD records[] = {
    {magic, "magic", offsetof(XDEX_DEF::HEADER, magic), 4, "uint32 LE", FW_DEF::VAL_TYPE_DATA},
    {version, "version", offsetof(XDEX_DEF::HEADER, version), 4, "uint32 LE", FW_DEF::VAL_TYPE_DATA},
    {checksum, "checksum", offsetof(XDEX_DEF::HEADER, checksum), 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {signature, "signature", offsetof(XDEX_DEF::HEADER, signature), 20, "array", FW_DEF::VAL_TYPE_DATA},
    {file_size, "file_size", offsetof(XDEX_DEF::HEADER, file_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {header_size, "header_size", offsetof(XDEX_DEF::HEADER, header_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {endian_tag, "endian_tag", offsetof(XDEX_DEF::HEADER, endian_tag), 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {link_size, "link_size", offsetof(XDEX_DEF::HEADER, link_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {link_off, "link_off", offsetof(XDEX_DEF::HEADER, link_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {map_off, "map_off", offsetof(XDEX_DEF::HEADER, map_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {string_ids_size, "string_ids_size", offsetof(XDEX_DEF::HEADER, string_ids_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {string_ids_off, "string_ids_off", offsetof(XDEX_DEF::HEADER, string_ids_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {type_ids_size, "type_ids_size", offsetof(XDEX_DEF::HEADER, type_ids_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {type_ids_off, "type_ids_off", offsetof(XDEX_DEF::HEADER, type_ids_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {proto_ids_size, "proto_ids_size", offsetof(XDEX_DEF::HEADER, proto_ids_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {proto_ids_off, "proto_ids_off", offsetof(XDEX_DEF::HEADER, proto_ids_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {field_ids_size, "field_ids_size", offsetof(XDEX_DEF::HEADER, field_ids_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {field_ids_off, "field_ids_off", offsetof(XDEX_DEF::HEADER, field_ids_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {method_ids_size, "method_ids_size", offsetof(XDEX_DEF::HEADER, method_ids_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {method_ids_off, "method_ids_off", offsetof(XDEX_DEF::HEADER, method_ids_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {class_defs_size, "class_defs_size", offsetof(XDEX_DEF::HEADER, class_defs_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {class_defs_off, "class_defs_off", offsetof(XDEX_DEF::HEADER, class_defs_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {data_size, "data_size", offsetof(XDEX_DEF::HEADER, data_size), 4, "uint32", FW_DEF::VAL_TYPE_SIZE},
    {data_off, "data_off", offsetof(XDEX_DEF::HEADER, data_off), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
};
}

namespace N_DEX_MAP_ITEM {
const FW_DEF::HEADER_RECORD records[] = {
    {type, QObject::tr("Type"), offsetof(XDEX_DEF::MAP_ITEM, nType), 2, "uint16", FW_DEF::VAL_TYPE_DATA},
    {count, QObject::tr("Count"), offsetof(XDEX_DEF::MAP_ITEM, nCount), 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {offset, QObject::tr("Offset"), offsetof(XDEX_DEF::MAP_ITEM, nOffset), 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
};
}

namespace N_DEX_STRING {
const FW_DEF::HEADER_RECORD records[] = {
    {string, QObject::tr("String"), 0, 2, "string", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_DEX_STRING_ID {
const FW_DEF::HEADER_RECORD records[] = {
    {string_data_off, "string_data_off", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_DEX_TYPE_ID {
const FW_DEF::HEADER_RECORD records[] = {
    {descriptor_idx, "descriptor_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_DEX_PROTO_ID {
const FW_DEF::HEADER_RECORD records[] = {
    {shorty_idx, "shorty_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {return_type_idx, "return_type_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {parameters_off, "parameters_off", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
};
}

namespace N_DEX_FIELD_ID {
const FW_DEF::HEADER_RECORD records[] = {
    {class_idx, "class_idx", 0, 2, "uint16", FW_DEF::VAL_TYPE_DATA},
    {type_idx, "type_idx", 0, 2, "uint16", FW_DEF::VAL_TYPE_DATA},
    {name_idx, "name_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
};
}

namespace N_DEX_METHOD_ID {
const FW_DEF::HEADER_RECORD records[] = {
    {class_idx, "class_idx", 0, 2, "uint16", FW_DEF::VAL_TYPE_DATA},
    {proto_idx, "proto_idx", 0, 2, "uint16", FW_DEF::VAL_TYPE_DATA},
    {name_idx, "name_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
};
}

namespace N_DEX_CLASS_ITEM_DEF {
const FW_DEF::HEADER_RECORD records[] = {
    {class_idx, "class_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {access_flags, "access_flags", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {superclass_idx, "superclass_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {interfaces_off, "interfaces_off", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {source_file_idx, "source_file_idx", 0, 4, "uint32", FW_DEF::VAL_TYPE_DATA},
    {annotations_off, "annotations_off", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {class_data_off, "class_data_off", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
    {static_values_off, "static_values_off", 0, 4, "uint32", FW_DEF::VAL_TYPE_OFFSET},
};
}
