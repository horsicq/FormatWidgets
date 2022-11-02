/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef PE_DEFS_H
#define PE_DEFS_H

#include "../formatwidget.h"
#include "xpe.h"

namespace SPE
{
enum TYPE
{
    TYPE_INFO=0,
    TYPE_VIRUSTOTAL,
    TYPE_HEX,
    TYPE_DISASM,
    TYPE_HASH,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_HEURISTICSCAN,
    TYPE_EXTRACTOR,
    TYPE_IMAGE_DOS_HEADER,
    TYPE_IMAGE_NT_HEADERS,
    TYPE_IMAGE_FILE_HEADER,
    TYPE_IMAGE_OPTIONAL_HEADER,
    TYPE_IMAGE_DIRECTORY_ENTRIES,
    TYPE_RICH,
    TYPE_SECTIONS,
    TYPE_EXPORT,
    TYPE_IMPORT,
    TYPE_RESOURCES,
    TYPE_RESOURCES_STRINGTABLE,
    TYPE_RESOURCES_VERSION,
    TYPE_RESOURCES_MANIFEST,
    TYPE_EXCEPTION,
    TYPE_RELOCS,
    TYPE_DEBUG,
    TYPE_TLS,
    TYPE_TLSCALLBACKS,
    TYPE_LOADCONFIG,
    TYPE_BOUNDIMPORT,
    TYPE_DELAYIMPORT,
    TYPE_NETHEADER,
    TYPE_NET_METADATA,
    TYPE_NET_METADATA_STREAM,
    TYPE_CERTIFICATE,
    TYPE_OVERLAY,
    __TYPE_size,
    TYPE_IMAGE_SECTION_HEADER,
    TYPE_EXPORT_FUNCTION,
    TYPE_IMPORT_FUNCTION,
    TYPE_RELOCS_POSITION,
    TYPE_DELAYIMPORT_FUNCTION,
    TYPE_CERTIFICATE_CHECK,
    TYPE_RESOURCES_TREE,
    __TYPE_size_ext
};
}

