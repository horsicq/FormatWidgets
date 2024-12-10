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
#ifndef XFORMATWIDGET_DEF_H
#define XFORMATWIDGET_DEF_H

#include <QString>
#include "xbinary.h"
#include "xshortcuts.h"
#include "xoptions.h"
#include "xinfodb.h"

namespace XFW_DEF {
struct OPTIONS {
    bool bIsImage;
    qint64 nImageBase;  // TODO default_const
    // TODO Last directory
    XBinary::FT fileType;
    bool bOffset;        // Show Offset title no Address TODO remove make auto if Address != nOffset
    QString sTitle;      // For dialogs
    bool bFilter;        // For Archive_widget
    bool bNoWindowOpen;  // For Archive_widget
};

enum TYPE {
    TYPE_GLOBALHEX = 0,
    TYPE_INFO,
    TYPE_VISUALIZATION,
    TYPE_VIRUSTOTAL,
    TYPE_HEX,
    TYPE_DISASM,
    TYPE_HASH,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_NFDSCAN,
    TYPE_EXTRACTOR,
    TYPE_SEARCH,
    TYPE_DIESCAN,
    TYPE_YARASCAN,
    TYPE_TOOLS,
    TYPE_GENERIC_STRINGTABLE_ANSI,
    TYPE_MSDOS_EXE_file,
    TYPE_MSDOS_IMAGE_DOS_HEADER,
    TYPE_Elf32_Ehdr,
    TYPE_Elf64_Ehdr,
    TYPE_mach_header,
    TYPE_mach_header_64,
    TYPE_load_command,
    TYPE_segment_command,
    TYPE_segment_command_64,
    TYPE_load_command_dylib,
    TYPE_load_command_rpath,
    TYPE_load_command_sub_umbrella,
    TYPE_load_command_sub_client,
    TYPE_load_command_sub_library,
    TYPE_load_command_symtab,
    TYPE_load_command_dysymtab,
    TYPE_load_command_segment_split_info,
    TYPE_load_command_atom_info,
    TYPE_load_command_function_starts,
    TYPE_load_command_dyld_exports_trie,
    TYPE_load_command_dyld_chained_fixups,
    TYPE_load_command_encryption_info,
    TYPE_load_command_dyld_info,
    TYPE_load_command_version_min,
    TYPE_load_command_uuid,
    TYPE_load_command_build_version,
    TYPE_load_command_main,
    TYPE_load_command_fileset_entry,
    TYPE_load_command_source_version,
    TYPE_load_command_dylinker,
    TYPE_load_command_data_in_code,
    TYPE_load_command_code_signature,
    TYPE_load_command_routines,
    TYPE_MACH_SymbolTable,
    TYPE_MACH_dyld_chained_fixups_header
};

enum WIDGETMODE {
    WIDGETMODE_UNKNOWN = 0,
    WIDGETMODE_HEADER,
    WIDGETMODE_TABLE,
    WIDGETMODE_DIALOG_HEADER
};

struct CWOPTIONS {
    TYPE _type;
    WIDGETMODE widgetMode;
    QWidget *pParent;
    XBinary::FT fileType;
    QIODevice *pDevice;
    bool bIsImage;
    qint64 nImageBase;
    XInfoDB *pXInfoDB;
    qint64 nDataOffset;  // TODO location
    qint64 nDataSize;
    qint64 nDataCount;
    QVariant var1;
    QVariant var2;
    XBinary::ENDIAN endian;
    XBinary::MODE mode;
};

enum VAL_TYPE {
    VAL_TYPE_UNKNOWN_ = 0,
    VAL_TYPE_DATA_INT = 1 << 0,
    VAL_TYPE_DATA_ARRAY = 1 << 1,
    VAL_TYPE_RELADDRESS_ = 1 << 2,
    VAL_TYPE_ADDRESS_ = 1 << 3,
    VAL_TYPE_OFFSET_ = 1 << 4,
    VAL_TYPE_SIZE = 1 << 5,
    VAL_TYPE_ANSI = 1 << 6,
    VAL_TYPE_UUID_ = 1 << 7,
    VAL_TYPE_COUNT = 1 << 8,
    VAL_TYPE_HEX = 1 << 9,
    VAL_TYPE_STRING = 1 << 10,
};

struct HEADER_RECORD {
    qint32 nPosition;
    QString sName;
    qint32 nOffset;
    qint32 nSize;
    QString sType;
    qint32 vtype;
};

enum TABLEDATA {
    TABLEDATA_TYPE = 0,
    TABLEDATA_HEADEROFFSET,
    TABLEDATA_HEADERSIZE,
    TABLEDATA_DATAOFFSET,
    TABLEDATA_DATASIZE,
    TABLEDATA_DATACOUNT
};

enum SECTION_DATA {
    SECTION_DATA_TYPE = 0,
    SECTION_DATA_NUMBER,
    SECTION_DATA_NAME,
    SECTION_DATA_OFFSET,
    SECTION_DATA_SIZE,
    SECTION_DATA_ADDRESS,
    //        SECTION_DATA_RELADDRESS,
    SECTION_DATA_VSIZE,
    SECTION_DATA_EXTRAOFFSET,
    SECTION_DATA_EXTRASIZE,
    SECTION_DATA_STRINGTABLEOFFSET,
    SECTION_DATA_STRINGTABLESIZE,
    SECTION_DATA_HEADEROFFSET,
    SECTION_DATA_VALUE1,
    SECTION_DATA_VALUE2,
    SECTION_DATA_VALUE3
};

enum WIDGET_DATA {
    WIDGET_DATA_TYPE = 0,
    WIDGET_DATA_WIDGETMODE,
    WIDGET_DATA_OFFSET,
    WIDGET_DATA_SIZE,
    WIDGET_DATA_COUNT,
    WIDGET_DATA_MODE,
    WIDGET_DATA_ENDIAN,
    WIDGET_DATA_VAR1,
    WIDGET_DATA_VAR2,
    WIDGET_DATA_NAME,
};
}  // namespace XFW_DEF

#endif  // XFORMATWIDGET_DEF_H
