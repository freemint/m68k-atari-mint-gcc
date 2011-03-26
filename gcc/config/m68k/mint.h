/* Definitions of target machine for GNU compiler.
   Atari ST TOS/MiNT.
   Copyright (C) 1994, 1995, 2007, 2008, 2009, 2010, 2011
   Free Software Foundation, Inc.

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

/* Here are four prefixes that are used by asm_fprintf to
   facilitate customization for alternate assembler syntaxes.
   Machines with no likelihood of an alternate syntax need not
   define these and need not use asm_fprintf.  */

/* The prefix for register names.  Note that REGISTER_NAMES
   is supposed to include this prefix. Also note that this is NOT an
   fprintf format string, it is a literal string */

#undef REGISTER_PREFIX
#define REGISTER_PREFIX "%"

/* The prefix for local (compiler generated) labels.
   These labels will not appear in the symbol table.  */

#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX "."

#undef ASM_COMMENT_START
#define ASM_COMMENT_START "|"

#undef WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE SHORT_TYPE_SIZE

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__MINT__");		\
      builtin_define_std ("atarist");		\
      builtin_assert ("machine=atari");		\
      builtin_assert ("system=mint");		\
    }						\
  while (0)

/* The following defines are nonstandard
   and are kept only for compatibility
   with older versions of GCC for MiNT.  */

#undef CPP_SPEC
#define CPP_SPEC			\
  "%{m68000:-D__M68000__} "		\
  "%{mc68020:-D__M68020__} "		\
  "%{m68020:-D__M68020__} "		\
  "%{m68030:-D__M68020__} "		\
  "%{m68040:-D__M68020__} "		\
  "%{m68060:-D__M68020__} "		\
  "%{m68020-40:-D__M68020__} "		\
  "%{m68020-60:-D__M68020__} "		\
  "%{!m680*:%{!mc680*:-D__M68000__}} "	\
  "%{mshort:-D__MSHORT__}"

#define STARTFILE_SPEC	"%{pg|p|profile:gcrt0.o%s;:crt0.o%s}"
#define LIB_SPEC	"-lc"

/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

/* The -g option generates stabs debug information.  */
#define DBX_DEBUGGING_INFO 1

/* This is the assembler directive to equate two values.  */
#undef SET_ASM_OP
#define SET_ASM_OP		"\t.set\t"

/* This is how we tell the assembler that a symbol is weak.  */
#undef ASM_WEAKEN_LABEL
#define ASM_WEAKEN_LABEL(FILE,NAME) \
  do { fputs ("\t.weak\t", FILE); assemble_name (FILE, NAME); \
       fputc ('\n', FILE); } while (0)

/* Don't default to pcc-struct-return, because gcc is the only compiler, and
   we want to retain compatibility with older gcc versions.  */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* The system headers are C++-aware.  */
#define NO_IMPLICIT_EXTERN_C

/* By default, the vtable entries are void pointers, the so the alignment
   is the same as pointer alignment.  The value of this macro specifies
   the alignment of the vtable entry in bits.  It should be defined only
   when special alignment is necessary.

   MiNT: The default value of 32 is too much and unsupported by a.out-mintprg.
*/
#define TARGET_VTABLE_ENTRY_ALIGN 16

/* If we have a definition of INCOMING_RETURN_ADDR_RTX, assume that
   the rest of the DWARF 2 frame unwind support is also provided.
   
   MiNT: DWARF 2 frame unwind is not supported by a.out-mint.
*/
#define DWARF2_UNWIND_INFO 0

/* config/m68k.md has an explicit reference to the program counter,
   prefix this by the register prefix.  */

#define ASM_RETURN_CASE_JUMP				\
  do {							\
    if (TARGET_COLDFIRE)				\
      {							\
	if (ADDRESS_REG_P (operands[0]))		\
	  return "jmp %%pc@(2,%0:l)";			\
	else						\
	  return "ext%.l %0\n\tjmp %%pc@(2,%0:l)";	\
      }							\
    else						\
      return "jmp %%pc@(2,%0:w)";			\
  } while (0)

/* The ADDR_DIFF_VEC must exactly follow the previous instruction.  */

#undef ADDR_VEC_ALIGN
#define ADDR_VEC_ALIGN(ADDR_VEC) 0

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as uninitialized global
   data.  */

#define BSS_SECTION_ASM_OP "\t.bss"

/* A C statement (sans semicolon) to output to the stdio stream
   FILE the assembler definition of uninitialized global DECL named
   NAME whose size is SIZE bytes and alignment is ALIGN bytes.
   Try to use asm_output_aligned_bss to implement this macro.  */

#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)

/* Disable -fpic and -fPIC since bsr.l _label@PLTPC
   is unsupported by the assembler.  */

#undef  SUBTARGET_OVERRIDE_OPTIONS
#define SUBTARGET_OVERRIDE_OPTIONS					\
do {									\
  if (flag_pic && !TARGET_PCREL)					\
      error ("-f%s is not supported on this target",			\
	       (flag_pic > 1) ? "PIC" : "pic");				\
} while (0)


/* Workaround for GCC bug #35067 about multiple thunks.  */

#undef MAKE_DECL_ONE_ONLY
#define MAKE_DECL_ONE_ONLY(DECL) (DECL_WEAK (DECL) = 1)

/* Avoid requiring -static with -fmudflap like in config/bfin/uclinux.h */
#define MFWRAP_SPEC " %{fmudflap|fmudflapth: \
 --wrap=malloc --wrap=free --wrap=calloc --wrap=realloc\
 --wrap=mmap --wrap=munmap --wrap=alloca\
 %{fmudflapth: --wrap=pthread_create\
}} %{fmudflap|fmudflapth: --wrap=main}"
