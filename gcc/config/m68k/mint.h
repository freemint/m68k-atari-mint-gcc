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


#define MOTOROLA		/* Use Motorola syntax */
#define USE_GAS			/* But GAS wants jbsr instead of jsr */

#include "m68k/m68k.h"

#if defined(__GNUC__)
#if defined(alloca)
#undef alloca
#endif
#define alloca(x) __builtin_alloca(x)
#endif


#define TARGET_DEFAULT 0


/* The prefix for local (compiler generated) labels.
 * These labels will not appear in the symbol table. */
#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX ""

/* The prefix to add to user-visible assembler symbols. */
#undef USER_LABEL_PREFIX
#define USER_LABEL_PREFIX "_"

#define ASM_COMMENT_START "|"

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
  "%{m68040:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020} %{!mno-68881:-D__M68881__}} " \
  "%{m68060:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020} %{!mno-68881:-D__M68881__}} " \
  "%{m68020-40:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020} %{!mno-68881:-D__M68881__}} " \
  "%{m68020-60:-D__M68020__ -D__mc68020__ %{!ansi:-Dmc68020} %{!mno-68881:-D__M68881__}} " \
  "%{!m680*:%{!mc680*:-D__M68000__ -D__mc68000__ %{!ansi:-Dmc68000}}} " \
  "%{m68881:-D__M68881__} " \
  COMMON_CPP_SPEC

/* Names to predefine in the preprocessor for this target machine
 */
#define CPP_PREDEFINES \
  "-D__MINT__ -Datarist -Acpu(m68k) -Amachine(atari) -Asystem(mint)"

#define __LIBC_MULTILIB \
  "%{m68020-*:020}"\
  "%{m68881:%{!m68020-*:020}}"\
  "%{m68040:%{!m68020-*:%{!m68881:020}}}"\
  "%{m68060:%{!m68020-*:%{!m68881:%{!m68040:020}}}}"

#define STARTFILE_SPEC	"%{pg:g}crt0.o%s"
/* #define LIB_SPEC	"-lc%{g:_g}%{pg:_p}%{mshort:16}"__LIBC_MULTILIB */
#define LIB_SPEC	"-lc%{mshort:16}"__LIBC_MULTILIB
#define LIBGCC_SPEC	"-lgcc"
#define LINK_SPEC	""
/* #define LINKER_NAME	"ld" */
/* #define MATH_LIBRARY	"-lm" */

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


#define PTRDIFF_TYPE	"long int"
#define SIZE_TYPE	"long unsigned int"


/* We must override m68k.h
 */
#undef WCHAR_TYPE
#undef WCHAR_TYPE_SIZE

#define WCHAR_TYPE	"short unsigned int"
#define WCHAR_TYPE_SIZE	16
#define WCHAR_UNSIGNED	1


/* Alignment of field after `int : 0' in a structure.
 * recent gcc's have this as 16, this is left in for the benfit of
 * older gcc
 */
#undef EMPTY_FIELD_BOUNDARY
#define EMPTY_FIELD_BOUNDARY 16

/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG
#define DBX_DEBUGGING_INFO

/* All configurations that don't use elf must be explicit about not using
   dwarf unwind information. egcs doesn't try too hard to check internal
   configuration files...  */
#define DWARF2_UNWIND_INFO 0

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

/* This is how to output an assembler line defining a `double' constant.  */

#undef ASM_OUTPUT_DOUBLE
#define ASM_OUTPUT_DOUBLE(FILE,VALUE)				\
  {								\
    if (REAL_VALUE_ISINF (VALUE))				\
      {								\
	if (REAL_VALUE_NEGATIVE (VALUE))			\
	  fprintf (FILE, "\t.double 0r-Inf\n");			\
	else							\
	  fprintf (FILE, "\t.double 0rInf\n");			\
      }								\
    else if (REAL_VALUE_ISNAN (VALUE))				\
      { long l[2];						\
	REAL_VALUE_TO_TARGET_DOUBLE (VALUE, l);			\
	fprintf (FILE, "\t.long 0x%lx\n\t.long 0x%lx\n", l[0], l[1]); \
      }								\
    else if (REAL_VALUE_MINUS_ZERO (VALUE))			\
      {								\
	fprintf (FILE, "\t.double 0r-0.0\n");			\
      }								\
    else							\
      { char dstr[30];						\
	REAL_VALUE_TO_DECIMAL (VALUE, "%.17g", dstr);		\
	fprintf (FILE, "\t.double 0r%s\n", dstr);		\
      }								\
    }

/* This is how to output an assembler line defining a `float' constant.  */

