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

/* The prefix for register names.  Note that REGISTER_NAMES
   is supposed to include this prefix. Also note that this is NOT an
   fprintf format string, it is a literal string */

#undef REGISTER_PREFIX
#define REGISTER_PREFIX "%"

/* The prefix for local (compiler generated) labels.
   These labels will not appear in the symbol table.  */

#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX "."

/* The prefix to add to user-visible assembler symbols.  */

#undef USER_LABEL_PREFIX
#define USER_LABEL_PREFIX ""

/* How to start an assembler comment.  */

#undef ASM_COMMENT_START
#define ASM_COMMENT_START "|"

/* Currently, JUMP_TABLES_IN_TEXT_SECTION must be defined in order to
   keep switch tables in the text section.  */

#define JUMP_TABLES_IN_TEXT_SECTION 1

/* Use the default action for outputting the case label.  */

#undef ASM_OUTPUT_CASE_LABEL
#define ASM_RETURN_CASE_JUMP				\
  do {							\
    if (TARGET_COLDFIRE)				\
      {							\
	if (ADDRESS_REG_P (operands[0]))		\
	  return "jmp %%pc@(2,%0:l)";			\
	else if (TARGET_LONG_JUMP_TABLE_OFFSETS)	\
	  return "jmp %%pc@(2,%0:l)";			\
	else						\
	  return "ext%.l %0\n\tjmp %%pc@(2,%0:l)";	\
      }							\
    else if (TARGET_LONG_JUMP_TABLE_OFFSETS)		\
      return "jmp %%pc@(2,%0:l)";			\
    else						\
      return "jmp %%pc@(2,%0:w)";			\
  } while (0)

/* As offset 2 is hardcoded in the jmp instruction above,
   the ADDR_VEC must immediately follow the jmp instruction.
   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=112413  */

#define ADDR_VEC_ALIGN(ADDR_VEC) 0

/* This is how to output an assembler line that says to advance the
   location counter to a multiple of 2**LOG bytes.  */

#undef ASM_OUTPUT_ALIGN
#define ASM_OUTPUT_ALIGN(FILE,LOG)				\
do {								\
  if ((LOG) > 0)						\
    fprintf ((FILE), "%s%u\n", ALIGN_ASM_OP, 1 << (LOG));	\
} while (0)

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as uninitialized global
   data.  */

#define BSS_SECTION_ASM_OP "\t.section\t.bss"

/* A C statement (sans semicolon) to output to the stdio stream
   FILE the assembler definition of uninitialized global DECL named
   NAME whose size is SIZE bytes and alignment is ALIGN bytes.
   Try to use asm_output_aligned_bss to implement this macro.  */

#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)

/* Output a common block.  */

#undef ASM_OUTPUT_COMMON
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".comm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%u\n", (int)(SIZE)))

/* Output a local common block.  */

#undef ASM_OUTPUT_LOCAL
#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".lcomm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%u\n", (int)(SIZE)))

/* Disable -fpic and -fPIC since bsr.l _label@PLTPC
   is unsupported by the assembler.  */

#undef  SUBTARGET_OVERRIDE_OPTIONS
#define SUBTARGET_OVERRIDE_OPTIONS					\
do {									\
  if (flag_pic && !TARGET_PCREL)					\
      error ("%<-f%s%> is not supported on this target",		\
	       (flag_pic > 1) ? "PIC" : "pic");				\
} while (0)

/* Define these to avoid dependence on meaning of `int'.  */
#undef  WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"
#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16
#undef  PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"
#undef  SIZE_TYPE
#define SIZE_TYPE "long unsigned int"

/* Don't default to pcc-struct-return, so that we can return small structures
   and unions in registers, which is slightly more efficient.  */
#define DEFAULT_PCC_STRUCT_RETURN 0

#if HAVE_INITFINI_ARRAY_SUPPORT
#define GCC_HAVE_INITFINI_ARRAY_SUPPORT builtin_define ("__GCC_HAVE_INITFINI_ARRAY_SUPPORT");
#else
#define GCC_HAVE_INITFINI_ARRAY_SUPPORT
#endif

#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__MINT__");		\
      GCC_HAVE_INITFINI_ARRAY_SUPPORT \
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
#define STARTFILE_SPEC "%{pg|p|profile:gcrt0.o%s;:crt0.o%s} crtbegin.o%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s"

