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
#ifndef PE_DEFS_H
#define PE_DEFS_H

#include "../formatwidget.h"
#include "xpe.h"

namespace SPE
{
    enum TYPE
    {
        TYPE_TOOLS=0,
        TYPE_IMAGE_DOS_HEADER,
        TYPE_IMAGE_NT_HEADERS,
        TYPE_IMAGE_FILE_HEADER,
        TYPE_IMAGE_OPTIONAL_HEADER,
        TYPE_IMAGE_DIRECTORY_ENTRIES,
        TYPE_SECTIONS,
        TYPE_EXPORT,
        TYPE_IMPORT,
        TYPE_RESOURCE,
        TYPE_RELOCS,
        TYPE_OVERLAY,
        __TYPE_size,
        TYPE_IMAGE_SECTION_HEADER,
        TYPE_EXPORT_FUNCTION,
        TYPE_IMPORT_FUNCTION,
        TYPE_RELOCS_POSITION
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

    extern const FormatWidget::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_NT_HEADERS
{
    enum DATA
    {
        Signature=0,
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records[__data_size];
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

    extern const FormatWidget::HEADER_RECORD records[__data_size];
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
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records32[__data_size];
    extern const FormatWidget::HEADER_RECORD records64[__data_size];
}

namespace N_IMAGE_DIRECORIES
{
    enum DATA
    {
        EXPORT=0,
        IMPORT,
        RESOURCE,
        EXCEPTION,
        SECURITY,
        BASERELOC,
        DEBUG,
        ARCHITECTURE,
        GLOBALPTR,
        TLS,
        LOAD_CONFIG,
        BOUND_IMPORT,
        IAT,
        DELAY_IMPORT,
        COM_DESCRIPTOR,
        reserved,
        __data_size
    };

    extern const FormatWidget::DIRECTORY_ENTRY_RECORD records[__data_size];
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
        Characteristics,
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records[__data_size+1];
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

    extern const FormatWidget::HEADER_RECORD records[__data_size];
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

    extern const FormatWidget::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_IMPORT
{
    enum DATA
    {
        OriginalFirstThunk,
        TimeDateStamp,
        ForwarderChain,
        Name,
        FirstThunk,
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records[__data_size];
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

    extern const FormatWidget::HEADER_RECORD records32[__data_size];
}

namespace N_IMAGE_RELOCS
{
    enum DATA
    {
        VirtualAddress=0,
        SizeOfBlock,
        __data_size
    };

    extern const FormatWidget::HEADER_RECORD records[__data_size];
}

namespace N_IMAGE_RELOCS_POSITION
{
enum DATA
{
    TypeOffset=0,
    __data_size
};

extern const FormatWidget::HEADER_RECORD records[__data_size];
}

#endif
