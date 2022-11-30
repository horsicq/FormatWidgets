/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "pe_defs.h"

namespace N_IMAGE_DOS_HEADER {
const FW_DEF::HEADER_RECORD records[] = {{e_magic, "e_magic", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_magic), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cblp, "e_cblp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cblp), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cp, "e_cp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cp), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_crlc, "e_crlc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_crlc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cparhdr, "e_cparhdr", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cparhdr), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_minalloc, "e_minalloc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_minalloc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_maxalloc, "e_maxalloc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_maxalloc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_ss, "e_ss", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ss), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_sp, "e_sp", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_sp), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_csum, "e_csum", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_csum), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_ip, "e_ip", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ip), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_cs, "e_cs", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_cs), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_lfarlc, "e_lfarlc", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_lfarlc), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_ovno, "e_ovno", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_ovno), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_0, "e_res_0", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_1, "e_res_1", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 2, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_2, "e_res_2", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 4, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res_3, "e_res_3", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res) + 6, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_oemid, "e_oemid", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_oemid), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_oeminfo, "e_oeminfo", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_oeminfo), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_0, "e_res2_0", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_1, "e_res2_1", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 2, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_2, "e_res2_2", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 4, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_3, "e_res2_3", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 6, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_4, "e_res2_4", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 8, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_5, "e_res2_5", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 10, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_6, "e_res2_6", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 12, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_7, "e_res2_7", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 14, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_8, "e_res2_8", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 16, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_res2_9, "e_res2_9", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_res2) + 18, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {e_lfanew, "e_lfanew", offsetof(XMSDOS_DEF::IMAGE_DOS_HEADEREX, e_lfanew), 4, "LONG", FW_DEF::VAL_TYPE_OFFSET}};
}

namespace N_IMAGE_NT_HEADERS {
const FW_DEF::HEADER_RECORD records[] = {{Signature, "Signature", offsetof(XPE_DEF::IMAGE_NT_HEADERS32, Signature), 4, "DWORD", FW_DEF::VAL_TYPE_DATA}};
}

