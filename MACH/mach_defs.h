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
    TYPE_DISASM,
    TYPE_HASH,
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
    TYPE_mach_LOADFVMLIB,
    TYPE_mach_IDFVMLIB,
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
    TYPE_mach_unix_thread_x86_32,
    TYPE_mach_unix_thread_x86_64,
    TYPE_mach_unix_thread_arm_32,
    TYPE_mach_unix_thread_arm_64,
    TYPE_mach_unix_thread_m68k_32,
    TYPE_mach_unix_thread_ppc_32,
    TYPE_STRINGTABLE,
    TYPE_SYMBOLTABLE,
    TYPE_FUNCTIONS,
    TYPE_DICE,
    TYPE_DYLD_INFO_rebase,
    TYPE_DYLD_INFO_bind,
    TYPE_DYLD_INFO_weak_bind,
    TYPE_DYLD_INFO_lazy_bind,
    TYPE_DYLD_INFO_export,
    TYPE_DYSYMTAB_toc,
    TYPE_DYSYMTAB_modtab,
    TYPE_DYSYMTAB_extrefsyms,
    TYPE_DYSYMTAB_indirectsyms,
    TYPE_DYSYMTAB_extrel,
    TYPE_DYSYMTAB_locrel,
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
    CB_maxprot,
    CB_initprot,
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

namespace N_mach_sections32_E
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
    flags_0,
    flags_1,
    flags_2,
    reserved1,
    reserved2,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_flag0,
    CB_flag1,
    CB_flag2,
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

namespace N_mach_sections64_E
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
    flags_0,
    flags_1,
    flags_2,
    reserved1,
    reserved2,
    reserved3,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_flag0,
    CB_flag1,
    CB_flag2,
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

namespace N_mach_fmv_library
{
enum DATA
{
    minor_version,
    header_addr,
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

namespace N_mach_unix_thread_x86_32
{
enum DATA
{
    eax,
    ebx,
    ecx,
    edx,
    edi,
    esi,
    ebp,
    esp,
    ss,
    eflags,
    eip,
    cs,
    ds,
    es,
    fs,
    gs,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread_x86_64
{
enum DATA
{
    rax,
    rbx,
    rcx,
    rdx,
    rdi,
    rsi,
    rbp,
    rsp,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    rip,
    rflags,
    cs,
    fs,
    gs,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread_arm_32
{
enum DATA
{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    sp,
    lr,
    pc,
    cpsr,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread_arm_64
{
enum DATA
{
    x0,
    x1,
    x2,
    x3,
    x4,
    x5,
    x6,
    x7,
    x8,
    x9,
    x10,
    x11,
    x12,
    x13,
    x14,
    x15,
    x16,
    x17,
    x18,
    x19,
    x20,
    x21,
    x22,
    x23,
    x24,
    x25,
    x26,
    x27,
    x28,
    fp,
    lr,
    sp,
    pc,
    cpsr,
    pad,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread_ppc_32
{
enum DATA
{
    srr0,
    srr1,
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    r16,
    r17,
    r18,
    r19,
    r20,
    r21,
    r22,
    r23,
    r24,
    r25,
    r26,
    r27,
    r28,
    r29,
    r30,
    r31,
    ct,
    xer,
    lr,
    ctr,
    mq,
    vrsave,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread_m68k_32
{
enum DATA
{
    dreg0,
    dreg1,
    dreg2,
    dreg3,
    dreg4,
    dreg5,
    dreg6,
    dreg7,
    areg0,
    areg1,
    areg2,
    areg3,
    areg4,
    areg5,
    areg6,
    areg7,
    pad0,
    sr,
    pc,
    __data_size
};
    extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_nlist
{
enum DATA
{
    n_strx,
    n_type,
    n_sect,
    n_desc,
    n_value,
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

namespace N_mach_data_in_code_entry
{
enum DATA
{
    offset,
    length,
    kind,
    __data_size
};

extern const FW_DEF::HEADER_RECORD records[__data_size];

enum CB
{
    CB_kind,
    __CB_size
};

enum INV
{
    __INV_size
};
}

namespace N_mach_modtab32
{
enum DATA
{
    module_name,
    iextdefsym,
    nextdefsym,
    irefsym,
    nrefsym,
    ilocalsym,
    nlocalsym,
    iextrel,
    nextrel,
    iinit_iterm,
    ninit_nterm,
    objc_module_info_addr,
    objc_module_info_size,
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

namespace N_mach_modtab64
{
enum DATA
{
    module_name,
    iextdefsym,
    nextdefsym,
    irefsym,
    nrefsym,
    ilocalsym,
    nlocalsym,
    iextrel,
    nextrel,
    iinit_iterm,
    ninit_nterm,
    objc_module_info_size,
    objc_module_info_addr,
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

namespace N_mach_table_of_contents
{
enum DATA
{
    symbol_index,
    module_index,
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

namespace N_mach_relocs
{
enum DATA
{
    r_address,
    value,
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

namespace N_mach_relocs_E
{
enum DATA
{
    r_address,
    r_symbolnum,
    r_pcrel,
    r_length,
    r_extern,
    r_type,
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

namespace N_mach_value
{
enum DATA
{
    value,
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

namespace N_mach_refsyms
{
enum DATA
{
    isym,
    flags,
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

#endif // MACH_DEFS_H
