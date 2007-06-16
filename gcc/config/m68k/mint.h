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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

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


#define DO_GLOBAL_DTORS_BODY					\
do {								\
  extern char* _base;						\
  char* text = _base + 0x100;					\
  func_ptr *p = __DTOR_LIST__ + 1;				\
  func_ptr *s = p;						\
  while (*p) {							\
    ++p;							\
  }								\
  --p;								\
  while (p >= s) {						\
    func_ptr f = (func_ptr) (text + (unsigned long) *p);	\
    (*f) ();							\
    --p;							\
  }								\
} while (0)


/* Every structure or union's size must be a multiple of 2 bytes.  */
#define STRUCTURE_SIZE_BOUNDARY 16

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG
#define DBX_DEBUGGING_INFO

/* 1 if N is a possible register number for a function value.  For
   m68k/SVR4 allow d0, a0, or fp0 as return registers, for integral,
   pointer, or floating types, respectively.  Reject fp0 if not using
   a 68881 coprocessor.  */

#undef FUNCTION_VALUE_REGNO_P
#define FUNCTION_VALUE_REGNO_P(N) \
  ((N) == 0 || (N) == 8 || (TARGET_68881 && (N) == 16))

/* Define this to be true when FUNCTION_VALUE_REGNO_P is true for
   more than one register.  */

#undef NEEDS_UNTYPED_CALL
#define NEEDS_UNTYPED_CALL 1

/* Define how to generate (in the callee) the output value of a
   function and how to find (in the caller) the value returned by a
   function.  VALTYPE is the data type of the value (as a tree).  If
   the precise function being called is known, FUNC is its
   FUNCTION_DECL; otherwise, FUNC is 0.  For m68k/SVR4 generate the
   result in d0, a0, or fp0 as appropriate.  */

#undef FUNCTION_VALUE
#define FUNCTION_VALUE(VALTYPE, FUNC)					\
  m68k_function_value (VALTYPE, FUNC)

/* For compatibility with the large body of existing code which does
   not always properly declare external functions returning pointer
   types, the m68k/SVR4 convention is to copy the value returned for
   pointer functions from a0 to d0 in the function epilogue, so that
   callers that have neglected to properly declare the callee can
   still find the correct return value.  */

#define FUNCTION_EXTRA_EPILOGUE(FILE, SIZE)				\
do {									\
  if (current_function_returns_pointer					\
      && ! find_equiv_reg (0, get_last_insn (), 0, 0, 0, 8, Pmode))	\
    asm_fprintf (FILE, "\tmove.l %Ra0,%Rd0\n");				\
} while (0);

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.
   For m68k/SVR4 look for integer values in d0, pointer values in d0
   (returned in both d0 and a0), and floating values in fp0.  */

#undef LIBCALL_VALUE
#define LIBCALL_VALUE(MODE)						\
  m68k_libcall_value (MODE)

/* For m68k SVR4, structures are returned using the reentrant
   technique.  */
#undef PCC_STATIC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 0

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

/* Make sure that ALIGN_ASM_OP uses a number of bytes.  */

#undef ALIGN_ASM_OP
#define ALIGN_ASM_OP "\t.balign\t"

/* This is how to output an assembler line that says to advance the
   location counter to a multiple of 2**LOG bytes.  */

#undef ASM_OUTPUT_ALIGN
#define ASM_OUTPUT_ALIGN(FILE,LOG)				\
  if ((LOG) > 0)						\
    fprintf ((FILE), "%s%u\n", ALIGN_ASM_OP, 1 << (LOG));

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
