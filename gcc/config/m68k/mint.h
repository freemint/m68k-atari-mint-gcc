/* Definitions of target machine for GCC for Atari ST TOS/MiNT.
   Copyright (C) 1994-2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* The prefix to add to user-visible assembler symbols.  */
#undef  USER_LABEL_PREFIX
#define USER_LABEL_PREFIX "_"

/* with fdlibm, most of the c99 functions are available, including sincos */
#undef  TARGET_LIBC_HAS_FUNCTION
#define TARGET_LIBC_HAS_FUNCTION bsd_libc_has_function

/* Define these to avoid dependence on meaning of `int'.  */
#undef  WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"
#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16
#undef  PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"
#undef  SIZE_TYPE
#define SIZE_TYPE "long unsigned int"

#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__MINT__");		\
      builtin_define_std ("atarist");		\
      builtin_assert ("machine=atari");		\
      builtin_assert ("system=mint");		\
    }						\
  while (0)

/* The following defines are nonstandard and are kept only for compatibility
   with older versions of GCC for MiNT.  */

#undef  CPP_SPEC
#define CPP_SPEC					\
  "%{m68000|mcpu=68000:-D__M68000__} "			\
  "%{m68020|mcpu=68020:-D__M68020__} "			\
  "%{m68030|mcpu=68030:-D__M68020__} "			\
  "%{m68040|mcpu=68040:-D__M68020__} "			\
  "%{m68060|mcpu=68060:-D__M68020__} "			\
  "%{m68020-40|mcpu=68020-40:-D__M68020__} "		\
  "%{m68020-60|mcpu=68020-60:-D__M68020__} "		\
  "%{!m680*:%{!mc680*:%{!mcpu=680*:-D__M68000__}}} "	\
  "%{mshort:-D__MSHORT__}"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "%{pg|p|profile:gcrt0.o%s;:crt0.o%s}"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC ""

#undef  LIB_SPEC
#define LIB_SPEC "-lc"

/* The MiNTLib doesn't have support for .init and .fini sections yet.  */
#undef INIT_SECTION_ASM_OP
#undef FINI_SECTION_ASM_OP

/* Disable DWARF 2 frame unwind support for exceptions.
   Currently, .eh_frame sections contain relocations at odd addresses. Bug?  */
#define DWARF2_UNWIND_INFO 0

/* The following defines are taken from m68kemb.h to mimic m68k-elf.  */

/* In order for bitfields to work on a 68000, or with -mnobitfield, we must
   define either PCC_BITFIELD_TYPE_MATTERS or STRUCTURE_SIZE_BOUNDARY.
   Defining STRUCTURE_SIZE_BOUNDARY results in structure packing problems,
   so we define PCC_BITFIELD_TYPE_MATTERS.  */
#define PCC_BITFIELD_TYPE_MATTERS 1

/* Don't default to pcc-struct-return, so that we can return small structures
   and unions in registers, which is slightly more efficient.  */
#define DEFAULT_PCC_STRUCT_RETURN 0