namespace N_IMAGE_DOS_HEADER
{
enum DATA
{
    e_magic=0,
    e_cblp,
    e_cp,
    e_crlc,
    e_cparhdr,
    e_minalloc,
    e_maxalloc,
    e_ss,
    e_sp,
    e_csum,
    e_ip,
    e_cs,
    e_lfarlc,
    e_ovno,
    e_res_0,
    e_res_1,
    e_res_2,
    e_res_3,
    e_oemid,
    e_oeminfo,
    e_res2_0,
    e_res2_1,
    e_res2_2,
    e_res2_3,
    e_res2_4,
    e_res2_5,
    e_res2_6,
    e_res2_7,
    e_res2_8,
    e_res2_9,
    e_lfanew,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_NT_HEADERS
{
enum DATA
{
    Signature=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_FILE_HEADER
{
enum DATA
{
    Machine=0,
    NumberOfSections,
    TimeDateStamp,
    PointerToSymbolTable,
    NumberOfSymbols,
    SizeOfOptionalHeader,
    Characteristics,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_OPTIONAL_HEADER
{
enum DATA
{
    Magic=0,
    MajorLinkerVersion,
    MinorLinkerVersion,
    SizeOfCode,
    SizeOfInitializedData,
    SizeOfUninitializedData,
    AddressOfEntryPoint,
    BaseOfCode,
    BaseOfData,
    ImageBase,
    SectionAlignment,
    FileAlignment,
    MajorOperatingSystemVersion,
    MinorOperatingSystemVersion,
    MajorImageVersion,
    MinorImageVersion,
    MajorSubsystemVersion,
    MinorSubsystemVersion,
    Win32VersionValue,
    SizeOfImage,
    SizeOfHeaders,
    CheckSum,
    Subsystem,
    DllCharacteristics,
    SizeOfStackReserve,
    SizeOfStackCommit,
    SizeOfHeapReserve,
    SizeOfHeapCommit,
    LoaderFlags,
    NumberOfRvaAndSizes,
    __data_size,
    OperatingSystemVersion
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_IMAGE_TLS
{
enum DATA
{
    StartAddressOfRawData=0,
    EndAddressOfRawData,
    AddressOfIndex,
    AddressOfCallBacks,
    SizeOfZeroFill,
    Characteristics,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_IMAGE_LOADCONFIG
{
enum DATA
{
    Size=0,
    TimeDateStamp,
    MajorVersion,
    MinorVersion,
    GlobalFlagsClear,
    GlobalFlagsSet,
    CriticalSectionDefaultTimeout,
    DeCommitFreeBlockThreshold,
    DeCommitTotalFreeThreshold,
    LockPrefixTable,
    MaximumAllocationSize,
    VirtualMemoryThreshold,
    ProcessAffinityMask,
    CSDVersion,
    DependentLoadFlags,
    EditList,
    SecurityCookie,
    SEHandlerTable,
    SEHandlerCount,
    // Standard
    GuardCFCheckFunctionPointer,
    GuardCFDispatchFunctionPointer,
    GuardCFFunctionTable,
    GuardCFFunctionCount,
    GuardFlags,
    CodeIntegrity_Flags,
    CodeIntegrity_Catalog,
    CodeIntegrity_CatalogOffset,
    CodeIntegrity_Reserved,
    GuardAddressTakenIatEntryTable,
    GuardAddressTakenIatEntryCount,
    GuardLongJumpTargetTable,
    GuardLongJumpTargetCount,
    DynamicValueRelocTable,
    CHPEMetadataPointer,
    GuardRFFailureRoutine,
    GuardRFFailureRoutineFunctionPointer,
    DynamicValueRelocTableOffset,
    DynamicValueRelocTableSection,
    Reserved2,
    GuardRFVerifyStackPointerFunctionPointer,
    HotPatchTableOffset,
    Reserved3,
    EnclaveConfigurationPointer,
    VolatileMetadataPointer,
    GuardEHContinuationTable,
    GuardEHContinuationCount,
    GuardXFGCheckFunctionPointer,
    GuardXFGDispatchFunctionPointer,
    GuardXFGTableDispatchFunctionPointer,
    CastGuardOsDeterminedFailureMode,
    GuardMemcpyFunctionPointer,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_IMAGE_DATA_DIRECTORY
{
enum CB
{
    __CB_size
};

enum INV
{
    __INV_size
};

enum DATA
{
    Address=0,
    Size,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_SECTION_HEADER
{
enum DATA
{
    Name=0,
    VirtualSize,
    VirtualAddress,
    SizeOfRawData,
    PointerToRawData,
    PointerToRelocations,
    PointerToLinenumbers,
    NumberOfRelocations,
    NumberOfLinenumbers,
    Characteristics, // TODO !!!
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size+1];

enum CB
{
    CB_CHARACTERISTICS=0,
    CB_ALIGH,
    __CB_size
};

enum INV
{
    INV_VirtualAddress,
    INV_PointerToRawData,
    __INV_size
};
}

namespace N_IMAGE_EXPORT
{
enum DATA
{
    Characteristics=0,
    TimeDateStamp,
    MajorVersion,
    MinorVersion,
    Name,
    Base,
    NumberOfFunctions,
    NumberOfNames,
    AddressOfFunctions,
    AddressOfNames,
    AddressOfNameOrdinals,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_RESOURCES
{
enum DATA
{
    ID1=0,
    ID2,
    ID3,
    ADDRESS,
    OFFSET,
    SIZE,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_DEBUG
{
enum DATA
{
    Characteristics=0,
    TimeDateStamp,
    MajorVersion,
    MinorVersion,
    Type,
    SizeOfData,
    AddressOfRawData,
    PointerToRawData,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_TYPE=0,
    __CB_size
};
enum INV
{
    __INV_size
};
}

namespace N_IMAGE_EXPORT_FUNCTION
{
enum DATA
{
    Ordinal=0,
    RVA,
    Name,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_IMPORT
{
enum DATA
{
    OriginalFirstThunk=0,
    TimeDateStamp,
    ForwarderChain,
    Name,
    FirstThunk,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    __CB_size
};
enum INV
{
    INV_OriginalFirstThunk=0,
    INV_Name,
    INV_FirstThunk,
    __INV_size
};
}

namespace N_IMAGE_DELAYIMPORT
{
enum DATA
{
    AllAttributes=0,
    DllNameRVA,
    ModuleHandleRVA,
    ImportAddressTableRVA,
    ImportNameTableRVA,
    BoundImportAddressTableRVA,
    UnloadInformationTableRVA,
    TimeDateStamp,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    __CB_size
};
enum INV
{
    __INV_size
};
}

namespace N_IMAGE_BOUNDIMPORT
{
enum DATA
{
    TimeDateStamp=0,
    OffsetModuleName,
    NumberOfModuleForwarderRefs,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    __CB_size
};
enum INV
{
    __INV_size
};
}

namespace N_IMAGE_IMPORT_FUNCTION
{
enum DATA
{
    Thunk=0,
    Ordinal,
    Hint,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_IMAGE_DELAYIMPORT_FUNCTION
{
enum DATA
{
    AddressThunk=0,
    BoundThunk,
    NameThunk,
    Ordinal,
    Hint,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_IMAGE_RELOCS
{
enum DATA
{
    VirtualAddress=0,
    SizeOfBlock,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    __CB_size
};
enum INV
{
    __INV_size
};
}

namespace N_IMAGE_NETHEADER
{
enum DATA
{
    cb=0,
    MajorRuntimeVersion,
    MinorRuntimeVersion,
    MetaData_Address,
    MetaData_Size,
    Flags,
    EntryPoint,
    Resources_Address,
    Resources_Size,
    StrongNameSignature_Address,
    StrongNameSignature_Size,
    CodeManagerTable_Address,
    CodeManagerTable_Size,
    VTableFixups_Address,
    VTableFixups_Size,
    ExportAddressTableJumps_Address,
    ExportAddressTableJumps_Size,
    ManagedNativeHeader_Address,
    ManagedNativeHeader_Size,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_NET_METADATA
{
enum DATA
{
    Signature=0,
    MajorVersion,
    MinorVersion,
    Reserved,
    VersionStringLength,
    Version,
    Flags,
    Streams,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_EXCEPTIONS
{
enum DATA
{
    BeginAddress=0,
    EndAddress,
    UnwindInfoAddress,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    __CB_size
};
enum INV
{
    __INV_size
};
}

namespace N_IMAGE_RELOCS_POSITION
{
enum DATA
{
    TypeOffset=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_RESOURCE_FIXEDFILEINFO
{
enum DATA
{
    dwSignature=0,
    dwStrucVersion,
    dwFileVersionMS,
    dwFileVersionLS,
    dwProductVersionMS,
    dwProductVersionLS,
    dwFileFlagsMask,
    dwFileFlags,
    dwFileOS,
    dwFileType,
    dwFileSubtype,
    dwFileDateMS,
    dwFileDateLS,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

#endif