#undef ASM_OUTPUT_FLOAT
#define ASM_OUTPUT_FLOAT(FILE,VALUE)				\
  {								\
    if (REAL_VALUE_ISINF (VALUE))				\
      {								\
	if (REAL_VALUE_NEGATIVE (VALUE))			\
	  fprintf (FILE, "\t.single 0r-Inf\n");			\
	else							\
	  fprintf (FILE, "\t.single 0rInf\n");			\
      }								\
    else if (REAL_VALUE_ISNAN (VALUE))				\
      { long l;							\
	REAL_VALUE_TO_TARGET_SINGLE (VALUE, l);			\
	fprintf (FILE, "\t.long 0x%lx\n", l);			\
      }								\
    else if (REAL_VALUE_MINUS_ZERO (VALUE))			\
      {								\
	fprintf (FILE, "\t.single 0r-0.0\n");			\
      }								\
    else							\
      { char dstr[30];						\
	REAL_VALUE_TO_DECIMAL (VALUE, "%.9g", dstr);		\
	fprintf (FILE, "\t.single 0r%s\n", dstr);		\
      }								\
    }

/* This is how to output an assembler lines defining floating operands.  */
  
#undef ASM_OUTPUT_FLOAT_OPERAND
#define ASM_OUTPUT_FLOAT_OPERAND(CODE,FILE,VALUE)		\
 do {								\
      if (CODE != 'f' || REAL_VALUE_ISNAN (VALUE))		\
	{							\
	  long l;						\
	  REAL_VALUE_TO_TARGET_SINGLE (VALUE, l);		\
	  asm_fprintf (FILE, "%I");				\
	  fprintf (FILE, "0x%lx", l);				\
	}							\
      else if (REAL_VALUE_ISINF (VALUE))			\
	{							\
	  if (REAL_VALUE_NEGATIVE (VALUE))			\
	    asm_fprintf (FILE, "%I0r-Inf");			\
	  else							\
	    asm_fprintf (FILE, "%I0rInf");			\
	}							\
      else if (REAL_VALUE_MINUS_ZERO (VALUE))			\
	{							\
	  asm_fprintf (FILE, "%I0r-0.0");			\
	}							\
      else							\
	{ char dstr[30];					\
	  REAL_VALUE_TO_DECIMAL (VALUE, "%.9g", dstr);		\
	  asm_fprintf (FILE, "%I0r%s", dstr);			\
	}							\
    } while (0)

