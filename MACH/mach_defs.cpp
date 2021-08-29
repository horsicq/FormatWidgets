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
    {magic,                 "magic",            offsetof(XMACH_DEF::mach_header,magic),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {cputype,               "cputype",          offsetof(XMACH_DEF::mach_header,cputype),       4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {cpusubtype,            "cpusubtype",       offsetof(XMACH_DEF::mach_header,cpusubtype),    4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {filetype,              "filetype",         offsetof(XMACH_DEF::mach_header,filetype),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {ncmds,                 "ncmds",            offsetof(XMACH_DEF::mach_header,ncmds),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {sizeofcmds,            "sizeofcmds",       offsetof(XMACH_DEF::mach_header,sizeofcmds),    4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::mach_header,flags),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};

const FW_DEF::HEADER_RECORD records64[]=
{
    {magic,                 "magic",            offsetof(XMACH_DEF::mach_header_64,magic),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {cputype,               "cputype",          offsetof(XMACH_DEF::mach_header_64,cputype),    4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {cpusubtype,            "cpusubtype",       offsetof(XMACH_DEF::mach_header_64,cpusubtype), 4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {filetype,              "filetype",         offsetof(XMACH_DEF::mach_header_64,filetype),   4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {ncmds,                 "ncmds",            offsetof(XMACH_DEF::mach_header_64,ncmds),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {sizeofcmds,            "sizeofcmds",       offsetof(XMACH_DEF::mach_header_64,sizeofcmds), 4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                 "flags",            offsetof(XMACH_DEF::mach_header_64,flags),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {reserved,              "reserved",         offsetof(XMACH_DEF::mach_header_64,reserved),   4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_commands
{
const FW_DEF::HEADER_RECORD records[]=
{
    {cmd,                  "cmd",               offsetof(XMACH_DEF::load_command,cmd),          4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {cmdsize,              "cmdsize",           offsetof(XMACH_DEF::load_command,cmdsize),      4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
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

namespace N_mach_sections32_E
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
    {flags_0,               "flags",            offsetof(XMACH_DEF::section,flags),         4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags_1,               "",                 -1,                                         0,      "",                     FW_DEF::VAL_TYPE_UNKNOWN},
    {flags_2,               "",                 -1,                                         0,      "",                     FW_DEF::VAL_TYPE_UNKNOWN},
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

namespace N_mach_sections64_E
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
    {flags_0,               "flags",            offsetof(XMACH_DEF::section_64,flags),      4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags_1,               "",                 -1,                                         0,      "",                     FW_DEF::VAL_TYPE_UNKNOWN},
    {flags_2,               "",                 -1,                                         0,      "",                     FW_DEF::VAL_TYPE_UNKNOWN},
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
    {name,                  QObject::tr("Name"),        sizeof(XMACH_DEF::load_command)+sizeof(XMACH_DEF::dylib),                           -1,     "Ansi string",          FW_DEF::VAL_TYPE_TEXT},
};
}

namespace N_mach_fmv_library
{
const FW_DEF::HEADER_RECORD records[]=
{
    {minor_version,         "minor_version",            sizeof(XMACH_DEF::load_command)+offsetof(XMACH_DEF::fvmlib,minor_version),          4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {header_addr,           "header_addr",              sizeof(XMACH_DEF::load_command)+offsetof(XMACH_DEF::fvmlib,header_addr),            4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {name,                  QObject::tr("Name"),        sizeof(XMACH_DEF::load_command)+sizeof(XMACH_DEF::fvmlib),                          -1,     "Ansi string",          FW_DEF::VAL_TYPE_TEXT},
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
    {version,               QObject::tr("Version"),     offsetof(XMACH_DEF::version_min_command,version),       4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {sdk,                   "sdk",                      offsetof(XMACH_DEF::version_min_command,sdk),           4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_source_version
{
const FW_DEF::HEADER_RECORD records[]=
{
    {version,               QObject::tr("Version"),     offsetof(XMACH_DEF::source_version_command,version),    8,      "uint64",          FW_DEF::VAL_TYPE_DATA},
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

namespace N_mach_unix_thread_x86_64
{
const FW_DEF::HEADER_RECORD records[]=
{
    {rax,               "rax",                  offsetof(XMACH_DEF::x86_thread_state64_t,rax),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rbx,               "rbx",                  offsetof(XMACH_DEF::x86_thread_state64_t,rbx),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rcx,               "rcx",                  offsetof(XMACH_DEF::x86_thread_state64_t,rcx),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rdx,               "rdx",                  offsetof(XMACH_DEF::x86_thread_state64_t,rdx),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rdi,               "rdi",                  offsetof(XMACH_DEF::x86_thread_state64_t,rdi),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rsi,               "rsi",                  offsetof(XMACH_DEF::x86_thread_state64_t,rsi),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rbp,               "rbp",                  offsetof(XMACH_DEF::x86_thread_state64_t,rbp),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rsp,               "esp",                  offsetof(XMACH_DEF::x86_thread_state64_t,rsp),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r8,                "r8",                   offsetof(XMACH_DEF::x86_thread_state64_t,r8),           8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r9,                "r9",                   offsetof(XMACH_DEF::x86_thread_state64_t,r9),           8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r10,               "r10",                  offsetof(XMACH_DEF::x86_thread_state64_t,r10),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r11,               "r11",                  offsetof(XMACH_DEF::x86_thread_state64_t,r11),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r12,               "r12",                  offsetof(XMACH_DEF::x86_thread_state64_t,r12),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r13,               "r13",                  offsetof(XMACH_DEF::x86_thread_state64_t,r13),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r14,               "r14",                  offsetof(XMACH_DEF::x86_thread_state64_t,r14),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {r15,               "r15",                  offsetof(XMACH_DEF::x86_thread_state64_t,r15),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rip,               "rip",                  offsetof(XMACH_DEF::x86_thread_state64_t,rip),          8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {rflags,            "rflags",               offsetof(XMACH_DEF::x86_thread_state64_t,rflags),       8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {cs,                "cs",                   offsetof(XMACH_DEF::x86_thread_state64_t,cs),           8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {fs,                "fs",                   offsetof(XMACH_DEF::x86_thread_state64_t,fs),           8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {gs,                "gs",                   offsetof(XMACH_DEF::x86_thread_state64_t,gs),           8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_unix_thread_arm_32
{
const FW_DEF::HEADER_RECORD records[]=
{
    {r0,                "r0",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*0,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r1,                "r1",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*1,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r2,                "r2",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*2,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r3,                "r3",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*3,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r4,                "r4",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*4,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r5,                "r5",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*5,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r6,                "r6",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*6,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r7,                "r7",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*7,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r8,                "r8",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*8,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r9,                "r9",                   offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*9,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r10,               "r10",                  offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*10,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r11,               "r11",                  offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*11,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r12,               "r12",                  offsetof(XMACH_DEF::arm_thread_state32_t,r)+sizeof(quint32)*12,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {sp,                "sp",                   offsetof(XMACH_DEF::arm_thread_state32_t,sp),                       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {lr,                "lr",                   offsetof(XMACH_DEF::arm_thread_state32_t,lr),                       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {pc,                "pc",                   offsetof(XMACH_DEF::arm_thread_state32_t,pc),                       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {cpsr,              "cpsr",                 offsetof(XMACH_DEF::arm_thread_state32_t,cpsr),                     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_unix_thread_arm_64
{
const FW_DEF::HEADER_RECORD records[]=
{
    {x0,                "x0",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*0,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x1,                "x1",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*1,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x2,                "x2",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*2,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x3,                "x3",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*3,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x4,                "x4",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*4,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x5,                "x5",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*5,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x6,                "x6",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*6,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x7,                "x7",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*7,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x8,                "x8",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*8,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x9,                "x9",                   offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*9,      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x10,               "x10",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*10,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x11,               "x11",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*11,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x12,               "x12",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*12,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x13,               "x13",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*13,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x14,               "x14",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*14,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x15,               "x15",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*15,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x16,               "x16",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*16,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x17,               "x17",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*17,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x18,               "x18",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*18,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x19,               "x19",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*19,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x20,               "x20",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*20,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x21,               "x21",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*21,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x22,               "x22",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*22,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x23,               "x23",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*23,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x24,               "x24",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*24,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x25,               "x25",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*25,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x26,               "x26",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*26,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x27,               "x27",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*27,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {x28,               "x28",                  offsetof(XMACH_DEF::arm_thread_state64_t,x)+sizeof(quint64)*28,     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {fp,                "fp",                   offsetof(XMACH_DEF::arm_thread_state64_t,fp),                       8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {lr,                "lr",                   offsetof(XMACH_DEF::arm_thread_state64_t,lr),                       8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {sp,                "sp",                   offsetof(XMACH_DEF::arm_thread_state64_t,sp),                       8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {pc,                "pc",                   offsetof(XMACH_DEF::arm_thread_state64_t,pc),                       8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {cpsr,              "cpsr",                 offsetof(XMACH_DEF::arm_thread_state64_t,cpsr),                     8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
    {pad,               "pad",                  offsetof(XMACH_DEF::arm_thread_state64_t,pad),                      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_unix_thread_ppc_32
{
const FW_DEF::HEADER_RECORD records[]=
{
    {srr0,              "srr0",                 offsetof(XMACH_DEF::ppc_thread_state32_t,srr0),                     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {srr1,              "srr1",                 offsetof(XMACH_DEF::ppc_thread_state32_t,srr1),                     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r0,                "r0",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*0,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r1,                "r1",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*1,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r2,                "r2",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*2,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r3,                "r3",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*3,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r4,                "r4",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*4,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r5,                "r5",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*5,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r6,                "r6",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*6,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r7,                "r7",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*7,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r8,                "r8",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*8,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r9,                "r9",                   offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*9,      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r10,               "r10",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*10,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r11,               "r11",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*11,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r12,               "r12",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*12,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r13,               "r13",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*13,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r14,               "r14",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*14,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r15,               "r15",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*15,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r16,               "r16",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*16,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r17,               "r17",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*17,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r18,               "r18",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*18,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r19,               "r19",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*19,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r10,               "r20",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*20,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r21,               "r21",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*21,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r22,               "r22",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*22,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r23,               "r23",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*23,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r24,               "r24",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*24,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r25,               "r25",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*25,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r26,               "r26",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*26,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r27,               "r27",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*27,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r28,               "r28",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*28,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r29,               "r29",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*29,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r30,               "r30",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*30,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {r31,               "r31",                  offsetof(XMACH_DEF::ppc_thread_state32_t,r)+sizeof(quint32)*31,     4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ct,                "ct",                   offsetof(XMACH_DEF::ppc_thread_state32_t,ct),                       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {xer,               "xer",                  offsetof(XMACH_DEF::ppc_thread_state32_t,xer),                      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {lr,                "lr",                   offsetof(XMACH_DEF::ppc_thread_state32_t,lr),                       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {ctr,               "ctr",                  offsetof(XMACH_DEF::ppc_thread_state32_t,ctr),                      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {mq,                "mq",                   offsetof(XMACH_DEF::ppc_thread_state32_t,mq),                       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {vrsave,            "vrsave",               offsetof(XMACH_DEF::ppc_thread_state32_t,vrsave),                   4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_unix_thread_m68k_32
{
const FW_DEF::HEADER_RECORD records[]=
{
    {dreg0,             "dreg0",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*0,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg1,             "dreg1",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*1,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg2,             "dreg2",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*2,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg3,             "dreg3",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*3,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg4,             "dreg4",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*4,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg5,             "dreg5",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*5,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg6,             "dreg6",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*6,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {dreg7,             "dreg7",                offsetof(XMACH_DEF::m68k_thread_state32_t,dreg)+sizeof(quint32)*7,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg0,             "areg0",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*0,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg1,             "areg1",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*1,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg2,             "areg2",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*2,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg3,             "areg3",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*3,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg4,             "areg4",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*4,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg5,             "areg5",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*5,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg6,             "areg6",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*6,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {areg7,             "areg7",                offsetof(XMACH_DEF::m68k_thread_state32_t,areg)+sizeof(quint32)*7,  4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {pad0,              "pad0",                 offsetof(XMACH_DEF::m68k_thread_state32_t,pad0),                    2,      "uint16",           FW_DEF::VAL_TYPE_DATA},
    {sr,                "sr",                   offsetof(XMACH_DEF::m68k_thread_state32_t,sr),                      2,      "uint16",           FW_DEF::VAL_TYPE_DATA},
    {pc,                "pc",                   offsetof(XMACH_DEF::m68k_thread_state32_t,pc),                      4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};

extern const FW_DEF::HEADER_RECORD records[__data_size];
}

namespace N_mach_nlist
{
const FW_DEF::HEADER_RECORD records32[]=
{
    {n_strx,                "n_strx",           offsetof(XMACH_DEF::nlist,n_strx),          4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {n_type,                "n_type",           offsetof(XMACH_DEF::nlist,n_type),          1,      "uint8",            FW_DEF::VAL_TYPE_DATA},
    {n_sect,                "n_sect",           offsetof(XMACH_DEF::nlist,n_sect),          1,      "uint8",            FW_DEF::VAL_TYPE_DATA},
    {n_desc,                "n_desc",           offsetof(XMACH_DEF::nlist,n_desc),          2,      "uint16",           FW_DEF::VAL_TYPE_DATA},
    {n_value,               "n_value",          offsetof(XMACH_DEF::nlist,n_value),         4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
};

const FW_DEF::HEADER_RECORD records64[]=
{
    {n_strx,                "n_strx",           offsetof(XMACH_DEF::nlist_64,n_strx),       4,      "uint32",           FW_DEF::VAL_TYPE_DATA},
    {n_type,                "n_type",           offsetof(XMACH_DEF::nlist_64,n_type),       1,      "uint8",            FW_DEF::VAL_TYPE_DATA},
    {n_sect,                "n_sect",           offsetof(XMACH_DEF::nlist_64,n_sect),       1,      "uint8",            FW_DEF::VAL_TYPE_DATA},
    {n_desc,                "n_desc",           offsetof(XMACH_DEF::nlist_64,n_desc),       2,      "uint16",           FW_DEF::VAL_TYPE_DATA},
    {n_value,               "n_value",          offsetof(XMACH_DEF::nlist_64,n_value),      8,      "uint64",           FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_data_in_code_entry
{
const FW_DEF::HEADER_RECORD records[]=
{
    {offset,                QObject::tr("Offset"),  offsetof(XMACH_DEF::data_in_code_entry,offset),         4,      "uint32",           FW_DEF::VAL_TYPE_OFFSET},
    {length,                QObject::tr("Length"),  offsetof(XMACH_DEF::data_in_code_entry,length),         1,      "uint8",            FW_DEF::VAL_TYPE_SIZE},
    {kind,                  "kind",                 offsetof(XMACH_DEF::data_in_code_entry,kind),           1,      "uint8",            FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_modtab32
{
const FW_DEF::HEADER_RECORD records[]=
{
    {module_name,               "module_name",              offsetof(XMACH_DEF::dylib_module,module_name),              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iextdefsym,                "iextdefsym",               offsetof(XMACH_DEF::dylib_module,iextdefsym),               4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {nextdefsym,                "nextdefsym",               offsetof(XMACH_DEF::dylib_module,nextdefsym),               4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {irefsym,                   "irefsym",                  offsetof(XMACH_DEF::dylib_module,irefsym),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nrefsym,                   "nrefsym",                  offsetof(XMACH_DEF::dylib_module,nrefsym),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {ilocalsym,                 "ilocalsym",                offsetof(XMACH_DEF::dylib_module,ilocalsym),                4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nlocalsym,                 "flags",                    offsetof(XMACH_DEF::dylib_module,nlocalsym),                4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iextrel,                   "iextrel",                  offsetof(XMACH_DEF::dylib_module,iextrel),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nextrel,                   "nextrel",                  offsetof(XMACH_DEF::dylib_module,nextrel),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iinit_iterm,               "iinit_iterm",              offsetof(XMACH_DEF::dylib_module,iinit_iterm),              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {ninit_nterm,               "ninit_nterm",              offsetof(XMACH_DEF::dylib_module,ninit_nterm),              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {objc_module_info_addr,     "objc_module_info_addr",    offsetof(XMACH_DEF::dylib_module,objc_module_info_addr),    4,      "uint32",               FW_DEF::VAL_TYPE_ADDRESS},
    {objc_module_info_size,     "objc_module_info_size",    offsetof(XMACH_DEF::dylib_module,objc_module_info_size),    4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_modtab64
{
const FW_DEF::HEADER_RECORD records[]=
{
    {module_name,               "module_name",              offsetof(XMACH_DEF::dylib_module,module_name),              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iextdefsym,                "iextdefsym",               offsetof(XMACH_DEF::dylib_module,iextdefsym),               4,      "uint32",               FW_DEF::VAL_TYPE_SIZE},
    {nextdefsym,                "nextdefsym",               offsetof(XMACH_DEF::dylib_module,nextdefsym),               4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {irefsym,                   "irefsym",                  offsetof(XMACH_DEF::dylib_module,irefsym),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nrefsym,                   "nrefsym",                  offsetof(XMACH_DEF::dylib_module,nrefsym),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {ilocalsym,                 "ilocalsym",                offsetof(XMACH_DEF::dylib_module,ilocalsym),                4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nlocalsym,                 "flags",                    offsetof(XMACH_DEF::dylib_module,nlocalsym),                4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iextrel,                   "iextrel",                  offsetof(XMACH_DEF::dylib_module,iextrel),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {nextrel,                   "nextrel",                  offsetof(XMACH_DEF::dylib_module,nextrel),                  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {iinit_iterm,               "iinit_iterm",              offsetof(XMACH_DEF::dylib_module,iinit_iterm),              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {ninit_nterm,               "ninit_nterm",              offsetof(XMACH_DEF::dylib_module,ninit_nterm),              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {objc_module_info_addr,     "objc_module_info_addr",    offsetof(XMACH_DEF::dylib_module,objc_module_info_addr),    4,      "uint32",               FW_DEF::VAL_TYPE_ADDRESS},
    {objc_module_info_size,     "objc_module_info_size",    offsetof(XMACH_DEF::dylib_module,objc_module_info_size),    8,      "uint64",               FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_mach_table_of_contents
{
const FW_DEF::HEADER_RECORD records[]=
{
    {symbol_index,              "symbol_index",             offsetof(XMACH_DEF::dylib_table_of_contents,symbol_index),  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {module_index,              "module_index",             offsetof(XMACH_DEF::dylib_table_of_contents,module_index),  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_relocs
{
const FW_DEF::HEADER_RECORD records[]=
{
    {r_address,                 "r_address",                offsetof(XMACH_DEF::relocation_info,r_address),     4,      "uint32",               FW_DEF::VAL_TYPE_ADDRESS},
    {value,                     QObject::tr("Value"),       offsetof(XMACH_DEF::relocation_info,s),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_relocs_E
{
const FW_DEF::HEADER_RECORD records[]=
{
    {r_address,                 "r_address",                offsetof(XMACH_DEF::relocation_info,r_address),     4,      "uint32",               FW_DEF::VAL_TYPE_ADDRESS},
    {r_symbolnum,               "r_symbolnum",              offsetof(XMACH_DEF::relocation_info,s),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {r_pcrel,                   "r_pcrel",                  offsetof(XMACH_DEF::relocation_info,s),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {r_length,                  "r_length",                 offsetof(XMACH_DEF::relocation_info,s),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {r_extern,                  "r_extern",                 offsetof(XMACH_DEF::relocation_info,s),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {r_type,                    "r_type",                   offsetof(XMACH_DEF::relocation_info,s),             4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_value
{
const FW_DEF::HEADER_RECORD records[]=
{
    {value,                     QObject::tr("Value"),       0,              4,       "uint32",                FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_mach_refsyms
{
const FW_DEF::HEADER_RECORD records[]=
{
    {isym,                      "isym",                     0,              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {flags,                     "flags",                    0,              4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}
