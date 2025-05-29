/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "elf_defs.h"

namespace N_Elf_Ehdr {
const FW_DEF::HEADER_RECORD records32[] = {
    {ei_mag, "ei_mag", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 0, 4, "uint32 LE", FW_DEF::VAL_TYPE_DATA},
    {ei_class, "ei_class", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 4, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_data, "ei_data", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 5, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_version, "ei_version", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 6, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_osabi, "ei_osabi", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 7, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_abiversion, "ei_abiversion", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 8, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_0, "ei_pad_0", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 9, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_1, "ei_pad_1", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 10, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_2, "ei_pad_2", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 11, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_3, "ei_pad_3", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 12, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_4, "ei_pad_4", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 13, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_5, "ei_pad_5", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 14, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_6, "ei_pad_6", offsetof(XELF_DEF::Elf32_Ehdr, e_ident) + 15, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {e_type, "e_type", offsetof(XELF_DEF::Elf32_Ehdr, e_type), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_machine, "e_machine", offsetof(XELF_DEF::Elf32_Ehdr, e_machine), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_version, "e_version", offsetof(XELF_DEF::Elf32_Ehdr, e_version), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {e_entry, "e_entry", offsetof(XELF_DEF::Elf32_Ehdr, e_entry), 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {e_phoff, "e_phoff", offsetof(XELF_DEF::Elf32_Ehdr, e_phoff), 4, "Elf32_Off", FW_DEF::VAL_TYPE_RELADDRESS},
    {e_shoff, "e_shoff", offsetof(XELF_DEF::Elf32_Ehdr, e_shoff), 4, "Elf32_Off", FW_DEF::VAL_TYPE_RELADDRESS},
    {e_flags, "e_flags", offsetof(XELF_DEF::Elf32_Ehdr, e_flags), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {e_ehsize, "e_ehsize", offsetof(XELF_DEF::Elf32_Ehdr, e_ehsize), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_phentsize, "e_phentsize", offsetof(XELF_DEF::Elf32_Ehdr, e_phentsize), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_phnum, "e_phnum", offsetof(XELF_DEF::Elf32_Ehdr, e_phnum), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shentsize, "e_shentsize", offsetof(XELF_DEF::Elf32_Ehdr, e_shentsize), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shnum, "e_shnum", offsetof(XELF_DEF::Elf32_Ehdr, e_shnum), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shstrndx, "e_shstrndx", offsetof(XELF_DEF::Elf32_Ehdr, e_shstrndx), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
};
const FW_DEF::HEADER_RECORD records64[] = {
    {ei_mag, "ei_mag", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 0, 4, "uint32 LE", FW_DEF::VAL_TYPE_DATA},
    {ei_class, "ei_class", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 4, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_data, "ei_data", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 5, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_version, "ei_version", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 6, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_osabi, "ei_osabi", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 7, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_abiversion, "ei_abiversion", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 8, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_0, "ei_pad_0", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 9, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_1, "ei_pad_1", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 10, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_2, "ei_pad_2", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 11, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_3, "ei_pad_3", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 12, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_4, "ei_pad_4", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 13, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_5, "ei_pad_5", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 14, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {ei_pad_6, "ei_pad_6", offsetof(XELF_DEF::Elf64_Ehdr, e_ident) + 15, 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {e_type, "e_type", offsetof(XELF_DEF::Elf64_Ehdr, e_type), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_machine, "e_machine", offsetof(XELF_DEF::Elf64_Ehdr, e_machine), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_version, "e_version", offsetof(XELF_DEF::Elf64_Ehdr, e_version), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {e_entry, "e_entry", offsetof(XELF_DEF::Elf64_Ehdr, e_entry), 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {e_phoff, "e_phoff", offsetof(XELF_DEF::Elf64_Ehdr, e_phoff), 8, "Elf64_Off", FW_DEF::VAL_TYPE_RELADDRESS},
    {e_shoff, "e_shoff", offsetof(XELF_DEF::Elf64_Ehdr, e_shoff), 8, "Elf64_Off", FW_DEF::VAL_TYPE_RELADDRESS},
    {e_flags, "e_flags", offsetof(XELF_DEF::Elf64_Ehdr, e_flags), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {e_ehsize, "e_ehsize", offsetof(XELF_DEF::Elf64_Ehdr, e_ehsize), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_phentsize, "e_phentsize", offsetof(XELF_DEF::Elf64_Ehdr, e_phentsize), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_phnum, "e_phnum", offsetof(XELF_DEF::Elf64_Ehdr, e_phnum), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shentsize, "e_shentsize", offsetof(XELF_DEF::Elf64_Ehdr, e_shentsize), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shnum, "e_shnum", offsetof(XELF_DEF::Elf64_Ehdr, e_shnum), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {e_shstrndx, "e_shstrndx", offsetof(XELF_DEF::Elf64_Ehdr, e_shstrndx), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
};
}  // namespace N_Elf_Ehdr

namespace N_Elf_Shdr {
const FW_DEF::HEADER_RECORD records32[] = {
    {sh_name, "sh_name", offsetof(XELF_DEF::Elf32_Shdr, sh_name), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_type, "sh_type", offsetof(XELF_DEF::Elf32_Shdr, sh_type), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_flags, "sh_flags", offsetof(XELF_DEF::Elf32_Shdr, sh_flags), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_addr, "sh_addr", offsetof(XELF_DEF::Elf32_Shdr, sh_addr), 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {sh_offset, "sh_offset", offsetof(XELF_DEF::Elf32_Shdr, sh_offset), 4, "Elf32_Off", FW_DEF::VAL_TYPE_OFFSET},
    {sh_size, "sh_size", offsetof(XELF_DEF::Elf32_Shdr, sh_size), 4, "Elf32_Word", FW_DEF::VAL_TYPE_SIZE},
    {sh_link, "sh_link", offsetof(XELF_DEF::Elf32_Shdr, sh_link), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_info, "sh_info", offsetof(XELF_DEF::Elf32_Shdr, sh_info), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_addralign, "sh_addralign", offsetof(XELF_DEF::Elf32_Shdr, sh_addralign), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_entsize, "sh_entsize", offsetof(XELF_DEF::Elf32_Shdr, sh_entsize), 4, "Elf32_Word", FW_DEF::VAL_TYPE_SIZE},
};
const FW_DEF::HEADER_RECORD records64[] = {
    {sh_name, "sh_name", offsetof(XELF_DEF::Elf64_Shdr, sh_name), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_type, "sh_type", offsetof(XELF_DEF::Elf64_Shdr, sh_type), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_flags, "sh_flags", offsetof(XELF_DEF::Elf64_Shdr, sh_flags), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_DATA},
    {sh_addr, "sh_addr", offsetof(XELF_DEF::Elf64_Shdr, sh_addr), 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {sh_offset, "sh_offset", offsetof(XELF_DEF::Elf64_Shdr, sh_offset), 8, "Elf64_Off", FW_DEF::VAL_TYPE_OFFSET},
    {sh_size, "sh_size", offsetof(XELF_DEF::Elf64_Shdr, sh_size), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_SIZE},
    {sh_link, "sh_link", offsetof(XELF_DEF::Elf64_Shdr, sh_link), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_info, "sh_info", offsetof(XELF_DEF::Elf64_Shdr, sh_info), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {sh_addralign, "sh_addralign", offsetof(XELF_DEF::Elf64_Shdr, sh_addralign), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_DATA},
    {sh_entsize, "sh_entsize", offsetof(XELF_DEF::Elf64_Shdr, sh_entsize), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_SIZE},
};
}  // namespace N_Elf_Shdr

namespace N_Elf_Phdr32 {
const FW_DEF::HEADER_RECORD records[] = {
    {p_type, "p_type", offsetof(XELF_DEF::Elf32_Phdr, p_type), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {p_offset, "p_offset", offsetof(XELF_DEF::Elf32_Phdr, p_offset), 4, "Elf32_Off", FW_DEF::VAL_TYPE_OFFSET},
    {p_vaddr, "p_vaddr", offsetof(XELF_DEF::Elf32_Phdr, p_vaddr), 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {p_paddr, "p_paddr", offsetof(XELF_DEF::Elf32_Phdr, p_paddr), 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {p_filesz, "p_filesz", offsetof(XELF_DEF::Elf32_Phdr, p_filesz), 4, "Elf32_Word", FW_DEF::VAL_TYPE_SIZE},
    {p_memsz, "p_memsz", offsetof(XELF_DEF::Elf32_Phdr, p_memsz), 4, "Elf32_Word", FW_DEF::VAL_TYPE_SIZE},
    {p_flags, "p_flags", offsetof(XELF_DEF::Elf32_Phdr, p_flags), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {p_align, "p_align", offsetof(XELF_DEF::Elf32_Phdr, p_align), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_Elf_Phdr64 {
const FW_DEF::HEADER_RECORD records[] = {
    {p_type, "p_type", offsetof(XELF_DEF::Elf64_Phdr, p_type), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {p_flags, "p_flags", offsetof(XELF_DEF::Elf64_Phdr, p_flags), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {p_offset, "p_offset", offsetof(XELF_DEF::Elf64_Phdr, p_offset), 8, "Elf64_Off", FW_DEF::VAL_TYPE_OFFSET},
    {p_vaddr, "p_vaddr", offsetof(XELF_DEF::Elf64_Phdr, p_vaddr), 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {p_paddr, "p_paddr", offsetof(XELF_DEF::Elf64_Phdr, p_paddr), 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {p_filesz, "p_filesz", offsetof(XELF_DEF::Elf64_Phdr, p_filesz), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_SIZE},
    {p_memsz, "p_memsz", offsetof(XELF_DEF::Elf64_Phdr, p_memsz), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_SIZE},
    {p_align, "p_align", offsetof(XELF_DEF::Elf64_Phdr, p_align), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_Elf_DynamicArrayTags {
const FW_DEF::HEADER_RECORD records32[] = {
    {d_tag, "d_tag", 0, 4, "Elf32_SWord", FW_DEF::VAL_TYPE_DATA},
    {d_value, "d_value", 4, 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
};
const FW_DEF::HEADER_RECORD records64[] = {
    {d_tag, "d_tag", 0, 8, "Elf64_Xword", FW_DEF::VAL_TYPE_DATA},
    {d_value, "d_value", 8, 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
};
}  // namespace N_Elf_DynamicArrayTags

namespace N_ELF_LIBRARIES {
const FW_DEF::HEADER_RECORD records[] = {
    {library_name, QObject::tr("Library name"), 0, 0, "", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_ELF_INTERPRETER {
const FW_DEF::HEADER_RECORD records[] = {
    {interpreter, QObject::tr("Interpreter"), 0, -1, "Ansi string", FW_DEF::VAL_TYPE_TEXT},
};
}

namespace N_ELF_NOTES {
const FW_DEF::HEADER_RECORD records[] = {
    {type, QObject::tr("Type"), 0, -1, "", FW_DEF::VAL_TYPE_DATA},
    {name, QObject::tr("Name"), 0, -1, "", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_ELF_RUNPATH {
const FW_DEF::HEADER_RECORD records[] = {
    {runpath, "runpath", 0, -1, "Ansi string", FW_DEF::VAL_TYPE_TEXT},
};
}

namespace N_Elf32_Sym {
const FW_DEF::HEADER_RECORD records[] = {
    {st_name, "st_name", offsetof(XELF_DEF::Elf32_Sym, st_name), 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {st_value, "st_value", offsetof(XELF_DEF::Elf32_Sym, st_value), 4, "Elf32_Addr", FW_DEF::VAL_TYPE_DATA},
    {st_size, "st_size", offsetof(XELF_DEF::Elf32_Sym, st_size), 4, "Elf32_Word", FW_DEF::VAL_TYPE_SIZE},
    {st_info, "st_info", offsetof(XELF_DEF::Elf32_Sym, st_info), 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {st_other, "st_other", offsetof(XELF_DEF::Elf32_Sym, st_other), 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {st_shndx, "st_shndx", offsetof(XELF_DEF::Elf32_Sym, st_shndx), 2, "Elf32_Half", FW_DEF::VAL_TYPE_DATA},
};
}

namespace N_Elf64_Sym {
const FW_DEF::HEADER_RECORD records[] = {
    {st_name, "st_name", offsetof(XELF_DEF::Elf64_Sym, st_name), 4, "Elf64_Word", FW_DEF::VAL_TYPE_DATA},
    {st_info, "st_info", offsetof(XELF_DEF::Elf64_Sym, st_info), 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {st_other, "st_other", offsetof(XELF_DEF::Elf64_Sym, st_other), 1, "unsigned char", FW_DEF::VAL_TYPE_DATA},
    {st_shndx, "st_shndx", offsetof(XELF_DEF::Elf64_Sym, st_shndx), 2, "Elf64_Half", FW_DEF::VAL_TYPE_DATA},
    {st_value, "st_value", offsetof(XELF_DEF::Elf64_Sym, st_value), 8, "Elf64_Addr", FW_DEF::VAL_TYPE_DATA},
    {st_size, "st_size", offsetof(XELF_DEF::Elf64_Sym, st_size), 8, "Elf64_Xword", FW_DEF::VAL_TYPE_SIZE},
};
}

namespace N_Elf_Rela {
const FW_DEF::HEADER_RECORD records32[] = {
    {r_offset, "r_offset", 0, 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {r_info, "r_info", 4, 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
    {r_addend, "r_addend", 8, 4, "Elf32_Sword", FW_DEF::VAL_TYPE_DATA},
};
const FW_DEF::HEADER_RECORD records64[] = {
    {r_offset, "r_offset", 0, 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {r_info, "r_info", 8, 8, "Elf64_Xword", FW_DEF::VAL_TYPE_DATA},
    {r_addend, "r_addend", 16, 8, "Elf64_Sxword", FW_DEF::VAL_TYPE_DATA},
};
}  // namespace N_Elf_Rela

namespace N_Elf_Rel {
const FW_DEF::HEADER_RECORD records32[] = {
    {r_offset, "r_offset", 0, 4, "Elf32_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {r_info, "r_info", 4, 4, "Elf32_Word", FW_DEF::VAL_TYPE_DATA},
};
const FW_DEF::HEADER_RECORD records64[] = {
    {r_offset, "r_offset", 0, 8, "Elf64_Addr", FW_DEF::VAL_TYPE_ADDRESS},
    {r_info, "r_info", 8, 8, "Elf64_Xword", FW_DEF::VAL_TYPE_DATA},
};
}  // namespace N_Elf_Rel
