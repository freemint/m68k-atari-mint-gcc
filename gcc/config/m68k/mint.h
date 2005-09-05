/* Definitions of target machine for GNU compiler.
   Atari ST TOS/MiNT.
   Copyright (C) 1994, 1995 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "m68k/m68k.h"

#define TARGET_DEFAULT 0

/* Pass the architecture flags to the assembler. */ 
#define COMMON_ASM_SPEC \
  "%{mc68000:-m68000} " \
  "%{m68000} " \
  "%{mc68020:-m68020} " \
  "%{m68020*:-m68020} " \
  "%{m68030} " \
  "%{m68040} " \
  "%{m68060} " \
  "%{m68881} %{mno-68881} " \
  "%{m68851} %{mno-68851} " \
  "%{m68302} %{mno-68302} " \
  "%{m68332} %{mno-68332}"

#define ASM_SPEC COMMON_ASM_SPEC " %{!m680*:%{!mc680*:-m68000}}"

#undef SUBTARGET_SWITCHES
#define SUBTARGET_SWITCHES			\
  { "int", 010000},				\
  { "noint", -010000},

#define COMMON_CPP_SPEC \
  "%{mshort:-D__MSHORT__}"

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

/* Names to predefine in the preprocessor for this target machine
 */
#define CPP_PREDEFINES \
  "-D__MINT__ -Datarist -Acpu(m68k) -Amachine(atari) -Asystem(mint)"

#define __LIBC_MULTILIB ""

#define STARTFILE_SPEC	"%{pg:g}crt0.o%s"
#define LIB_SPEC	"-lc%{g:_g}%{pg:_p}"__LIBC_MULTILIB
#define LIBGCC_SPEC	"-lgcc"
#define LINK_SPEC	""
#define LINKER_NAME	"ld"

#define MULTILIB_DEFAULTS { "m68000" }

#define HAVE_ATEXIT

#define DO_GLOBAL_CTORS_BODY	\
do {	                        \
  extern char* _base;		\
  func_ptr *p = (func_ptr*) ((char*) &__CTOR_LIST__ + sizeof (func_ptr)); \
  while (*p) {			\
    if (((char *) (*p)) < (_base + 0x100)) \
      *p = (func_ptr) (_base + (unsigned long) *p + 0x100); \
    p++;			\
    (*(p-1)) ();		\
  }				\
} while (0)

#define DO_GLOBAL_DTORS_BODY	\
do {	                        \
  extern char* _base;		\
  func_ptr *p = (func_ptr*) ((char*) &__DTOR_LIST__ + sizeof (func_ptr)); \
  while (*p) {			\
    if (((char *) (*p)) < (_base + 0x100)) \
      *p = (func_ptr) (_base + (unsigned long) *p + 0x100); \
    p++;			\
    (*(p-1)) ();		\
  }				\
} while (0)


#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE	"long int"

#undef SIZE_TYPE
#define SIZE_TYPE	"long unsigned int"

#undef WCHAR_TYPE
#define WCHAR_TYPE	"short unsigned int"

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE	16

/* Alignment of field after `int : 0' in a structure.  */
/* recent gcc's have this as 16, this is left in for the benefit of */
 /* older gcc */
#undef EMPTY_FIELD_BOUNDARY
#define EMPTY_FIELD_BOUNDARY 16

/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

/* Biggest alignment supported by the object file format of this machine. */
#undef MAX_OFILE_ALIGNMENT
#define MAX_OFILE_ALIGNMENT 32

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG
#define DBX_DEBUGGING_INFO

#ifdef abort
#undef abort
#define abort fancy_abort
#endif

/* Allow folding division by zero.  */
#define REAL_INFINITY

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

/* Output a float value (represented as a C double) as an immediate operand.
   This macro is a 68k-specific macro.  */
  
#undef ASM_OUTPUT_FLOAT_OPERAND
#ifdef AS_BUG_FLOATING_CONSTANT
#define ASM_OUTPUT_FLOAT_OPERAND(CODE,FILE,VALUE)		\
 do { long l;							\
      REAL_VALUE_TO_TARGET_SINGLE (VALUE, l);			\
      fprintf ((FILE), "&0x%lx", l);				\
     } while (0)
#else
#define ASM_OUTPUT_FLOAT_OPERAND(CODE,FILE,VALUE)		\
 do {								\
      if (CODE == 'f')						\
        {							\
          char dstr[30];					\
      	  real_to_decimal (dstr, &(VALUE), sizeof (dstr), 9, 0); \
          fprintf ((FILE), "&0f%s", dstr);			\
        }							\
      else							\
        {							\
          long l;						\
          REAL_VALUE_TO_TARGET_SINGLE (VALUE, l);		\
          fprintf ((FILE), "&0x%lx", l);			\
        }							\
     } while (0)
#endif /* AS_BUG_FLOATING_CONSTANT */

/* Output a double value (represented as a C double) as an immediate operand.
   This macro is a 68k-specific macro.  */
#undef ASM_OUTPUT_DOUBLE_OPERAND
#define ASM_OUTPUT_DOUBLE_OPERAND(FILE,VALUE)				\
 do { char dstr[30];							\
      real_to_decimal (dstr, &(VALUE), sizeof (dstr), 0, 1);		\
      fprintf (FILE, "&0f%s", dstr);					\
    } while (0)

#define ASM_OUTPUT_EXTERNAL_LIBCALL(asm_out_file,fun)

/* This is the assembler directive to equate two values.  */
#undef SET_ASM_OP
#define SET_ASM_OP    ".set"

/* This is how we tell the assembler that a symbol is weak.  */
#undef ASM_WEAKEN_LABEL
#define ASM_WEAKEN_LABEL(FILE,NAME) \
  do { fputs ("\t.weak\t", FILE); assemble_name (FILE, NAME); \
       fputc ('\n', FILE); } while (0)

/* Don't default to pcc-struct-return, because gcc is the only compiler, and
   we want to retain compatibility with older gcc versions.  */
#undef PCC_STATIC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 0

/* Define this macro if the system header files support C++ as well
   as C.  This macro inhibits the usual method of using system header
   files in C++, which is to pretend that the file's contents are
   enclosed in `extern "C" {...}'.  */
#define NO_IMPLICIT_EXTERN_C
