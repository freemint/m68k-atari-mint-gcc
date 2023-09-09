/* Exception handling and frame unwind runtime interface routines.
   Copyright (C) 2001-2023 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* @@@ Really this should be out of line, but this also causes link
   compatibility problems with the base ABI.  This is slightly better
   than duplicating code, however.  */

#ifndef GCC_UNWIND_PE_H
#define GCC_UNWIND_PE_H

/* If using C++, references to abort have to be qualified with std::.  */
#if __cplusplus
#define __gxx_abort std::abort
#else
#define __gxx_abort abort
#endif

/* Pointer encodings, from dwarf2.h.  */
#define DW_EH_PE_absptr         0x00
#define DW_EH_PE_omit           0xff

#define DW_EH_PE_uleb128        0x01
#define DW_EH_PE_udata2         0x02
#define DW_EH_PE_udata4         0x03
#define DW_EH_PE_udata8         0x04
#define DW_EH_PE_sleb128        0x09
#define DW_EH_PE_sdata2         0x0A
#define DW_EH_PE_sdata4         0x0B
#define DW_EH_PE_sdata8         0x0C
#define DW_EH_PE_signed         0x08

#define DW_EH_PE_pcrel          0x10
#define DW_EH_PE_textrel        0x20
#define DW_EH_PE_datarel        0x30
#define DW_EH_PE_funcrel        0x40
#define DW_EH_PE_aligned        0x50

#define DW_EH_PE_indirect	0x80


#ifndef NO_SIZE_OF_ENCODED_VALUE

/* Given an encoding, return the number of bytes the format occupies.
   This is only defined for fixed-size encodings, and so does not
   include leb128.  */

static unsigned int
size_of_encoded_value (unsigned char encoding) __attribute__ ((unused));

static unsigned int
size_of_encoded_value (unsigned char encoding)
{
  if (encoding == DW_EH_PE_omit)
    return 0;

  switch (encoding & 0x07)
    {
    case DW_EH_PE_absptr:
      return sizeof (void *);
    case DW_EH_PE_udata2:
      return 2;
    case DW_EH_PE_udata4:
      return 4;
    case DW_EH_PE_udata8:
      return 8;
    }
  __gxx_abort ();
}

#endif

#ifndef NO_BASE_OF_ENCODED_VALUE

/* Given an encoding and an _Unwind_Context, return the base to which
   the encoding is relative.  This base may then be passed to
   read_encoded_value_with_base for use when the _Unwind_Context is
   not available.  */

static _Unwind_Ptr
base_of_encoded_value (unsigned char encoding, struct _Unwind_Context *context)
{
  if (encoding == DW_EH_PE_omit)
    return 0;

  switch (encoding & 0x70)
    {
    case DW_EH_PE_absptr:
    case DW_EH_PE_pcrel:
    case DW_EH_PE_aligned:
      return 0;

    case DW_EH_PE_textrel:
      return _Unwind_GetTextRelBase (context);
    case DW_EH_PE_datarel:
      return _Unwind_GetDataRelBase (context);
    case DW_EH_PE_funcrel:
      return _Unwind_GetRegionStart (context);
    }
  __gxx_abort ();
}

#endif

/* Read an unsigned leb128 value from P, store the value in VAL, return
   P incremented past the value.  We assume that a word is large enough to
   hold any value so encoded; if it is smaller than a pointer on some target,
   pointers should not be leb128 encoded on that target.  */

static const unsigned char *
read_uleb128 (const unsigned char *p, _uleb128_t *val)
{
  unsigned int shift = 0;
  unsigned char byte;
  _uleb128_t result;

  result = 0;
  do
    {
      byte = *p++;
      result |= ((_uleb128_t)byte & 0x7f) << shift;
      shift += 7;
    }
  while (byte & 0x80);

  *val = result;
  return p;
}

/* Similar, but read a signed leb128 value.  */

