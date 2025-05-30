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
#ifndef XFORMATWIDGET_DEF_H
#define XFORMATWIDGET_DEF_H

#include <QString>
#include "xbinary.h"
#include "xshortcuts.h"
#include "xoptions.h"
#include "xinfodb.h"
#include "xdemangle.h"

namespace XFW_DEF {
enum VMODE {
    VMODE_FILETYPE = 0,
    VMODE_STRUCT
};

enum TYPE {
    TYPE_UNKNOWN = 0,
    TYPE_GLOBALHEX,
    TYPE_INFO,
    TYPE_VISUALIZATION,
    TYPE_VIRUSTOTAL,
    TYPE_HEX,
    TYPE_DISASM,
    TYPE_HASH,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_REGIONS,
    TYPE_MEMORYMAP,
    TYPE_SYMBOLS,
    TYPE_ENTROPY,
    TYPE_NFDSCAN,
    TYPE_EXTRACTOR,
    TYPE_SEARCH,
    TYPE_DIESCAN,
    TYPE_YARASCAN,
    TYPE_TOOLS,
    TYPE_GENERIC_STRINGTABLE_ANSI,
    TYPE_GENERIC_RESOURCES,
    TYPE_GENERIC_TABLE,
    TYPE_MSDOS_START,
    TYPE_MSDOS_EXE_file,
    TYPE_MSDOS_IMAGE_DOS_HEADER,
    TYPE_MSDOS_END,
    TYPE_ELF_START,
    TYPE_Elf32_Ehdr,
    TYPE_Elf64_Ehdr,
    TYPE_ELF_END,
    TYPE_MACH_START,
    TYPE_MACH_mach_header,
    TYPE_MACH_mach_header_64,
    TYPE_MACH_load_command,
    TYPE_MACH_segment_command,
    TYPE_MACH_segment_command_64,
    TYPE_MACH_dylib_command,
    TYPE_MACH_rpath_command,
    TYPE_MACH_sub_umbrella_command,
    TYPE_MACH_sub_client_command,
    TYPE_MACH_sub_library_command,
    TYPE_MACH_symtab_command,
    TYPE_MACH_dysymtab_command,
    TYPE_MACH_segment_split_info_command,
    TYPE_MACH_atom_info_command,
    TYPE_MACH_function_starts_command,
    TYPE_MACH_dyld_exports_trie_command,
    TYPE_MACH_dyld_chained_fixups_command,
    TYPE_MACH_encryption_info_command,
    TYPE_MACH_encryption_info_command_64,
    TYPE_MACH_dyld_info_command,
    TYPE_MACH_version_min_command,
    TYPE_MACH_uuid_command,
    TYPE_MACH_build_version_command,
    TYPE_MACH_main_command,
    TYPE_MACH_fileset_entry_command,
    TYPE_MACH_source_version_command,
    TYPE_MACH_dylinker_command,
    TYPE_MACH_data_in_code_command,
    TYPE_MACH_code_signature_command,
    TYPE_MACH_routines_command,
    TYPE_MACH_routines_command_64,
    TYPE_MACH_fvm_library_command,
    TYPE_MACH_unix_thread_command,
    TYPE_MACH_twolevel_hints_command,
    TYPE_MACH_twolevel_hint,
    TYPE_MACH_build_tool_version,
    TYPE_MACH_x86_thread_state32_t,
    TYPE_MACH_x86_thread_state64_t,
    TYPE_MACH_arm_thread_state32_t,
    TYPE_MACH_arm_thread_state64_t,
    TYPE_MACH_ppc_thread_state32_t,
    // TYPE_MACH_ppc_thread_state64_t,
    TYPE_MACH_m68k_thread_state32_t,
    TYPE_MACH_functions,
    TYPE_MACH_nlist,
    TYPE_MACH_nlist_64,
    TYPE_MACH_dyld_chained_fixups_header,
    TYPE_MACH_dyld_chained_starts_in_segment,
    TYPE_MACH_dyld_chained_import,
    TYPE_MACH_dyld_chained_import_addend,
    TYPE_MACH_dyld_chained_import_addend64,
    TYPE_MACH_SC_SuperBlob,
    TYPE_MACH_CS_BlobIndex,
    TYPE_MACH_CS_CodeDirectory,
    TYPE_MACH_section,
    TYPE_MACH_section_64,
    TYPE_MACH_trie_export_commands,
    TYPE_MACH_trie_export_table,
    TYPE_MACH_rebase,
    TYPE_MACH_bind,
    TYPE_MACH_weak,
    TYPE_MACH_lazy_bind,
    TYPE_MACH_segment_split_info,
    TYPE_MACH_END,
    TYPE_NE_START,
    TYPE_NE_IMAGE_DOS_HEADER,
    TYPE_NE_END,
    TYPE_LE_START,
    TYPE_LE_IMAGE_DOS_HEADER,
    TYPE_LE_END,
    TYPE_LX_START,
    TYPE_LX_IMAGE_DOS_HEADER,
    TYPE_LX_END,
    TYPE_PE_START,
    TYPE_PE_IMAGE_DOS_HEADER,
    TYPE_PE_DOS_STUB,
    TYPE_PE_IMAGE_NT_HEADERS,
    TYPE_PE_IMAGE_FILE_HEADER,
    TYPE_PE_IMAGE_OPTIONAL_HEADER32,
    TYPE_PE_IMAGE_OPTIONAL_HEADER64,
    TYPE_PE_IMAGE_DATA_DIRECTORY,
    TYPE_PE_IMAGE_SECTION_HEADER,
    TYPE_PE_IMAGE_EXPORT_DIRECTORY,
    TYPE_PE_CERTIFICATE,
    TYPE_PE_IMAGE_COR20_HEADER,
    TYPE_PE_NET_METADATA,
    TYPE_PE_NET_METADATA_STREAM_MAIN,
    TYPE_PE_NET_METADATA_TABLE,
    TYPE_PE_END,
    TYPE_DEX_START,
    TYPE_DEX_HEADER,
    TYPE_DEX_END,
    TYPE_7ZIP_START,
    TYPE_7ZIP_SIGNATUREHEADER,
    TYPE_7ZIP_PROPERTIES,
    TYPE_7ZIP_END,
    TYPE_RAR_START,
    TYPE_RAR_MAIN_ARCHIVE_HEADER,
    TYPE_RAR_FILE_HEADER,
    TYPE_RAR_SERVICE_HEADER,
    TYPE_RAR_ARCHIVE_ENCRYPTION_HEADER,
    TYPE_RAR_END_OF_ARCHIVE_HEADER,
    TYPE_RAR_END,
};

enum WIDGETMODE {
    WIDGETMODE_UNKNOWN = 0,
    WIDGETMODE_HEADER,
    WIDGETMODE_TABLE,
    WIDGETMODE_TABLE_HEX,
    WIDGETMODE_DIALOG_HEADER,
    WIDGETMODE_HEX,
    WIDGETMODE_DISASM,
};

struct OPTIONS {
    qint64 nOffset;
    bool bIsImage;
    qint64 nImageBase;
    XBinary::FT fileType;
    VMODE vmode;

