/* libgcc routines for 68000 w/o floating-point hardware.
   Copyright (C) 1994, 1996, 1997, 1998, 2008, 2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifdef  L_mulsi3
	.text
	FUNC(__mulsi3)
	.globl	SYM (__mulsi3)
	.globl	SYM (__mulsi3_internal)
#ifdef __ELF__
	.hidden	SYM (__mulsi3_internal)
#endif
SYM (__mulsi3):
SYM (__mulsi3_internal):
	move.l  d2,-(sp)
	movel   d0, a0          | d0a0 = x0:x1
	movel   d1, a1		| d1a1 = y0:y1
	swap	d0              | d0   = x1:x0 
	muluw   d1, d0          | d0 = y1*x0 
	swap	d1		| d1 = y1:y0
	movel   a0, d2		| d2 = x0:x1
	muluw   d2, d1          | d1 = x1*y0	
#ifndef __mcoldfire__
	addw	d1, d0
#else
	addl	d1, d0
#endif
	swap	d0
	clrw	d0
	movel	a1,d1		| d1 = y0:y1
	muluw	d2, d1		| d1 = x1*y1

	move.l  (sp)+,d2
	addl	d1, d0

	rts
#endif /* L_mulsi3 */

#ifdef  L_udivsi3
	.text
	FUNC(__udivsi3)
	.globl	SYM (__udivsi3)
	.globl	SYM (__udivsi3_internal)
#ifdef __ELF__
	.hidden	SYM (__udivsi3_internal)
#endif
SYM (__udivsi3):
SYM (__udivsi3_internal):
#ifndef __mcoldfire__
	move.l  d2,-(sp)
	movel	d0, a0

	cmpl	IMM (0x10000), d1 /* divisor >= 2 ^ 16 ?   */
	jcc	udivsi3_fast_L3		/* then try next algorithm */
	movel	d0, d2
	clrw	d2
	swap	d2
	divu	d1, d2          /* high quotient in lower word */
	movew	d2, d0		/* save high quotient */
	swap	d0
	movew	a0, d2		/* get low dividend + high rest */
	divu	d1, d2		/* low quotient */
	movew	d2, d0
	jra	udivsi3_fast_L6

udivsi3_fast_L3:	movel	d1, d2		/* use d2 as divisor backup */
udivsi3_fast_L4:	lsrl	IMM (1), d1	/* shift divisor */
	lsrl	IMM (1), d0	/* shift dividend */
	cmpl	IMM (0x10000), d1 /* still divisor >= 2 ^ 16 ?  */
	jcc	udivsi3_fast_L4
	divu	d1, d0		/* now we have 16-bit divisor */
	andl	IMM (0xffff), d0 /* mask out divisor, ignore remainder */

/* Multiply the 16-bit tentative quotient with the 32-bit divisor.  Because of
   the operand ranges, this might give a 33-bit product.  If this product is
   greater than the dividend, the tentative quotient was too large. */
	movel	d2, d1
	mulu	d0, d1		/* low part, 32 bits */
	swap	d2
	mulu	d0, d2		/* high part, at most 17 bits */
	swap	d2		/* align high part with low part */
	tstw	d2		/* high part 17 bits? */
	jne	udivsi3_fast_L5		/* if 17 bits, quotient was too large */
	addl	d2, d1		/* add parts */
	jcs	udivsi3_fast_L5		/* if sum is 33 bits, quotient was too large */
	cmpl	a0, d1		/* compare the sum with the dividend */
	jls	udivsi3_fast_L6		/* if sum > dividend, quotient was too large */
udivsi3_fast_L5:	subql	IMM (1), d0	/* adjust quotient */

udivsi3_fast_L6:
	move.l  (sp)+,d2
	rts

#else /* __mcoldfire__ */

/* ColdFire implementation of non-restoring division algorithm from
   Hennessy & Patterson, Appendix A. */
	moveml	d2-d4,sp@
	clrl	d2		| clear p
	moveq	IMM (31),d4
udivsi3_fast_L1:	addl	d0,d0		| shift reg pair (p,a) one bit left
	addxl	d2,d2
	movl	d2,d3		| subtract b from p, store in tmp.
	subl	d1,d3
	jcs	udivsi3_fast_L2		| if no carry,
	bset	IMM (0),d0	| set the low order bit of a to 1,
	movl	d3,d2		| and store tmp in p.
udivsi3_fast_L2:	subql	IMM (1),d4
	jcc	udivsi3_fast_L1
	moveml	sp@,d2-d4	| restore data registers
	rts
#endif /* __mcoldfire__ */

#endif /* L_udivsi3 */

#ifdef  L_divsi3
	.text
	FUNC(__divsi3)
	.globl	SYM (__divsi3)
	.globl	SYM (__divsi3_internal)
#ifdef __ELF__
	.hidden	SYM (__divsi3_internal)
#endif
SYM (__divsi3):
SYM (__divsi3_internal):
	move.l  d2,-(sp)
	moveq	IMM (1), d2	/* sign of result stored in d2 (=1 or =-1) */
	tstl	d1
	jpl	divsi3_fast_L1
	negl	d1
#ifndef __mcoldfire__
	negw	d2		/* change sign because divisor <0  */
#else
	negl	d2		/* change sign because divisor <0  */
#endif
divsi3_fast_L1:	tstl	d0		/* d0 = dividend */
	jpl	divsi3_fast_L2
	negl	d0
#ifndef __mcoldfire__
	negw	d2
#else
	negl	d2
#endif

divsi3_fast_L2:	movew	d2,a1		/* Called function MUST NOT clobber a1 */
	PICCALL	SYM (__udivsi3_internal)	/* divide abs(dividend) by abs(divisor) */

	movew	a1,d2
	jpl	divsi3_fast_L3
	negl	d0

divsi3_fast_L3:
	move.l  (sp)+,d2
	rts
#endif /* L_divsi3 */

#ifdef  L_umodsi3
	.text
	FUNC(__umodsi3)
	.globl	SYM (__umodsi3)
SYM (__umodsi3):
	move.l  d2,-(sp)
	movel	d0, d2
	movel	d1, a1		/* a1 MUST NOT be clobbered by calls*/
	PICCALL	SYM (__udivsi3_internal)
	movel	a1, d1		/* d1 = divisor */
#ifndef __mcoldfire__
	PICCALL	SYM (__mulsi3_internal)	/* d0 = (a/b)*b */
#else
	mulsl	d1,d0
#endif
	movel	d2, d1		/* d1 = dividend */
	subl	d0, d1		/* d1 = a - (a/b)*b */
	movel	d1, d0
	move.l  (sp)+,d2
	rts
#endif /* L_umodsi3 */

#ifdef  L_modsi3
	.text
	FUNC(__modsi3)
	.globl	SYM (__modsi3)
SYM (__modsi3):
	move.l  d2,-(sp)
	movel	d0, sp@-
	movel	d1, sp@-
	PICCALL	SYM (__divsi3_internal)
	movel	sp@+, d1	/* d1 = divisor */
#ifndef __mcoldfire__
	PICCALL	SYM (__mulsi3_internal)	/* d0 = (a/b)*b */
#else
	mulsl	d1,d0
#endif
	movel	sp@+, d1	/* d1 = dividend */
	subl	d0, d1		/* d1 = a - (a/b)*b */
	movel	d1, d0
	move.l  (sp)+,d2
	rts
#endif /* L_modsi3 */
