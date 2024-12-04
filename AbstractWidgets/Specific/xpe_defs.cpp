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
#include "xpe_defs.h"

namespace X_IMAGE_NT_HEADERS {
const XFW_DEF::HEADER_RECORD records[] = {{Signature, "Signature", offsetof(XPE_DEF::IMAGE_NT_HEADERS32, Signature), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_}};
}

namespace X_IMAGE_FILE_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {Machine, "Machine", offsetof(XPE_DEF::IMAGE_FILE_HEADER, Machine), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {NumberOfSections, "NumberOfSections", offsetof(XPE_DEF::IMAGE_FILE_HEADER, NumberOfSections), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_FILE_HEADER, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {PointerToSymbolTable, "PointerToSymbolTable", offsetof(XPE_DEF::IMAGE_FILE_HEADER, PointerToSymbolTable), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {NumberOfSymbols, "NumberOfSymbols", offsetof(XPE_DEF::IMAGE_FILE_HEADER, NumberOfSymbols), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfOptionalHeader, "SizeOfOptionalHeader", offsetof(XPE_DEF::IMAGE_FILE_HEADER, SizeOfOptionalHeader), 2, "WORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_FILE_HEADER, Characteristics), 2, "WORD", XFW_DEF::VAL_TYPE_INT_}};
}

namespace X_IMAGE_OPTIONAL_HEADER {
const XFW_DEF::HEADER_RECORD records32[] = {
    {Magic, "Magic", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Magic), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorLinkerVersion, "MajorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorLinkerVersion), 1, "BYTE", XFW_DEF::VAL_TYPE_INT_},
    {MinorLinkerVersion, "MinorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorLinkerVersion), 1, "BYTE", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfCode, "SizeOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfInitializedData, "SizeOfInitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfInitializedData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfUninitializedData, "SizeOfUninitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfUninitializedData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {AddressOfEntryPoint, "AddressOfEntryPoint", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, AddressOfEntryPoint), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {BaseOfCode, "BaseOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, BaseOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {BaseOfData, "BaseOfData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, BaseOfData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {ImageBase, "ImageBase", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, ImageBase), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {SectionAlignment, "SectionAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SectionAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {FileAlignment, "FileAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, FileAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorOperatingSystemVersion, "MajorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_INT_},
    {MinorOperatingSystemVersion, "MinorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_INT_},
    {MajorImageVersion, "MajorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorImageVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorImageVersion, "MinorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorImageVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorSubsystemVersion, "MajorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorSubsystemVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorSubsystemVersion, "MinorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorSubsystemVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Win32VersionValue, "Win32VersionValue", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Win32VersionValue), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfImage, "SizeOfImage", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfImage), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfHeaders, "SizeOfHeaders", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeaders), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {CheckSum, "CheckSum", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, CheckSum), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {Subsystem, "Subsystem", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Subsystem), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {DllCharacteristics, "DllCharacteristics", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, DllCharacteristics), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfStackReserve, "SizeOfStackReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfStackReserve), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfStackCommit, "SizeOfStackCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfStackCommit), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfHeapReserve, "SizeOfHeapReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeapReserve), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfHeapCommit, "SizeOfHeapCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeapCommit), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {LoaderFlags, "LoaderFlags", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, LoaderFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {NumberOfRvaAndSizes, "NumberOfRvaAndSizes", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, NumberOfRvaAndSizes), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_}};
const XFW_DEF::HEADER_RECORD records64[] = {
    {Magic, "Magic", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Magic), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorLinkerVersion, "MajorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorLinkerVersion), 1, "BYTE", XFW_DEF::VAL_TYPE_INT_},
    {MinorLinkerVersion, "MinorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorLinkerVersion), 1, "BYTE", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfCode, "SizeOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfInitializedData, "SizeOfInitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfInitializedData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfUninitializedData, "SizeOfUninitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfUninitializedData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {AddressOfEntryPoint, "AddressOfEntryPoint", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, AddressOfEntryPoint), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {BaseOfCode, "BaseOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, BaseOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {BaseOfData, "", 0, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_},
    {ImageBase, "ImageBase", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, ImageBase), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {SectionAlignment, "SectionAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SectionAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {FileAlignment, "FileAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, FileAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorOperatingSystemVersion, "MajorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_INT_},
    {MinorOperatingSystemVersion, "MinorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_INT_},
    {MajorImageVersion, "MajorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorImageVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorImageVersion, "MinorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorImageVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorSubsystemVersion, "MajorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorSubsystemVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorSubsystemVersion, "MinorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorSubsystemVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Win32VersionValue, "Win32VersionValue", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Win32VersionValue), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfImage, "SizeOfImage", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfImage), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfHeaders, "SizeOfHeaders", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeaders), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {CheckSum, "CheckSum", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, CheckSum), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {Subsystem, "Subsystem", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Subsystem), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {DllCharacteristics, "DllCharacteristics", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, DllCharacteristics), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfStackReserve, "SizeOfStackReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfStackReserve), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfStackCommit, "SizeOfStackCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfStackCommit), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfHeapReserve, "SizeOfHeapReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeapReserve), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {SizeOfHeapCommit, "SizeOfHeapCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeapCommit), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {LoaderFlags, "LoaderFlags", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, LoaderFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {NumberOfRvaAndSizes, "NumberOfRvaAndSizes", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, NumberOfRvaAndSizes), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_}};
}  // namespace X_IMAGE_OPTIONAL_HEADER

namespace X_IMAGE_TLS {
const XFW_DEF::HEADER_RECORD records32[] = {
    {StartAddressOfRawData, "StartAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, StartAddressOfRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {EndAddressOfRawData, "EndAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, EndAddressOfRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {AddressOfIndex, "AddressOfIndex", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, AddressOfIndex), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {AddressOfCallBacks, "AddressOfCallBacks", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, AddressOfCallBacks), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {SizeOfZeroFill, "SizeOfZeroFill", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, SizeOfZeroFill), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_}};
const XFW_DEF::HEADER_RECORD records64[] = {
    {StartAddressOfRawData, "StartAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, StartAddressOfRawData), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {EndAddressOfRawData, "EndAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, EndAddressOfRawData), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {AddressOfIndex, "AddressOfIndex", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, AddressOfIndex), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {AddressOfCallBacks, "AddressOfCallBacks", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, AddressOfCallBacks), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {SizeOfZeroFill, "SizeOfZeroFill", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, SizeOfZeroFill), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_}};
}  // namespace X_IMAGE_TLS

namespace X_IMAGE_LOADCONFIG {
const XFW_DEF::HEADER_RECORD records32[] = {
    {Size, "Size", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {GlobalFlagsClear, "GlobalFlagsClear", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GlobalFlagsClear), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {GlobalFlagsSet, "GlobalFlagsSet", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GlobalFlagsSet), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CriticalSectionDefaultTimeout), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {DeCommitFreeBlockThreshold, "DeCommitFreeBlockThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DeCommitFreeBlockThreshold), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {DeCommitTotalFreeThreshold, "DeCommitTotalFreeThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DeCommitTotalFreeThreshold), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {LockPrefixTable, "LockPrefixTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, LockPrefixTable), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MaximumAllocationSize, "MaximumAllocationSize", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MaximumAllocationSize), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {VirtualMemoryThreshold, "VirtualMemoryThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, VirtualMemoryThreshold), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {ProcessAffinityMask, "ProcessAffinityMask", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, ProcessAffinityMask), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {CSDVersion, "CSDVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CSDVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {DependentLoadFlags, "DependentLoadFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DependentLoadFlags), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {VirtualMemoryThreshold, "EditList", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, EditList), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VirtualMemoryThreshold, "SecurityCookie", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SecurityCookie), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VirtualMemoryThreshold, "SEHandlerTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerTable), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VirtualMemoryThreshold, "SEHandlerCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerCount), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {GuardCFCheckFunctionPointer, "GuardCFCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFCheckFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardCFDispatchFunctionPointer, "GuardCFDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFDispatchFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardCFFunctionTable, "GuardCFFunctionTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFFunctionTable), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardCFFunctionCount, "GuardCFFunctionCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFFunctionCount), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {GuardFlags, "GuardFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeIntegrity_Flags, "CodeIntegrity_Flags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeIntegrity_Catalog, "CodeIntegrity_Catalog", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 2, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeIntegrity_CatalogOffset, "CodeIntegrity_CatalogOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 4, 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {CodeIntegrity_Reserved, "CodeIntegrity_Reserved", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 8, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {GuardAddressTakenIatEntryTable, "GuardAddressTakenIatEntryTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardAddressTakenIatEntryTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardAddressTakenIatEntryCount, "GuardAddressTakenIatEntryCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardAddressTakenIatEntryCount), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {GuardLongJumpTargetTable, "GuardLongJumpTargetTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardLongJumpTargetTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardLongJumpTargetCount, "GuardLongJumpTargetCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardLongJumpTargetCount), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {DynamicValueRelocTable, "DynamicValueRelocTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTable), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {CHPEMetadataPointer, "CHPEMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CHPEMetadataPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardRFFailureRoutine, "GuardRFFailureRoutine", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFFailureRoutine), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardRFFailureRoutineFunctionPointer, "GuardRFFailureRoutineFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFFailureRoutineFunctionPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {DynamicValueRelocTableOffset, "DynamicValueRelocTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTableOffset), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {DynamicValueRelocTableSection, "DynamicValueRelocTableSection", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTableSection), 2, "WORD",
     XFW_DEF::VAL_TYPE_INT_},
    {Reserved2, "Reserved2", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Reserved2), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {GuardRFVerifyStackPointerFunctionPointer, "GuardRFVerifyStackPointerFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFVerifyStackPointerFunctionPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {HotPatchTableOffset, "HotPatchTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, HotPatchTableOffset), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {Reserved3, "Reserved3", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Reserved3), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {EnclaveConfigurationPointer, "EnclaveConfigurationPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, EnclaveConfigurationPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VolatileMetadataPointer, "VolatileMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, VolatileMetadataPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardEHContinuationTable, "GuardEHContinuationTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardEHContinuationTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardEHContinuationCount, "GuardEHContinuationCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardEHContinuationCount), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardXFGCheckFunctionPointer, "GuardXFGCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGCheckFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardXFGDispatchFunctionPointer, "GuardXFGDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGDispatchFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardXFGTableDispatchFunctionPointer, "GuardXFGTableDispatchFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGTableDispatchFunctionPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {CastGuardOsDeterminedFailureMode, "CastGuardOsDeterminedFailureMode", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CastGuardOsDeterminedFailureMode), 4,
     "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardMemcpyFunctionPointer, "GuardMemcpyFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardMemcpyFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {Size, "Size", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {GlobalFlagsClear, "GlobalFlagsClear", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GlobalFlagsClear), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {GlobalFlagsSet, "GlobalFlagsSet", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GlobalFlagsSet), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CriticalSectionDefaultTimeout), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {DeCommitFreeBlockThreshold, "DeCommitFreeBlockThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DeCommitFreeBlockThreshold), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_},
    {DeCommitTotalFreeThreshold, "DeCommitTotalFreeThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DeCommitTotalFreeThreshold), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_},
    {LockPrefixTable, "LockPrefixTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, LockPrefixTable), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_},
    {MaximumAllocationSize, "MaximumAllocationSize", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MaximumAllocationSize), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_},
    {VirtualMemoryThreshold, "VirtualMemoryThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, VirtualMemoryThreshold), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_},
    {ProcessAffinityMask, "ProcessAffinityMask", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, ProcessAffinityMask), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_},
    {CSDVersion, "CSDVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CSDVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {DependentLoadFlags, "DependentLoadFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DependentLoadFlags), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {VirtualMemoryThreshold, "EditList", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, EditList), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VirtualMemoryThreshold, "SecurityCookie", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SecurityCookie), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VirtualMemoryThreshold, "SEHandlerTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SEHandlerTable), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VirtualMemoryThreshold, "SEHandlerCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SEHandlerCount), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_},
    {GuardCFCheckFunctionPointer, "GuardCFCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFCheckFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardCFDispatchFunctionPointer, "GuardCFDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFDispatchFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardCFFunctionTable, "GuardCFFunctionTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFFunctionTable), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardCFFunctionCount, "GuardCFFunctionCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFFunctionCount), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_},
    {GuardFlags, "GuardFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeIntegrity_Flags, "CodeIntegrity_Flags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeIntegrity_Catalog, "CodeIntegrity_Catalog", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 2, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeIntegrity_CatalogOffset, "CodeIntegrity_CatalogOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 4, 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {CodeIntegrity_Reserved, "CodeIntegrity_Reserved", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 8, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {GuardAddressTakenIatEntryTable, "GuardAddressTakenIatEntryTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardAddressTakenIatEntryTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardAddressTakenIatEntryCount, "GuardAddressTakenIatEntryCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardAddressTakenIatEntryCount), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_},
    {GuardLongJumpTargetTable, "GuardLongJumpTargetTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardLongJumpTargetTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardLongJumpTargetCount, "GuardLongJumpTargetCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardLongJumpTargetCount), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_},
    {DynamicValueRelocTable, "DynamicValueRelocTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {CHPEMetadataPointer, "CHPEMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CHPEMetadataPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardRFFailureRoutine, "GuardRFFailureRoutine", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFFailureRoutine), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardRFFailureRoutineFunctionPointer, "GuardRFFailureRoutineFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFFailureRoutineFunctionPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {DynamicValueRelocTableOffset, "DynamicValueRelocTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTableOffset), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {DynamicValueRelocTableSection, "DynamicValueRelocTableSection", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTableSection), 2, "WORD",
     XFW_DEF::VAL_TYPE_INT_},
    {Reserved2, "Reserved2", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Reserved2), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {GuardRFVerifyStackPointerFunctionPointer, "GuardRFVerifyStackPointerFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFVerifyStackPointerFunctionPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {HotPatchTableOffset, "HotPatchTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, HotPatchTableOffset), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {Reserved3, "Reserved3", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Reserved3), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {EnclaveConfigurationPointer, "EnclaveConfigurationPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, EnclaveConfigurationPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {VolatileMetadataPointer, "VolatileMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, VolatileMetadataPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardEHContinuationTable, "GuardEHContinuationTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardEHContinuationTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardEHContinuationCount, "GuardEHContinuationCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardEHContinuationCount), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardXFGCheckFunctionPointer, "GuardXFGCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGCheckFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardXFGDispatchFunctionPointer, "GuardXFGDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGDispatchFunctionPointer), 8,
     "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardXFGTableDispatchFunctionPointer, "GuardXFGTableDispatchFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGTableDispatchFunctionPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {CastGuardOsDeterminedFailureMode, "CastGuardOsDeterminedFailureMode", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CastGuardOsDeterminedFailureMode), 8,
     "ULONGLONG", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {GuardMemcpyFunctionPointer, "GuardMemcpyFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardMemcpyFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
};
}  // namespace X_IMAGE_LOADCONFIG