#undef ASM_OUTPUT_DOUBLE_OPERAND
#define ASM_OUTPUT_DOUBLE_OPERAND(FILE,VALUE)				\
 do { if (REAL_VALUE_ISINF (VALUE))					\
	{								\
	  if (REAL_VALUE_NEGATIVE (VALUE))				\
	    asm_fprintf (FILE, "%I0r-Inf");				\
	  else								\
	    asm_fprintf (FILE, "%I0rInf");				\
	}								\
      else if (REAL_VALUE_ISNAN (VALUE))				\
	{								\
	  long l[2];							\
	  REAL_VALUE_TO_TARGET_DOUBLE (VALUE, l);			\
	  asm_fprintf (FILE, "%I");					\
	  fprintf (FILE, "0x%lx%08lx", l[0], l[1]);			\
	}								\
      else if (REAL_VALUE_MINUS_ZERO (VALUE))				\
	{								\
	  asm_fprintf (FILE, "%I0r-0.0");				\
	}								\
      else								\
	{ char dstr[30];						\
	  REAL_VALUE_TO_DECIMAL (VALUE, "%.17g", dstr);			\
	  asm_fprintf (FILE, "%I0r%s", dstr);				\
	}								\
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

#if 0
/* Define this macro if references to a symbol must be treated
   differently depending on something about the variable or function
   named by the symbol (such as what section it is in).

   The macro definition, if any, is executed immediately after the
   rtl for DECL has been created and stored in `DECL_RTL (DECL)'.
   The value of the rtl will be a `mem' whose address is a
   `symbol_ref'.

   The usual thing for this macro to do is to record a flag in the
   `symbol_ref' (such as `SYMBOL_REF_FLAG') or to store a modified
   name string in the `symbol_ref' (if one bit is not enough
   information).

   On the Atari we use this to indicate if a symbol is in text or
   data space, as well as to distinguish functions from data. */

#define ENCODE_SECTION_INFO(DECL)\
do									\
  {									\
    if (TREE_CODE (DECL) == FUNCTION_DECL)				\
      {									\
        SYMBOL_REF_FLAG (XEXP (DECL_RTL (DECL), 0)) = 1;		\
	SYMBOL_REF_USED (XEXP (DECL_RTL (DECL), 0)) = 1;		\
      }									\
    else								\
      {									\
	rtx rtl = (TREE_CODE_CLASS (TREE_CODE (DECL)) != 'd'		\
		   ? TREE_CST_RTL (DECL) : DECL_RTL (DECL));		\
	if (RTX_UNCHANGING_P (rtl) && !MEM_VOLATILE_P (rtl))		\
	  SYMBOL_REF_FLAG (XEXP (rtl, 0)) = 1;				\
      }									\
  }									\
while (0)

#undef SELECT_RTX_SECTION
#define SELECT_RTX_SECTION(MODE, X) readonly_data_section ();
#endif

/* Don't default to pcc-struct-return, because gcc is the only compiler, and
   we want to retain compatibility with older gcc versions.  */
#undef PCC_STATIC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 0

/* Define this macro if the system header files support C++ as well
   as C.  This macro inhibits the usual method of using system header
   files in C++, which is to pretend that the file's contents are
   enclosed in `extern "C" {...}'.  */
#define NO_IMPLICIT_EXTERN_C

/* Currently, JUMP_TABLES_IN_TEXT_SECTION must be defined in order to
 * keep switch tables in the text section.  */
#define JUMP_TABLES_IN_TEXT_SECTION 1

/* Use the default action for outputting the case label. */
#undef ASM_OUTPUT_CASE_LABEL
#define ASM_RETURN_CASE_JUMP			\
  do {						\
    if (TARGET_5200)				\
      return "ext%.l %0\n\tjmp %%pc@(2,%0:l)";	\
    else					\
      return "jmp %%pc@(2,%0:w)";		\
  } while (0)

/* In m68k svr4, a symbol_ref rtx can be a valid PIC operand if it is
 * an operand of a function call. */
#undef LEGITIMATE_PIC_OPERAND_P
#define LEGITIMATE_PIC_OPERAND_P(X) \
  ((! symbolic_operand (X, VOIDmode) \
    && ! (GET_CODE (X) == CONST_DOUBLE && CONST_DOUBLE_MEM (X)	\
	  && GET_CODE (CONST_DOUBLE_MEM (X)) == MEM		\
	  && symbolic_operand (XEXP (CONST_DOUBLE_MEM (X), 0), VOIDmode))) \
   || (GET_CODE (X) == SYMBOL_REF && SYMBOL_REF_FLAG (X)))

/* Turn off function cse if we are doing PIC. We always want function
 * call to be done as `bsr foo@PLTPC', so it will force the assembler
 * to create the PLT entry for `foo'.  Doing function cse will cause
 * the address of `foo' to be loaded into a register, which is exactly
 * what we want to avoid when we are doing PIC on svr4 m68k. */
#undef SUBTARGET_OVERRIDE_OPTIONS
#define SUBTARGET_OVERRIDE_OPTIONS \
  if (flag_pic) flag_no_function_cse = 1;

/* Finalize the trampoline by flushing the insn cache. */
#undef FINALIZE_TRAMPOLINE
#define FINALIZE_TRAMPOLINE(TRAMP)					\
  emit_library_call (gen_rtx_SYMBOL_REF (Pmode, "__clear_cache"),	\
		     0, VOIDmode, 2, TRAMP, Pmode,			\
		     plus_constant (TRAMP, TRAMPOLINE_SIZE), Pmode);

/* Output code to add DELTA to the first argument, and then jump to FUNCTION.
 * Used for C++ multiple inheritance. */
#define ASM_OUTPUT_MI_THUNK(FILE, THUNK_FNDECL, DELTA, FUNCTION)	\
do {									\
  if (DELTA > 0 && DELTA <= 8)						\
    asm_fprintf (FILE, "\taddq.l %I%d,4(%Rsp)\n", DELTA);		\
  else if (DELTA < 0 && DELTA >= -8)					\
    asm_fprintf (FILE, "\tsubq.l %I%d,4(%Rsp)\n", -DELTA);		\
  else									\
    asm_fprintf (FILE, "\tadd.l %I%d,4(%Rsp)\n", DELTA);		\
									\
  if (flag_pic)								\
    {									\
      fprintf (FILE, "\tbra.l ");					\
      assemble_name (FILE, XSTR (XEXP (DECL_RTL (FUNCTION), 0), 0));	\
      fprintf (FILE, "@PLTPC\n");					\
    }									\
  else									\
    {									\
      fprintf (FILE, "\tjmp ");						\
      assemble_name (FILE, XSTR (XEXP (DECL_RTL (FUNCTION), 0), 0));	\
      fprintf (FILE, "\n");						\
    }									\
} while (0)

/* Output assembler code to FILE to increment profiler label # LABELNO
 * for profiling a function entry. */
#undef FUNCTION_PROFILER
#define FUNCTION_PROFILER(FILE, LABELNO) \
  asm_fprintf (FILE, "\tlea %LLP%d,%Ra0\n\tjsr mcount\n", (LABELNO))
