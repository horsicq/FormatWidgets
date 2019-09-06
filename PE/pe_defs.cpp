// copyright (c) 2017-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "pe_defs.h"

namespace N_IMAGE_DOS_HEADER
{
const FormatWidget::HEADER_RECORD records[]=
{
    {e_magic,       "e_magic",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cblp,        "e_cblp",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cp,          "e_cp",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_crlc,        "e_crlc",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cparhdr,     "e_cparhdr",    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_minalloc,    "e_minalloc",   2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_maxalloc,    "e_maxalloc",   2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_ss,          "e_ss",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_sp,          "e_sp",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_csum,        "e_csum",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_ip,          "e_ip",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_cs,          "e_cs",         2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_lfarlc,      "e_lfarlc",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_ovno,        "e_ovno",       2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_0,       "e_res_0",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_1,       "e_res_1",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_2,       "e_res_2",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res_3,       "e_res_3",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_oemid,       "e_oemid",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_oeminfo,     "e_oeminfo",    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_0,      "e_res2_0",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_1,      "e_res2_1",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_2,      "e_res2_2",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_3,      "e_res2_3",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_4,      "e_res2_4",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_5,      "e_res2_5",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_6,      "e_res2_6",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_7,      "e_res2_7",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_8,      "e_res2_8",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_res2_9,      "e_res2_9",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {e_lfanew,      "e_lfanew",     4,      "LONG",         FormatWidget::VAL_TYPE_OFFSET}
};
}

namespace N_IMAGE_NT_HEADERS
{
const FormatWidget::HEADER_RECORD records[]=
{
    {Signature,     "Signature",    4,      "DWORD",         FormatWidget::VAL_TYPE_DATA}
};
}

namespace N_IMAGE_FILE_HEADER
{
const FormatWidget::HEADER_RECORD records[]=
{
    {Machine,               "Machine",              2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {NumberOfSections,      "NumberOfSections",     2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {TimeDateStamp,         "TimeDateStamp",        4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {PointerToSymbolTable,  "PointerToSymbolTable", 4,      "DWORD",        FormatWidget::VAL_TYPE_OFFSET},
    {NumberOfSymbols,       "NumberOfSymbols",      4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {SizeOfOptionalHeader,  "SizeOfOptionalHeader", 2,      "WORD",         FormatWidget::VAL_TYPE_SIZE},
    {Characteristics,       "Characteristics",      2,      "WORD",         FormatWidget::VAL_TYPE_FLAGS}
};
}

namespace N_IMAGE_OPTIONAL_HEADER
{
const FormatWidget::HEADER_RECORD records32[]=
{
    {Magic,                         "Magic",                        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MajorLinkerVersion,            "MajorLinkerVersion",           1,      "BYTE",         FormatWidget::VAL_TYPE_DATA},
    {MinorLinkerVersion,            "MinorLinkerVersion",           1,      "BYTE",         FormatWidget::VAL_TYPE_DATA},
    {SizeOfCode,                    "SizeOfCode",                   4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfInitializedData,         "SizeOfInitializedData",        4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfUninitializedData,       "SizeOfUninitializedData",      4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {AddressOfEntryPoint,           "AddressOfEntryPoint",          4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {BaseOfCode,                    "BaseOfCode",                   4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {BaseOfData,                    "BaseOfData",                   4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {ImageBase,                     "ImageBase",                    4,      "DWORD",        FormatWidget::VAL_TYPE_ADDRESS},
    {SectionAlignment,              "SectionAlignment",             4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {FileAlignment,                 "FileAlignment",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {MajorOperatingSystemVersion,   "MajorOperatingSystemVersion",  2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorOperatingSystemVersion,   "MinorOperatingSystemVersion",  2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MajorImageVersion,             "MajorImageVersion",            2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorImageVersion,             "MinorImageVersion",            2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MajorSubsystemVersion,         "MajorSubsystemVersion",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorSubsystemVersion,         "MinorSubsystemVersion",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {Win32VersionValue,             "Win32VersionValue",            4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {SizeOfImage,                   "SizeOfImage",                  4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfHeaders,                 "SizeOfHeaders",                4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {CheckSum,                      "CheckSum",                     4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {Subsystem,                     "Subsystem",                    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {DllCharacteristics,            "DllCharacteristics",           2,      "WORD",         FormatWidget::VAL_TYPE_FLAGS},
    {SizeOfStackReserve,            "SizeOfStackReserve",           4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfStackCommit,             "SizeOfStackCommit",            4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfHeapReserve,             "SizeOfHeapReserve",            4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfHeapCommit,              "SizeOfHeapCommit",             4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {LoaderFlags,                   "LoaderFlags",                  4,      "DWORD",        FormatWidget::VAL_TYPE_FLAGS},
    {NumberOfRvaAndSizes,           "NumberOfRvaAndSizes",          4,      "DWORD",        FormatWidget::VAL_TYPE_DATA}
};
const FormatWidget::HEADER_RECORD records64[]=
{
    {Magic,                         "Magic",                        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MajorLinkerVersion,            "MajorLinkerVersion",           1,      "BYTE",         FormatWidget::VAL_TYPE_DATA},
    {MinorLinkerVersion,            "MinorLinkerVersion",           1,      "BYTE",         FormatWidget::VAL_TYPE_DATA},
    {SizeOfCode,                    "SizeOfCode",                   4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfInitializedData,         "SizeOfInitializedData",        4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfUninitializedData,       "SizeOfUninitializedData",      4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {AddressOfEntryPoint,           "AddressOfEntryPoint",          4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {BaseOfCode,                    "BaseOfCode",                   4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {BaseOfData,                    "",                             0,      "",             FormatWidget::VAL_TYPE_UNKNOWN},
    {ImageBase,                     "ImageBase",                    8,      "ULONGLONG",    FormatWidget::VAL_TYPE_ADDRESS},
    {SectionAlignment,              "SectionAlignment",             4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {FileAlignment,                 "FileAlignment",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {MajorOperatingSystemVersion,   "MajorOperatingSystemVersion",  2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorOperatingSystemVersion,   "MinorOperatingSystemVersion",  2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MajorImageVersion,             "MajorImageVersion",            2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorImageVersion,             "MinorImageVersion",            2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MajorSubsystemVersion,         "MajorSubsystemVersion",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorSubsystemVersion,         "MinorSubsystemVersion",        2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {Win32VersionValue,             "Win32VersionValue",            4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {SizeOfImage,                   "SizeOfImage",                  4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {SizeOfHeaders,                 "SizeOfHeaders",                4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {CheckSum,                      "CheckSum",                     4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {Subsystem,                     "Subsystem",                    2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {DllCharacteristics,            "DllCharacteristics",           2,      "WORD",         FormatWidget::VAL_TYPE_FLAGS},
    {SizeOfStackReserve,            "SizeOfStackReserve",           8,      "ULONGLONG",    FormatWidget::VAL_TYPE_SIZE},
    {SizeOfStackCommit,             "SizeOfStackCommit",            8,      "ULONGLONG",    FormatWidget::VAL_TYPE_SIZE},
    {SizeOfHeapReserve,             "SizeOfHeapReserve",            8,      "ULONGLONG",    FormatWidget::VAL_TYPE_SIZE},
    {SizeOfHeapCommit,              "SizeOfHeapCommit",             8,      "ULONGLONG",    FormatWidget::VAL_TYPE_SIZE},
    {LoaderFlags,                   "LoaderFlags",                  4,      "DWORD",        FormatWidget::VAL_TYPE_FLAGS},
    {NumberOfRvaAndSizes,           "NumberOfRvaAndSizes",          4,      "DWORD",        FormatWidget::VAL_TYPE_DATA}
};
}

namespace N_IMAGE_TLS
{
const FormatWidget::HEADER_RECORD records32[]=
{
    {StartAddressOfRawData,         "StartAddressOfRawData",        4,      "DWORD",        FormatWidget::VAL_TYPE_ADDRESS},
    {EndAddressOfRawData,           "EndAddressOfRawData",          4,      "DWORD",        FormatWidget::VAL_TYPE_ADDRESS},
    {AddressOfIndex,                "AddressOfIndex",               4,      "DWORD",        FormatWidget::VAL_TYPE_ADDRESS},
    {AddressOfCallBacks,            "AddressOfCallBacks",           4,      "DWORD",        FormatWidget::VAL_TYPE_ADDRESS},
    {SizeOfZeroFill,                "SizeOfZeroFill",               4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {Characteristics,               "Characteristics",              4,      "DWORD",        FormatWidget::VAL_TYPE_FLAGS}
};
const FormatWidget::HEADER_RECORD records64[]=
{
    {StartAddressOfRawData,         "StartAddressOfRawData",        8,      "ULONGLONG",    FormatWidget::VAL_TYPE_ADDRESS},
    {EndAddressOfRawData,           "EndAddressOfRawData",          8,      "ULONGLONG",    FormatWidget::VAL_TYPE_ADDRESS},
    {AddressOfIndex,                "AddressOfIndex",               8,      "ULONGLONG",    FormatWidget::VAL_TYPE_ADDRESS},
    {AddressOfCallBacks,            "AddressOfCallBacks",           8,      "ULONGLONG",    FormatWidget::VAL_TYPE_ADDRESS},
    {SizeOfZeroFill,                "SizeOfZeroFill",               4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {Characteristics,               "Characteristics",              4,      "DWORD",        FormatWidget::VAL_TYPE_FLAGS}
};
}

namespace N_IMAGE_LOADCONFIG
{
const FormatWidget::HEADER_RECORD records32[]=
{
    {Size,                          "Size",                         4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {TimeDateStamp,                 "TimeDateStamp",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {MajorVersion,                  "MajorVersion",                 2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorVersion,                  "MinorVersion",                 2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {GlobalFlagsClear,              "GlobalFlagsClear",             4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {GlobalFlagsSet,                "GlobalFlagsSet",               4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout",4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
};
const FormatWidget::HEADER_RECORD records64[]=
{
    {Size,                          "Size",                         4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {TimeDateStamp,                 "TimeDateStamp",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {MajorVersion,                  "MajorVersion",                 2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorVersion,                  "MinorVersion",                 2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {GlobalFlagsClear,              "GlobalFlagsClear",             4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {GlobalFlagsSet,                "GlobalFlagsSet",               4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {CriticalSectionDefaultTimeout, "CriticalSectionDefaultTimeout",4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_DIRECORIES
{
const FormatWidget::DIRECTORY_ENTRY_RECORD records[]=
{
    {EXPORT,            "EXPORT",                   {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {IMPORT,            "IMPORT",                   {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {RESOURCE,          "RESOURCE",                 {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {EXCEPTION,         "EXCEPTION",                {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {SECURITY,          "SECURITY",                 {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}}, // TODO Check
    {BASERELOC,         "BASERELOC",                {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {DEBUG,             "DEBUG",                    {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {ARCHITECTURE,      "ARCHITECTURE",             {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {GLOBALPTR,         "GLOBALPTR",                {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {TLS,               "TLS",                      {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {LOAD_CONFIG,       "LOAD_CONFIG",              {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {BOUND_IMPORT,      "BOUND_IMPORT",             {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {IAT,               "IAT",                      {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {DELAY_IMPORT,      "DELAY_IMPORT",             {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {COM_DESCRIPTOR,    "COM_DESCRIPTOR",           {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}},
    {reserved,          "reserved",                 {4,4},    {"DWORD","DWORD"},        {FormatWidget::VAL_TYPE_RELADDRESS,FormatWidget::VAL_TYPE_SIZE}}
};
}

namespace N_IMAGE_SECTION_HEADER
{
const FormatWidget::HEADER_RECORD records[]=
{
    {Name,                  "Name",                     8,      "BYTE[8]",      FormatWidget::VAL_TYPE_TEXT},
    {VirtualSize,           "VirtualSize",              4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {VirtualAddress,        "VirtualAddress",           4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {SizeOfRawData,         "SizeOfRawData",            4,      "DWORD",        FormatWidget::VAL_TYPE_SIZE},
    {PointerToRawData,      "PointerToRawData",         4,      "DWORD",        FormatWidget::VAL_TYPE_OFFSET},
    {PointerToRelocations,  "PointerToRelocations",     4,      "DWORD",        FormatWidget::VAL_TYPE_OFFSET},
    {PointerToLinenumbers,  "PointerToLinenumbers",     4,      "DWORD",        FormatWidget::VAL_TYPE_OFFSET},
    {NumberOfRelocations,   "NumberOfRelocations",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {NumberOfLinenumbers,   "NumberOfLinenumbers",      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {Characteristics,       "Characteristics",          4,      "DWORD",        FormatWidget::VAL_TYPE_FLAGS},
    {Characteristics,       "",                         0,      "",             FormatWidget::VAL_TYPE_UNKNOWN}
};
}

namespace N_IMAGE_EXPORT
{
const FormatWidget::HEADER_RECORD records[]=
{
    {Characteristics,           "Characteristics",              4,      "DWORD",        FormatWidget::VAL_TYPE_FLAGS},
    {TimeDateStamp,             "TimeDateStamp",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {MajorVersion,              "MajorVersion",                 2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {MinorVersion,              "MinorVersion",                 2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {Name,                      "Name",                         4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {Base,                      "Base",                         4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {NumberOfFunctions,         "NumberOfFunctions",            4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {NumberOfNames,             "NumberOfNames",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {AddressOfFunctions,        "AddressOfFunctions",           4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {AddressOfNames,            "AddressOfNames",               4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {AddressOfNameOrdinals,     "AddressOfNameOrdinals",        4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS}
};
}

namespace N_IMAGE_EXPORT_FUNCTION
{
const FormatWidget::HEADER_RECORD records[]=
{
    {Ordinal,                   "Ordinal",                      2,      "WORD",         FormatWidget::VAL_TYPE_DATA},
    {RVA,                       "RVA",                          4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {Name,                      "Name",                         4,      "SZCHAR",       FormatWidget::VAL_TYPE_RELADDRESS}
};
}

namespace N_IMAGE_IMPORT
{
const FormatWidget::HEADER_RECORD records[]=
{
    {OriginalFirstThunk,        "OriginalFirstThunk",           4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {TimeDateStamp,             "TimeDateStamp",                4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {ForwarderChain,            "ForwarderChain",               4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {Name,                      "Name",                         4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {FirstThunk,                "FirstThunk",                   4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS}
};
}

namespace N_IMAGE_IMPORT_FUNCTION
{
const FormatWidget::HEADER_RECORD records32[]=
{
    {Thunk,                     "Thunk",                        4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {Ordinal,                   "Ordinal",                      4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
    {Hint,                      "Hint",                         4,      "DWORD",        FormatWidget::VAL_TYPE_DATA}
};
}

namespace N_IMAGE_RELOCS
{
const FormatWidget::HEADER_RECORD records[]=
{
    {VirtualAddress,            "VirtualAddress",               4,      "DWORD",        FormatWidget::VAL_TYPE_RELADDRESS},
    {SizeOfBlock,               "SizeOfBlock",                  4,      "DWORD",        FormatWidget::VAL_TYPE_DATA},
};
}

namespace N_IMAGE_RELOCS_POSITION
{
const FormatWidget::HEADER_RECORD records[]=
{
    {TypeOffset,                "TypeOffset",                   2,      "WORD",        FormatWidget::VAL_TYPE_DATA},
};
}