namespace X_IMAGE_DATA_DIRECTORY {
const XFW_DEF::HEADER_RECORD records[] = {
    {Address, QObject::tr("Address"), 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {Size, QObject::tr("Size"), 4, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_IMAGE_SECTION_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {Name, QObject::tr("Name"), offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Name), 8, "BYTE[8]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_TEXT_},
    {VirtualSize, "VirtualSize", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Misc.VirtualSize), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {VirtualAddress, "VirtualAddress", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, VirtualAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {SizeOfRawData, "SizeOfRawData", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, SizeOfRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {PointerToRawData, "PointerToRawData", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {PointerToRelocations, "PointerToRelocations", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToRelocations), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {PointerToLinenumbers, "PointerToLinenumbers", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToLinenumbers), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {NumberOfRelocations, "NumberOfRelocations", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, NumberOfRelocations), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {NumberOfLinenumbers, "NumberOfLinenumbers", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, NumberOfLinenumbers), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {Characteristics, "", -1, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_}};
}

namespace X_IMAGE_RESOURCES {
const XFW_DEF::HEADER_RECORD records[] = {
    {ID1, "ID1", -1, 0, "", XFW_DEF::VAL_TYPE_INT_},
    {ID2, "ID2", -1, 0, "", XFW_DEF::VAL_TYPE_INT_},
    {ID3, "ID3", -1, 0, "", XFW_DEF::VAL_TYPE_INT_},
    {ADDRESS, QObject::tr("Address"), -1, 0, "", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {OFFSET, QObject::tr("Offset"), -1, 0, "", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {SIZE, QObject::tr("Size"), -1, 0, "", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_IMAGE_EXPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Name, "Name", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Name), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {Base, "Base", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Base), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {NumberOfFunctions, "NumberOfFunctions", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, NumberOfFunctions), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {NumberOfNames, "NumberOfNames", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, NumberOfNames), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {AddressOfFunctions, "AddressOfFunctions", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfFunctions), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {AddressOfNames, "AddressOfNames", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfNames), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {AddressOfNameOrdinals, "AddressOfNameOrdinals", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfNameOrdinals), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_}};
}

namespace X_IMAGE_EXPORT_FUNCTION {
const XFW_DEF::HEADER_RECORD records[] = {{Ordinal, "Ordinal", 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
                                          {RVA, "RVA", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                          {Name, QObject::tr("Name"), 0, 4, "SZCHAR", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_}};
}

namespace X_IMAGE_IMPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {OriginalFirstThunk, "OriginalFirstThunk", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, OriginalFirstThunk), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {ForwarderChain, "ForwarderChain", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, ForwarderChain), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {Name, QObject::tr("Name"), offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, Name), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {FirstThunk, "FirstThunk", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, FirstThunk), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_}};
}

namespace X_IMAGE_IMPORT_FUNCTION {
const XFW_DEF::HEADER_RECORD records32[] = {{Thunk, "Thunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {Ordinal, "Ordinal", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
                                            {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_}};
const XFW_DEF::HEADER_RECORD records64[] = {{Thunk, "Thunk", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {Ordinal, "Ordinal", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_INT_},
                                            {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_}};
}  // namespace X_IMAGE_IMPORT_FUNCTION

namespace X_IMAGE_DELAYIMPORT_FUNCTION {
const XFW_DEF::HEADER_RECORD records32[] = {{AddressThunk, "AddressThunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {BoundThunk, "BoundThunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {NameThunk, "NameThunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {Ordinal, "Ordinal", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
                                            {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_}};
const XFW_DEF::HEADER_RECORD records64[] = {{AddressThunk, "AddressThunk", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {BoundThunk, "BoundThunk", 0, 8, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {NameThunk, "NameThunk", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
                                            {Ordinal, "Ordinal", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_INT_},
                                            {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_}};
}  // namespace X_IMAGE_DELAYIMPORT_FUNCTION

namespace X_IMAGE_BOUNDIMPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {TimeDateStamp, "TimeDateStamp", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {OffsetModuleName, "OffsetModuleName", 4, 2, "WORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {NumberOfModuleForwarderRefs, "NumberOfModuleForwarderRefs", 6, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_IMAGE_RELOCS {
const XFW_DEF::HEADER_RECORD records[] = {
    {VirtualAddress, "VirtualAddress", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {SizeOfBlock, "SizeOfBlock", 4, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_IMAGE_DEBUG {
const XFW_DEF::HEADER_RECORD records[] = {
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Type, "Type", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, Type), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {SizeOfData, "SizeOfData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, SizeOfData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {AddressOfRawData, "AddressOfRawData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, AddressOfRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {PointerToRawData, "PointerToRawData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, PointerToRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}

namespace X_IMAGE_DELAYIMPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {AllAttributes, "AllAttributes", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, AllAttributes), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {DllNameRVA, "DllNameRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, DllNameRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {ModuleHandleRVA, "ModuleHandleRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ModuleHandleRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {ImportAddressTableRVA, "ImportAddressTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ImportAddressTableRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {ImportNameTableRVA, "ImportNameTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ImportNameTableRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {BoundImportAddressTableRVA, "BoundImportAddressTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, BoundImportAddressTableRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {UnloadInformationTableRVA, "UnloadInformationTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, UnloadInformationTableRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_IMAGE_EXCEPTIONS {
const XFW_DEF::HEADER_RECORD records[] = {
    {BeginAddress, "BeginAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, BeginAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {EndAddress, "EndAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, EndAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {UnwindInfoAddress, "UnwindInfoAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, UnwindInfoAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
};
}

namespace X_IMAGE_NETHEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {cb, "cb", offsetof(XPE_DEF::IMAGE_COR20_HEADER, cb), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {MajorRuntimeVersion, "MajorRuntimeVersion", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MajorRuntimeVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorRuntimeVersion, "MinorRuntimeVersion", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MinorRuntimeVersion), 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MetaData_Address, "MetaData_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MetaData.VirtualAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {MetaData_Size, "MetaData_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MetaData.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {Flags, "Flags", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Flags), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {EntryPoint, "EntryPoint", offsetof(XPE_DEF::IMAGE_COR20_HEADER, EntryPointRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {Resources_Address, "Resources_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Resources.VirtualAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {Resources_Size, "Resources_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Resources.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {StrongNameSignature_Address, "StrongNameSignature_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, StrongNameSignature.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {StrongNameSignature_Size, "StrongNameSignature_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, StrongNameSignature.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {CodeManagerTable_Address, "CodeManagerTable_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, CodeManagerTable.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {CodeManagerTable_Size, "CodeManagerTable_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, CodeManagerTable.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {VTableFixups_Address, "VTableFixups_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, VTableFixups.VirtualAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {VTableFixups_Size, "VTableFixups_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, VTableFixups.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {ExportAddressTableJumps_Address, "ExportAddressTableJumps_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ExportAddressTableJumps.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {ExportAddressTableJumps_Size, "ExportAddressTableJumps_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ExportAddressTableJumps.Size), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_},
    {ManagedNativeHeader_Address, "ManagedNativeHeader_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ManagedNativeHeader.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_RELADDRESS_},
    {ManagedNativeHeader_Size, "ManagedNativeHeader_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ManagedNativeHeader.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_IMAGE_NET_METADATA {
const XFW_DEF::HEADER_RECORD records[] = {
    {Signature, "Signature", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {MajorVersion, "MajorVersion", 4, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {MinorVersion, "MinorVersion", 6, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Reserved, "Reserved", 8, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {VersionStringLength, "VersionStringLength", 12, 4, "DWORD", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {Version, "Version", 16, 1, "TEXT", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_TEXT_},
    {Flags, "Flags", -1, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
    {Streams, "Streams", -1, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_IMAGE_RESOURCE_FIXEDFILEINFO {
const XFW_DEF::HEADER_RECORD records[] = {
    {dwSignature, "dwSignature", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwSignature), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwStrucVersion, "dwStrucVersion", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwStrucVersion), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileVersionMS, "dwFileVersionMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileVersionMS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileVersionLS, "dwFileVersionLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileVersionLS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwProductVersionMS, "dwProductVersionMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwProductVersionMS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwProductVersionLS, "dwProductVersionLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwProductVersionLS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileFlagsMask, "dwFileFlagsMask", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileFlagsMask), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileFlags, "dwFileFlags", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileOS, "dwFileOS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileOS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileType, "dwFileType", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileType), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileSubtype, "dwFileSubtype", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileSubtype), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileDateMS, "dwFileDateMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileDateMS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
    {dwFileDateLS, "dwFileDateLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileDateLS), 4, "DWORD", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_IMAGE_RELOCS_POSITION {
const XFW_DEF::HEADER_RECORD records[] = {
    {TypeOffset, "TypeOffset", 0, 2, "WORD", XFW_DEF::VAL_TYPE_INT_},
};
}
