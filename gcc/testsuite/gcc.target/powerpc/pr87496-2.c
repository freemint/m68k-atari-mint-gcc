/* PR target/87496 */
/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power7" } } */
/* { dg-options "-O2 -mcpu=power7 -mabi=ieeelongdouble -mlong-double-64 -Wno-psabi" } */

int i;

/* { dg-error "'-mabi=ieeelongdouble' requires '-mlong-double-128'" "PR87496" { target *-*-* } 0 } */
/* { dg-warning "using IEEE extended precision long double" "" { target *-*-* } 0 } */
