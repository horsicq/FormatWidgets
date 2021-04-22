// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#ifndef MACH_DEFS_H
#define MACH_DEFS_H

#include "../formatwidget.h"
#include "xmach.h"

namespace SMACH
{
enum TYPE
{
    TYPE_HEX=0,
    TYPE_HASH,
    TYPE_DISASM,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_HEURISTICSCAN,
    TYPE_mach_header,
    TYPE_mach_commands,
    TYPE_mach_segments,
    TYPE_mach_sections,
    TYPE_mach_libraries,
    TYPE_mach_weak_libraries,
    TYPE_mach_id_library,
    TYPE_mach_dyld_info_only,
    TYPE_mach_uuid,
    TYPE_mach_symtab,
    TYPE_mach_dysymtab,
    TYPE_mach_version_min,
    TYPE_mach_dylinker,
    TYPE_mach_rpath,
    TYPE_mach_source_version,
    TYPE_mach_encryption_info,
    TYPE_mach_function_starts,
    TYPE_mach_data_in_code,
    TYPE_mach_code_signature,
    TYPE_mach_main,
    TYPE_mach_unix_thread,
    __TYPE_size
};
}

namespace N_mach_header
{
enum DATA
{
    magic=0,
    cputype,
    cpusubtype,
    filetype,
    ncmds,
    sizeofcmds,
    flags,
    reserved,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size-1];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_mach_commands
{
enum DATA
{
    cmd=0,
    cmdsize,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_CMD,
    __CB_size
};

enum INV
{
    __INV_size
};
}

namespace N_mach_segments
{
enum DATA
{
    segname=0,
    vmaddr,
    vmsize,
    fileoff,
    filesize,
    maxprot,
    initprot,
    nsects,
    flags,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size];
extern const FW_DEF::HEADER_RECORD records64[__data_size];

enum CB
{
    __CB_size
};

enum INV
{
    __INV_size
};
}

namespace N_mach_sections32
{
enum DATA
{
    sectname=0,
    segname,
    addr,
    size,
    offset,
    align,
    reloff,
    nreloc,
    flags,
    reserved1,
    reserved2,
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

namespace N_mach_sections64
{
enum DATA
{
    sectname=0,
    segname,
    addr,
    size,
    offset,
    align,
    reloff,
    nreloc,
    flags,
    reserved1,
    reserved2,
    reserved3,
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

namespace N_mach_library
{
enum DATA
{
    timestamp,
    current_version,
    compatibility_version,
    name,
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

namespace N_mach_dyld_info
{
enum DATA
{
    rebase_off,
    rebase_size,
    bind_off,
    bind_size,
    weak_bind_off,
    weak_bind_size,
    lazy_bind_off,
    lazy_bind_size,
    export_off,
    export_size,
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

namespace N_mach_uuid
{
enum DATA
{
    uuid=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_dylinker
{
enum DATA
{
    dylinker=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_rpath
{
enum DATA
{
    path=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_symtab
{
enum DATA
{
    symoff,
    nsyms,
    stroff,
    strsize,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_dysymtab
{
enum DATA
{
    ilocalsym,
    nlocalsym,
    iextdefsym,
    nextdefsym,
    iundefsym,
    nundefsym,
    tocoff,
    ntoc,
    modtaboff,
    nmodtab,
    extrefsymoff,
    nextrefsyms,
    indirectsymoff,
    nindirectsyms,
    extreloff,
    nextrel,
    locreloff,
    nlocrel,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_version_min
{
enum DATA
{
    version,
    sdk,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_source_version
{
enum DATA
{
    version=0,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_encryption_info
{
enum DATA
{
    cryptoff=0,
    cryptsize,
    cryptid,
    pad,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records32[__data_size-1];
extern const FW_DEF::HEADER_RECORD records64[__data_size];
}

namespace N_mach_linkedit_data
{
enum DATA
{
    dataoff,
    datasize,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_main
{
enum DATA
{
    entryoff,
    stacksize,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread
{
enum DATA
{
    flavor,
    count,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

#endif // MACH_DEFS_H