namespace N_IMAGE_FILE_HEADER {
const FW_DEF::HEADER_RECORD records[] = {
    {Machine, "Machine", offsetof(XPE_DEF::IMAGE_FILE_HEADER, Machine), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {NumberOfSections, "NumberOfSections", offsetof(XPE_DEF::IMAGE_FILE_HEADER, NumberOfSections), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_FILE_HEADER, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {PointerToSymbolTable, "PointerToSymbolTable", offsetof(XPE_DEF::IMAGE_FILE_HEADER, PointerToSymbolTable), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {NumberOfSymbols, "NumberOfSymbols", offsetof(XPE_DEF::IMAGE_FILE_HEADER, NumberOfSymbols), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {SizeOfOptionalHeader, "SizeOfOptionalHeader", offsetof(XPE_DEF::IMAGE_FILE_HEADER, SizeOfOptionalHeader), 2, "WORD", FW_DEF::VAL_TYPE_SIZE},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_FILE_HEADER, Characteristics), 2, "WORD", FW_DEF::VAL_TYPE_FLAGS}};
}

namespace N_IMAGE_OPTIONAL_HEADER {
const FW_DEF::HEADER_RECORD records32[] = {
    {Magic, "Magic", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Magic), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MajorLinkerVersion, "MajorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorLinkerVersion), 1, "BYTE", FW_DEF::VAL_TYPE_DATA},
    {MinorLinkerVersion, "MinorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorLinkerVersion), 1, "BYTE", FW_DEF::VAL_TYPE_DATA},
    {SizeOfCode, "SizeOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfCode), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfInitializedData, "SizeOfInitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfInitializedData), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfUninitializedData, "SizeOfUninitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfUninitializedData), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {AddressOfEntryPoint, "AddressOfEntryPoint", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, AddressOfEntryPoint), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {BaseOfCode, "BaseOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, BaseOfCode), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {BaseOfData, "BaseOfData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, BaseOfData), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {ImageBase, "ImageBase", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, ImageBase), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {SectionAlignment, "SectionAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SectionAlignment), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {FileAlignment, "FileAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, FileAlignment), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {MajorOperatingSystemVersion, "MajorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorOperatingSystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorOperatingSystemVersion, "MinorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorOperatingSystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MajorImageVersion, "MajorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorImageVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorImageVersion, "MinorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorImageVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MajorSubsystemVersion, "MajorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MajorSubsystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorSubsystemVersion, "MinorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, MinorSubsystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Win32VersionValue, "Win32VersionValue", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Win32VersionValue), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {SizeOfImage, "SizeOfImage", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfImage), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfHeaders, "SizeOfHeaders", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeaders), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {CheckSum, "CheckSum", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, CheckSum), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {Subsystem, "Subsystem", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, Subsystem), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {DllCharacteristics, "DllCharacteristics", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, DllCharacteristics), 2, "WORD", FW_DEF::VAL_TYPE_FLAGS},
    {SizeOfStackReserve, "SizeOfStackReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfStackReserve), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfStackCommit, "SizeOfStackCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfStackCommit), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfHeapReserve, "SizeOfHeapReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeapReserve), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfHeapCommit, "SizeOfHeapCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, SizeOfHeapCommit), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {LoaderFlags, "LoaderFlags", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, LoaderFlags), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS},
    {NumberOfRvaAndSizes, "NumberOfRvaAndSizes", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER32, NumberOfRvaAndSizes), 4, "DWORD", FW_DEF::VAL_TYPE_DATA}};
const FW_DEF::HEADER_RECORD records64[] = {
    {Magic, "Magic", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Magic), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MajorLinkerVersion, "MajorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorLinkerVersion), 1, "BYTE", FW_DEF::VAL_TYPE_DATA},
    {MinorLinkerVersion, "MinorLinkerVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorLinkerVersion), 1, "BYTE", FW_DEF::VAL_TYPE_DATA},
    {SizeOfCode, "SizeOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfCode), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfInitializedData, "SizeOfInitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfInitializedData), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfUninitializedData, "SizeOfUninitializedData", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfUninitializedData), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {AddressOfEntryPoint, "AddressOfEntryPoint", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, AddressOfEntryPoint), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {BaseOfCode, "BaseOfCode", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, BaseOfCode), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {BaseOfData, "", 0, 0, "", FW_DEF::VAL_TYPE_UNKNOWN},
    {ImageBase, "ImageBase", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, ImageBase), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {SectionAlignment, "SectionAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SectionAlignment), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {FileAlignment, "FileAlignment", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, FileAlignment), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {MajorOperatingSystemVersion, "MajorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorOperatingSystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorOperatingSystemVersion, "MinorOperatingSystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorOperatingSystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MajorImageVersion, "MajorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorImageVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorImageVersion, "MinorImageVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorImageVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MajorSubsystemVersion, "MajorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MajorSubsystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorSubsystemVersion, "MinorSubsystemVersion", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, MinorSubsystemVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Win32VersionValue, "Win32VersionValue", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Win32VersionValue), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {SizeOfImage, "SizeOfImage", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfImage), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfHeaders, "SizeOfHeaders", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeaders), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {CheckSum, "CheckSum", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, CheckSum), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {Subsystem, "Subsystem", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, Subsystem), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {DllCharacteristics, "DllCharacteristics", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, DllCharacteristics), 2, "WORD", FW_DEF::VAL_TYPE_FLAGS},
    {SizeOfStackReserve, "SizeOfStackReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfStackReserve), 8, "ULONGLONG", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfStackCommit, "SizeOfStackCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfStackCommit), 8, "ULONGLONG", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfHeapReserve, "SizeOfHeapReserve", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeapReserve), 8, "ULONGLONG", FW_DEF::VAL_TYPE_SIZE},
    {SizeOfHeapCommit, "SizeOfHeapCommit", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, SizeOfHeapCommit), 8, "ULONGLONG", FW_DEF::VAL_TYPE_SIZE},
    {LoaderFlags, "LoaderFlags", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, LoaderFlags), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS},
    {NumberOfRvaAndSizes, "NumberOfRvaAndSizes", offsetof(XPE_DEF::IMAGE_OPTIONAL_HEADER64, NumberOfRvaAndSizes), 4, "DWORD", FW_DEF::VAL_TYPE_DATA}};
}  // namespace N_IMAGE_OPTIONAL_HEADER

