// copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#include "mach_defs.h"

namespace N_mach_header
{
const FW_DEF::HEADER_RECORD records32[]=
{
    {magic,                 "magic",            offsetof(XMACH_DEF::mach_header,magic),         4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {cputype,               "cputype",          offsetof(XMACH_DEF::mach_header,cputype),       4,      "cpu_type_t",           FW_DEF::VAL_TYPE_DATA},
    {cpusubtype,            "cpusubtype",       offsetof(XMACH_DEF::mach_header,cpusubtype),    4,      "cpu_subtype_t",        FW_DEF::VAL_TYPE_DATA},
    {filetype,              "filetype",         offsetof(XMACH_DEF::mach_header,filetype),      4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {ncmds,                 "ncmds",            offsetof(XMACH_DEF::mach_header,ncmds),         4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {sizeofcmds,            "sizeofcmds",       offsetof(XMACH_DEF::mach_header,sizeofcmds),    4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::mach_header,flags),         4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
};

const FW_DEF::HEADER_RECORD records64[]=
{
    {magic,                 "magic",            offsetof(XMACH_DEF::mach_header_64,magic),      4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {cputype,               "cputype",          offsetof(XMACH_DEF::mach_header_64,cputype),    4,      "cpu_type_t",           FW_DEF::VAL_TYPE_DATA},
    {cpusubtype,            "cpusubtype",       offsetof(XMACH_DEF::mach_header_64,cpusubtype), 4,      "cpu_subtype_t",        FW_DEF::VAL_TYPE_DATA},
    {filetype,              "filetype",         offsetof(XMACH_DEF::mach_header_64,filetype),   4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {ncmds,                 "ncmds",            offsetof(XMACH_DEF::mach_header_64,ncmds),      4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {sizeofcmds,            "sizeofcmds",       offsetof(XMACH_DEF::mach_header_64,sizeofcmds), 4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::mach_header_64,flags),      4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {reserved,              "reserved",         offsetof(XMACH_DEF::mach_header_64,reserved),   4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_commands
{
const FW_DEF::HEADER_RECORD records[]=
{
    {cmd,                  "cmd",               offsetof(XMACH_DEF::load_command,cmd),          4,      "uint32_t",             FW_DEF::VAL_TYPE_DATA},
    {cmdsize,              "cmdsize",           offsetof(XMACH_DEF::load_command,cmdsize),      4,      "uint32_t",             FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_segments
{
const FW_DEF::HEADER_RECORD records32[]=
{
    {segname,               "segname",          offsetof(XMACH_DEF::segment_command,segname),       16,     "char[16]",             FW_DEF::VAL_TYPE_TEXT},
    {vmaddr,                "vmaddr",           offsetof(XMACH_DEF::segment_command,vmaddr),        4,      "uint32",               FW_DEF::VAL_TYPE_ADDRESS},
    {vmsize,                "vmsize",           offsetof(XMACH_DEF::segment_command,vmsize),        4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {fileoff,               "fileoff",          offsetof(XMACH_DEF::segment_command,fileoff),       4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {filesize,              "filesize",         offsetof(XMACH_DEF::segment_command,filesize),      4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {maxprot,               "maxprot",          offsetof(XMACH_DEF::segment_command,maxprot),       4,      "int32",                FW_DEF::VAL_TYPE_DATA},
    {initprot,              "initprot",         offsetof(XMACH_DEF::segment_command,initprot),      4,      "int32",                FW_DEF::VAL_TYPE_DATA},
    {nsects,                "nsects",           offsetof(XMACH_DEF::segment_command,nsects),        4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::segment_command,flags),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};

const FW_DEF::HEADER_RECORD records64[]=
{
    {segname,               "segname",          offsetof(XMACH_DEF::segment_command_64,segname),    16,     "char[16]",             FW_DEF::VAL_TYPE_TEXT},
    {vmaddr,                "vmaddr",           offsetof(XMACH_DEF::segment_command_64,vmaddr),     8,      "uint64",               FW_DEF::VAL_TYPE_ADDRESS},
    {vmsize,                "vmsize",           offsetof(XMACH_DEF::segment_command_64,vmsize),     8,      "uint64",               FW_DEF::VAL_TYPE_SIZE},
    {fileoff,               "fileoff",          offsetof(XMACH_DEF::segment_command_64,fileoff),    8,      "uint64",               FW_DEF::VAL_TYPE_OFFSET},
    {filesize,              "filesize",         offsetof(XMACH_DEF::segment_command_64,filesize),   8,      "uint64",               FW_DEF::VAL_TYPE_SIZE},
    {maxprot,               "maxprot",          offsetof(XMACH_DEF::segment_command_64,maxprot),    4,      "int32",                FW_DEF::VAL_TYPE_DATA},
    {initprot,              "initprot",         offsetof(XMACH_DEF::segment_command_64,initprot),   4,      "int32",                FW_DEF::VAL_TYPE_DATA},
    {nsects,                "nsects",           offsetof(XMACH_DEF::segment_command_64,nsects),     4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::segment_command_64,flags),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_sections32
{
const FW_DEF::HEADER_RECORD records[]=
{
    {sectname,              "sectname",         offsetof(XMACH_DEF::section,sectname),      16,     "char[16]",             FW_DEF::VAL_TYPE_TEXT},
    {segname,               "segname",          offsetof(XMACH_DEF::section,segname),       16,     "char[16]",             FW_DEF::VAL_TYPE_TEXT},
    {addr,                  "addr",             offsetof(XMACH_DEF::section,addr),          4,      "uint32",               FW_DEF::VAL_TYPE_ADDRESS},
    {size,                  "size",             offsetof(XMACH_DEF::section,size),          4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {offset,                "offset",           offsetof(XMACH_DEF::section,offset),        4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {align,                 "align",            offsetof(XMACH_DEF::section,align),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reloff,                "reloff",           offsetof(XMACH_DEF::section,reloff),        4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nreloc,                "nreloc",           offsetof(XMACH_DEF::section,nreloc),        4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::section,flags),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reserved1,             "reserved1",        offsetof(XMACH_DEF::section,reserved1),     4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reserved2,             "reserved2",        offsetof(XMACH_DEF::section,reserved2),     4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_sections64
{
const FW_DEF::HEADER_RECORD records[]=
{
    {sectname,              "sectname",         offsetof(XMACH_DEF::section_64,sectname),   16,     "char[16]",             FW_DEF::VAL_TYPE_TEXT},
    {segname,               "segname",          offsetof(XMACH_DEF::section_64,segname),    16,     "char[16]",             FW_DEF::VAL_TYPE_TEXT},
    {addr,                  "addr",             offsetof(XMACH_DEF::section_64,addr),       8,      "uint64",               FW_DEF::VAL_TYPE_ADDRESS},
    {size,                  "size",             offsetof(XMACH_DEF::section_64,size),       8,      "uint64",               FW_DEF::VAL_TYPE_SIZE},
    {offset,                "offset",           offsetof(XMACH_DEF::section_64,offset),     4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {align,                 "align",            offsetof(XMACH_DEF::section_64,align),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reloff,                "reloff",           offsetof(XMACH_DEF::section_64,reloff),     4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nreloc,                "nreloc",           offsetof(XMACH_DEF::section_64,nreloc),     4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::section_64,flags),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reserved1,             "reserved1",        offsetof(XMACH_DEF::section_64,reserved1),  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reserved2,             "reserved2",        offsetof(XMACH_DEF::section_64,reserved2),  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reserved3,             "reserved3",        offsetof(XMACH_DEF::section_64,reserved3),  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_library
{
const FW_DEF::HEADER_RECORD records[]=
{
    {timestamp,             "timestamp",                sizeof(XMACH_DEF::load_command)+offsetof(XMACH_DEF::dylib,timestamp),               4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {current_version,       "current_version",          sizeof(XMACH_DEF::load_command)+offsetof(XMACH_DEF::dylib,current_version),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {compatibility_version, "compatibility_version",    sizeof(XMACH_DEF::load_command)+offsetof(XMACH_DEF::dylib,compatibility_version),   4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {name,                  "name",                     sizeof(XMACH_DEF::load_command)+sizeof(XMACH_DEF::dylib),                           -1,     "Ansi string",          FW_DEF::VAL_TYPE_TEXT},
};
}

namespace N_mach_dyld_info
{
const FW_DEF::HEADER_RECORD records[]=
{
    {rebase_off,            "rebase_off",               offsetof(XMACH_DEF::dyld_info_command,rebase_off),              4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {rebase_size,           "rebase_size",              offsetof(XMACH_DEF::dyld_info_command,rebase_size),             4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {bind_off,              "bind_off",                 offsetof(XMACH_DEF::dyld_info_command,bind_off),                4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {bind_size,             "bind_size",                offsetof(XMACH_DEF::dyld_info_command,bind_size),               4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {weak_bind_off,         "weak_bind_off",            offsetof(XMACH_DEF::dyld_info_command,weak_bind_off),           4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {weak_bind_size,        "weak_bind_size",           offsetof(XMACH_DEF::dyld_info_command,weak_bind_size),          4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {lazy_bind_off,         "lazy_bind_off",            offsetof(XMACH_DEF::dyld_info_command,lazy_bind_off),           4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {lazy_bind_size,        "lazy_bind_size",           offsetof(XMACH_DEF::dyld_info_command,lazy_bind_size),          4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {export_off,            "export_off",               offsetof(XMACH_DEF::dyld_info_command,export_off),              4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {export_size,           "export_size",              offsetof(XMACH_DEF::dyld_info_command,export_size),             4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_uuid
{
const FW_DEF::HEADER_RECORD records[]=
{
    {uuid,                  "uuid",                     sizeof(XMACH_DEF::load_command),            16,         "bytes",                FW_DEF::VAL_TYPE_UUID},
};
}

namespace N_mach_dylinker
{
const FW_DEF::HEADER_RECORD records[]=
{
    {dylinker,              "dylinker",                 sizeof(XMACH_DEF::load_command),            -1,         "Ansi string",          FW_DEF::VAL_TYPE_TEXT},
};
}

namespace N_mach_rpath
{
const FW_DEF::HEADER_RECORD records[]=
{
    {path,                  "path",                     sizeof(XMACH_DEF::load_command),            -1,         "Ansi string",          FW_DEF::VAL_TYPE_TEXT},
};
}

namespace N_mach_symtab
{
const FW_DEF::HEADER_RECORD records[]=
{
    {symoff,                "symoff",                   offsetof(XMACH_DEF::symtab_command,symoff),         4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nsyms,                 "nsyms",                    offsetof(XMACH_DEF::symtab_command,nsyms),          4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {stroff,                "stroff",                   offsetof(XMACH_DEF::symtab_command,stroff),         4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {strsize,               "strsize",                  offsetof(XMACH_DEF::symtab_command,strsize),        4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_dysymtab
{
const FW_DEF::HEADER_RECORD records[]=
{
    {ilocalsym,             "ilocalsym",                offsetof(XMACH_DEF::dysymtab_command,ilocalsym),        4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nlocalsym,             "nlocalsym",                offsetof(XMACH_DEF::dysymtab_command,nlocalsym),        4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iextdefsym,            "iextdefsym",               offsetof(XMACH_DEF::dysymtab_command,iextdefsym),       4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nextdefsym,            "nextdefsym",               offsetof(XMACH_DEF::dysymtab_command,nextdefsym),       4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iundefsym,             "iundefsym",                offsetof(XMACH_DEF::dysymtab_command,iundefsym),        4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nundefsym,             "nundefsym",                offsetof(XMACH_DEF::dysymtab_command,nundefsym),        4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {tocoff,                "tocoff",                   offsetof(XMACH_DEF::dysymtab_command,tocoff),           4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {ntoc,                  "ntoc",                     offsetof(XMACH_DEF::dysymtab_command,ntoc),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {modtaboff,             "modtaboff",                offsetof(XMACH_DEF::dysymtab_command,modtaboff),        4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nmodtab,               "nmodtab",                  offsetof(XMACH_DEF::dysymtab_command,nmodtab),          4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {extrefsymoff,          "extrefsymoff",             offsetof(XMACH_DEF::dysymtab_command,extrefsymoff),     4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nextrefsyms,           "nextrefsyms",              offsetof(XMACH_DEF::dysymtab_command,nextrefsyms),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {indirectsymoff,        "indirectsymoff",           offsetof(XMACH_DEF::dysymtab_command,indirectsymoff),   4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nindirectsyms,         "nindirectsyms",            offsetof(XMACH_DEF::dysymtab_command,nindirectsyms),    4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {extreloff,             "extreloff",                offsetof(XMACH_DEF::dysymtab_command,extreloff),        4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nextrel,               "nextrel",                  offsetof(XMACH_DEF::dysymtab_command,nextrel),          4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {locreloff,             "locreloff",                offsetof(XMACH_DEF::dysymtab_command,locreloff),        4,      "uint32",               FW_DEF::VAL_TYPE_OFFSET},
    {nlocrel,               "nlocrel",                  offsetof(XMACH_DEF::dysymtab_command,nlocrel),          4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_version_min
{
const FW_DEF::HEADER_RECORD records[]=
{
    {version,               "version",                  offsetof(XMACH_DEF::version_min_command,version),       4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {sdk,                   "sdk",                      offsetof(XMACH_DEF::version_min_command,sdk),           4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_source_version
{
const FW_DEF::HEADER_RECORD records[]=
{
    {version,               "version",                  offsetof(XMACH_DEF::source_version_command,version),    8,      "uint64",          FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_encryption_info
{
const FW_DEF::HEADER_RECORD records32[]=
{
    {cryptoff,              "cryptoff",                 offsetof(XMACH_DEF::encryption_info_command,cryptoff),      4,      "uint32",           FW_DEF::VAL_TYPE_OFFSET},
    {cryptsize,             "cryptsize",                offsetof(XMACH_DEF::encryption_info_command,cryptsize),     4,      "uint32",           FW_DEF::VAL_TYPE_SIZE},
    {cryptid,               "cryptid",                  offsetof(XMACH_DEF::encryption_info_command,cryptid),       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};

const FW_DEF::HEADER_RECORD records64[]=
{
    {cryptoff,              "cryptoff",                 offsetof(XMACH_DEF::encryption_info_command_64,cryptoff),   4,      "uint32",           FW_DEF::VAL_TYPE_OFFSET},
    {cryptsize,             "cryptsize",                offsetof(XMACH_DEF::encryption_info_command_64,cryptsize),  4,      "uint32",           FW_DEF::VAL_TYPE_SIZE},
    {cryptid,               "cryptid",                  offsetof(XMACH_DEF::encryption_info_command_64,cryptid),    4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {pad,                   "pad",                      offsetof(XMACH_DEF::encryption_info_command_64,pad),        4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};
}
namespace N_mach_linkedit_data
{
const FW_DEF::HEADER_RECORD records[]=
{
    {dataoff,               "dataoff",                  offsetof(XMACH_DEF::linkedit_data_command,dataoff),         4,      "uint32",           FW_DEF::VAL_TYPE_OFFSET},
    {datasize,              "datasize",                 offsetof(XMACH_DEF::linkedit_data_command,datasize),        4,      "uint32",           FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_main
{
const FW_DEF::HEADER_RECORD records[]=
{
    {entryoff,              "entryoff",                 offsetof(XMACH_DEF::entry_point_command,entryoff),          8,      "uint64",           FW_DEF::VAL_TYPE_ADDRESS},
    {stacksize,             "stacksize",                offsetof(XMACH_DEF::entry_point_command,stacksize),         8,      "uint64",           FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_unix_thread
{
const FW_DEF::HEADER_RECORD records[]=
{
    {flavor,                "flavor",                   offsetof(XMACH_DEF::unix_thread_command,flavor),            4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {count,                 "count",                    offsetof(XMACH_DEF::unix_thread_command,count),             4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_unix_thread_x86_32
{
const FW_DEF::HEADER_RECORD records[]=
{
    {eax,               "eax",                  offsetof(XMACH_DEF::x86_thread_state32_t,eax),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ebx,               "ebx",                  offsetof(XMACH_DEF::x86_thread_state32_t,ebx),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ecx,               "ecx",                  offsetof(XMACH_DEF::x86_thread_state32_t,ecx),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {edx,               "edx",                  offsetof(XMACH_DEF::x86_thread_state32_t,edx),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {edi,               "edi",                  offsetof(XMACH_DEF::x86_thread_state32_t,edi),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {esi,               "esi",                  offsetof(XMACH_DEF::x86_thread_state32_t,esi),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ebp,               "ebp",                  offsetof(XMACH_DEF::x86_thread_state32_t,ebp),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {esp,               "esp",                  offsetof(XMACH_DEF::x86_thread_state32_t,esp),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ss,                "ss",                   offsetof(XMACH_DEF::x86_thread_state32_t,ss),           4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {eflags,            "eflags",               offsetof(XMACH_DEF::x86_thread_state32_t,eflags),       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {eip,               "eip",                  offsetof(XMACH_DEF::x86_thread_state32_t,eip),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {cs,                "cs",                   offsetof(XMACH_DEF::x86_thread_state32_t,cs),           4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ds,                "ds",                   offsetof(XMACH_DEF::x86_thread_state32_t,ds),           4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {es,                "es",                   offsetof(XMACH_DEF::x86_thread_state32_t,es),           4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {fs,                "fs",                   offsetof(XMACH_DEF::x86_thread_state32_t,fs),           4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {gs,                "gs",                   offsetof(XMACH_DEF::x86_thread_state32_t,gs),           4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};
}

