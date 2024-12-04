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

namespace X_mach_header {
const XFW_DEF::HEADER_RECORD records32[] = {
    {magic, "magic", offsetof(XMACH_DEF::mach_header, magic), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cputype, "cputype", offsetof(XMACH_DEF::mach_header, cputype), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cpusubtype, "cpusubtype", offsetof(XMACH_DEF::mach_header, cpusubtype), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {filetype, "filetype", offsetof(XMACH_DEF::mach_header, filetype), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ncmds, "ncmds", offsetof(XMACH_DEF::mach_header, ncmds), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {sizeofcmds, "sizeofcmds", offsetof(XMACH_DEF::mach_header, sizeofcmds), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {flags, "flags", offsetof(XMACH_DEF::mach_header, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {magic, "magic", offsetof(XMACH_DEF::mach_header_64, magic), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cputype, "cputype", offsetof(XMACH_DEF::mach_header_64, cputype), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cpusubtype, "cpusubtype", offsetof(XMACH_DEF::mach_header_64, cpusubtype), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {filetype, "filetype", offsetof(XMACH_DEF::mach_header_64, filetype), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ncmds, "ncmds", offsetof(XMACH_DEF::mach_header_64, ncmds), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {sizeofcmds, "sizeofcmds", offsetof(XMACH_DEF::mach_header_64, sizeofcmds), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {flags, "flags", offsetof(XMACH_DEF::mach_header_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved, "reserved", offsetof(XMACH_DEF::mach_header_64, reserved), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}  // namespace X_mach_header

namespace X_mach_commands {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::load_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::load_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_segments {
const XFW_DEF::HEADER_RECORD records32[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::segment_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::segment_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {segname, "segname", offsetof(XMACH_DEF::segment_command, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {vmaddr, "vmaddr", offsetof(XMACH_DEF::segment_command, vmaddr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {vmsize, "vmsize", offsetof(XMACH_DEF::segment_command, vmsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {fileoff, "fileoff", offsetof(XMACH_DEF::segment_command, fileoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {filesize, "filesize", offsetof(XMACH_DEF::segment_command, filesize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {maxprot, "maxprot", offsetof(XMACH_DEF::segment_command, maxprot), 4, "int32", XFW_DEF::VAL_TYPE_INT_},
    {initprot, "initprot", offsetof(XMACH_DEF::segment_command, initprot), 4, "int32", XFW_DEF::VAL_TYPE_INT_},
    {nsects, "nsects", offsetof(XMACH_DEF::segment_command, nsects), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags, "flags", offsetof(XMACH_DEF::segment_command, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::segment_command_64, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::segment_command_64, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {segname, "segname", offsetof(XMACH_DEF::segment_command_64, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {vmaddr, "vmaddr", offsetof(XMACH_DEF::segment_command_64, vmaddr), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {vmsize, "vmsize", offsetof(XMACH_DEF::segment_command_64, vmsize), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {fileoff, "fileoff", offsetof(XMACH_DEF::segment_command_64, fileoff), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {filesize, "filesize", offsetof(XMACH_DEF::segment_command_64, filesize), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {maxprot, "maxprot", offsetof(XMACH_DEF::segment_command_64, maxprot), 4, "int32", XFW_DEF::VAL_TYPE_INT_},
    {initprot, "initprot", offsetof(XMACH_DEF::segment_command_64, initprot), 4, "int32", XFW_DEF::VAL_TYPE_INT_},
    {nsects, "nsects", offsetof(XMACH_DEF::segment_command_64, nsects), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags, "flags", offsetof(XMACH_DEF::segment_command_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}  // namespace X_mach_segments

namespace X_mach_sections32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {sectname, "sectname", offsetof(XMACH_DEF::section, sectname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {segname, "segname", offsetof(XMACH_DEF::section, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {addr, "addr", offsetof(XMACH_DEF::section, addr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {size, "size", offsetof(XMACH_DEF::section, size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {offset, "offset", offsetof(XMACH_DEF::section, offset), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {align, "align", offsetof(XMACH_DEF::section, align), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reloff, "reloff", offsetof(XMACH_DEF::section, reloff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nreloc, "nreloc", offsetof(XMACH_DEF::section, nreloc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags, "flags", offsetof(XMACH_DEF::section, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved1, "reserved1", offsetof(XMACH_DEF::section, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved2, "reserved2", offsetof(XMACH_DEF::section, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_sections32_E {
const XFW_DEF::HEADER_RECORD records[] = {
    {sectname, "sectname", offsetof(XMACH_DEF::section, sectname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {segname, "segname", offsetof(XMACH_DEF::section, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {addr, "addr", offsetof(XMACH_DEF::section, addr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {size, "size", offsetof(XMACH_DEF::section, size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {offset, "offset", offsetof(XMACH_DEF::section, offset), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {align, "align", offsetof(XMACH_DEF::section, align), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reloff, "reloff", offsetof(XMACH_DEF::section, reloff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nreloc, "nreloc", offsetof(XMACH_DEF::section, nreloc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags_0, "flags", offsetof(XMACH_DEF::section, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags_1, "", -1, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_},
    {flags_2, "", -1, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_},
    {reserved1, "reserved1", offsetof(XMACH_DEF::section, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved2, "reserved2", offsetof(XMACH_DEF::section, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_sections64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {sectname, "sectname", offsetof(XMACH_DEF::section_64, sectname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {segname, "segname", offsetof(XMACH_DEF::section_64, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {addr, "addr", offsetof(XMACH_DEF::section_64, addr), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {size, "size", offsetof(XMACH_DEF::section_64, size), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {offset, "offset", offsetof(XMACH_DEF::section_64, offset), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {align, "align", offsetof(XMACH_DEF::section_64, align), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reloff, "reloff", offsetof(XMACH_DEF::section_64, reloff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nreloc, "nreloc", offsetof(XMACH_DEF::section_64, nreloc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags, "flags", offsetof(XMACH_DEF::section_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved1, "reserved1", offsetof(XMACH_DEF::section_64, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved2, "reserved2", offsetof(XMACH_DEF::section_64, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved3, "reserved3", offsetof(XMACH_DEF::section_64, reserved3), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_sections64_E {
const XFW_DEF::HEADER_RECORD records[] = {
    {sectname, "sectname", offsetof(XMACH_DEF::section_64, sectname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {segname, "segname", offsetof(XMACH_DEF::section_64, segname), 16, "char[16]", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_ANSI | XFW_DEF::VAL_TYPE_STRING},
    {addr, "addr", offsetof(XMACH_DEF::section_64, addr), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {size, "size", offsetof(XMACH_DEF::section_64, size), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {offset, "offset", offsetof(XMACH_DEF::section_64, offset), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {align, "align", offsetof(XMACH_DEF::section_64, align), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reloff, "reloff", offsetof(XMACH_DEF::section_64, reloff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nreloc, "nreloc", offsetof(XMACH_DEF::section_64, nreloc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags_0, "flags", offsetof(XMACH_DEF::section_64, flags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags_1, "", -1, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_},
    {flags_2, "", -1, 0, "", XFW_DEF::VAL_TYPE_UNKNOWN_},
    {reserved1, "reserved1", offsetof(XMACH_DEF::section_64, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved2, "reserved2", offsetof(XMACH_DEF::section_64, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved3, "reserved3", offsetof(XMACH_DEF::section_64, reserved3), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_dylib {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::load_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::load_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {name, "name", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, name), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {timestamp, "timestamp", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, timestamp), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {current_version, "current_version", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, current_version), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {compatibility_version, "compatibility_version", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::dylib, compatibility_version), 4, "uint32",
     XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_fmv_library {
const XFW_DEF::HEADER_RECORD records[] = {
    {name, "name", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::fvmlib, name), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {minor_version, "minor_version", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::fvmlib, minor_version), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {header_addr, "header_addr", sizeof(XMACH_DEF::load_command) + offsetof(XMACH_DEF::fvmlib, header_addr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_dyld_info {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::dyld_info_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::dyld_info_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {rebase_off, "rebase_off", offsetof(XMACH_DEF::dyld_info_command, rebase_off), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {rebase_size, "rebase_size", offsetof(XMACH_DEF::dyld_info_command, rebase_size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {bind_off, "bind_off", offsetof(XMACH_DEF::dyld_info_command, bind_off), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {bind_size, "bind_size", offsetof(XMACH_DEF::dyld_info_command, bind_size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {weak_bind_off, "weak_bind_off", offsetof(XMACH_DEF::dyld_info_command, weak_bind_off), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {weak_bind_size, "weak_bind_size", offsetof(XMACH_DEF::dyld_info_command, weak_bind_size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {lazy_bind_off, "lazy_bind_off", offsetof(XMACH_DEF::dyld_info_command, lazy_bind_off), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {lazy_bind_size, "lazy_bind_size", offsetof(XMACH_DEF::dyld_info_command, lazy_bind_size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {export_off, "export_off", offsetof(XMACH_DEF::dyld_info_command, export_off), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {export_size, "export_size", offsetof(XMACH_DEF::dyld_info_command, export_size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_uuid {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::uuid_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::uuid_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {uuid, "uuid", offsetof(XMACH_DEF::uuid_command, uuid), 16, "bytes", XFW_DEF::VAL_TYPE_ARRAY_ | XFW_DEF::VAL_TYPE_UUID_},
};
}

namespace X_mach_dylinker {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::dylinker_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::dylinker_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {name, "name", offsetof(XMACH_DEF::dylinker_command, name), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}

namespace X_mach_rpath {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::rpath_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::rpath_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {path, "path", offsetof(XMACH_DEF::rpath_command, path), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}

namespace X_mach_sub_umbrella {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::sub_umbrella_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::sub_umbrella_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {sub_umbrella, "sub_umbrella", offsetof(XMACH_DEF::sub_umbrella_command, sub_umbrella), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}

namespace X_mach_sub_client {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::sub_client_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::sub_client_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {client, "client", offsetof(XMACH_DEF::sub_client_command, client), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}

namespace X_mach_sub_library {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::sub_library_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::sub_library_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {sub_library, "sub_library", offsetof(XMACH_DEF::sub_library_command, sub_library), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}

namespace X_mach_symtab {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::symtab_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::symtab_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {symoff, "symoff", offsetof(XMACH_DEF::symtab_command, symoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nsyms, "nsyms", offsetof(XMACH_DEF::symtab_command, nsyms), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {stroff, "stroff", offsetof(XMACH_DEF::symtab_command, stroff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {strsize, "strsize", offsetof(XMACH_DEF::symtab_command, strsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_dysymtab {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::dysymtab_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::dysymtab_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {ilocalsym, "ilocalsym", offsetof(XMACH_DEF::dysymtab_command, ilocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nlocalsym, "nlocalsym", offsetof(XMACH_DEF::dysymtab_command, nlocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iextdefsym, "iextdefsym", offsetof(XMACH_DEF::dysymtab_command, iextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nextdefsym, "nextdefsym", offsetof(XMACH_DEF::dysymtab_command, nextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iundefsym, "iundefsym", offsetof(XMACH_DEF::dysymtab_command, iundefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nundefsym, "nundefsym", offsetof(XMACH_DEF::dysymtab_command, nundefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {tocoff, "tocoff", offsetof(XMACH_DEF::dysymtab_command, tocoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {ntoc, "ntoc", offsetof(XMACH_DEF::dysymtab_command, ntoc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {modtaboff, "modtaboff", offsetof(XMACH_DEF::dysymtab_command, modtaboff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nmodtab, "nmodtab", offsetof(XMACH_DEF::dysymtab_command, nmodtab), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {extrefsymoff, "extrefsymoff", offsetof(XMACH_DEF::dysymtab_command, extrefsymoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nextrefsyms, "nextrefsyms", offsetof(XMACH_DEF::dysymtab_command, nextrefsyms), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {indirectsymoff, "indirectsymoff", offsetof(XMACH_DEF::dysymtab_command, indirectsymoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nindirectsyms, "nindirectsyms", offsetof(XMACH_DEF::dysymtab_command, nindirectsyms), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {extreloff, "extreloff", offsetof(XMACH_DEF::dysymtab_command, extreloff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nextrel, "nextrel", offsetof(XMACH_DEF::dysymtab_command, nextrel), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {locreloff, "locreloff", offsetof(XMACH_DEF::dysymtab_command, locreloff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {nlocrel, "nlocrel", offsetof(XMACH_DEF::dysymtab_command, nlocrel), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_version_min {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::version_min_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::version_min_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {version, "version", offsetof(XMACH_DEF::version_min_command, version), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {sdk, "sdk", offsetof(XMACH_DEF::version_min_command, sdk), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_build_version {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::build_version_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::build_version_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {platform, "platform", offsetof(XMACH_DEF::build_version_command, platform), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {minos, "minos", offsetof(XMACH_DEF::build_version_command, minos), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {sdk, "sdk", offsetof(XMACH_DEF::build_version_command, sdk), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ntools, "ntools", offsetof(XMACH_DEF::build_version_command, ntools), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_source_version {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::source_version_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::source_version_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {version, "version", offsetof(XMACH_DEF::source_version_command, version), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_encryption_info {
const XFW_DEF::HEADER_RECORD records32[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::encryption_info_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::encryption_info_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {cryptoff, "cryptoff", offsetof(XMACH_DEF::encryption_info_command, cryptoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {cryptsize, "cryptsize", offsetof(XMACH_DEF::encryption_info_command, cryptsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {cryptid, "cryptid", offsetof(XMACH_DEF::encryption_info_command, cryptid), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::encryption_info_command_64, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::encryption_info_command_64, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {cryptoff, "cryptoff", offsetof(XMACH_DEF::encryption_info_command_64, cryptoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {cryptsize, "cryptsize", offsetof(XMACH_DEF::encryption_info_command_64, cryptsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {cryptid, "cryptid", offsetof(XMACH_DEF::encryption_info_command_64, cryptid), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {pad, "pad", offsetof(XMACH_DEF::encryption_info_command_64, pad), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}  // namespace X_mach_encryption_info
namespace X_mach_linkedit_data {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::linkedit_data_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::linkedit_data_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {dataoff, "dataoff", offsetof(XMACH_DEF::linkedit_data_command, dataoff), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {datasize, "datasize", offsetof(XMACH_DEF::linkedit_data_command, datasize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_entry_point {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::entry_point_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::entry_point_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {entryoff, "entryoff", offsetof(XMACH_DEF::entry_point_command, entryoff), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {stacksize, "stacksize", offsetof(XMACH_DEF::entry_point_command, stacksize), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_fileset_entry {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::fileset_entry_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::fileset_entry_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {vmaddr, "vmaddr", offsetof(XMACH_DEF::fileset_entry_command, vmaddr), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {fileoff, "fileoff", offsetof(XMACH_DEF::fileset_entry_command, fileoff), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {entry_id, "entry_id", offsetof(XMACH_DEF::fileset_entry_command, entry_id), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {reserved, "reserved", offsetof(XMACH_DEF::fileset_entry_command, reserved), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_unix_thread {
const XFW_DEF::HEADER_RECORD records[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::unix_thread_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::unix_thread_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {flavor, "flavor", offsetof(XMACH_DEF::unix_thread_command, flavor), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {count, "count", offsetof(XMACH_DEF::unix_thread_command, count), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_routines {
const XFW_DEF::HEADER_RECORD records32[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::routines_command, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::routines_command, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {init_address, "init_address", offsetof(XMACH_DEF::routines_command, init_address), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {init_module, "init_module", offsetof(XMACH_DEF::routines_command, init_module), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved1, "reserved1", offsetof(XMACH_DEF::routines_command, reserved1), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved2, "reserved2", offsetof(XMACH_DEF::routines_command, reserved2), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved3, "reserved3", offsetof(XMACH_DEF::routines_command, reserved3), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved4, "reserved4", offsetof(XMACH_DEF::routines_command, reserved4), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved5, "reserved5", offsetof(XMACH_DEF::routines_command, reserved5), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {reserved6, "reserved6", offsetof(XMACH_DEF::routines_command, reserved6), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {cmd, "cmd", offsetof(XMACH_DEF::routines_command_64, cmd), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cmdsize, "cmdsize", offsetof(XMACH_DEF::routines_command_64, cmdsize), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {init_address, "init_address", offsetof(XMACH_DEF::routines_command_64, init_address), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {init_module, "init_module", offsetof(XMACH_DEF::routines_command_64, init_module), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {reserved1, "reserved1", offsetof(XMACH_DEF::routines_command_64, reserved1), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {reserved2, "reserved2", offsetof(XMACH_DEF::routines_command_64, reserved2), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {reserved3, "reserved3", offsetof(XMACH_DEF::routines_command_64, reserved3), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {reserved4, "reserved4", offsetof(XMACH_DEF::routines_command_64, reserved4), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {reserved5, "reserved5", offsetof(XMACH_DEF::routines_command_64, reserved5), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {reserved6, "reserved6", offsetof(XMACH_DEF::routines_command_64, reserved6), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_unix_thread_x86_32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {eax, "eax", offsetof(XMACH_DEF::x86_thread_state32_t, eax), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ebx, "ebx", offsetof(XMACH_DEF::x86_thread_state32_t, ebx), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ecx, "ecx", offsetof(XMACH_DEF::x86_thread_state32_t, ecx), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {edx, "edx", offsetof(XMACH_DEF::x86_thread_state32_t, edx), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {edi, "edi", offsetof(XMACH_DEF::x86_thread_state32_t, edi), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {esi, "esi", offsetof(XMACH_DEF::x86_thread_state32_t, esi), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ebp, "ebp", offsetof(XMACH_DEF::x86_thread_state32_t, ebp), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {esp, "esp", offsetof(XMACH_DEF::x86_thread_state32_t, esp), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ss, "ss", offsetof(XMACH_DEF::x86_thread_state32_t, ss), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {eflags, "eflags", offsetof(XMACH_DEF::x86_thread_state32_t, eflags), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {eip, "eip", offsetof(XMACH_DEF::x86_thread_state32_t, eip), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cs, "cs", offsetof(XMACH_DEF::x86_thread_state32_t, cs), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ds, "ds", offsetof(XMACH_DEF::x86_thread_state32_t, ds), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {es, "es", offsetof(XMACH_DEF::x86_thread_state32_t, es), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {fs, "fs", offsetof(XMACH_DEF::x86_thread_state32_t, fs), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {gs, "gs", offsetof(XMACH_DEF::x86_thread_state32_t, gs), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_unix_thread_x86_64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {rax, "rax", offsetof(XMACH_DEF::x86_thread_state64_t, rax), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rbx, "rbx", offsetof(XMACH_DEF::x86_thread_state64_t, rbx), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rcx, "rcx", offsetof(XMACH_DEF::x86_thread_state64_t, rcx), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rdx, "rdx", offsetof(XMACH_DEF::x86_thread_state64_t, rdx), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rdi, "rdi", offsetof(XMACH_DEF::x86_thread_state64_t, rdi), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rsi, "rsi", offsetof(XMACH_DEF::x86_thread_state64_t, rsi), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rbp, "rbp", offsetof(XMACH_DEF::x86_thread_state64_t, rbp), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rsp, "esp", offsetof(XMACH_DEF::x86_thread_state64_t, rsp), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r8, "r8", offsetof(XMACH_DEF::x86_thread_state64_t, r8), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r9, "r9", offsetof(XMACH_DEF::x86_thread_state64_t, r9), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r10, "r10", offsetof(XMACH_DEF::x86_thread_state64_t, r10), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r11, "r11", offsetof(XMACH_DEF::x86_thread_state64_t, r11), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r12, "r12", offsetof(XMACH_DEF::x86_thread_state64_t, r12), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r13, "r13", offsetof(XMACH_DEF::x86_thread_state64_t, r13), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r14, "r14", offsetof(XMACH_DEF::x86_thread_state64_t, r14), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {r15, "r15", offsetof(XMACH_DEF::x86_thread_state64_t, r15), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rip, "rip", offsetof(XMACH_DEF::x86_thread_state64_t, rip), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {rflags, "rflags", offsetof(XMACH_DEF::x86_thread_state64_t, rflags), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {cs, "cs", offsetof(XMACH_DEF::x86_thread_state64_t, cs), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {fs, "fs", offsetof(XMACH_DEF::x86_thread_state64_t, fs), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {gs, "gs", offsetof(XMACH_DEF::x86_thread_state64_t, gs), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_unix_thread_arm_32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {r0, "r0", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r1, "r1", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r2, "r2", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r3, "r3", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r4, "r4", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r5, "r5", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r6, "r6", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r7, "r7", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r8, "r8", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 8, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r9, "r9", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 9, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r10, "r10", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 10, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r11, "r11", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 11, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r12, "r12", offsetof(XMACH_DEF::arm_thread_state32_t, r) + sizeof(quint32) * 12, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {sp, "sp", offsetof(XMACH_DEF::arm_thread_state32_t, sp), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {lr, "lr", offsetof(XMACH_DEF::arm_thread_state32_t, lr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {pc, "pc", offsetof(XMACH_DEF::arm_thread_state32_t, pc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {cpsr, "cpsr", offsetof(XMACH_DEF::arm_thread_state32_t, cpsr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_unix_thread_arm_64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {x0, "x0", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 0, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x1, "x1", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 1, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x2, "x2", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 2, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x3, "x3", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 3, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x4, "x4", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 4, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x5, "x5", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 5, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x6, "x6", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 6, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x7, "x7", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 7, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x8, "x8", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 8, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x9, "x9", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 9, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x10, "x10", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 10, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x11, "x11", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 11, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x12, "x12", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 12, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x13, "x13", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 13, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x14, "x14", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 14, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x15, "x15", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 15, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x16, "x16", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 16, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x17, "x17", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 17, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x18, "x18", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 18, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x19, "x19", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 19, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x20, "x20", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 20, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x21, "x21", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 21, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x22, "x22", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 22, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x23, "x23", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 23, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x24, "x24", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 24, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x25, "x25", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 25, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x26, "x26", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 26, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x27, "x27", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 27, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {x28, "x28", offsetof(XMACH_DEF::arm_thread_state64_t, x) + sizeof(quint64) * 28, 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {fp, "fp", offsetof(XMACH_DEF::arm_thread_state64_t, fp), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {lr, "lr", offsetof(XMACH_DEF::arm_thread_state64_t, lr), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {sp, "sp", offsetof(XMACH_DEF::arm_thread_state64_t, sp), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {pc, "pc", offsetof(XMACH_DEF::arm_thread_state64_t, pc), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {cpsr, "cpsr", offsetof(XMACH_DEF::arm_thread_state64_t, cpsr), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
    {pad, "pad", offsetof(XMACH_DEF::arm_thread_state64_t, pad), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_unix_thread_ppc_32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {srr0, "srr0", offsetof(XMACH_DEF::ppc_thread_state32_t, srr0), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {srr1, "srr1", offsetof(XMACH_DEF::ppc_thread_state32_t, srr1), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r0, "r0", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r1, "r1", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r2, "r2", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r3, "r3", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r4, "r4", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r5, "r5", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r6, "r6", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r7, "r7", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r8, "r8", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 8, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r9, "r9", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 9, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r10, "r10", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 10, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r11, "r11", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 11, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r12, "r12", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 12, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r13, "r13", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 13, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r14, "r14", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 14, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r15, "r15", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 15, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r16, "r16", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 16, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r17, "r17", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 17, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r18, "r18", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 18, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r19, "r19", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 19, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r10, "r20", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 20, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r21, "r21", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 21, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r22, "r22", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 22, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r23, "r23", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 23, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r24, "r24", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 24, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r25, "r25", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 25, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r26, "r26", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 26, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r27, "r27", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 27, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r28, "r28", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 28, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r29, "r29", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 29, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r30, "r30", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 30, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r31, "r31", offsetof(XMACH_DEF::ppc_thread_state32_t, r) + sizeof(quint32) * 31, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ct, "ct", offsetof(XMACH_DEF::ppc_thread_state32_t, ct), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {xer, "xer", offsetof(XMACH_DEF::ppc_thread_state32_t, xer), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {lr, "lr", offsetof(XMACH_DEF::ppc_thread_state32_t, lr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ctr, "ctr", offsetof(XMACH_DEF::ppc_thread_state32_t, ctr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {mq, "mq", offsetof(XMACH_DEF::ppc_thread_state32_t, mq), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {vrsave, "vrsave", offsetof(XMACH_DEF::ppc_thread_state32_t, vrsave), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_unix_thread_ppc_32

namespace X_mach_unix_thread_m68k_32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {dreg0, "dreg0", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg1, "dreg1", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg2, "dreg2", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg3, "dreg3", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg4, "dreg4", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg5, "dreg5", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg6, "dreg6", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {dreg7, "dreg7", offsetof(XMACH_DEF::m68k_thread_state32_t, dreg) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg0, "areg0", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg1, "areg1", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 1, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg2, "areg2", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 2, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg3, "areg3", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 3, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg4, "areg4", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 4, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg5, "areg5", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 5, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg6, "areg6", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 6, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {areg7, "areg7", offsetof(XMACH_DEF::m68k_thread_state32_t, areg) + sizeof(quint32) * 7, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {pad0, "pad0", offsetof(XMACH_DEF::m68k_thread_state32_t, pad0), 2, "uint16", XFW_DEF::VAL_TYPE_INT_},
    {sr, "sr", offsetof(XMACH_DEF::m68k_thread_state32_t, sr), 2, "uint16", XFW_DEF::VAL_TYPE_INT_},
    {pc, "pc", offsetof(XMACH_DEF::m68k_thread_state32_t, pc), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};

extern const XFW_DEF::HEADER_RECORD records[__data_size];
}  // namespace X_mach_unix_thread_m68k_32

namespace X_mach_nlist {
const XFW_DEF::HEADER_RECORD records32[] = {
    {n_strx, "n_strx", offsetof(XMACH_DEF::nlist, n_strx), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {n_type, "n_type", offsetof(XMACH_DEF::nlist, n_type), 1, "uint8", XFW_DEF::VAL_TYPE_INT_},
    {n_sect, "n_sect", offsetof(XMACH_DEF::nlist, n_sect), 1, "uint8", XFW_DEF::VAL_TYPE_INT_},
    {n_desc, "n_desc", offsetof(XMACH_DEF::nlist, n_desc), 2, "uint16", XFW_DEF::VAL_TYPE_INT_},
    {n_value, "n_value", offsetof(XMACH_DEF::nlist, n_value), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};

const XFW_DEF::HEADER_RECORD records64[] = {
    {n_strx, "n_strx", offsetof(XMACH_DEF::nlist_64, n_strx), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {n_type, "n_type", offsetof(XMACH_DEF::nlist_64, n_type), 1, "uint8", XFW_DEF::VAL_TYPE_INT_},
    {n_sect, "n_sect", offsetof(XMACH_DEF::nlist_64, n_sect), 1, "uint8", XFW_DEF::VAL_TYPE_INT_},
    {n_desc, "n_desc", offsetof(XMACH_DEF::nlist_64, n_desc), 2, "uint16", XFW_DEF::VAL_TYPE_INT_},
    {n_value, "n_value", offsetof(XMACH_DEF::nlist_64, n_value), 8, "uint64", XFW_DEF::VAL_TYPE_INT_},
};
}  // namespace X_mach_nlist

namespace X_mach_data_in_code_entry {
const XFW_DEF::HEADER_RECORD records[] = {
    {offset, "offset", offsetof(XMACH_DEF::data_in_code_entry, offset), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
    {length, "length", offsetof(XMACH_DEF::data_in_code_entry, length), 1, "uint8", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {kind, "kind", offsetof(XMACH_DEF::data_in_code_entry, kind), 1, "uint8", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_modtab32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {module_name, "module_name", offsetof(XMACH_DEF::dylib_module, module_name), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iextdefsym, "iextdefsym", offsetof(XMACH_DEF::dylib_module, iextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {nextdefsym, "nextdefsym", offsetof(XMACH_DEF::dylib_module, nextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {irefsym, "irefsym", offsetof(XMACH_DEF::dylib_module, irefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nrefsym, "nrefsym", offsetof(XMACH_DEF::dylib_module, nrefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ilocalsym, "ilocalsym", offsetof(XMACH_DEF::dylib_module, ilocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nlocalsym, "flags", offsetof(XMACH_DEF::dylib_module, nlocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iextrel, "iextrel", offsetof(XMACH_DEF::dylib_module, iextrel), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nextrel, "nextrel", offsetof(XMACH_DEF::dylib_module, nextrel), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iinit_iterm, "iinit_iterm", offsetof(XMACH_DEF::dylib_module, iinit_iterm), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ninit_nterm, "ninit_nterm", offsetof(XMACH_DEF::dylib_module, ninit_nterm), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {objc_module_info_addr, "objc_module_info_addr", offsetof(XMACH_DEF::dylib_module, objc_module_info_addr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {objc_module_info_size, "objc_module_info_size", offsetof(XMACH_DEF::dylib_module, objc_module_info_size), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_modtab64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {module_name, "module_name", offsetof(XMACH_DEF::dylib_module, module_name), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iextdefsym, "iextdefsym", offsetof(XMACH_DEF::dylib_module, iextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {nextdefsym, "nextdefsym", offsetof(XMACH_DEF::dylib_module, nextdefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {irefsym, "irefsym", offsetof(XMACH_DEF::dylib_module, irefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nrefsym, "nrefsym", offsetof(XMACH_DEF::dylib_module, nrefsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ilocalsym, "ilocalsym", offsetof(XMACH_DEF::dylib_module, ilocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nlocalsym, "flags", offsetof(XMACH_DEF::dylib_module, nlocalsym), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iextrel, "iextrel", offsetof(XMACH_DEF::dylib_module, iextrel), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {nextrel, "nextrel", offsetof(XMACH_DEF::dylib_module, nextrel), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {iinit_iterm, "iinit_iterm", offsetof(XMACH_DEF::dylib_module, iinit_iterm), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {ninit_nterm, "ninit_nterm", offsetof(XMACH_DEF::dylib_module, ninit_nterm), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {objc_module_info_addr, "objc_module_info_addr", offsetof(XMACH_DEF::dylib_module, objc_module_info_addr), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {objc_module_info_size, "objc_module_info_size", offsetof(XMACH_DEF::dylib_module, objc_module_info_size), 8, "uint64", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
};
}

namespace X_mach_table_of_contents {
const XFW_DEF::HEADER_RECORD records[] = {
    {symbol_index, "symbol_index", offsetof(XMACH_DEF::dylib_table_of_contents, symbol_index), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {module_index, "module_index", offsetof(XMACH_DEF::dylib_table_of_contents, module_index), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_relocs {
const XFW_DEF::HEADER_RECORD records[] = {
    {r_address, "r_address", offsetof(XMACH_DEF::relocation_info, r_address), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {value, "value", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_relocs_E {
const XFW_DEF::HEADER_RECORD records[] = {
    {r_address, "r_address", offsetof(XMACH_DEF::relocation_info, r_address), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_ADDRESS_},
    {r_symbolnum, "r_symbolnum", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r_pcrel, "r_pcrel", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r_length, "r_length", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r_extern, "r_extern", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {r_type, "r_type", offsetof(XMACH_DEF::relocation_info, s), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_value {
const XFW_DEF::HEADER_RECORD records[] = {
    {value, "value", 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_refsyms {
const XFW_DEF::HEADER_RECORD records[] = {
    {isym, "isym", 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {flags, "flags", 0, 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_SuperBlob {
const XFW_DEF::HEADER_RECORD records[] = {
    {magic, "magic", offsetof(XMACH_DEF::__SC_SuperBlob, magic), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {length, "length", offsetof(XMACH_DEF::__SC_SuperBlob, length), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_SIZE_},
    {count, "count", offsetof(XMACH_DEF::__SC_SuperBlob, count), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
};
}

namespace X_mach_BlobIndex {
const XFW_DEF::HEADER_RECORD records[] = {
    {type, "type", offsetof(XMACH_DEF::__BlobIndex, type), 4, "uint32", XFW_DEF::VAL_TYPE_INT_},
    {offset, "offset", offsetof(XMACH_DEF::__BlobIndex, offset), 4, "uint32", XFW_DEF::VAL_TYPE_INT_ | XFW_DEF::VAL_TYPE_OFFSET_},
};
}
