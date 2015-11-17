/* Generate macros based on the combined_fn enum.
   Copyright (C) 2015 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* Automatically generate code fragments related to combined_fn.

   The program looks for math built-in functions that have float, double
   and long double variants, such as {sqrtf, sqrt, sqrtl}, and that may
   or may not have an associated internal function as well.  It also looks
   for integer built-in functions that have int, long, long long and
   intmax_t variants, such as {clz, clzl, clzll, clzimax}, and that
   again may or may not have an associated internal function as well.

   When run with -c, the generator prints a list of macros such as:

      CASE_CFN_SQRT

   for each group of functions described above, with 'case CFN_*'
   statements for each built-in and internal function in the group.
   For example, there are both built-in and internal implementations
   of SQRT, so "CASE_CFN_SQRT:" is equivalent to:

      case CFN_BUILT_IN_SQRTF:
      case CFN_BUILT_IN_SQRT:
      case CFN_BUILT_IN_SQRTL:
      case CFN_SQRT:

   The macros for groups with no internal function drop the last line.  */

#include "bconfig.h"
#include "system.h"
#include "coretypes.h"
#include "hash-table.h"
#include "hash-set.h"
#include "errors.h"

typedef hash_set <nofree_string_hash> string_set;

/* Add all names in null-terminated list NAMES to SET.  */

static void
add_to_set (string_set *set, const char *const *names)
{
  for (unsigned int i = 0; names[i]; ++i)
    set->add (names[i]);
}

/* Return true if *BUILTINS contains BUILT_IN_<NAME><SUFFIX> for all
   suffixes in null-terminated list SUFFIXES.  */

static bool
is_group (string_set *builtins, const char *name, const char *const *suffixes)
{
  for (unsigned int i = 0; suffixes[i]; ++i)
    if (!builtins->contains (ACONCAT (("BUILT_IN_", name, suffixes[i], NULL))))
      return false;
  return true;
}

/* Print a macro for all combined functions related to NAME, with the
   null-terminated list of suffixes in SUFFIXES.  INTERNAL_P says whether
   CFN_<NAME> also exists.  */

static void
print_case_cfn (const char *name, bool internal_p,
		const char *const *suffixes)
{
  printf ("#define CASE_CFN_%s", name);
  if (internal_p)
    printf (" \\\n  case CFN_%s", name);
  for (unsigned int i = 0; suffixes[i]; ++i)
    printf ("%s \\\n  case CFN_BUILT_IN_%s%s",
	    internal_p || i > 0 ? ":" : "", name, suffixes[i]);
  printf ("\n");
}

const char *const builtin_names[] = {
#define DEF_BUILTIN(ENUM, N, C, T, LT, B, F, NA, AT, IM, COND) \
  #ENUM,
#include "builtins.def"
  NULL
};

const char *const internal_fn_flt_names[] = {
#define DEF_INTERNAL_FLT_FN(NAME, FLAGS, OPTAB, TYPE) \
  #NAME,
#include "internal-fn.def"
  NULL
};

const char *const internal_fn_int_names[] = {
#define DEF_INTERNAL_INT_FN(NAME, FLAGS, OPTAB, TYPE) \
  #NAME,
#include "internal-fn.def"
  NULL
};

static const char *const flt_suffixes[] = { "F", "", "L", NULL };
static const char *const int_suffixes[] = { "", "L", "LL", "IMAX", NULL };

static const char *const *const suffix_lists[] = {
  flt_suffixes,
  int_suffixes,
  NULL
};

int
main (int argc, char **argv)
{
  /* Check arguments.  */
  progname = argv[0];
  if (argc != 2
      || argv[1][0] != '-'
      || argv[1][1] != 'c'
      || argv[1][2])
    fatal ("usage: %s -c > file", progname);

  /* Collect the set of built-in and internal functions.  */
  string_set builtins;
  string_set internal_fns;
  add_to_set (&builtins, builtin_names);
  add_to_set (&internal_fns, internal_fn_flt_names);
  add_to_set (&internal_fns, internal_fn_int_names);

  /* Check the functions.  */
  for (unsigned int i = 0; internal_fn_flt_names[i]; ++i)
    {
      const char *name = internal_fn_flt_names[i];
      if (!is_group (&builtins, name, flt_suffixes))
	error ("DEF_INTERNAL_FLT_FN (%s) has no associated built-in"
	       " functions", name);
    }
  for (unsigned int i = 0; internal_fn_int_names[i]; ++i)
    {
      const char *name = internal_fn_int_names[i];
      if (!is_group (&builtins, name, int_suffixes))
	error ("DEF_INTERNAL_INT_FN (%s) has no associated built-in"
	       " functions", name);
    }

  /* Go through the built-in functions in declaration order, outputting
     definitions as appropriate.  */
  for (unsigned int i = 0; builtin_names[i]; ++i)
    {
      const char *name = builtin_names[i];
      if (strncmp (name, "BUILT_IN_", 9) == 0)
	{
	  const char *root = name + 9;
	  for (unsigned int j = 0; suffix_lists[j]; ++j)
	    if (is_group (&builtins, root, suffix_lists[j]))
	      {
		bool internal_p = internal_fns.contains (root);
		print_case_cfn (root, internal_p, suffix_lists[j]);
	      }
	}
    }

  if (fflush (stdout) || fclose (stdout) || have_error)
    return FATAL_EXIT_CODE;
  return SUCCESS_EXIT_CODE;
}
