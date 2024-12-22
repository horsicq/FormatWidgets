/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#ifndef XMACH_DEFS_H
#define XMACH_DEFS_H

#include "../xformatwidget_def.h"
namespace XTYPE_MACH {
namespace X_mach_header {
enum DATA {
    magic = 0,
    cputype,
    cpusubtype,
    filetype,
    ncmds,
    sizeofcmds,
    flags,
    reserved,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size - 1];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_mach_header

namespace X_load_commands {
enum DATA {
    cmd = 0,
    cmdsize,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_load_commands

namespace X_segment_command {
enum DATA {
    cmd = 0,
    cmdsize,
    segname,
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

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_segment_command

namespace X_section {
enum DATA {
    sectname = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_section

namespace X_section_64 {
enum DATA {
    sectname = 0,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_section_64

namespace X_dylib_command {
enum DATA {
    cmd = 0,
    cmdsize,
    name,
    timestamp,
    current_version,
    compatibility_version,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_dylib_command

namespace X_fvmlib_command {
enum DATA {
    cmd = 0,
    cmdsize,
    name,
    minor_version,
    header_addr,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_fvmlib_command

namespace X_dyld_info_command {
enum DATA {
    cmd = 0,
    cmdsize,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_dyld_info_command

namespace X_uuid_command {
enum DATA {
    cmd = 0,
    cmdsize,
    uuid,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_uuid_command

namespace X_dylinker_command {
enum DATA {
    cmd = 0,
    cmdsize,
    name,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_dylinker_command

namespace X_rpath_command {
enum DATA {
    cmd = 0,
    cmdsize,
    path,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_rpath_command

namespace X_symtab_command {
enum DATA {
    cmd = 0,
    cmdsize,
    symoff,
    nsyms,
    stroff,
    strsize,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_symtab_command

namespace X_sub_umbrella_command {
enum DATA {
    cmd = 0,
    cmdsize,
    sub_umbrella,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_sub_umbrella_command

namespace X_sub_client_command {
enum DATA {
    cmd = 0,
    cmdsize,
    client,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_sub_client_command

namespace X_sub_library_command {
enum DATA {
    cmd = 0,
    cmdsize,
    sub_library,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_sub_library_command

namespace X_dysymtab_command {
enum DATA {
    cmd = 0,
    cmdsize,
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_dysymtab_command

namespace X_version_min_command {
enum DATA {
    cmd = 0,
    cmdsize,
    version,
    sdk,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_version_min_command

namespace X_build_version_command {
enum DATA {
    cmd = 0,
    cmdsize,
    platform,
    minos,
    sdk,
    ntools,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_build_version_command

namespace X_source_version_command {
enum DATA {
    cmd = 0,
    cmdsize,
    version,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_source_version_command

namespace X_encryption_info_command {
enum DATA {
    cmd = 0,
    cmdsize,
    cryptoff,
    cryptsize,
    cryptid,
    pad,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size - 1];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_encryption_info_command

namespace X_linkedit_data_command {
enum DATA {
    cmd = 0,
    cmdsize,
    dataoff,
    datasize,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_linkedit_data_command

namespace X_entry_point_command {
enum DATA {
    cmd = 0,
    cmdsize,
    entryoff,
    stacksize,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_entry_point_command

namespace X_fileset_entry_command {
enum DATA {
    cmd = 0,
    cmdsize,
    vmaddr,
    fileoff,
    entry_id,
    reserved,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_fileset_entry_command

namespace X_unix_thread_command {
enum DATA {
    cmd = 0,
    cmdsize,
    flavor,
    count,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_unix_thread_command

namespace X_routines_command {
enum DATA {
    cmd = 0,
    cmdsize,
    init_address,
    init_module,
    reserved1,
    reserved2,
    reserved3,
    reserved4,
    reserved5,
    reserved6,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_routines_command

namespace X_x86_thread_state32_t {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_x86_thread_state32_t

namespace X_x86_thread_state64_t {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_x86_thread_state64_t

namespace X_mach_unix_thread_arm_32 {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_unix_thread_arm_32

namespace X_mach_unix_thread_arm_64 {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_unix_thread_arm_64

namespace X_mach_unix_thread_ppc_32 {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_unix_thread_ppc_32

namespace X_mach_unix_thread_m68k_32 {
enum DATA {
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
extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_unix_thread_m68k_32

namespace X_nlist {
enum DATA {
    n_strx,
    n_type,
    n_sect,
    n_desc,
    n_value,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records32[__data_size];
extern const XFW_DEF::HEADER_RECORD records64[__data_size];
}  // namespace X_nlist

namespace X_mach_data_in_code_entry {
enum DATA {
    offset,
    length,
    kind,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_data_in_code_entry

namespace X_mach_modtab32 {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_modtab32

namespace X_mach_modtab64 {
enum DATA {
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

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_modtab64

namespace X_mach_table_of_contents {
enum DATA {
    symbol_index,
    module_index,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_table_of_contents

namespace X_mach_relocs {
enum DATA {
    r_address,
    value,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_relocs

namespace X_mach_relocs_E {
enum DATA {
    r_address,
    r_symbolnum,
    r_pcrel,
    r_length,
    r_extern,
    r_type,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_relocs_E

namespace X_mach_value {
enum DATA {
    value,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_value

namespace X_mach_refsyms {
enum DATA {
    isym,
    flags,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_refsyms

namespace X_SC_SuperBlob {
enum DATA {
    magic,
    length,
    count,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_SC_SuperBlob

namespace X_CS_BlobIndex {
enum DATA {
    type,
    offset,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_CS_BlobIndex

namespace X_CS_CodeDirectory {

enum DATA {
    magic,
    length,
    version,
    flags,
    hashOffset,
    identOffset,
    nSpecialSlots,
    nCodeSlots,
    codeLimit,
    hashSize,
    hashType,
    platform,
    pageSize,
    spare2,
    scatterOffset,
    teamOffset,
    spare3,
    codeLimit64,
    execSegBase,
    execSegLimit,
    execSegFlags,
    runtime,
    preEncryptOffset,
    linkageHashType,
    linkageTruncated,
    spare4,
    linkageOffset,
    linkageSize,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_CS_CodeDirectory

namespace X_dyld_chained_fixups_header {
enum DATA {
    fixups_version,
    starts_offset,
    imports_offset,
    symbols_offset,
    imports_count,
    imports_format,
    symbols_format,
    __data_size
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_dyld_chained_fixups_header

// namespace X_mach_dyld_chained_starts_in_image {

// enum DATA {
//     offset,
//     count,
//     __data_size
// };

// extern const XFW_DEF::HEADER_RECORD records[__data_size];
// }  // namespace X_mach_dyld_chained_starts_in_image
}  // namespace XTYPE_MACH

#endif  // XMACH_DEFS_H
