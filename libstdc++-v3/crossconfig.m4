dnl
dnl This file contains details for non-native builds.
dnl

AC_DEFUN([GLIBCXX_CROSSCONFIG],[
# Base decisions on target environment.
case "${host}" in
  arm*-*-symbianelf*)
    # This is a freestanding configuration; there is nothing to do here.
    ;;

  mips*-sde-elf*)
    # These definitions are for the SDE C library rather than newlib.
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS)
    GLIBCXX_CHECK_COMPILER_FEATURES
    GLIBCXX_CHECK_LINKER_FEATURES
    GLIBCXX_CHECK_MATH_SUPPORT
    GLIBCXX_CHECK_STDLIB_SUPPORT

    AC_DEFINE(HAVE_FINITE)
    AC_DEFINE(HAVE_HYPOT)
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_ISINF)

    AC_DEFINE(HAVE_LDEXPF)
    AC_DEFINE(HAVE_MODF)
    AC_DEFINE(HAVE_SQRTF)
    ;;

  *-darwin*)
    # Darwin versions vary, but the linker should work in a cross environment,
    # so we just check for all the features here.
    # Check for available headers.

    # Don't call GLIBCXX_CHECK_LINKER_FEATURES, Darwin doesn't have a GNU ld
    GLIBCXX_CHECK_MATH_SUPPORT
    GLIBCXX_CHECK_STDLIB_SUPPORT
    ;;

  *djgpp)
    # GLIBCXX_CHECK_MATH_SUPPORT
    AC_DEFINE(HAVE_ISINF)
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_FINITE)
    AC_DEFINE(HAVE_SINCOS)
    AC_DEFINE(HAVE_HYPOT)
    ;;

  *-freebsd*)
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS) 
    GLIBCXX_CHECK_LINKER_FEATURES
    AC_DEFINE(HAVE_SETENV)
    AC_DEFINE(HAVE_FINITEF)
    AC_DEFINE(HAVE_FINITE)
    AC_DEFINE(HAVE_FREXPF)
    AC_DEFINE(HAVE_HYPOT)
    AC_DEFINE(HAVE_HYPOTF)
    AC_DEFINE(HAVE_ISINF)
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_ISNANF)

    AC_DEFINE(HAVE_ACOSF)
    AC_DEFINE(HAVE_ASINF)
    AC_DEFINE(HAVE_ATAN2F)
    AC_DEFINE(HAVE_ATANF)
    AC_DEFINE(HAVE_CEILF)
    AC_DEFINE(HAVE_COSF)
    AC_DEFINE(HAVE_COSHF)
    AC_DEFINE(HAVE_EXPF)
    AC_DEFINE(HAVE_FABSF)
    AC_DEFINE(HAVE_FLOORF)
    AC_DEFINE(HAVE_FMODF)
    AC_DEFINE(HAVE_FREXPF)
    AC_DEFINE(HAVE_LDEXPF)
    AC_DEFINE(HAVE_LOG10F)
    AC_DEFINE(HAVE_LOGF)
    AC_DEFINE(HAVE_MODFF)
    AC_DEFINE(HAVE_POWF)
    AC_DEFINE(HAVE_SINF)
    AC_DEFINE(HAVE_SINHF)
    AC_DEFINE(HAVE_SQRTF)
    AC_DEFINE(HAVE_TANF)
    AC_DEFINE(HAVE_TANHF)
    if test x"long_double_math_on_this_cpu" = x"yes"; then
      AC_DEFINE(HAVE_FINITEL)
      AC_DEFINE(HAVE_ISINFL)
      AC_DEFINE(HAVE_ISNANL)
    fi
    ;;
  *-hpux*)
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS)
    GLIBCXX_CHECK_LINKER_FEATURES

    # GLIBCXX_CHECK_MATH_SUPPORT
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_HYPOT)
    AC_DEFINE(HAVE_ACOSF)
    AC_DEFINE(HAVE_ASINF)
    AC_DEFINE(HAVE_ATANF)
    AC_DEFINE(HAVE_COSF)
    AC_DEFINE(HAVE_COSHF)
    AC_DEFINE(HAVE_SINF)
    AC_DEFINE(HAVE_SINHF)
    AC_DEFINE(HAVE_TANF)
    AC_DEFINE(HAVE_TANHF)
    AC_DEFINE(HAVE_EXPF)
    AC_DEFINE(HAVE_ATAN2F)
    AC_DEFINE(HAVE_FABSF)
    AC_DEFINE(HAVE_FMODF)
    AC_DEFINE(HAVE_FREXPF)
    AC_DEFINE(HAVE_LOGF)
    AC_DEFINE(HAVE_LOG10F)
    AC_DEFINE(HAVE_MODF)
    AC_DEFINE(HAVE_POWF)
    AC_DEFINE(HAVE_SQRTF)

    # GLIBCXX_CHECK_STDLIB_SUPPORT
    AC_DEFINE(HAVE_STRTOLD)

    GCC_CHECK_TLS
    case "$target" in
      *-hpux10*)
	AC_DEFINE(HAVE_ISINF)
	AC_DEFINE(HAVE_ISINFF)
	AC_DEFINE(HAVE_ISNANF)
	AC_DEFINE(HAVE_FINITE)
	AC_DEFINE(HAVE_FINITEF)
	;;
    esac
    ;;
  *-linux* | *-uclinux* | *-gnu* | *-kfreebsd*-gnu | *-knetbsd*-gnu)
    GLIBCXX_CHECK_COMPILER_FEATURES
    GLIBCXX_CHECK_LINKER_FEATURES
    GLIBCXX_CHECK_MATH_SUPPORT
    GLIBCXX_CHECK_STDLIB_SUPPORT
    AC_DEFINE(_GLIBCXX_USE_RANDOM_TR1)
    GCC_CHECK_TLS
    AM_ICONV
    ;;
  *-mingw32*)
    GLIBCXX_CHECK_LINKER_FEATURES
    GLIBCXX_CHECK_MATH_SUPPORT
    GLIBCXX_CHECK_STDLIB_SUPPORT
    ;;
  *-mint*)
    # With MiNT, the linker should work in a cross environment,
    # so we just check for all the features here.

    # Check for available headers.
    AC_CHECK_HEADERS([nan.h ieeefp.h endian.h sys/isa_defs.h machine/endian.h \
    machine/param.h sys/machine.h fp.h locale.h float.h inttypes.h gconv.h \
    sys/types.h sys/ipc.h sys/sem.h])

    GLIBCXX_CHECK_LINKER_FEATURES
    GLIBCXX_CHECK_MATH_SUPPORT
    GLIBCXX_CHECK_BUILTIN_MATH_SUPPORT
    GLIBCXX_CHECK_COMPLEX_MATH_SUPPORT
    GLIBCXX_CHECK_STDLIB_SUPPORT

    # For showmanyc_helper().
    AC_CHECK_HEADERS(sys/ioctl.h sys/filio.h)
    GLIBCXX_CHECK_POLL
    GLIBCXX_CHECK_S_ISREG_OR_S_IFREG

    # For xsputn_2().
    AC_CHECK_HEADERS(sys/uio.h)
    GLIBCXX_CHECK_WRITEV

    # For the __streamoff_base_type typedef.
    GLIBCXX_CHECK_INT64_T

    # For LFS support.
    GLIBCXX_CHECK_LFS

    # For C99 support to TR1.
    GLIBCXX_CHECK_C99_TR1

    # For dev/random and dev/urandom for TR1.
    AC_DEFINE(_GLIBCXX_USE_RANDOM_TR1)

    # For TLS support.
    GCC_CHECK_TLS

    # For _Unwind_GetIPInfo.
    GCC_CHECK_UNWIND_GETIPINFO

    AC_LC_MESSAGES

    AC_TRY_COMPILE(
      [#include <setjmp.h>],
      [sigjmp_buf env;
       while (! sigsetjmp (env, 1))
	 siglongjmp (env, 1);
      ],
      [AC_DEFINE(HAVE_SIGSETJMP, 1, [Define if sigsetjmp is available.])])

    AC_FUNC_MMAP

    # For iconv support.
    AM_ICONV
    ;;
  *-netbsd*)
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS) 
    GLIBCXX_CHECK_LINKER_FEATURES
    AC_DEFINE(HAVE_FINITEF)
    AC_DEFINE(HAVE_FINITE)
    AC_DEFINE(HAVE_FREXPF)
    AC_DEFINE(HAVE_HYPOTF)
    AC_DEFINE(HAVE_ISINF)
    AC_DEFINE(HAVE_ISINFF)
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_ISNANF)
    if test x"long_double_math_on_this_cpu" = x"yes"; then
      AC_DEFINE(HAVE_FINITEL)
      AC_DEFINE(HAVE_ISINFL)
      AC_DEFINE(HAVE_ISNANL)
    fi
    ;;
  *-netware)
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS)
    GLIBCXX_CHECK_LINKER_FEATURES
    AC_DEFINE(HAVE_HYPOT)
    AC_DEFINE(HAVE_ISINF)
    AC_DEFINE(HAVE_ISNAN)
    ;;
  *-qnx6.1* | *-qnx6.2*)
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS) 
    GLIBCXX_CHECK_LINKER_FEATURES
    AC_DEFINE(HAVE_COSF)
    AC_DEFINE(HAVE_COSL)
    AC_DEFINE(HAVE_COSHF)
    AC_DEFINE(HAVE_COSHL)
    AC_DEFINE(HAVE_LOGF)
    AC_DEFINE(HAVE_LOGL)
    AC_DEFINE(HAVE_LOG10F)
    AC_DEFINE(HAVE_LOG10L)
    AC_DEFINE(HAVE_SINF)
    AC_DEFINE(HAVE_SINL)
    AC_DEFINE(HAVE_SINHF)
    AC_DEFINE(HAVE_SINHL)
    ;;
  *-solaris*)
    case "$target" in
      *-solaris2.7 | *-solaris2.8 | *-solaris2.9 | *-solaris2.10)
         GLIBCXX_CHECK_LINKER_FEATURES
         AC_DEFINE(HAVE_MBSTATE_T)
         AC_DEFINE(HAVE_FINITE)
         AC_DEFINE(HAVE_FPCLASS)
         # All of the dependencies for wide character support are here, so
         # turn it on. 
         AC_DEFINE(_GLIBCXX_USE_WCHAR_T) 
        ;;
    esac
    case "$target" in
      *-*-solaris2.10)
      # These two C99 functions are present only in Solaris >= 10
      AC_DEFINE(HAVE_STRTOF)
      AC_DEFINE(HAVE_STRTOLD)
     ;;
    esac
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_ISNANF)
    AC_DEFINE(HAVE_MODFF)
    AC_DEFINE(HAVE_HYPOT)
    ;;
  *-tpf)
    SECTION_FLAGS='-ffunction-sections -fdata-sections'
    AC_SUBST(SECTION_FLAGS)
    GLIBCXX_CHECK_LINKER_FEATURES
    AC_DEFINE(HAVE_FINITE)
    AC_DEFINE(HAVE_FINITEF)
    AC_DEFINE(HAVE_FREXPF)
    AC_DEFINE(HAVE_HYPOTF)
    AC_DEFINE(HAVE_ISINF)
    AC_DEFINE(HAVE_ISINFF)
    AC_DEFINE(HAVE_ISNAN)
    AC_DEFINE(HAVE_ISNANF)
    AC_DEFINE(HAVE_SINCOS)
    AC_DEFINE(HAVE_SINCOSF)
    if test x"long_double_math_on_this_cpu" = x"yes"; then
      AC_DEFINE(HAVE_FINITEL)
      AC_DEFINE(HAVE_HYPOTL)
      AC_DEFINE(HAVE_ISINFL)
      AC_DEFINE(HAVE_ISNANL)
    fi
    ;;
  *-vxworks)
    AC_DEFINE(HAVE_ACOSF)
    AC_DEFINE(HAVE_ASINF)
    AC_DEFINE(HAVE_ATAN2F)
    AC_DEFINE(HAVE_ATANF)
    AC_DEFINE(HAVE_CEILF)
    AC_DEFINE(HAVE_COSF)
    AC_DEFINE(HAVE_COSHF)
    AC_DEFINE(HAVE_EXPF)
    AC_DEFINE(HAVE_FABSF)
    AC_DEFINE(HAVE_FLOORF)
    AC_DEFINE(HAVE_FMODF)
    AC_DEFINE(HAVE_HYPOT)
    AC_DEFINE(HAVE_LOG10F)
    AC_DEFINE(HAVE_LOGF)
    AC_DEFINE(HAVE_POWF)
    AC_DEFINE(HAVE_SINF)
    AC_DEFINE(HAVE_SINHF)
    AC_DEFINE(HAVE_SQRTF)
    AC_DEFINE(HAVE_TANF)
    AC_DEFINE(HAVE_TANHF)
    ;;
  *)
    AC_MSG_ERROR([No support for this host/target combination.])
   ;;
esac
])
