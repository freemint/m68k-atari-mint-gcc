#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

SRC_DIR=${PWD}

cd ..
mkdir .build && cd .build

CFLAGS_FOR_TARGET="-O2 -fomit-frame-pointer" CXXFLAGS_FOR_TARGET="-O2 -fomit-frame-pointer" "${SRC_DIR}/configure" \
	--prefix=/usr \
	--target=m68k-atari-mint \
	--with-sysroot=/usr/m68k-atari-mint/sys-root \
	--disable-nls \
	--enable-languages="c,c++" \
	--disable-libstdcxx-pch \
	--disable-libgomp \
	--disable-lto
make
make install-strip DESTDIR="${INSTALL_DIR}"

cd "${INSTALL_DIR}/usr/lib/gcc/m68k-atari-mint/${PROJECT_VERSION}/include-fixed"
for f in $(find . -type f)
do
	case "$f" in
		./README | ./limits.h | ./syslimits.h) ;;
		*) echo "Removing fixed include file $f"; rm "$f" ;;
	esac
done
for d in $(find . -depth -type d)
do
	test "$d" = "." || rmdir "$d"
done
cd -