namespace N_IMAGE_TLS {
const FW_DEF::HEADER_RECORD records32[] = {
    {StartAddressOfRawData, "StartAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, StartAddressOfRawData), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {EndAddressOfRawData, "EndAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, EndAddressOfRawData), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {AddressOfIndex, "AddressOfIndex", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, AddressOfIndex), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {AddressOfCallBacks, "AddressOfCallBacks", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, AddressOfCallBacks), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {SizeOfZeroFill, "SizeOfZeroFill", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, SizeOfZeroFill), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY32, Characteristics), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS}};
const FW_DEF::HEADER_RECORD records64[] = {
    {StartAddressOfRawData, "StartAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, StartAddressOfRawData), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {EndAddressOfRawData, "EndAddressOfRawData", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, EndAddressOfRawData), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {AddressOfIndex, "AddressOfIndex", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, AddressOfIndex), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {AddressOfCallBacks, "AddressOfCallBacks", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, AddressOfCallBacks), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {SizeOfZeroFill, "SizeOfZeroFill", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, SizeOfZeroFill), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_TLS_DIRECTORY64, Characteristics), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS}};
}  // namespace N_IMAGE_TLS

namespace N_IMAGE_LOADCONFIG {
const FW_DEF::HEADER_RECORD records32[] = {
    {Size, "Size", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Size), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MajorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MinorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {GlobalFlagsClear, "GlobalFlagsClear", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GlobalFlagsClear), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {GlobalFlagsSet, "GlobalFlagsSet", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GlobalFlagsSet), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CriticalSectionDefaultTimeout), 4, "DWORD",
     FW_DEF::VAL_TYPE_DATA},
    {DeCommitFreeBlockThreshold, "DeCommitFreeBlockThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DeCommitFreeBlockThreshold), 4, "DWORD",
     FW_DEF::VAL_TYPE_DATA},
    {DeCommitTotalFreeThreshold, "DeCommitTotalFreeThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DeCommitTotalFreeThreshold), 4, "DWORD",
     FW_DEF::VAL_TYPE_DATA},
    {LockPrefixTable, "LockPrefixTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, LockPrefixTable), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {MaximumAllocationSize, "MaximumAllocationSize", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, MaximumAllocationSize), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {VirtualMemoryThreshold, "VirtualMemoryThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, VirtualMemoryThreshold), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {ProcessAffinityMask, "ProcessAffinityMask", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, ProcessAffinityMask), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {CSDVersion, "CSDVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CSDVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {DependentLoadFlags, "DependentLoadFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DependentLoadFlags), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {VirtualMemoryThreshold, "EditList", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, EditList), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {VirtualMemoryThreshold, "SecurityCookie", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SecurityCookie), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {VirtualMemoryThreshold, "SEHandlerTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerTable), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {VirtualMemoryThreshold, "SEHandlerCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerCount), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {GuardCFCheckFunctionPointer, "GuardCFCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFCheckFunctionPointer), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardCFDispatchFunctionPointer, "GuardCFDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFDispatchFunctionPointer), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardCFFunctionTable, "GuardCFFunctionTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFFunctionTable), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardCFFunctionCount, "GuardCFFunctionCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardCFFunctionCount), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {GuardFlags, "GuardFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardFlags), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {CodeIntegrity_Flags, "CodeIntegrity_Flags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {CodeIntegrity_Catalog, "CodeIntegrity_Catalog", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 2, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {CodeIntegrity_CatalogOffset, "CodeIntegrity_CatalogOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 4, 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {CodeIntegrity_Reserved, "CodeIntegrity_Reserved", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) + 8, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {GuardAddressTakenIatEntryTable, "GuardAddressTakenIatEntryTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardAddressTakenIatEntryTable), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardAddressTakenIatEntryCount, "GuardAddressTakenIatEntryCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardAddressTakenIatEntryCount), 4, "DWORD",
     FW_DEF::VAL_TYPE_DATA},
    {GuardLongJumpTargetTable, "GuardLongJumpTargetTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardLongJumpTargetTable), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardLongJumpTargetCount, "GuardLongJumpTargetCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardLongJumpTargetCount), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {DynamicValueRelocTable, "DynamicValueRelocTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTable), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {CHPEMetadataPointer, "CHPEMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CHPEMetadataPointer), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardRFFailureRoutine, "GuardRFFailureRoutine", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFFailureRoutine), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardRFFailureRoutineFunctionPointer, "GuardRFFailureRoutineFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFFailureRoutineFunctionPointer), 4,
     "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {DynamicValueRelocTableOffset, "DynamicValueRelocTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTableOffset), 4, "DWORD",
     FW_DEF::VAL_TYPE_OFFSET},
    {DynamicValueRelocTableSection, "DynamicValueRelocTableSection", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, DynamicValueRelocTableSection), 2, "WORD",
     FW_DEF::VAL_TYPE_DATA},
    {Reserved2, "Reserved2", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Reserved2), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {GuardRFVerifyStackPointerFunctionPointer, "GuardRFVerifyStackPointerFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardRFVerifyStackPointerFunctionPointer), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {HotPatchTableOffset, "HotPatchTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, HotPatchTableOffset), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {Reserved3, "Reserved3", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, Reserved3), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {EnclaveConfigurationPointer, "EnclaveConfigurationPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, EnclaveConfigurationPointer), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {VolatileMetadataPointer, "VolatileMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, VolatileMetadataPointer), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardEHContinuationTable, "GuardEHContinuationTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardEHContinuationTable), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardEHContinuationCount, "GuardEHContinuationCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardEHContinuationCount), 4, "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardXFGCheckFunctionPointer, "GuardXFGCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGCheckFunctionPointer), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardXFGDispatchFunctionPointer, "GuardXFGDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGDispatchFunctionPointer), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardXFGTableDispatchFunctionPointer, "GuardXFGTableDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardXFGTableDispatchFunctionPointer), 4,
     "DWORD", FW_DEF::VAL_TYPE_ADDRESS},
    {CastGuardOsDeterminedFailureMode, "CastGuardOsDeterminedFailureMode", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, CastGuardOsDeterminedFailureMode), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardMemcpyFunctionPointer, "GuardMemcpyFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32, GuardMemcpyFunctionPointer), 4, "DWORD",
     FW_DEF::VAL_TYPE_ADDRESS},
};
const FW_DEF::HEADER_RECORD records64[] = {
    {Size, "Size", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Size), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MajorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MinorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {GlobalFlagsClear, "GlobalFlagsClear", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GlobalFlagsClear), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {GlobalFlagsSet, "GlobalFlagsSet", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GlobalFlagsSet), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CriticalSectionDefaultTimeout), 4, "DWORD",
     FW_DEF::VAL_TYPE_DATA},
    {DeCommitFreeBlockThreshold, "DeCommitFreeBlockThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DeCommitFreeBlockThreshold), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_DATA},
    {DeCommitTotalFreeThreshold, "DeCommitTotalFreeThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DeCommitTotalFreeThreshold), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_DATA},
    {LockPrefixTable, "LockPrefixTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, LockPrefixTable), 8, "ULONGLONG", FW_DEF::VAL_TYPE_DATA},
    {MaximumAllocationSize, "MaximumAllocationSize", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, MaximumAllocationSize), 8, "ULONGLONG", FW_DEF::VAL_TYPE_DATA},
    {VirtualMemoryThreshold, "VirtualMemoryThreshold", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, VirtualMemoryThreshold), 8, "ULONGLONG", FW_DEF::VAL_TYPE_DATA},
    {ProcessAffinityMask, "ProcessAffinityMask", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, ProcessAffinityMask), 8, "ULONGLONG", FW_DEF::VAL_TYPE_DATA},
    {CSDVersion, "CSDVersion", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CSDVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {DependentLoadFlags, "DependentLoadFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DependentLoadFlags), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {VirtualMemoryThreshold, "EditList", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, EditList), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {VirtualMemoryThreshold, "SecurityCookie", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SecurityCookie), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {VirtualMemoryThreshold, "SEHandlerTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SEHandlerTable), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {VirtualMemoryThreshold, "SEHandlerCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, SEHandlerCount), 8, "ULONGLONG", FW_DEF::VAL_TYPE_DATA},
    {GuardCFCheckFunctionPointer, "GuardCFCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFCheckFunctionPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardCFDispatchFunctionPointer, "GuardCFDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFDispatchFunctionPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardCFFunctionTable, "GuardCFFunctionTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFFunctionTable), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardCFFunctionCount, "GuardCFFunctionCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardCFFunctionCount), 8, "ULONGLONG", FW_DEF::VAL_TYPE_DATA},
    {GuardFlags, "GuardFlags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardFlags), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {CodeIntegrity_Flags, "CodeIntegrity_Flags", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {CodeIntegrity_Catalog, "CodeIntegrity_Catalog", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 2, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {CodeIntegrity_CatalogOffset, "CodeIntegrity_CatalogOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 4, 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {CodeIntegrity_Reserved, "CodeIntegrity_Reserved", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity) + 8, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {GuardAddressTakenIatEntryTable, "GuardAddressTakenIatEntryTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardAddressTakenIatEntryTable), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardAddressTakenIatEntryCount, "GuardAddressTakenIatEntryCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardAddressTakenIatEntryCount), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_DATA},
    {GuardLongJumpTargetTable, "GuardLongJumpTargetTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardLongJumpTargetTable), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardLongJumpTargetCount, "GuardLongJumpTargetCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardLongJumpTargetCount), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_DATA},
    {DynamicValueRelocTable, "DynamicValueRelocTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTable), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {CHPEMetadataPointer, "CHPEMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CHPEMetadataPointer), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardRFFailureRoutine, "GuardRFFailureRoutine", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFFailureRoutine), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardRFFailureRoutineFunctionPointer, "GuardRFFailureRoutineFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFFailureRoutineFunctionPointer), 8,
     "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {DynamicValueRelocTableOffset, "DynamicValueRelocTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTableOffset), 4, "DWORD",
     FW_DEF::VAL_TYPE_OFFSET},
    {DynamicValueRelocTableSection, "DynamicValueRelocTableSection", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, DynamicValueRelocTableSection), 2, "WORD",
     FW_DEF::VAL_TYPE_DATA},
    {Reserved2, "Reserved2", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Reserved2), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {GuardRFVerifyStackPointerFunctionPointer, "GuardRFVerifyStackPointerFunctionPointer",
     offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardRFVerifyStackPointerFunctionPointer), 8, "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {HotPatchTableOffset, "HotPatchTableOffset", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, HotPatchTableOffset), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {Reserved3, "Reserved3", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, Reserved3), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {EnclaveConfigurationPointer, "EnclaveConfigurationPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, EnclaveConfigurationPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {VolatileMetadataPointer, "VolatileMetadataPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, VolatileMetadataPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardEHContinuationTable, "GuardEHContinuationTable", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardEHContinuationTable), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardEHContinuationCount, "GuardEHContinuationCount", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardEHContinuationCount), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardXFGCheckFunctionPointer, "GuardXFGCheckFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGCheckFunctionPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardXFGDispatchFunctionPointer, "GuardXFGDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGDispatchFunctionPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
    {GuardXFGTableDispatchFunctionPointer, "GuardXFGTableDispatchFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardXFGTableDispatchFunctionPointer), 8,
     "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {CastGuardOsDeterminedFailureMode, "CastGuardOsDeterminedFailureMode", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, CastGuardOsDeterminedFailureMode), 8,
     "ULONGLONG", FW_DEF::VAL_TYPE_ADDRESS},
    {GuardMemcpyFunctionPointer, "GuardMemcpyFunctionPointer", offsetof(XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64, GuardMemcpyFunctionPointer), 8, "ULONGLONG",
     FW_DEF::VAL_TYPE_ADDRESS},
};
}  // namespace N_IMAGE_LOADCONFIG