static const unsigned char *
read_sleb128 (const unsigned char *p, _sleb128_t *val)
{
  unsigned int shift = 0;
  unsigned char byte;
  _uleb128_t result;

  result = 0;
  do
    {
      byte = *p++;
      result |= ((_uleb128_t)byte & 0x7f) << shift;
      shift += 7;
    }
  while (byte & 0x80);

  /* Sign-extend a negative value.  */
  if (shift < 8 * sizeof(result) && (byte & 0x40) != 0)
    result |= -(((_uleb128_t)1L) << shift);

  *val = (_sleb128_t) result;
  return p;
}

/* Read unaligned data from the instruction buffer.  */

typedef unsigned __attribute__ ((mode (HI))) __uw16;
typedef unsigned __attribute__ ((mode (SI))) __uw32;
typedef unsigned __attribute__ ((mode (DI))) __uw64;
typedef signed __attribute__ ((mode (HI))) __sw16;
typedef signed __attribute__ ((mode (SI))) __sw32;
typedef signed __attribute__ ((mode (DI))) __sw64;

union unaligned
{
  void *p;
  __uw16 u2;
  __uw32 u4;
  __uw64 u8;
  __sw16 s2;
  __sw32 s4;
  __sw64 s8;
  unsigned char c[8];
} __attribute__ ((packed));

static inline int
read_1u (const void *p) { return *(const unsigned char *) p; }

static inline int
read_1s (const void *p) { return *(const signed char *) p; }

#if __GCC_TARGET_STRICT_ALIGNMENT__

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
static inline int
read_2u (const void *p) { const union unaligned *up = (const union unaligned *)p; return ((__uw16)up->c[0] << 8) | ((__uw16)up->c[1]); }
static inline __uw32
read_4u (const void *p) { const union unaligned *up = (const union unaligned *)p; return ((__uw32)up->c[0] << 24) | ((__uw32)up->c[1] << 16) | ((__uw32)up->c[2] << 8) | ((__uw32)up->c[3]); }
static inline __uw64
read_8u (const void *p) { const union unaligned *up = (const union unaligned *)p; return ((__uw64)read_4u(&up->c[0]) << 32) | ((__uw64)read_4u(&up->c[4])); }
#else
static inline int
read_2u (const void *p) { const union unaligned *up = (const union unaligned *)p; return ((__uw16)up->c[1] << 8) | ((__uw16)up->c[0]); }
static inline __uw32
read_4u (const void *p) { const union unaligned *up = (const union unaligned *)p; return ((__uw32)up->c[3] << 24) | ((__uw32)up->c[2] << 16) | ((__uw32)up->c[1] << 8) | ((__uw32)up->c[0]); }
static inline __uw64
read_8u (const void *p) { const union unaligned *up = (const union unaligned *)p; return ((__uw64)read_4u(&up->c[4]) << 32) | ((__uw64)read_4u(&up->c[0])); }
#endif

static inline int
read_2s (const void *p) { __sw16 x = read_2u(p); return x; }
static inline __sw32
read_4s (const void *p) { __sw32 x = read_4u(p); return x; }
static inline __sw64
read_8s (const void *p) { __sw64 x = read_8u(p); return x; }

#else /* !__GCC_TARGET_STRICT_ALIGNMENT__ */

static inline int
read_2u (const void *p) { const union unaligned *up = (const union unaligned *)p; return up->u2; }

static inline int
read_2s (const void *p) { const union unaligned *up = (const union unaligned *)p; return up->s2; }

static inline __uw32
read_4u (const void *p) { const union unaligned *up = (const union unaligned *)p; return up->u4; }

static inline __sw32
read_4s (const void *p) { const union unaligned *up = (const union unaligned *)p; return up->s4; }

static inline __uw64
read_8u (const void *p) { const union unaligned *up = (const union unaligned *)p; return up->u8; }

static inline __sw64
read_8s (const void *p) { const union unaligned *up = (const union unaligned *)p; return up->s8; }
#endif