#undef  LIB_SPEC
#define LIB_SPEC "-lc"

/* The MiNTLib doesn't have support for .init and .fini sections yet.  */
#undef INIT_SECTION_ASM_OP
#undef FINI_SECTION_ASM_OP

/* avoid pulling in the tm_clone support which we don't need */
#define USE_TM_CLONE_REGISTRY 0

/* Install the __sync libcalls.  */
#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS  m68k_init_sync_libfuncs

/* with fdlibm, most of the c99 functions are available, including sincos */
#undef  TARGET_LIBC_HAS_FUNCTION
#define TARGET_LIBC_HAS_FUNCTION bsd_libc_has_function

/* Define how the m68k registers should be numbered for Dwarf output.
   The numbering provided here should be compatible with the native
   SVR4 debugger in the m68k/SVR4 reference port, where d0-d7
   are 0-7, a0-a8 are 8-15, and fp0-fp7 are 16-23.  */

#undef DEBUGGER_REGNO
#define DEBUGGER_REGNO(REGNO) (REGNO)

/* After initial relocation, exception handling tables are never written.  */
#define EH_TABLES_CAN_BE_READ_ONLY 1

/* If we have a definition of INCOMING_RETURN_ADDR_RTX, assume that
   the rest of the DWARF 2 frame unwind support is also provided.
   
   All configurations that don't use elf must be explicit about not using
   dwarf unwind information.

   MiNT: DWARF 2 frame unwind is not supported by a.out-mint.
*/
#undef DWARF2_UNWIND_INFO
/* If configured with --disable-sjlj-exceptions, use DWARF2
   else default to SJLJ.  */
#if defined(USING_ELFOS_H) && defined (CONFIG_SJLJ_EXCEPTIONS) && !CONFIG_SJLJ_EXCEPTIONS
/* The logic of this #if must be kept synchronised with the logic
   for selecting the tmake_eh_file fragment in libgcc/config.host.  */
#define DWARF2_UNWIND_INFO 1
#else
#define DWARF2_UNWIND_INFO 0
#endif

#if DWARF2_UNWIND_INFO
/* the default of DW_EH_PE_absptr creates relocations at odd addresses, which we cannot handle */
#undef ASM_PREFERRED_EH_DATA_FORMAT
#define ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL)			   \
  (flag_pic								   \
   && !((TARGET_ID_SHARED_LIBRARY || TARGET_SEP_DATA)			   \
	&& ((GLOBAL) || (CODE)))					   \
   ? ((GLOBAL) ? DW_EH_PE_indirect : 0) | DW_EH_PE_pcrel | DW_EH_PE_sdata4 \
   : DW_EH_PE_aligned)
#endif

#ifdef USING_ELFOS_H
/*
 * Definitions for crtstuff.c.
 * Only for elf; others use libgcc2.c instead
 */
#define CTOR_LIST_BEGIN \
STATIC func_ptr __CTOR_LIST__[1] \
  __attribute__ ((__used__, section(".ctors"), aligned(__alignof__(func_ptr)))) \
  = { (func_ptr) (-1) }; \
  /* STATIC ELF_ALIAS(__CTOR_LIST__) */

#define DTOR_LIST_BEGIN \
STATIC func_ptr __DTOR_LIST__[1] \
  __attribute__ ((__used__, section(".dtors"), aligned(__alignof__(func_ptr)))) \
  = { (func_ptr) (-1) }; \
  /* STATIC ELF_ALIAS(__DTOR_LIST__) */

#define CTOR_LIST_END \
STATIC func_ptr __CTOR_END__[1] \
  __attribute__((__used__)) \
  __attribute__((section(".ctors"), aligned(__alignof__(func_ptr)))) \
  = { (func_ptr) 0 }; \
  /* STATIC ELF_ALIAS(__CTOR_END__) */

#define DTOR_LIST_END \
STATIC func_ptr __DTOR_END__[1] \
  __attribute__((__used__)) \
  __attribute__((section(".dtors"), aligned(__alignof__(func_ptr)))) \
  = { (func_ptr) 0 }; \
  /* STATIC ELF_ALIAS(__DTOR_END__) */

#endif