namespace N_IMAGE_DATA_DIRECTORY {
const FW_DEF::HEADER_RECORD records[] = {
    {Address, QObject::tr("Address"), 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {Size, QObject::tr("Size"), 4, 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_IMAGE_SECTION_HEADER {
const FW_DEF::HEADER_RECORD records[] = {
    {Name, QObject::tr("Name"), offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Name), 8, "BYTE[8]", FW_DEF::VAL_TYPE_TEXT},
    {VirtualSize, "VirtualSize", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Misc.VirtualSize), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {VirtualAddress, "VirtualAddress", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, VirtualAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {SizeOfRawData, "SizeOfRawData", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, SizeOfRawData), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {PointerToRawData, "PointerToRawData", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToRawData), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {PointerToRelocations, "PointerToRelocations", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToRelocations), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {PointerToLinenumbers, "PointerToLinenumbers", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, PointerToLinenumbers), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
    {NumberOfRelocations, "NumberOfRelocations", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, NumberOfRelocations), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {NumberOfLinenumbers, "NumberOfLinenumbers", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, NumberOfLinenumbers), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_SECTION_HEADER, Characteristics), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS},
    {Characteristics, "", -1, 0, "", FW_DEF::VAL_TYPE_UNKNOWN}};
}

namespace N_IMAGE_RESOURCES {
const FW_DEF::HEADER_RECORD records[] = {
    {ID1, "ID1", -1, 0, "", FW_DEF::VAL_TYPE_DATA},
    {ID2, "ID2", -1, 0, "", FW_DEF::VAL_TYPE_DATA},
    {ID3, "ID3", -1, 0, "", FW_DEF::VAL_TYPE_DATA},
    {ADDRESS, QObject::tr("Address"), -1, 0, "", FW_DEF::VAL_TYPE_ADDRESS},
    {OFFSET, QObject::tr("Offset"), -1, 0, "", FW_DEF::VAL_TYPE_OFFSET},
    {SIZE, QObject::tr("Size"), -1, 0, "", FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_IMAGE_EXPORT {
const FW_DEF::HEADER_RECORD records[] = {
    {Characteristics, "Characteristics", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Characteristics), 4, "DWORD", FW_DEF::VAL_TYPE_FLAGS},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, MajorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, MinorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Name, "Name", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Name), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {Base, "Base", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, Base), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {NumberOfFunctions, "NumberOfFunctions", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, NumberOfFunctions), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {NumberOfNames, "NumberOfNames", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, NumberOfNames), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {AddressOfFunctions, "AddressOfFunctions", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfFunctions), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {AddressOfNames, "AddressOfNames", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfNames), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {AddressOfNameOrdinals, "AddressOfNameOrdinals", offsetof(XPE_DEF::IMAGE_EXPORT_DIRECTORY, AddressOfNameOrdinals), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS}};
}

namespace N_IMAGE_EXPORT_FUNCTION {
const FW_DEF::HEADER_RECORD records[] = {{Ordinal, "Ordinal", 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
                                         {RVA, "RVA", 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                         {Name, QObject::tr("Name"), 0, 4, "SZCHAR", FW_DEF::VAL_TYPE_RELADDRESS}};
}

namespace N_IMAGE_IMPORT {
const FW_DEF::HEADER_RECORD records[] = {
    {OriginalFirstThunk, "OriginalFirstThunk", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, OriginalFirstThunk), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {ForwarderChain, "ForwarderChain", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, ForwarderChain), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {Name, QObject::tr("Name"), offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, Name), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {FirstThunk, "FirstThunk", offsetof(XPE_DEF::IMAGE_IMPORT_DESCRIPTOR, FirstThunk), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS}};
}

namespace N_IMAGE_IMPORT_FUNCTION {
const FW_DEF::HEADER_RECORD records32[] = {{Thunk, "Thunk", 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {Ordinal, "Ordinal", 0, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
                                           {Hint, "Hint", 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA}};
const FW_DEF::HEADER_RECORD records64[] = {{Thunk, "Thunk", 0, 8, "QWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {Ordinal, "Ordinal", 0, 8, "QWORD", FW_DEF::VAL_TYPE_DATA},
                                           {Hint, "Hint", 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA}};
}  // namespace N_IMAGE_IMPORT_FUNCTION

namespace N_IMAGE_DELAYIMPORT_FUNCTION {
const FW_DEF::HEADER_RECORD records32[] = {{AddressThunk, "AddressThunk", 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {BoundThunk, "BoundThunk", 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {NameThunk, "NameThunk", 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {Ordinal, "Ordinal", 0, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
                                           {Hint, "Hint", 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA}};
const FW_DEF::HEADER_RECORD records64[] = {{AddressThunk, "AddressThunk", 0, 8, "QWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {BoundThunk, "BoundThunk", 0, 8, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {NameThunk, "NameThunk", 0, 8, "QWORD", FW_DEF::VAL_TYPE_RELADDRESS},
                                           {Ordinal, "Ordinal", 0, 8, "QWORD", FW_DEF::VAL_TYPE_DATA},
                                           {Hint, "Hint", 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA}};
}  // namespace N_IMAGE_DELAYIMPORT_FUNCTION

namespace N_IMAGE_BOUNDIMPORT {
const FW_DEF::HEADER_RECORD records[] = {
    {TimeDateStamp, "TimeDateStamp", 0, 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {OffsetModuleName, "OffsetModuleName", 4, 2, "WORD", FW_DEF::VAL_TYPE_OFFSET},
    {NumberOfModuleForwarderRefs, "NumberOfModuleForwarderRefs", 6, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_RELOCS {
const FW_DEF::HEADER_RECORD records[] = {
    {VirtualAddress, "VirtualAddress", 0, 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {SizeOfBlock, "SizeOfBlock", 4, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_DEBUG {
const FW_DEF::HEADER_RECORD records[] = {
    {Characteristics, "Characteristics", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, Characteristics), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
    {MajorVersion, "MajorVersion", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, MajorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorVersion, "MinorVersion", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, MinorVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Type, "Type", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, Type), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {SizeOfData, "SizeOfData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, SizeOfData), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {AddressOfRawData, "AddressOfRawData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, AddressOfRawData), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {PointerToRawData, "PointerToRawData", offsetof(XPE_DEF::S_IMAGE_DEBUG_DIRECTORY, PointerToRawData), 4, "DWORD", FW_DEF::VAL_TYPE_OFFSET},
};
}

namespace N_IMAGE_DELAYIMPORT {
const FW_DEF::HEADER_RECORD records[] = {
    {AllAttributes, "AllAttributes", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, AllAttributes), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {DllNameRVA, "DllNameRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, DllNameRVA), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {ModuleHandleRVA, "ModuleHandleRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ModuleHandleRVA), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {ImportAddressTableRVA, "ImportAddressTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ImportAddressTableRVA), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {ImportNameTableRVA, "ImportNameTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, ImportNameTableRVA), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {BoundImportAddressTableRVA, "BoundImportAddressTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, BoundImportAddressTableRVA), 4, "DWORD",
     FW_DEF::VAL_TYPE_RELADDRESS},
    {UnloadInformationTableRVA, "UnloadInformationTableRVA", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, UnloadInformationTableRVA), 4, "DWORD",
     FW_DEF::VAL_TYPE_RELADDRESS},
    {TimeDateStamp, "TimeDateStamp", offsetof(XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR, TimeDateStamp), 4, "DWORD", FW_DEF::VAL_TYPE_UNIXTIME},
};
}

namespace N_IMAGE_EXCEPTIONS {
const FW_DEF::HEADER_RECORD records[] = {
    {BeginAddress, "BeginAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, BeginAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {EndAddress, "EndAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, EndAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {UnwindInfoAddress, "UnwindInfoAddress", offsetof(XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY, UnwindInfoAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
};
}

namespace N_IMAGE_NETHEADER {
const FW_DEF::HEADER_RECORD records[] = {
    {cb, "cb", offsetof(XPE_DEF::IMAGE_COR20_HEADER, cb), 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {MajorRuntimeVersion, "MajorRuntimeVersion", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MajorRuntimeVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorRuntimeVersion, "MinorRuntimeVersion", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MinorRuntimeVersion), 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MetaData_Address, "MetaData_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MetaData.VirtualAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {MetaData_Size, "MetaData_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, MetaData.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {Flags, "Flags", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Flags), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {EntryPoint, "EntryPoint", offsetof(XPE_DEF::IMAGE_COR20_HEADER, EntryPointRVA), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {Resources_Address, "Resources_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Resources.VirtualAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {Resources_Size, "Resources_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, Resources.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {StrongNameSignature_Address, "StrongNameSignature_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, StrongNameSignature.VirtualAddress), 4, "DWORD",
     FW_DEF::VAL_TYPE_RELADDRESS},
    {StrongNameSignature_Size, "StrongNameSignature_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, StrongNameSignature.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {CodeManagerTable_Address, "CodeManagerTable_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, CodeManagerTable.VirtualAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {CodeManagerTable_Size, "CodeManagerTable_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, CodeManagerTable.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {VTableFixups_Address, "VTableFixups_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, VTableFixups.VirtualAddress), 4, "DWORD", FW_DEF::VAL_TYPE_RELADDRESS},
    {VTableFixups_Size, "VTableFixups_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, VTableFixups.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {ExportAddressTableJumps_Address, "ExportAddressTableJumps_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ExportAddressTableJumps.VirtualAddress), 4, "DWORD",
     FW_DEF::VAL_TYPE_RELADDRESS},
    {ExportAddressTableJumps_Size, "ExportAddressTableJumps_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ExportAddressTableJumps.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {ManagedNativeHeader_Address, "ManagedNativeHeader_Address", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ManagedNativeHeader.VirtualAddress), 4, "DWORD",
     FW_DEF::VAL_TYPE_RELADDRESS},
    {ManagedNativeHeader_Size, "ManagedNativeHeader_Size", offsetof(XPE_DEF::IMAGE_COR20_HEADER, ManagedNativeHeader.Size), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_NET_METADATA {
const FW_DEF::HEADER_RECORD records[] = {
    {Signature, "Signature", 0, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {MajorVersion, "MajorVersion", 4, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {MinorVersion, "MinorVersion", 6, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Reserved, "Reserved", 8, 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {VersionStringLength, "VersionStringLength", 12, 4, "DWORD", FW_DEF::VAL_TYPE_SIZE},
    {Version, "Version", 16, 1, "TEXT", FW_DEF::VAL_TYPE_TEXT},
    {Flags, "Flags", -1, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
    {Streams, "Streams", -1, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_RESOURCE_FIXEDFILEINFO {
const FW_DEF::HEADER_RECORD records[] = {
    {dwSignature, "dwSignature", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwSignature), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwStrucVersion, "dwStrucVersion", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwStrucVersion), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileVersionMS, "dwFileVersionMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileVersionMS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileVersionLS, "dwFileVersionLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileVersionLS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwProductVersionMS, "dwProductVersionMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwProductVersionMS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwProductVersionLS, "dwProductVersionLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwProductVersionLS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileFlagsMask, "dwFileFlagsMask", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileFlagsMask), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileFlags, "dwFileFlags", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileFlags), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileOS, "dwFileOS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileOS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileType, "dwFileType", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileType), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileSubtype, "dwFileSubtype", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileSubtype), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileDateMS, "dwFileDateMS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileDateMS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
    {dwFileDateLS, "dwFileDateLS", offsetof(XPE_DEF::tagVS_FIXEDFILEINFO, dwFileDateLS), 4, "DWORD", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_RELOCS_POSITION {
const FW_DEF::HEADER_RECORD records[] = {
    {TypeOffset, "TypeOffset", 0, 2, "WORD", FW_DEF::VAL_TYPE_DATA},
};
}
