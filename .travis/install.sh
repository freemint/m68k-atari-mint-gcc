#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

SYSROOT_DIR="/" ./.travis/install_bintray.sh m68k-atari-mint-binutils-gdb
BINTRAY_REPO="freemint" ./.travis/install_bintray.sh mintlib fdlibm
