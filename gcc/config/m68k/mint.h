/* Definitions of target machine for GNU compiler.
   Atari ST TOS/MiNT.
   Copyright (C) 1994, 1995 Free Software Foundation, Inc.

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
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

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

#define CPP_SPEC \
  "%{m68000:-D__M68000__ -D__mc68000__ %{!ansi:-Dmc68000}} " \
  "%{mc68020:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{m68020:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{m68030:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{m68040:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{m68060:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{m68020-40:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{m68020-60:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020}} " \
  "%{!m680*:%{!mc680*:-D__M68000__ -D__mc68000__ %{!ansi:-Dmc68000}}} " \
  "%{m68881:-D__M68881__} " \
  COMMON_CPP_SPEC

#define STARTFILE_SPEC	"%{pg:g}crt0.o%s"
#define LIB_SPEC	"-lc%{g:_g}%{pg:_p}"
#define LINKER_NAME	"ld"

#define HAVE_ATEXIT

#define DO_GLOBAL_CTORS_BODY					\
do {								\
  extern char* _base;						\
  char* text = _base + 0x100;					\
  func_ptr *p = __CTOR_LIST__ + 1;				\
  while (*p) {							\
    func_ptr f = (func_ptr) (text + (unsigned long) *p);	\
    (*f) ();							\
    ++p;							\
  }								\
} while (0)


#define DO_GLOBAL_DTORS_BODY		\
do {					\
  extern char* _base;			\
  char* text = _base + 0x100;		\
  func_ptr *p = __DTOR_LIST__ + 1;	\
  func_ptr *s = p;			\
  while (*p) {				\
    ++p;				\
  }					\
  --p;					\
  while (p >= s) {			\
    func_ptr f = (func_ptr) (text + (unsigned long) *p);	\
    (*f) ();				\
    --p;				\
  }					\
} while (0)


#define SIZE_TYPE	"long unsigned int"
#define PTRDIFF_TYPE	"long int"

/* We must override m68k.h
 */
#undef WCHAR_TYPE
#define WCHAR_TYPE	"short unsigned int"

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE	16

/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG
#define DBX_DEBUGGING_INFO

/* Generate calls to memcpy, memcmp and memset.  */
#define TARGET_MEM_FUNCTIONS

/* The value of `EDOM' on the target machine.  */
#define TARGET_EDOM 89

/* This is how to create a rtl expression that refers to the global
   "variable" `errno'.  */
#define GEN_ERRNO_RTX \
  gen_rtx (MEM, TARGET_SHORT ? HImode : SImode, \
	   gen_rtx (SYMBOL_REF, Pmode, "errno"))

#define FUNCTION_VALUEX(MODE)					\
  gen_rtx (REG, (MODE), (TARGET_68881 && FLOAT_MODE_P (MODE) ? 16 : 0))

#undef FUNCTION_VALUE
#define FUNCTION_VALUE(VALTYPE,FUNC) FUNCTION_VALUEX (TYPE_MODE (VALTYPE))
#undef LIBCALL_VALUE
#define LIBCALL_VALUE(MODE) FUNCTION_VALUEX (MODE)

/* 1 if N is a possible register number for a function value.  */

#undef FUNCTION_VALUE_REGNO_P
#define FUNCTION_VALUE_REGNO_P(N) ((N) == 0 || (TARGET_68881 && (N) == 16))

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

/* Show we can debug even without a frame pointer.  */
#define CAN_DEBUG_WITHOUT_FP

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
