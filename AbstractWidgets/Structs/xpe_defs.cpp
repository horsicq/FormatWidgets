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

namespace XTYPE_PE {

namespace X_IMAGE_NT_HEADERS {
const XFW_DEF::HEADER_RECORD records[] = {
    {Signature, "Signature", offsetof(XPE_DEF::IMAGE_NT_HEADERS32, Signature), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_FILE_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {Machine, "Machine", offsetof(XPE_DEF::IMAGE_FILE_HEADER, Machine), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {NumberOfSections, "NumberOfSections", offsetof(XPE_DEF::IMAGE_FILE_HEADER, NumberOfSections), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_FILE_HEADER, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_TIMEDATE, -1},
    {PointerToSymbolTable, "PointerToSymbolTable", offsetof(XPE_DEF::IMAGE_FILE_HEADER, PointerToSymbolTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {NumberOfSymbols, "NumberOfSymbols", offsetof(XPE_DEF::IMAGE_FILE_HEADER, NumberOfSymbols), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {SizeOfOptionalHeader, "SizeOfOptionalHeader", offsetof(XPE_DEF::IMAGE_FILE_HEADER, SizeOfOptionalHeader), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADEREND, -1},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_FILE_HEADER, Characteristics), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_OPTIONAL_HEADER {
const XFW_DEF::HEADER_RECORD records32[] = {
    {Magic, "Magic", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Magic), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorLinkerVersion, "MajorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorLinkerVersion), 1, "BYTE",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorLinkerVersion, "MinorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorLinkerVersion), 1, "BYTE",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {SizeOfCode, "SizeOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfInitializedData, "SizeOfInitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfInitializedData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfUninitializedData, "SizeOfUninitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfUninitializedData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {AddressOfEntryPoint, "AddressOfEntryPoint", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, AddressOfEntryPoint), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BaseOfCode, "BaseOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, BaseOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BaseOfData, "BaseOfData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, BaseOfData), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {ImageBase, "ImageBase", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, ImageBase), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {SectionAlignment, "SectionAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SectionAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {FileAlignment, "FileAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, FileAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorOperatingSystemVersion, "MajorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MinorOperatingSystemVersion, "MinorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorImageVersion, "MajorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorImageVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorImageVersion, "MinorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorImageVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MajorSubsystemVersion, "MajorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorSubsystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorSubsystemVersion, "MinorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorSubsystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {Win32VersionValue, "Win32VersionValue", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Win32VersionValue), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {SizeOfImage, "SizeOfImage", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfImage), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfHeaders, "SizeOfHeaders", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeaders), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {CheckSum, "CheckSum", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, CheckSum), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Subsystem, "Subsystem", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Subsystem), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DllCharacteristics, "DllCharacteristics", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, DllCharacteristics), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {SizeOfStackReserve, "SizeOfStackReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfStackReserve), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfStackCommit, "SizeOfStackCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfStackCommit), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfHeapReserve, "SizeOfHeapReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeapReserve), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfHeapCommit, "SizeOfHeapCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeapCommit), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     -1},
    {LoaderFlags, "LoaderFlags", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, LoaderFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {NumberOfRvaAndSizes, "NumberOfRvaAndSizes", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, NumberOfRvaAndSizes), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {Magic, "Magic", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Magic), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorLinkerVersion, "MajorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorLinkerVersion), 1, "BYTE",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorLinkerVersion, "MinorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorLinkerVersion), 1, "BYTE",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {SizeOfCode, "SizeOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfInitializedData, "SizeOfInitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfInitializedData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfUninitializedData, "SizeOfUninitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfUninitializedData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {AddressOfEntryPoint, "AddressOfEntryPoint", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, AddressOfEntryPoint), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BaseOfCode, "BaseOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, BaseOfCode), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BaseOfData, "", -1, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_, -1},
    {ImageBase, "ImageBase", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, ImageBase), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {SectionAlignment, "SectionAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SectionAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {FileAlignment, "FileAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, FileAlignment), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorOperatingSystemVersion, "MajorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MinorOperatingSystemVersion, "MinorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorOperatingSystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorImageVersion, "MajorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorImageVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorImageVersion, "MinorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorImageVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MajorSubsystemVersion, "MajorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorSubsystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorSubsystemVersion, "MinorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorSubsystemVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {Win32VersionValue, "Win32VersionValue", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Win32VersionValue), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {SizeOfImage, "SizeOfImage", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfImage), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfHeaders, "SizeOfHeaders", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeaders), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {CheckSum, "CheckSum", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, CheckSum), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Subsystem, "Subsystem", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Subsystem), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DllCharacteristics, "DllCharacteristics", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, DllCharacteristics), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {SizeOfStackReserve, "SizeOfStackReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfStackReserve), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfStackCommit, "SizeOfStackCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfStackCommit), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfHeapReserve, "SizeOfHeapReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeapReserve), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {SizeOfHeapCommit, "SizeOfHeapCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeapCommit), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {LoaderFlags, "LoaderFlags", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, LoaderFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {NumberOfRvaAndSizes, "NumberOfRvaAndSizes", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, NumberOfRvaAndSizes), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
}  // namespace X_IMAGE_OPTIONAL_HEADER

namespace X_IMAGE_TLS {
const XFW_DEF::HEADER_RECORD records32[] = {
    {StartAddressOfRawData, "StartAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, StartAddressOfRawData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {EndAddressOfRawData, "EndAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, EndAddressOfRawData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {AddressOfIndex, "AddressOfIndex", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, AddressOfIndex), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {AddressOfCallBacks, "AddressOfCallBacks", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, AddressOfCallBacks), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {SizeOfZeroFill, "SizeOfZeroFill", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, SizeOfZeroFill), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {StartAddressOfRawData, "StartAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, StartAddressOfRawData), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {EndAddressOfRawData, "EndAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, EndAddressOfRawData), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {AddressOfIndex, "AddressOfIndex", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, AddressOfIndex), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {AddressOfCallBacks, "AddressOfCallBacks", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, AddressOfCallBacks), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {SizeOfZeroFill, "SizeOfZeroFill", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, SizeOfZeroFill), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_IMAGE_TLS

namespace X_IMAGE_LOADCONFIG {
const XFW_DEF::HEADER_RECORD records32[] = {
    {Size, "Size", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Size), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GlobalFlagsClear, "GlobalFlagsClear", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GlobalFlagsClear), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GlobalFlagsSet, "GlobalFlagsSet", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GlobalFlagsSet), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CriticalSectionDefaultTimeout), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DeCommitFreeBlockThreshold, "DeCommitFreeBlockThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DeCommitFreeBlockThreshold), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DeCommitTotalFreeThreshold, "DeCommitTotalFreeThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DeCommitTotalFreeThreshold), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {LockPrefixTable, "LockPrefixTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, LockPrefixTable), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MaximumAllocationSize, "MaximumAllocationSize", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MaximumAllocationSize), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT,
     -1},
    {VirtualMemoryThreshold, "VirtualMemoryThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, VirtualMemoryThreshold), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT,
     -1},
    {ProcessAffinityMask, "ProcessAffinityMask", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, ProcessAffinityMask), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CSDVersion, "CSDVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CSDVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DependentLoadFlags, "DependentLoadFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DependentLoadFlags), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {VirtualMemoryThreshold, "EditList", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, EditList), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {VirtualMemoryThreshold, "SecurityCookie", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SecurityCookie), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VirtualMemoryThreshold, "SEHandlerTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VirtualMemoryThreshold, "SEHandlerCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerCount), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardCFCheckFunctionPointer, "GuardCFCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFCheckFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardCFDispatchFunctionPointer, "GuardCFDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFDispatchFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardCFFunctionTable, "GuardCFFunctionTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFFunctionTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardCFFunctionCount, "GuardCFFunctionCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFFunctionCount), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardFlags, "GuardFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CodeIntegrity_Flags, "CodeIntegrity_Flags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CodeIntegrity_Catalog, "CodeIntegrity_Catalog", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 2, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CodeIntegrity_CatalogOffset, "CodeIntegrity_CatalogOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 4, 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {CodeIntegrity_Reserved, "CodeIntegrity_Reserved", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 8, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardAddressTakenIatEntryTable, "GuardAddressTakenIatEntryTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardAddressTakenIatEntryTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardAddressTakenIatEntryCount, "GuardAddressTakenIatEntryCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardAddressTakenIatEntryCount), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardLongJumpTargetTable, "GuardLongJumpTargetTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardLongJumpTargetTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardLongJumpTargetCount, "GuardLongJumpTargetCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardLongJumpTargetCount), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DynamicValueRelocTable, "DynamicValueRelocTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {CHPEMetadataPointer, "CHPEMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CHPEMetadataPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardRFFailureRoutine, "GuardRFFailureRoutine", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFFailureRoutine), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardRFFailureRoutineFunctionPointer, "GuardRFFailureRoutineFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFFailureRoutineFunctionPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {DynamicValueRelocTableOffset, "DynamicValueRelocTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTableOffset), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {DynamicValueRelocTableSection, "DynamicValueRelocTableSection", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTableSection), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Reserved2, "Reserved2", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Reserved2), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardRFVerifyStackPointerFunctionPointer, "GuardRFVerifyStackPointerFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFVerifyStackPointerFunctionPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {HotPatchTableOffset, "HotPatchTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, HotPatchTableOffset), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {Reserved3, "Reserved3", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Reserved3), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {EnclaveConfigurationPointer, "EnclaveConfigurationPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, EnclaveConfigurationPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VolatileMetadataPointer, "VolatileMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, VolatileMetadataPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardEHContinuationTable, "GuardEHContinuationTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardEHContinuationTable), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardEHContinuationCount, "GuardEHContinuationCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardEHContinuationCount), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardXFGCheckFunctionPointer, "GuardXFGCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGCheckFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardXFGDispatchFunctionPointer, "GuardXFGDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGDispatchFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardXFGTableDispatchFunctionPointer, "GuardXFGTableDispatchFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGTableDispatchFunctionPointer), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {CastGuardOsDeterminedFailureMode, "CastGuardOsDeterminedFailureMode", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CastGuardOsDeterminedFailureMode), 4,
     "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardMemcpyFunctionPointer, "GuardMemcpyFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardMemcpyFunctionPointer), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {Size, "Size", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Size), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GlobalFlagsClear, "GlobalFlagsClear", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GlobalFlagsClear), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GlobalFlagsSet, "GlobalFlagsSet", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GlobalFlagsSet), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CriticalSectionDefaultTimeout), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DeCommitFreeBlockThreshold, "DeCommitFreeBlockThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DeCommitFreeBlockThreshold), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DeCommitTotalFreeThreshold, "DeCommitTotalFreeThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DeCommitTotalFreeThreshold), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {LockPrefixTable, "LockPrefixTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, LockPrefixTable), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MaximumAllocationSize, "MaximumAllocationSize", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MaximumAllocationSize), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {VirtualMemoryThreshold, "VirtualMemoryThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, VirtualMemoryThreshold), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ProcessAffinityMask, "ProcessAffinityMask", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, ProcessAffinityMask), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CSDVersion, "CSDVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CSDVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DependentLoadFlags, "DependentLoadFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DependentLoadFlags), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {VirtualMemoryThreshold, "EditList", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, EditList), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VirtualMemoryThreshold, "SecurityCookie", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SecurityCookie), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VirtualMemoryThreshold, "SEHandlerTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SEHandlerTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VirtualMemoryThreshold, "SEHandlerCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SEHandlerCount), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardCFCheckFunctionPointer, "GuardCFCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFCheckFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardCFDispatchFunctionPointer, "GuardCFDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFDispatchFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardCFFunctionTable, "GuardCFFunctionTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFFunctionTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardCFFunctionCount, "GuardCFFunctionCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFFunctionCount), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT,
     -1},
    {GuardFlags, "GuardFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CodeIntegrity_Flags, "CodeIntegrity_Flags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CodeIntegrity_Catalog, "CodeIntegrity_Catalog", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 2, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {CodeIntegrity_CatalogOffset, "CodeIntegrity_CatalogOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 4, 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {CodeIntegrity_Reserved, "CodeIntegrity_Reserved", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 8, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardAddressTakenIatEntryTable, "GuardAddressTakenIatEntryTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardAddressTakenIatEntryTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardAddressTakenIatEntryCount, "GuardAddressTakenIatEntryCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardAddressTakenIatEntryCount), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardLongJumpTargetTable, "GuardLongJumpTargetTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardLongJumpTargetTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardLongJumpTargetCount, "GuardLongJumpTargetCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardLongJumpTargetCount), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DynamicValueRelocTable, "DynamicValueRelocTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {CHPEMetadataPointer, "CHPEMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CHPEMetadataPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardRFFailureRoutine, "GuardRFFailureRoutine", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFFailureRoutine), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardRFFailureRoutineFunctionPointer, "GuardRFFailureRoutineFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFFailureRoutineFunctionPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {DynamicValueRelocTableOffset, "DynamicValueRelocTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTableOffset), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {DynamicValueRelocTableSection, "DynamicValueRelocTableSection", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTableSection), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Reserved2, "Reserved2", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Reserved2), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {GuardRFVerifyStackPointerFunctionPointer, "GuardRFVerifyStackPointerFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFVerifyStackPointerFunctionPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {HotPatchTableOffset, "HotPatchTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, HotPatchTableOffset), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {Reserved3, "Reserved3", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Reserved3), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {EnclaveConfigurationPointer, "EnclaveConfigurationPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, EnclaveConfigurationPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {VolatileMetadataPointer, "VolatileMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, VolatileMetadataPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardEHContinuationTable, "GuardEHContinuationTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardEHContinuationTable), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardEHContinuationCount, "GuardEHContinuationCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardEHContinuationCount), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardXFGCheckFunctionPointer, "GuardXFGCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGCheckFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardXFGDispatchFunctionPointer, "GuardXFGDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGDispatchFunctionPointer), 8,
     "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardXFGTableDispatchFunctionPointer, "GuardXFGTableDispatchFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGTableDispatchFunctionPointer), 8, "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS,
     -1},
    {CastGuardOsDeterminedFailureMode, "CastGuardOsDeterminedFailureMode", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CastGuardOsDeterminedFailureMode), 8,
     "ULONGLONG", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {GuardMemcpyFunctionPointer, "GuardMemcpyFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardMemcpyFunctionPointer), 8, "ULONGLONG",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
};
}  // namespace X_IMAGE_LOADCONFIG

namespace X_IMAGE_DATA_DIRECTORY {
const XFW_DEF::HEADER_RECORD records[] = {
    {Address, QObject::tr("Address"), 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Size, QObject::tr("Size"), 4, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, Address},
};
}

namespace X_IMAGE_SECTION_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {Name, QObject::tr("Name"), offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Name), 8, "BYTE[8]",
     XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING, -1},
    {VirtualSize, "VirtualSize", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Misc.VirtualSize), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {VirtualAddress, "VirtualAddress", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, VirtualAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS,
     -1},
    {SizeOfRawData, "SizeOfRawData", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, SizeOfRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     PointerToRawData},
    {PointerToRawData, "PointerToRawData", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToRawData), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET,
     -1},
    {PointerToRelocations, "PointerToRelocations", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToRelocations), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {PointerToLinenumbers, "PointerToLinenumbers", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToLinenumbers), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {NumberOfRelocations, "NumberOfRelocations", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, NumberOfRelocations), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {NumberOfLinenumbers, "NumberOfLinenumbers", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, NumberOfLinenumbers), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_RESOURCES {
const XFW_DEF::HEADER_RECORD records[] = {
    {ID1, "ID1", -1, 0, "", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ID2, "ID2", -1, 0, "", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ID3, "ID3", -1, 0, "", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ADDRESS, QObject::tr("Address"), -1, 0, "", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {OFFSET, QObject::tr("Offset"), -1, 0, "", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {SIZE, QObject::tr("Size"), -1, 0, "", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
};
}

namespace X_IMAGE_EXPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Name, "Name", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Name), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Base, "Base", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Base), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {NumberOfFunctions, "NumberOfFunctions", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, NumberOfFunctions), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {NumberOfNames, "NumberOfNames", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, NumberOfNames), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {AddressOfFunctions, "AddressOfFunctions", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfFunctions), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {AddressOfNames, "AddressOfNames", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfNames), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS,
     -1},
    {AddressOfNameOrdinals, "AddressOfNameOrdinals", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfNameOrdinals), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
};
}