    qint64 nStructOffset;
    qint64 nStructSize;
    qint32 nStructCount;
    XFW_DEF::TYPE type;
    XFW_DEF::WIDGETMODE widgetMode;
    XBinary::MODE mode;
    XBinary::ENDIAN endian;
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
    XDemangle::MODE demangleMode;
    XBinary::_MEMORY_MAP memoryMap;
};

enum VAL_TYPE {
    VAL_TYPE_UNKNOWN_ = 0,
    VAL_TYPE_DATA_INT = 1 << 0,
    VAL_TYPE_DATA_ARRAY = 1 << 1,
    VAL_TYPE_RELADDRESS = 1 << 2,
    VAL_TYPE_ADDRESS = 1 << 3,
    VAL_TYPE_OFFSET = 1 << 4,
    VAL_TYPE_SIZE = 1 << 5,
    VAL_TYPE_ANSI = 1 << 6,
    VAL_TYPE_UUID_ = 1 << 7,
    VAL_TYPE_COUNT = 1 << 8,
    VAL_TYPE_HEX = 1 << 9,
    VAL_TYPE_STRING = 1 << 10,
    VAL_TYPE_CODE = 1 << 11,
    VAL_TYPE_VERSION = 1 << 12,
    VAL_TYPE_RELTOHEADER = 1 << 13,
    VAL_TYPE_RELTOPARENT = 1 << 14,
    VAL_TYPE_RELTOHEADEREND = 1 << 15,
    VAL_TYPE_TIMEDATE = 1 << 16,
    VAL_TYPE_ULEB128 = 1 << 17,
    VAL_TYPE_FULL = 1 << 18,
    VAL_TYPE_DEMANGLE = 1 << 19,
    VAL_TYPE_BOOL = 1 << 20,
};

struct HEADER_RECORD {
    qint32 nPosition;
    QString sName;
    qint32 nOffset;
    qint32 nSize;
    QString sType;
    qint32 vtype;
    qint32 nSubPosition;
};

enum TABLEDATA {
    TABLEDATA_TYPE = 0,
    TABLEDATA_HEADEROFFSET,
    TABLEDATA_HEADERSIZE,
    TABLEDATA_DATAOFFSET,
    TABLEDATA_DATASIZE,
    TABLEDATA_COUNT
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
