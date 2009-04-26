/* Definitions of target machine for GNU compiler.
   Atari ST TOS/MiNT.
   Copyright (C) 1994, 1995, 2007, 2008, 2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
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

#define ASM_COMMENT_START "|"

#undef SIZE_TYPE
#define SIZE_TYPE "long unsigned int"

#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"

#undef WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define_std ("__MINT__");		\
      builtin_define_std ("atarist");		\
      builtin_assert ("machine=atari");		\
      builtin_assert ("system=mint");		\
    }						\
  while (0)

#define COMMON_CPP_SPEC \
  "%{mshort:-D__MSHORT__} %{fPIC:-D__PIC__ -D__pic__} %{fpic:-D__PIC__ -D__pic__}"

#undef CPP_SPEC
#define CPP_SPEC							\
  "%{m68000:-D__M68000__ -D__mc68000__ %{!ansi:-Dmc68000}} "		\
  "%{mc68020:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{m68020:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{m68030:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{m68040:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{m68060:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{m68020-40:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{m68020-60:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} "		\
  "%{!m680*:%{!mc680*:-D__M68000__ -D__mc68000__ %{!ansi:-Dmc68000}}} "	\
  "%{m68881:-D__M68881__} "						\
  COMMON_CPP_SPEC

#define STARTFILE_SPEC	"%{pg:g}crt0.o%s"
#define LIB_SPEC	"-lc%{g:_g}%{pg:_p}"
#define LINKER_NAME	"ld"

#define HAVE_ATEXIT

/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG
#define DBX_DEBUGGING_INFO

/* Define how to generate (in the callee) the output value of a
   function and how to find (in the caller) the value returned by a
   function.  VALTYPE is the data type of the value (as a tree).  If
   the precise function being called is known, FUNC is its
   FUNCTION_DECL; otherwise, FUNC is 0.  */

#define FUNCTION_VALUEX(MODE) \
  gen_rtx_REG ((MODE), (TARGET_68881 && FLOAT_MODE_P (MODE) ? 16 : 0))

#undef FUNCTION_VALUE
#define FUNCTION_VALUE(VALTYPE, FUNC) \
  FUNCTION_VALUEX (TYPE_MODE (VALTYPE))

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */

#undef LIBCALL_VALUE
#define LIBCALL_VALUE(MODE) \
  FUNCTION_VALUEX (MODE)

/* 1 if N is a possible register number for a function value.  */

#undef FUNCTION_VALUE_REGNO_P
#define FUNCTION_VALUE_REGNO_P(N) \
  ((N) == D0_REG || (TARGET_68881 && (N) == FP0_REG))

/* Define this to be true when FUNCTION_VALUE_REGNO_P is true for
   more than one register.  */

#undef NEEDS_UNTYPED_CALL
#define NEEDS_UNTYPED_CALL (TARGET_68881)

/* This is the assembler directive to equate two values.  */
#undef SET_ASM_OP
#define SET_ASM_OP		"\t.set\t"

/* This is how we tell the assembler that a symbol is weak.  */
#undef ASM_WEAKEN_LABEL
#define ASM_WEAKEN_LABEL(FILE,NAME) \
  do { fputs ("\t.weak\t", FILE); assemble_name (FILE, NAME); \
       fputc ('\n', FILE); } while (0)

/* Don't default to pcc-struct-return, because gcc is the only compiler.  */
#undef PCC_STATIC_STRUCT_RETURN
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
