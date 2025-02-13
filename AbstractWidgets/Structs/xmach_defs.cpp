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
#include "xmach_defs.h"
namespace XTYPE_MACH {

namespace X_mach_header {
const XFW_DEF::HEADER_RECORD records32[] = {
    {magic, "magic", offsetof(XMACH_DEF::mach_header, magic), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cputype, "cputype", offsetof(XMACH_DEF::mach_header, cputype), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cpusubtype, "cpusubtype", offsetof(XMACH_DEF::mach_header, cpusubtype), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {filetype, "filetype", offsetof(XMACH_DEF::mach_header, filetype), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ncmds, "ncmds", offsetof(XMACH_DEF::mach_header, ncmds), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {sizeofcmds, "sizeofcmds", offsetof(XMACH_DEF::mach_header, sizeofcmds), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADEREND, -1},
    {flags, "flags", offsetof(XMACH_DEF::mach_header, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {magic, "magic", offsetof(XMACH_DEF::mach_header_64, magic), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cputype, "cputype", offsetof(XMACH_DEF::mach_header_64, cputype), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cpusubtype, "cpusubtype", offsetof(XMACH_DEF::mach_header_64, cpusubtype), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {filetype, "filetype", offsetof(XMACH_DEF::mach_header_64, filetype), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ncmds, "ncmds", offsetof(XMACH_DEF::mach_header_64, ncmds), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {sizeofcmds, "sizeofcmds", offsetof(XMACH_DEF::mach_header_64, sizeofcmds), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADEREND, -1},
    {flags, "flags", offsetof(XMACH_DEF::mach_header_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved, "reserved", offsetof(XMACH_DEF::mach_header_64, reserved), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_mach_header

namespace X_load_commands {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::load_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::load_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
};
}  // namespace X_load_commands

namespace X_segment_command {
const XFW_DEF::HEADER_RECORD records32[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::segment_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::segment_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {segname, "segname", offsetof(XMACH_DEF::segment_command, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING,
     -1},
    {vmaddr, "vmaddr", offsetof(XMACH_DEF::segment_command, vmaddr), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {vmsize, "vmsize", offsetof(XMACH_DEF::segment_command, vmsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {fileoff, "fileoff", offsetof(XMACH_DEF::segment_command, fileoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {filesize, "filesize", offsetof(XMACH_DEF::segment_command, filesize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, fileoff},
    {maxprot, "maxprot", offsetof(XMACH_DEF::segment_command, maxprot), 4, "int32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {initprot, "initprot", offsetof(XMACH_DEF::segment_command, initprot), 4, "int32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nsects, "nsects", offsetof(XMACH_DEF::segment_command, nsects), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {flags, "flags", offsetof(XMACH_DEF::segment_command, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::segment_command_64, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::segment_command_64, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {segname, "segname", offsetof(XMACH_DEF::segment_command_64, segname), 16, "char[16]",
     XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING, -1},
    {vmaddr, "vmaddr", offsetof(XMACH_DEF::segment_command_64, vmaddr), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {vmsize, "vmsize", offsetof(XMACH_DEF::segment_command_64, vmsize), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {fileoff, "fileoff", offsetof(XMACH_DEF::segment_command_64, fileoff), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {filesize, "filesize", offsetof(XMACH_DEF::segment_command_64, filesize), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, fileoff},
    {maxprot, "maxprot", offsetof(XMACH_DEF::segment_command_64, maxprot), 4, "int32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {initprot, "initprot", offsetof(XMACH_DEF::segment_command_64, initprot), 4, "int32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nsects, "nsects", offsetof(XMACH_DEF::segment_command_64, nsects), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {flags, "flags", offsetof(XMACH_DEF::segment_command_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_segment_command

namespace X_section {
const XFW_DEF::HEADER_RECORD records[] = {
    {sectname, "sectname", offsetof(XMACH_DEF::section, sectname), 16, "char[16]", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING, -1},
    {segname, "segname", offsetof(XMACH_DEF::section, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING, -1},
    {addr, "addr", offsetof(XMACH_DEF::section, addr), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {size, "size", offsetof(XMACH_DEF::section, size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, offset},
    {offset, "offset", offsetof(XMACH_DEF::section, offset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {align, "align", offsetof(XMACH_DEF::section, align), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {reloff, "reloff", offsetof(XMACH_DEF::section, reloff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nreloc, "nreloc", offsetof(XMACH_DEF::section, nreloc), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {flags, "flags", offsetof(XMACH_DEF::section, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved1, "reserved1", offsetof(XMACH_DEF::section, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved2, "reserved2", offsetof(XMACH_DEF::section, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}
namespace X_section_64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {sectname, "sectname", offsetof(XMACH_DEF::section_64, sectname), 16, "char[16]", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING,
     -1},
    {segname, "segname", offsetof(XMACH_DEF::section_64, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING, -1},
    {addr, "addr", offsetof(XMACH_DEF::section_64, addr), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {size, "size", offsetof(XMACH_DEF::section_64, size), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, offset},
    {offset, "offset", offsetof(XMACH_DEF::section_64, offset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {align, "align", offsetof(XMACH_DEF::section_64, align), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {reloff, "reloff", offsetof(XMACH_DEF::section_64, reloff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nreloc, "nreloc", offsetof(XMACH_DEF::section_64, nreloc), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {flags, "flags", offsetof(XMACH_DEF::section_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved1, "reserved1", offsetof(XMACH_DEF::section_64, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved2, "reserved2", offsetof(XMACH_DEF::section_64, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved3, "reserved3", offsetof(XMACH_DEF::section_64, reserved3), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_dylib_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::load_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::load_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {name, "name", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, name), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER | XFW_DEF::VAL_TYPE_ANSI, -1},
    {timestamp, "timestamp", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, timestamp), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {current_version, "current_version", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, current_version), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
    {compatibility_version, "compatibility_version", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, compatibility_version), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
};
}

namespace X_fvmlib_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::fvmlib_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::fvmlib_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {name, "name", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::fvmlib, name), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER | XFW_DEF::VAL_TYPE_ANSI, -1},
    {minor_version, "minor_version", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::fvmlib, minor_version), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
    {header_addr, "header_addr", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::fvmlib, header_addr), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
};
}

namespace X_dyld_info_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::dyld_info_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::dyld_info_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {rebase_off, "rebase_off", offsetof(XMACH_DEF::dyld_info_command, rebase_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {rebase_size, "rebase_size", offsetof(XMACH_DEF::dyld_info_command, rebase_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, rebase_off},
    {bind_off, "bind_off", offsetof(XMACH_DEF::dyld_info_command, bind_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {bind_size, "bind_size", offsetof(XMACH_DEF::dyld_info_command, bind_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, bind_off},
    {weak_bind_off, "weak_bind_off", offsetof(XMACH_DEF::dyld_info_command, weak_bind_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {weak_bind_size, "weak_bind_size", offsetof(XMACH_DEF::dyld_info_command, weak_bind_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     weak_bind_off},
    {lazy_bind_off, "lazy_bind_off", offsetof(XMACH_DEF::dyld_info_command, lazy_bind_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {lazy_bind_size, "lazy_bind_size", offsetof(XMACH_DEF::dyld_info_command, lazy_bind_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE,
     lazy_bind_off},
    {export_off, "export_off", offsetof(XMACH_DEF::dyld_info_command, export_off), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {export_size, "export_size", offsetof(XMACH_DEF::dyld_info_command, export_size), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, export_off},
};
}

namespace X_uuid_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::uuid_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::uuid_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {uuid, "uuid", offsetof(XMACH_DEF::uuid_command, uuid), 16, "bytes", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_HEX, -1},
};
}

namespace X_dylinker_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::dylinker_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::dylinker_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {name, "name", offsetof(XMACH_DEF::dylinker_command, name), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER | XFW_DEF::VAL_TYPE_ANSI, -1},
};
}

namespace X_rpath_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::rpath_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::rpath_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {path, "path", offsetof(XMACH_DEF::rpath_command, path), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER | XFW_DEF::VAL_TYPE_ANSI, -1},
};
}

namespace X_sub_umbrella_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::sub_umbrella_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::sub_umbrella_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {sub_umbrella, "sub_umbrella", offsetof(XMACH_DEF::sub_umbrella_command, sub_umbrella), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_sub_client_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::sub_client_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::sub_client_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {client, "client", offsetof(XMACH_DEF::sub_client_command, client), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_sub_library_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::sub_library_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::sub_library_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {sub_library, "sub_library", offsetof(XMACH_DEF::sub_library_command, sub_library), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
};
}

namespace X_symtab_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::symtab_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::symtab_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {symoff, "symoff", offsetof(XMACH_DEF::symtab_command, symoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nsyms, "nsyms", offsetof(XMACH_DEF::symtab_command, nsyms), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {stroff, "stroff", offsetof(XMACH_DEF::symtab_command, stroff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {strsize, "strsize", offsetof(XMACH_DEF::symtab_command, strsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, stroff},
};
}

namespace X_dysymtab_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::dysymtab_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::dysymtab_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {ilocalsym, "ilocalsym", offsetof(XMACH_DEF::dysymtab_command, ilocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nlocalsym, "nlocalsym", offsetof(XMACH_DEF::dysymtab_command, nlocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {iextdefsym, "iextdefsym", offsetof(XMACH_DEF::dysymtab_command, iextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nextdefsym, "nextdefsym", offsetof(XMACH_DEF::dysymtab_command, nextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {iundefsym, "iundefsym", offsetof(XMACH_DEF::dysymtab_command, iundefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nundefsym, "nundefsym", offsetof(XMACH_DEF::dysymtab_command, nundefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {tocoff, "tocoff", offsetof(XMACH_DEF::dysymtab_command, tocoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {ntoc, "ntoc", offsetof(XMACH_DEF::dysymtab_command, ntoc), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {modtaboff, "modtaboff", offsetof(XMACH_DEF::dysymtab_command, modtaboff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nmodtab, "nmodtab", offsetof(XMACH_DEF::dysymtab_command, nmodtab), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {extrefsymoff, "extrefsymoff", offsetof(XMACH_DEF::dysymtab_command, extrefsymoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nextrefsyms, "nextrefsyms", offsetof(XMACH_DEF::dysymtab_command, nextrefsyms), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {indirectsymoff, "indirectsymoff", offsetof(XMACH_DEF::dysymtab_command, indirectsymoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nindirectsyms, "nindirectsyms", offsetof(XMACH_DEF::dysymtab_command, nindirectsyms), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {extreloff, "extreloff", offsetof(XMACH_DEF::dysymtab_command, extreloff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nextrel, "nextrel", offsetof(XMACH_DEF::dysymtab_command, nextrel), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
    {locreloff, "locreloff", offsetof(XMACH_DEF::dysymtab_command, locreloff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nlocrel, "nlocrel", offsetof(XMACH_DEF::dysymtab_command, nlocrel), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
}

namespace X_version_min_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::version_min_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::version_min_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {version, "version", offsetof(XMACH_DEF::version_min_command, version), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL,
     -1},
    {sdk, "sdk", offsetof(XMACH_DEF::version_min_command, sdk), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
};
}

namespace X_build_version_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::build_version_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::build_version_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {platform, "platform", offsetof(XMACH_DEF::build_version_command, platform), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {minos, "minos", offsetof(XMACH_DEF::build_version_command, minos), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
    {sdk, "sdk", offsetof(XMACH_DEF::build_version_command, sdk), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
    {ntools, "ntools", offsetof(XMACH_DEF::build_version_command, ntools), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
}

namespace X_build_tool_version {
const XFW_DEF::HEADER_RECORD records[] = {
    {tool, "tool", offsetof(XMACH_DEF::build_tool_version, tool), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {version, "version", offsetof(XMACH_DEF::build_tool_version, version), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL,
     -1},
};
}

namespace X_source_version_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::source_version_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::source_version_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {version, "version", offsetof(XMACH_DEF::source_version_command, version), 8, "uint64",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
};
}

namespace X_encryption_info_command {
const XFW_DEF::HEADER_RECORD records32[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::encryption_info_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::encryption_info_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {cryptoff, "cryptoff", offsetof(XMACH_DEF::encryption_info_command, cryptoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {cryptsize, "cryptsize", offsetof(XMACH_DEF::encryption_info_command, cryptsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, cryptoff},
    {cryptid, "cryptid", offsetof(XMACH_DEF::encryption_info_command, cryptid), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::encryption_info_command_64, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::encryption_info_command_64, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {cryptoff, "cryptoff", offsetof(XMACH_DEF::encryption_info_command_64, cryptoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {cryptsize, "cryptsize", offsetof(XMACH_DEF::encryption_info_command_64, cryptsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, cryptoff},
    {cryptid, "cryptid", offsetof(XMACH_DEF::encryption_info_command_64, cryptid), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pad, "pad", offsetof(XMACH_DEF::encryption_info_command_64, pad), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_encryption_info_command
namespace X_linkedit_data_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::linkedit_data_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::linkedit_data_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {dataoff, "dataoff", offsetof(XMACH_DEF::linkedit_data_command, dataoff), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {datasize, "datasize", offsetof(XMACH_DEF::linkedit_data_command, datasize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, dataoff},
};
}

namespace X_entry_point_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::entry_point_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::entry_point_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {entryoff, "entryoff", offsetof(XMACH_DEF::entry_point_command, entryoff), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_CODE, -1},
    {stacksize, "stacksize", offsetof(XMACH_DEF::entry_point_command, stacksize), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
};
}

namespace X_fileset_entry_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::fileset_entry_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::fileset_entry_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {vmaddr, "vmaddr", offsetof(XMACH_DEF::fileset_entry_command, vmaddr), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {fileoff, "fileoff", offsetof(XMACH_DEF::fileset_entry_command, fileoff), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {entry_id, "entry_id", offsetof(XMACH_DEF::fileset_entry_command, entry_id), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {reserved, "reserved", offsetof(XMACH_DEF::fileset_entry_command, reserved), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_unix_thread_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::unix_thread_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::unix_thread_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {flavor, "flavor", offsetof(XMACH_DEF::unix_thread_command, flavor), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {count, "count", offsetof(XMACH_DEF::unix_thread_command, count), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
}

namespace X_twolevel_hints_command {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::twolevel_hints_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::twolevel_hints_command, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {offset, "offset", offsetof(XMACH_DEF::twolevel_hints_command, offset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {nhints, "count", offsetof(XMACH_DEF::twolevel_hints_command, nhints), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
}

namespace X_twolevel_hint {
const XFW_DEF::HEADER_RECORD records[] = {
    {hint, "hint", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_routines_command {
const XFW_DEF::HEADER_RECORD records32[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::routines_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::routines_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER,
     -1},
    {init_address, "init_address", offsetof(XMACH_DEF::routines_command, init_address), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {init_module, "init_module", offsetof(XMACH_DEF::routines_command, init_module), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved1, "reserved1", offsetof(XMACH_DEF::routines_command, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved2, "reserved2", offsetof(XMACH_DEF::routines_command, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved3, "reserved3", offsetof(XMACH_DEF::routines_command, reserved3), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved4, "reserved4", offsetof(XMACH_DEF::routines_command, reserved4), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved5, "reserved5", offsetof(XMACH_DEF::routines_command, reserved5), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved6, "reserved6", offsetof(XMACH_DEF::routines_command, reserved6), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::routines_command_64, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::routines_command_64, cmdsize), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {init_address, "init_address", offsetof(XMACH_DEF::routines_command_64, init_address), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {init_module, "init_module", offsetof(XMACH_DEF::routines_command_64, init_module), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved1, "reserved1", offsetof(XMACH_DEF::routines_command_64, reserved1), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved2, "reserved2", offsetof(XMACH_DEF::routines_command_64, reserved2), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved3, "reserved3", offsetof(XMACH_DEF::routines_command_64, reserved3), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved4, "reserved4", offsetof(XMACH_DEF::routines_command_64, reserved4), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved5, "reserved5", offsetof(XMACH_DEF::routines_command_64, reserved5), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {reserved6, "reserved6", offsetof(XMACH_DEF::routines_command_64, reserved6), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_routines_command

namespace X_x86_thread_state32_t {
const XFW_DEF::HEADER_RECORD records[] = {
    {eax, "eax", offsetof(XMACH_DEF::x86_thread_state32_t, eax), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ebx, "ebx", offsetof(XMACH_DEF::x86_thread_state32_t, ebx), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ecx, "ecx", offsetof(XMACH_DEF::x86_thread_state32_t, ecx), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {edx, "edx", offsetof(XMACH_DEF::x86_thread_state32_t, edx), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {edi, "edi", offsetof(XMACH_DEF::x86_thread_state32_t, edi), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {esi, "esi", offsetof(XMACH_DEF::x86_thread_state32_t, esi), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ebp, "ebp", offsetof(XMACH_DEF::x86_thread_state32_t, ebp), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {esp, "esp", offsetof(XMACH_DEF::x86_thread_state32_t, esp), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ss, "ss", offsetof(XMACH_DEF::x86_thread_state32_t, ss), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {eflags, "eflags", offsetof(XMACH_DEF::x86_thread_state32_t, eflags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {eip, "eip", offsetof(XMACH_DEF::x86_thread_state32_t, eip), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {cs, "cs", offsetof(XMACH_DEF::x86_thread_state32_t, cs), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ds, "ds", offsetof(XMACH_DEF::x86_thread_state32_t, ds), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {es, "es", offsetof(XMACH_DEF::x86_thread_state32_t, es), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {fs, "fs", offsetof(XMACH_DEF::x86_thread_state32_t, fs), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {gs, "gs", offsetof(XMACH_DEF::x86_thread_state32_t, gs), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_x86_thread_state64_t {
const XFW_DEF::HEADER_RECORD records[] = {
    {rax, "rax", offsetof(XMACH_DEF::x86_thread_state64_t, rax), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rbx, "rbx", offsetof(XMACH_DEF::x86_thread_state64_t, rbx), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rcx, "rcx", offsetof(XMACH_DEF::x86_thread_state64_t, rcx), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rdx, "rdx", offsetof(XMACH_DEF::x86_thread_state64_t, rdx), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rdi, "rdi", offsetof(XMACH_DEF::x86_thread_state64_t, rdi), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rsi, "rsi", offsetof(XMACH_DEF::x86_thread_state64_t, rsi), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rbp, "rbp", offsetof(XMACH_DEF::x86_thread_state64_t, rbp), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rsp, "esp", offsetof(XMACH_DEF::x86_thread_state64_t, rsp), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r8, "r8", offsetof(XMACH_DEF::x86_thread_state64_t, r8), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r9, "r9", offsetof(XMACH_DEF::x86_thread_state64_t, r9), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r10, "r10", offsetof(XMACH_DEF::x86_thread_state64_t, r10), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r11, "r11", offsetof(XMACH_DEF::x86_thread_state64_t, r11), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r12, "r12", offsetof(XMACH_DEF::x86_thread_state64_t, r12), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r13, "r13", offsetof(XMACH_DEF::x86_thread_state64_t, r13), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r14, "r14", offsetof(XMACH_DEF::x86_thread_state64_t, r14), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r15, "r15", offsetof(XMACH_DEF::x86_thread_state64_t, r15), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {rip, "rip", offsetof(XMACH_DEF::x86_thread_state64_t, rip), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {rflags, "rflags", offsetof(XMACH_DEF::x86_thread_state64_t, rflags), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {cs, "cs", offsetof(XMACH_DEF::x86_thread_state64_t, cs), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {fs, "fs", offsetof(XMACH_DEF::x86_thread_state64_t, fs), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {gs, "gs", offsetof(XMACH_DEF::x86_thread_state64_t, gs), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_arm_thread_state32_t {
const XFW_DEF::HEADER_RECORD records[] = {
    {r0, "r0", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r1, "r1", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r2, "r2", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r3, "r3", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r4, "r4", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r5, "r5", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r6, "r6", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r7, "r7", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r8, "r8", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 8, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r9, "r9", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 9, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r10, "r10", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 10, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r11, "r11", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 11, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r12, "r12", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 12, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {sp, "sp", offsetof(XMACH_DEF::arm_thread_state32_t, sp), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {lr, "lr", offsetof(XMACH_DEF::arm_thread_state32_t, lr), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pc, "pc", offsetof(XMACH_DEF::arm_thread_state32_t, pc), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {cpsr, "cpsr", offsetof(XMACH_DEF::arm_thread_state32_t, cpsr), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_arm_thread_state64_t {
const XFW_DEF::HEADER_RECORD records[] = {
    {x0, "x0", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 0, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x1, "x1", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 1, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x2, "x2", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 2, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x3, "x3", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 3, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x4, "x4", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 4, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x5, "x5", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 5, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x6, "x6", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 6, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x7, "x7", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 7, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x8, "x8", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 8, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x9, "x9", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 9, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x10, "x10", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 10, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x11, "x11", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 11, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x12, "x12", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 12, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x13, "x13", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 13, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x14, "x14", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 14, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x15, "x15", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 15, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x16, "x16", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 16, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x17, "x17", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 17, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x18, "x18", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 18, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x19, "x19", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 19, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x20, "x20", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 20, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x21, "x21", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 21, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x22, "x22", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 22, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x23, "x23", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 23, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x24, "x24", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 24, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x25, "x25", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 25, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x26, "x26", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 26, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x27, "x27", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 27, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {x28, "x28", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 28, 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {fp, "fp", offsetof(XMACH_DEF::arm_thread_state64_t, fp), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {lr, "lr", offsetof(XMACH_DEF::arm_thread_state64_t, lr), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {sp, "sp", offsetof(XMACH_DEF::arm_thread_state64_t, sp), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pc, "pc", offsetof(XMACH_DEF::arm_thread_state64_t, pc), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {cpsr, "cpsr", offsetof(XMACH_DEF::arm_thread_state64_t, cpsr), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pad, "pad", offsetof(XMACH_DEF::arm_thread_state64_t, pad), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_ppc_thread_state32_t {
const XFW_DEF::HEADER_RECORD records[] = {
    {srr0, "srr0", offsetof(XMACH_DEF::ppc_thread_state32_t, srr0), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
    {srr1, "srr1", offsetof(XMACH_DEF::ppc_thread_state32_t, srr1), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r0, "r0", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r1, "r1", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r2, "r2", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r3, "r3", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r4, "r4", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r5, "r5", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r6, "r6", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r7, "r7", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r8, "r8", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 8, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r9, "r9", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 9, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r10, "r10", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 10, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r11, "r11", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 11, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r12, "r12", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 12, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r13, "r13", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 13, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r14, "r14", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 14, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r15, "r15", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 15, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r16, "r16", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 16, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r17, "r17", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 17, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r18, "r18", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 18, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r19, "r19", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 19, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r10, "r20", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 20, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r21, "r21", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 21, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r22, "r22", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 22, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r23, "r23", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 23, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r24, "r24", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 24, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r25, "r25", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 25, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r26, "r26", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 26, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r27, "r27", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 27, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r28, "r28", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 28, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r29, "r29", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 29, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r30, "r30", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 30, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r31, "r31", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 31, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ct, "ct", offsetof(XMACH_DEF::ppc_thread_state32_t, ct), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {xer, "xer", offsetof(XMACH_DEF::ppc_thread_state32_t, xer), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {lr, "lr", offsetof(XMACH_DEF::ppc_thread_state32_t, lr), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ctr, "ctr", offsetof(XMACH_DEF::ppc_thread_state32_t, ctr), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {mq, "mq", offsetof(XMACH_DEF::ppc_thread_state32_t, mq), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {vrsave, "vrsave", offsetof(XMACH_DEF::ppc_thread_state32_t, vrsave), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_ppc_thread_state32_t

namespace X_m68k_thread_state32_t {
const XFW_DEF::HEADER_RECORD records[] = {
    {dreg0, "dreg0", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg1, "dreg1", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg2, "dreg2", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg3, "dreg3", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg4, "dreg4", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg5, "dreg5", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg6, "dreg6", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {dreg7, "dreg7", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg0, "areg0", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg1, "areg1", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg2, "areg2", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg3, "areg3", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg4, "areg4", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg5, "areg5", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg6, "areg6", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {areg7, "areg7", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pad0, "pad0", offsetof(XMACH_DEF::m68k_thread_state32_t, pad0), 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {sr, "sr", offsetof(XMACH_DEF::m68k_thread_state32_t, sr), 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pc, "pc", offsetof(XMACH_DEF::m68k_thread_state32_t, pc), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS | XFW_DEF::VAL_TYPE_CODE, -1},
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_m68k_thread_state32_t

namespace X_nlist {
const XFW_DEF::HEADER_RECORD records32[] = {
    {n_strx, "n_strx", offsetof(XMACH_DEF::nlist, n_strx), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_type, "n_type", offsetof(XMACH_DEF::nlist, n_type), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_sect, "n_sect", offsetof(XMACH_DEF::nlist, n_sect), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_desc, "n_desc", offsetof(XMACH_DEF::nlist, n_desc), 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_value, "n_value", offsetof(XMACH_DEF::nlist, n_value), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {n_strx, "n_strx", offsetof(XMACH_DEF::nlist_64, n_strx), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_type, "n_type", offsetof(XMACH_DEF::nlist_64, n_type), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_sect, "n_sect", offsetof(XMACH_DEF::nlist_64, n_sect), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_desc, "n_desc", offsetof(XMACH_DEF::nlist_64, n_desc), 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {n_value, "n_value", offsetof(XMACH_DEF::nlist_64, n_value), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}  // namespace X_nlist

namespace X_mach_data_in_code_entry {
const XFW_DEF::HEADER_RECORD records[] = {
    {offset, "offset", offsetof(XMACH_DEF::data_in_code_entry, offset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET, -1},
    {length, "length", offsetof(XMACH_DEF::data_in_code_entry, length), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {kind, "kind", offsetof(XMACH_DEF::data_in_code_entry, kind), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_mach_modtab32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {module_name, "module_name", offsetof(XMACH_DEF::dylib_module, module_name), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {iextdefsym, "iextdefsym", offsetof(XMACH_DEF::dylib_module, iextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {nextdefsym, "nextdefsym", offsetof(XMACH_DEF::dylib_module, nextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {irefsym, "irefsym", offsetof(XMACH_DEF::dylib_module, irefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nrefsym, "nrefsym", offsetof(XMACH_DEF::dylib_module, nrefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ilocalsym, "ilocalsym", offsetof(XMACH_DEF::dylib_module, ilocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nlocalsym, "flags", offsetof(XMACH_DEF::dylib_module, nlocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {iextrel, "iextrel", offsetof(XMACH_DEF::dylib_module, iextrel), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nextrel, "nextrel", offsetof(XMACH_DEF::dylib_module, nextrel), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {iinit_iterm, "iinit_iterm", offsetof(XMACH_DEF::dylib_module, iinit_iterm), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ninit_nterm, "ninit_nterm", offsetof(XMACH_DEF::dylib_module, ninit_nterm), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {objc_module_info_addr, "objc_module_info_addr", offsetof(XMACH_DEF::dylib_module, objc_module_info_addr), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {objc_module_info_size, "objc_module_info_size", offsetof(XMACH_DEF::dylib_module, objc_module_info_size), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
};
}

namespace X_mach_modtab64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {module_name, "module_name", offsetof(XMACH_DEF::dylib_module, module_name), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {iextdefsym, "iextdefsym", offsetof(XMACH_DEF::dylib_module, iextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {nextdefsym, "nextdefsym", offsetof(XMACH_DEF::dylib_module, nextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {irefsym, "irefsym", offsetof(XMACH_DEF::dylib_module, irefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nrefsym, "nrefsym", offsetof(XMACH_DEF::dylib_module, nrefsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ilocalsym, "ilocalsym", offsetof(XMACH_DEF::dylib_module, ilocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nlocalsym, "flags", offsetof(XMACH_DEF::dylib_module, nlocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {iextrel, "iextrel", offsetof(XMACH_DEF::dylib_module, iextrel), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nextrel, "nextrel", offsetof(XMACH_DEF::dylib_module, nextrel), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {iinit_iterm, "iinit_iterm", offsetof(XMACH_DEF::dylib_module, iinit_iterm), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {ninit_nterm, "ninit_nterm", offsetof(XMACH_DEF::dylib_module, ninit_nterm), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {objc_module_info_addr, "objc_module_info_addr", offsetof(XMACH_DEF::dylib_module, objc_module_info_addr), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {objc_module_info_size, "objc_module_info_size", offsetof(XMACH_DEF::dylib_module, objc_module_info_size), 8, "uint64",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
};
}

namespace X_mach_table_of_contents {
const XFW_DEF::HEADER_RECORD records[] = {
    {symbol_index, "symbol_index", offsetof(XMACH_DEF::dylib_table_of_contents, symbol_index), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {module_index, "module_index", offsetof(XMACH_DEF::dylib_table_of_contents, module_index), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_mach_relocs {
const XFW_DEF::HEADER_RECORD records[] = {
    {r_address, "r_address", offsetof(XMACH_DEF::relocation_info, r_address), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {value, "value", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_mach_relocs_E {
const XFW_DEF::HEADER_RECORD records[] = {
    {r_address, "r_address", offsetof(XMACH_DEF::relocation_info, r_address), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS, -1},
    {r_symbolnum, "r_symbolnum", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r_pcrel, "r_pcrel", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r_length, "r_length", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r_extern, "r_extern", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {r_type, "r_type", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_mach_value {
const XFW_DEF::HEADER_RECORD records[] = {
    {value, "value", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_mach_refsyms {
const XFW_DEF::HEADER_RECORD records[] = {
    {isym, "isym", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {flags, "flags", 0, 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

namespace X_SC_SuperBlob {
const XFW_DEF::HEADER_RECORD records[] = {
    {magic, "magic", offsetof(XMACH_DEF::__SC_SuperBlob, magic), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {length, "length", offsetof(XMACH_DEF::__SC_SuperBlob, length), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {count, "count", offsetof(XMACH_DEF::__SC_SuperBlob, count), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT, -1},
};
}

namespace X_CS_BlobIndex {
const XFW_DEF::HEADER_RECORD records[] = {
    {type, "type", offsetof(XMACH_DEF::CS_BlobIndex, type), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {offset, "offset", offsetof(XMACH_DEF::CS_BlobIndex, offset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOPARENT, -1},
};
}

namespace X_CS_CodeDirectory {
const XFW_DEF::HEADER_RECORD records[] = {
    {magic, "magic", offsetof(XMACH_DEF::CS_CodeDirectory, magic), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {length, "length", offsetof(XMACH_DEF::CS_CodeDirectory, length), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, -1},
    {version, "version", offsetof(XMACH_DEF::CS_CodeDirectory, version), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL,
     -1},
    {flags, "flags", offsetof(XMACH_DEF::CS_CodeDirectory, flags), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {hashOffset, "hashOffset", offsetof(XMACH_DEF::CS_CodeDirectory, hashOffset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {identOffset, "identOffset", offsetof(XMACH_DEF::CS_CodeDirectory, identOffset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {nSpecialSlots, "nSpecialSlots", offsetof(XMACH_DEF::CS_CodeDirectory, nSpecialSlots), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {nCodeSlots, "nCodeSlots", offsetof(XMACH_DEF::CS_CodeDirectory, nCodeSlots), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {codeLimit, "codeLimit", offsetof(XMACH_DEF::CS_CodeDirectory, codeLimit), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {hashSize, "hashSize", offsetof(XMACH_DEF::CS_CodeDirectory, hashSize), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {hashType, "hashType", offsetof(XMACH_DEF::CS_CodeDirectory, hashType), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {platform, "platform", offsetof(XMACH_DEF::CS_CodeDirectory, platform), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {pageSize, "pageSize", offsetof(XMACH_DEF::CS_CodeDirectory, pageSize), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {spare2, "spare2", offsetof(XMACH_DEF::CS_CodeDirectory, spare2), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {scatterOffset, "scatterOffset", offsetof(XMACH_DEF::CS_CodeDirectory, scatterOffset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {teamOffset, "teamOffset", offsetof(XMACH_DEF::CS_CodeDirectory, teamOffset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {spare3, "spare3", offsetof(XMACH_DEF::CS_CodeDirectory, spare3), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {codeLimit64, "codeLimit64", offsetof(XMACH_DEF::CS_CodeDirectory, codeLimit64), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {execSegBase, "execSegBase", offsetof(XMACH_DEF::CS_CodeDirectory, execSegBase), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {execSegLimit, "execSegLimit", offsetof(XMACH_DEF::CS_CodeDirectory, execSegLimit), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {execSegFlags, "execSegFlags", offsetof(XMACH_DEF::CS_CodeDirectory, execSegFlags), 8, "uint64", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {runtime, "runtime", offsetof(XMACH_DEF::CS_CodeDirectory, runtime), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {preEncryptOffset, "preEncryptOffset", offsetof(XMACH_DEF::CS_CodeDirectory, preEncryptOffset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {linkageHashType, "linkageHashType", offsetof(XMACH_DEF::CS_CodeDirectory, linkageHashType), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {linkageTruncated, "linkageTruncated", offsetof(XMACH_DEF::CS_CodeDirectory, linkageTruncated), 1, "uint8", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {spare4, "spare4", offsetof(XMACH_DEF::CS_CodeDirectory, spare4), 2, "uint16", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {linkageOffset, "linkageOffset", offsetof(XMACH_DEF::CS_CodeDirectory, linkageOffset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {linkageSize, "linkageSize", offsetof(XMACH_DEF::CS_CodeDirectory, linkageSize), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE, linkageOffset},
};
}

namespace X_dyld_chained_fixups_header {
const XFW_DEF::HEADER_RECORD records[] = {
    {fixups_version, "fixups_version", offsetof(XMACH_DEF::dyld_chained_fixups_header, fixups_version), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_VERSION | XFW_DEF::VAL_TYPE_FULL, -1},
    {starts_offset, "starts_offset", offsetof(XMACH_DEF::dyld_chained_fixups_header, starts_offset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {imports_offset, "imports_offset", offsetof(XMACH_DEF::dyld_chained_fixups_header, imports_offset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {symbols_offset, "symbols_offset", offsetof(XMACH_DEF::dyld_chained_fixups_header, symbols_offset), 4, "uint32",
     XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET | XFW_DEF::VAL_TYPE_RELTOHEADER, -1},
    {imports_count, "imports_count", offsetof(XMACH_DEF::dyld_chained_fixups_header, imports_count), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_COUNT,
     -1},
    {imports_format, "imports_format", offsetof(XMACH_DEF::dyld_chained_fixups_header, imports_format), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
    {symbols_format, "symbols_format", offsetof(XMACH_DEF::dyld_chained_fixups_header, symbols_format), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
};
}

// namespace X_mach_dyld_chained_starts_in_image {
// const XFW_DEF::HEADER_RECORD records[] = {
//     {seg_count, "seg_count", offsetof(XMACH_DEF::dyld_chained_starts_in_image, seg_count), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT, -1},
//     {seg_info_offset, "seg_info_offset", offsetof(XMACH_DEF::dyld_chained_starts_in_image, seg_info_offset), 4, "uint32", XFW_DEF::VAL_TYPE_DATA_INT |
//     XFW_DEF::VAL_TYPE_OFFSET_, -1},
// };
// }

}  // namespace XTYPE_MACH
