/* Definitions of host machine for GNU compiler.
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

#include "m68k/xm-m68k.h"

/* make sure this is the case. some versions did'nt def this */
#ifndef HOST_WORDS_BIG_ENDIAN
#define HOST_WORDS_BIG_ENDIAN
#endif

/* make alloca defn more cross-compilable */

#if defined(__GNUC__)
#if defined(alloca)
#undef alloca
#endif
#define alloca(x) __builtin_alloca(x)
#endif

#define NO_STAB_H
#define HAVE_GETWD
