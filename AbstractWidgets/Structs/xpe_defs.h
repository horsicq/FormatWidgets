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
#ifndef XPE_DEFS_H
#define XPE_DEFS_H

#include "../xformatwidget_def.h"

namespace XTYPE_PE {

namespace X_IMAGE_NT_HEADERS {
enum DATA {
    Signature = 0,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_NT_HEADERS

namespace X_IMAGE_FILE_HEADER {
enum DATA {
    Machine = 0,
    NumberOfSections,
    TimeDateStamp,
    PointerToSymbolTable,
    NumberOfSymbols,
    SizeOfOptionalHeader,
    Characteristics,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_FILE_HEADER

namespace X_IMAGE_OPTIONAL_HEADER {
enum DATA {
    Magic = 0,
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

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_IMAGE_OPTIONAL_HEADER

namespace X_IMAGE_TLS {
enum DATA {
    StartAddressOfRawData = 0,
    EndAddressOfRawData,
    AddressOfIndex,
    AddressOfCallBacks,
    SizeOfZeroFill,
    Characteristics,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_IMAGE_TLS

namespace X_IMAGE_LOADCONFIG {
enum DATA {
    Size = 0,
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

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_IMAGE_LOADCONFIG

namespace X_IMAGE_DATA_DIRECTORY {
enum DATA {
    Address = 0,
    Size,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_DATA_DIRECTORY

namespace X_IMAGE_SECTION_HEADER {
enum DATA {
    Name = 0,
    VirtualSize,
    VirtualAddress,
    SizeOfRawData,
    PointerToRawData,
    PointerToRelocations,
    PointerToLinenumbers,
    NumberOfRelocations,
    NumberOfLinenumbers,
    Characteristics,  // TODO !!!
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size + 1];
}  // namespace X_IMAGE_SECTION_HEADER

namespace X_IMAGE_EXPORT {
enum DATA {
    Characteristics = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_EXPORT

namespace X_IMAGE_RESOURCES {
enum DATA {
    ID1 = 0,
    ID2,
    ID3,
    ADDRESS,
    OFFSET,
    SIZE,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_RESOURCES

namespace X_IMAGE_DEBUG {
enum DATA {
    Characteristics = 0,
    TimeDateStamp,
    MajorVersion,
    MinorVersion,
    Type,
    SizeOfData,
    AddressOfRawData,
    PointerToRawData,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_DEBUG

namespace X_IMAGE_EXPORT_FUNCTION {
enum DATA {
    Ordinal = 0,
    RVA,
    Name,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_EXPORT_FUNCTION

namespace X_IMAGE_IMPORT {
enum DATA {
    OriginalFirstThunk = 0,
    TimeDateStamp,
    ForwarderChain,
    Name,
    FirstThunk,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_IMPORT

namespace X_IMAGE_DELAYIMPORT {
enum DATA {
    AllAttributes = 0,
    DllNameRVA,
    ModuleHandleRVA,
    ImportAddressTableRVA,
    ImportNameTableRVA,
    BoundImportAddressTableRVA,
    UnloadInformationTableRVA,
    TimeDateStamp,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_DELAYIMPORT

namespace X_IMAGE_BOUNDIMPORT {
enum DATA {
    TimeDateStamp = 0,
    OffsetModuleName,
    NumberOfModuleForwarderRefs,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_BOUNDIMPORT

namespace X_IMAGE_IMPORT_FUNCTION {
enum DATA {
    Thunk = 0,
    Ordinal,
    Hint,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_IMAGE_IMPORT_FUNCTION

namespace X_IMAGE_DELAYIMPORT_FUNCTION {
enum DATA {
    AddressThunk = 0,
    BoundThunk,
    NameThunk,
    Ordinal,
    Hint,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_IMAGE_DELAYIMPORT_FUNCTION

namespace X_IMAGE_RELOCS {
enum DATA {
    VirtualAddress = 0,
    SizeOfBlock,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_RELOCS

namespace X_IMAGE_NETHEADER {
enum DATA {
    cb = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_NETHEADER

namespace X_IMAGE_NET_METADATA {
enum DATA {
    Signature = 0,
    MajorVersion,
    MinorVersion,
    Reserved,
    VersionStringLength,
    Version,
    Flags,
    Streams,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_NET_METADATA

namespace X_IMAGE_EXCEPTIONS {
enum DATA {
    BeginAddress = 0,
    EndAddress,
    UnwindInfoAddress,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];

}  // namespace X_IMAGE_EXCEPTIONS

namespace X_IMAGE_RELOCS_POSITION {
enum DATA {
    TypeOffset = 0,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_RELOCS_POSITION

namespace X_IMAGE_RESOURCE_FIXEDFILEINFO {
enum DATA {
    dwSignature = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_IMAGE_RESOURCE_FIXEDFILEINFO
}  // namespace XTYPE_PE

#endif