namespace X_IMAGE_EXPORT_FUNCTION {
const XFW_DEF::HEADER_RECORD records[] = {
    {Ordinal, "Ordinal", 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {RVA, "RVA", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Name, QObject::tr("Name"), 0, 4, "SZCHAR", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
};
}

namespace X_IMAGE_IMPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {OriginalFirstThunk, "OriginalFirstThunk", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, OriginalFirstThunk), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ForwarderChain, "ForwarderChain", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, ForwarderChain), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS,
     -1},
    {Name, QObject::tr("Name"), offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, Name), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {FirstThunk, "FirstThunk", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, FirstThunk), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
};
}

namespace X_IMAGE_IMPORT_FUNCTION {
const XFW_DEF::HEADER_RECORD records32[] = {
    {Thunk, "Thunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Ordinal, "Ordinal", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {Thunk, "Thunk", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Ordinal, "Ordinal", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_IMAGE_IMPORT_FUNCTION

namespace X_IMAGE_DELAYIMPORT_FUNCTION {
const XFW_DEF::HEADER_RECORD records32[] = {
    {AddressThunk, "AddressThunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BoundThunk, "BoundThunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {NameThunk, "NameThunk", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Ordinal, "Ordinal", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {AddressThunk, "AddressThunk", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BoundThunk, "BoundThunk", 0, 8, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {NameThunk, "NameThunk", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Ordinal, "Ordinal", 0, 8, "QWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Hint, "Hint", 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_IMAGE_DELAYIMPORT_FUNCTION

namespace X_IMAGE_BOUNDIMPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {TimeDateStamp, "TimeDateStamp", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {OffsetModuleName, "OffsetModuleName", 4, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {NumberOfModuleForwarderRefs, "NumberOfModuleForwarderRefs", 6, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_RELOCS {
const XFW_DEF::HEADER_RECORD records[] = {
    {VirtualAddress, "VirtualAddress", 0, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {SizeOfBlock, "SizeOfBlock", 4, 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_DEBUG {
const XFW_DEF::HEADER_RECORD records[] = {
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, Characteristics), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, MajorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, MinorVersion), 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Type, "Type", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, Type), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {SizeOfData, "SizeOfData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, SizeOfData), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {AddressOfRawData, "AddressOfRawData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, AddressOfRawData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {PointerToRawData, "PointerToRawData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, PointerToRawData), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_IMAGE_DELAYIMPORT {
const XFW_DEF::HEADER_RECORD records[] = {
    {AllAttributes, "AllAttributes", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, AllAttributes), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {DllNameRVA, "DllNameRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, DllNameRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {ModuleHandleRVA, "ModuleHandleRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ModuleHandleRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {ImportAddressTableRVA, "ImportAddressTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ImportAddressTableRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {ImportNameTableRVA, "ImportNameTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ImportNameTableRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {BoundImportAddressTableRVA, "BoundImportAddressTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, BoundImportAddressTableRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {UnloadInformationTableRVA, "UnloadInformationTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, UnloadInformationTableRVA), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, TimeDateStamp), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_EXCEPTIONS {
const XFW_DEF::HEADER_RECORD records[] = {
    {BeginAddress, "BeginAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, BeginAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS,
     -1},
    {EndAddress, "EndAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, EndAddress), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {UnwindInfoAddress, "UnwindInfoAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, UnwindInfoAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
};
}

namespace X_IMAGE_COR20_HEADER {
const XFW_DEF::HEADER_RECORD records[] = {
    {cb, "cb", offsetof(XPE_DEF::IMAGE_COR20_HEADER, cb), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {MajorRuntimeVersion, "MajorRuntimeVersion", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MajorRuntimeVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MinorRuntimeVersion, "MinorRuntimeVersion", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MinorRuntimeVersion), 2, "WORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION, -1},
    {MetaData_Address, "MetaData_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MetaData.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {MetaData_Size, "MetaData_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MetaData.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     MetaData_Address},
    {Flags, "Flags", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Flags), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {EntryPoint, "EntryPoint", offsetof(XPE_DEF::IMAGE_COR20_HEADER, EntryPointRVA), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {Resources_Address, "Resources_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Resources.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {Resources_Size, "Resources_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Resources.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     Resources_Address},
    {StrongNameSignature_Address, "StrongNameSignature_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, StrongNameSignature.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {StrongNameSignature_Size, "StrongNameSignature_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, StrongNameSignature.Size), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, StrongNameSignature_Address},
    {CodeManagerTable_Address, "CodeManagerTable_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, CodeManagerTable.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {CodeManagerTable_Size, "CodeManagerTable_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, CodeManagerTable.Size), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, CodeManagerTable_Address},
    {VTableFixups_Address, "VTableFixups_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, VTableFixups.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {VTableFixups_Size, "VTableFixups_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, VTableFixups.Size), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     VTableFixups_Address},
    {ExportAddressTableJumps_Address, "ExportAddressTableJumps_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ExportAddressTableJumps.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {ExportAddressTableJumps_Size, "ExportAddressTableJumps_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ExportAddressTableJumps.Size), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, ExportAddressTableJumps_Address},
    {ManagedNativeHeader_Address, "ManagedNativeHeader_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ManagedNativeHeader.VirtualAddress), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_RELADDRESS, -1},
    {ManagedNativeHeader_Size, "ManagedNativeHeader_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ManagedNativeHeader.Size), 4, "DWORD",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, ManagedNativeHeader_Address},
};
}

namespace X_IMAGE_RESOURCE_FIXEDFILEINFO {
const XFW_DEF::HEADER_RECORD records[] = {
    {dwSignature, "dwSignature", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwSignature), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwStrucVersion, "dwStrucVersion", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwStrucVersion), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileVersionMS, "dwFileVersionMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileVersionMS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileVersionLS, "dwFileVersionLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileVersionLS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwProductVersionMS, "dwProductVersionMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwProductVersionMS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwProductVersionLS, "dwProductVersionLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwProductVersionLS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileFlagsMask, "dwFileFlagsMask", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileFlagsMask), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileFlags, "dwFileFlags", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileFlags), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileOS, "dwFileOS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileOS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileType, "dwFileType", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileType), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileSubtype, "dwFileSubtype", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileSubtype), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileDateMS, "dwFileDateMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileDateMS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dwFileDateLS, "dwFileDateLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileDateLS), 4, "DWORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_IMAGE_RELOCS_POSITION {
const XFW_DEF::HEADER_RECORD records[] = {
    {TypeOffset, "TypeOffset", 0, 2, "WORD", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

}  // namespace XTYPE_PE
