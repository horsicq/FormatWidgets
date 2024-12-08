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
#include "xelf_defs.h"

namespace X_Elf_Ehdr {
const XFW_DEF::HEADER_RECORD records32[] = {
    {ei_mag, "ei_mag", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 0, 4, "uint32 LE", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_class, "ei_class", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 4, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_data, "ei_data", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 5, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_version, "ei_version", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 6, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_osabi, "ei_osabi", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 7, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_abiversion, "ei_abiversion", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 8, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_pad, "ei_pad", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 9, 7, "unsigned char[7]", XFW_DEF::VAL_TYPE_DATA_ARRAY},
    {e_type, "e_type", offsetof(XELF_DEF::Elf32_Ehdr, e_type), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_machine, "e_machine", offsetof(XELF_DEF::Elf32_Ehdr, e_machine), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_version, "e_version", offsetof(XELF_DEF::Elf32_Ehdr, e_version), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_entry, "e_entry", offsetof(XELF_DEF::Elf32_Ehdr, e_entry), 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {e_phoff, "e_phoff", offsetof(XELF_DEF::Elf32_Ehdr, e_phoff), 4, "Elf32_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {e_shoff, "e_shoff", offsetof(XELF_DEF::Elf32_Ehdr, e_shoff), 4, "Elf32_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {e_flags, "e_flags", offsetof(XELF_DEF::Elf32_Ehdr, e_flags), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_ehsize, "e_ehsize", offsetof(XELF_DEF::Elf32_Ehdr, e_ehsize), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {e_phentsize, "e_phentsize", offsetof(XELF_DEF::Elf32_Ehdr, e_phentsize), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {e_phnum, "e_phnum", offsetof(XELF_DEF::Elf32_Ehdr, e_phnum), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_shentsize, "e_shentsize", offsetof(XELF_DEF::Elf32_Ehdr, e_shentsize), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {e_shnum, "e_shnum", offsetof(XELF_DEF::Elf32_Ehdr, e_shnum), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_shstrndx, "e_shstrndx", offsetof(XELF_DEF::Elf32_Ehdr, e_shstrndx), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {ei_mag, "ei_mag", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 0, 4, "uint32 LE", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_class, "ei_class", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 4, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_data, "ei_data", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 5, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_version, "ei_version", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 6, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_osabi, "ei_osabi", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 7, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_abiversion, "ei_abiversion", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 8, 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {ei_pad, "ei_pad", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 9, 7, "unsigned char[7]", XFW_DEF::VAL_TYPE_DATA_ARRAY},
    {e_type, "e_type", offsetof(XELF_DEF::Elf64_Ehdr, e_type), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_machine, "e_machine", offsetof(XELF_DEF::Elf64_Ehdr, e_machine), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_version, "e_version", offsetof(XELF_DEF::Elf64_Ehdr, e_version), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_entry, "e_entry", offsetof(XELF_DEF::Elf64_Ehdr, e_entry), 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {e_phoff, "e_phoff", offsetof(XELF_DEF::Elf64_Ehdr, e_phoff), 8, "Elf64_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {e_shoff, "e_shoff", offsetof(XELF_DEF::Elf64_Ehdr, e_shoff), 8, "Elf64_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {e_flags, "e_flags", offsetof(XELF_DEF::Elf64_Ehdr, e_flags), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_ehsize, "e_ehsize", offsetof(XELF_DEF::Elf64_Ehdr, e_ehsize), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {e_phentsize, "e_phentsize", offsetof(XELF_DEF::Elf64_Ehdr, e_phentsize), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {e_phnum, "e_phnum", offsetof(XELF_DEF::Elf64_Ehdr, e_phnum), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_shentsize, "e_shentsize", offsetof(XELF_DEF::Elf64_Ehdr, e_shentsize), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {e_shnum, "e_shnum", offsetof(XELF_DEF::Elf64_Ehdr, e_shnum), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {e_shstrndx, "e_shstrndx", offsetof(XELF_DEF::Elf64_Ehdr, e_shstrndx), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT},
};
}  // namespace X_Elf_Ehdr

namespace X_Elf_Shdr {
const XFW_DEF::HEADER_RECORD records32[] = {
    {sh_name, "sh_name", offsetof(XELF_DEF::Elf32_Shdr, sh_name), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_type, "sh_type", offsetof(XELF_DEF::Elf32_Shdr, sh_type), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_flags, "sh_flags", offsetof(XELF_DEF::Elf32_Shdr, sh_flags), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_addr, "sh_addr", offsetof(XELF_DEF::Elf32_Shdr, sh_addr), 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {sh_offset, "sh_offset", offsetof(XELF_DEF::Elf32_Shdr, sh_offset), 4, "Elf32_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {sh_size, "sh_size", offsetof(XELF_DEF::Elf32_Shdr, sh_size), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {sh_link, "sh_link", offsetof(XELF_DEF::Elf32_Shdr, sh_link), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_info, "sh_info", offsetof(XELF_DEF::Elf32_Shdr, sh_info), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_addralign, "sh_addralign", offsetof(XELF_DEF::Elf32_Shdr, sh_addralign), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_entsize, "sh_entsize", offsetof(XELF_DEF::Elf32_Shdr, sh_entsize), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {sh_name, "sh_name", offsetof(XELF_DEF::Elf64_Shdr, sh_name), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_type, "sh_type", offsetof(XELF_DEF::Elf64_Shdr, sh_type), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_flags, "sh_flags", offsetof(XELF_DEF::Elf64_Shdr, sh_flags), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_addr, "sh_addr", offsetof(XELF_DEF::Elf64_Shdr, sh_addr), 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {sh_offset, "sh_offset", offsetof(XELF_DEF::Elf64_Shdr, sh_offset), 8, "Elf64_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {sh_size, "sh_size", offsetof(XELF_DEF::Elf64_Shdr, sh_size), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {sh_link, "sh_link", offsetof(XELF_DEF::Elf64_Shdr, sh_link), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_info, "sh_info", offsetof(XELF_DEF::Elf64_Shdr, sh_info), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_addralign, "sh_addralign", offsetof(XELF_DEF::Elf64_Shdr, sh_addralign), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT},
    {sh_entsize, "sh_entsize", offsetof(XELF_DEF::Elf64_Shdr, sh_entsize), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
};
}  // namespace X_Elf_Shdr

namespace X_Elf_Phdr32 {
const XFW_DEF::HEADER_RECORD records[] = {
    {p_type, "p_type", offsetof(XELF_DEF::Elf32_Phdr, p_type), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {p_offset, "p_offset", offsetof(XELF_DEF::Elf32_Phdr, p_offset), 4, "Elf32_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {p_vaddr, "p_vaddr", offsetof(XELF_DEF::Elf32_Phdr, p_vaddr), 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {p_paddr, "p_paddr", offsetof(XELF_DEF::Elf32_Phdr, p_paddr), 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {p_filesz, "p_filesz", offsetof(XELF_DEF::Elf32_Phdr, p_filesz), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {p_memsz, "p_memsz", offsetof(XELF_DEF::Elf32_Phdr, p_memsz), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {p_flags, "p_flags", offsetof(XELF_DEF::Elf32_Phdr, p_flags), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {p_align, "p_align", offsetof(XELF_DEF::Elf32_Phdr, p_align), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
};
}

namespace X_Elf_Phdr64 {
const XFW_DEF::HEADER_RECORD records[] = {
    {p_type, "p_type", offsetof(XELF_DEF::Elf64_Phdr, p_type), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {p_flags, "p_flags", offsetof(XELF_DEF::Elf64_Phdr, p_flags), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {p_offset, "p_offset", offsetof(XELF_DEF::Elf64_Phdr, p_offset), 8, "Elf64_Off", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_OFFSET_},
    {p_vaddr, "p_vaddr", offsetof(XELF_DEF::Elf64_Phdr, p_vaddr), 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {p_paddr, "p_paddr", offsetof(XELF_DEF::Elf64_Phdr, p_paddr), 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {p_filesz, "p_filesz", offsetof(XELF_DEF::Elf64_Phdr, p_filesz), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {p_memsz, "p_memsz", offsetof(XELF_DEF::Elf64_Phdr, p_memsz), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {p_align, "p_align", offsetof(XELF_DEF::Elf64_Phdr, p_align), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT},
};
}

namespace X_Elf_DynamicArrayTags {
const XFW_DEF::HEADER_RECORD records32[] = {
    {d_tag, "d_tag", 0, 4, "Elf32_SWord", XFW_DEF::VAL_TYPE_DATA_INT},
    {d_value, "d_value", 4, 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {d_tag, "d_tag", 0, 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT},
    {d_value, "d_value", 8, 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
};
}  // namespace X_Elf_DynamicArrayTags

namespace X_ELF_LIBRARIES {
const XFW_DEF::HEADER_RECORD records[] = {
    {library_name, QObject::tr("Library name"), 0, 0, "", XFW_DEF::VAL_TYPE_DATA_INT},
};
}

namespace X_ELF_INTERPRETER {
const XFW_DEF::HEADER_RECORD records[] = {
    {interpreter, QObject::tr("Interpreter"), 0, -1, "Ansi string", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI},
};
}

namespace X_ELF_NOTES {
const XFW_DEF::HEADER_RECORD records[] = {
    {type, QObject::tr("Type"), 0, -1, "", XFW_DEF::VAL_TYPE_DATA_INT},
    {name, QObject::tr("Name"), 0, -1, "", XFW_DEF::VAL_TYPE_DATA_INT},
};
}

namespace X_ELF_RUNPATH {
const XFW_DEF::HEADER_RECORD records[] = {
    {runpath, "runpath", 0, -1, "Ansi string", XFW_DEF::VAL_TYPE_DATA_ARRAY | XFW_DEF::VAL_TYPE_ANSI},
};
}

namespace X_Elf32_Sym {
const XFW_DEF::HEADER_RECORD records[] = {
    {st_name, "st_name", offsetof(XELF_DEF::Elf32_Sym, st_name), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_value, "st_value", offsetof(XELF_DEF::Elf32_Sym, st_value), 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_size, "st_size", offsetof(XELF_DEF::Elf32_Sym, st_size), 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
    {st_info, "st_info", offsetof(XELF_DEF::Elf32_Sym, st_info), 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_other, "st_other", offsetof(XELF_DEF::Elf32_Sym, st_other), 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_shndx, "st_shndx", offsetof(XELF_DEF::Elf32_Sym, st_shndx), 2, "Elf32_Half", XFW_DEF::VAL_TYPE_DATA_INT},
};
}

namespace X_Elf64_Sym {
const XFW_DEF::HEADER_RECORD records[] = {
    {st_name, "st_name", offsetof(XELF_DEF::Elf64_Sym, st_name), 4, "Elf64_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_info, "st_info", offsetof(XELF_DEF::Elf64_Sym, st_info), 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_other, "st_other", offsetof(XELF_DEF::Elf64_Sym, st_other), 1, "unsigned char", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_shndx, "st_shndx", offsetof(XELF_DEF::Elf64_Sym, st_shndx), 2, "Elf64_Half", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_value, "st_value", offsetof(XELF_DEF::Elf64_Sym, st_value), 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT},
    {st_size, "st_size", offsetof(XELF_DEF::Elf64_Sym, st_size), 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_SIZE},
};
}

namespace X_Elf_Rela {
const XFW_DEF::HEADER_RECORD records32[] = {
    {r_offset, "r_offset", 0, 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {r_info, "r_info", 4, 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
    {r_addend, "r_addend", 8, 4, "Elf32_Sword", XFW_DEF::VAL_TYPE_DATA_INT},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {r_offset, "r_offset", 0, 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {r_info, "r_info", 8, 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT},
    {r_addend, "r_addend", 16, 8, "Elf64_Sxword", XFW_DEF::VAL_TYPE_DATA_INT},
};
}  // namespace X_Elf_Rela

namespace X_Elf_Rel {
const XFW_DEF::HEADER_RECORD records32[] = {
    {r_offset, "r_offset", 0, 4, "Elf32_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {r_info, "r_info", 4, 4, "Elf32_Word", XFW_DEF::VAL_TYPE_DATA_INT},
};
const XFW_DEF::HEADER_RECORD records64[] = {
    {r_offset, "r_offset", 0, 8, "Elf64_Addr", XFW_DEF::VAL_TYPE_DATA_INT | XFW_DEF::VAL_TYPE_ADDRESS_},
    {r_info, "r_info", 8, 8, "Elf64_Xword", XFW_DEF::VAL_TYPE_DATA_INT},
};
}  // namespace X_Elf_Rel