static inline void *
read_pointer (const void *p) {
#if __SIZEOF_POINTER__ == 8
  return (void *)read_8u(p);
#elif __SIZEOF_POINTER__ == 4
  return (void *)read_4u(p);
#elif __SIZEOF_POINTER__ == 2
  return (void *)read_2u(p);
#else
  #error "unsupported pointer size"
#endif
}

/* Load an encoded value from memory at P.  The value is returned in VAL;
   The function returns P incremented past the value.  BASE is as given
   by base_of_encoded_value for this encoding in the appropriate context.  */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"

static const unsigned char *
read_encoded_value_with_base (unsigned char encoding, _Unwind_Ptr base,
			      const unsigned char *p, _Unwind_Ptr *val)
{
  const unsigned char *u = p;
  _Unwind_Internal_Ptr result;

  if (encoding == DW_EH_PE_aligned)
    {
      _Unwind_Internal_Ptr a = (_Unwind_Internal_Ptr) p;
      a = (a + sizeof (void *) - 1) & - sizeof(void *);
      result = *(_Unwind_Internal_Ptr *) a;
      p = (const unsigned char *) (_Unwind_Internal_Ptr) (a + sizeof (void *));
    }
  else
    {
      switch (encoding & 0x0f)
	{
	case DW_EH_PE_absptr:
	  result = (_Unwind_Internal_Ptr) read_pointer(p);
	  p += sizeof (void *);
	  break;

	case DW_EH_PE_uleb128:
	  {
	    _uleb128_t tmp;
	    p = read_uleb128 (p, &tmp);
	    result = (_Unwind_Internal_Ptr) tmp;
	  }
	  break;

	case DW_EH_PE_sleb128:
	  {
	    _sleb128_t tmp;
	    p = read_sleb128 (p, &tmp);
	    result = (_Unwind_Internal_Ptr) tmp;
	  }
	  break;

	case DW_EH_PE_udata2:
	  result = read_2u (p);
	  p += 2;
	  break;
	case DW_EH_PE_udata4:
	  result = read_4u (p);
	  p += 4;
	  break;
	case DW_EH_PE_udata8:
	  result = read_8u (p);
	  p += 8;
	  break;

	case DW_EH_PE_sdata2:
	  result = read_2s (p);
	  p += 2;
	  break;
	case DW_EH_PE_sdata4:
	  result = read_4s (p);
	  p += 4;
	  break;
	case DW_EH_PE_sdata8:
	  result = read_8s (p);
	  p += 8;
	  break;

	default:
	  __gxx_abort ();
	}

      if (result != 0)
	{
#if __FDPIC__
	  /* FDPIC relative addresses imply taking the GOT address
	     into account.  */
	  if ((encoding & DW_EH_PE_pcrel) && (encoding & DW_EH_PE_indirect))
	    {
	      result += _Unwind_gnu_Find_got ((_Unwind_Ptr) u);
	      result = *(_Unwind_Internal_Ptr *) result;
	    }
	  else
	    {
	      result += ((encoding & 0x70) == DW_EH_PE_pcrel
			 ? (_Unwind_Internal_Ptr) u : base);
	      if (encoding & DW_EH_PE_indirect)
		result = *(_Unwind_Internal_Ptr *) result;
	    }
#else
	  result += ((encoding & 0x70) == DW_EH_PE_pcrel
		     ? (_Unwind_Internal_Ptr) u : base);
	  if (encoding & DW_EH_PE_indirect)
	    result = *(_Unwind_Internal_Ptr *) result;
#endif
	}
    }

  *val = result;
  return p;
}

#pragma GCC diagnostic pop

#ifndef NO_BASE_OF_ENCODED_VALUE

/* Like read_encoded_value_with_base, but get the base from the context
   rather than providing it directly.  */

static inline const unsigned char *
read_encoded_value (struct _Unwind_Context *context, unsigned char encoding,
		    const unsigned char *p, _Unwind_Ptr *val)
{
  return read_encoded_value_with_base (encoding,
		base_of_encoded_value (encoding, context),
		p, val);
}

#endif

#endif /* unwind-pe.h */
