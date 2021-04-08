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

namespace N_mach_libraries
{
const FW_DEF::HEADER_RECORD records[]=
{
    {timestamp,             "timestamp",                0,  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {current_version,       "current_version",          0,  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
    {compatibility_version, "compatibility_version",    0,  4,      "uint32",               FW_DEF::VAL_TYPE_DATA},
};
}
