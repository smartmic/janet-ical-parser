#!/bin/sh

# SPDX-FileCopyrightText: 2026 Martin Michel <martin@famic.de>
#
# SPDX-License-Identifier: MPL-2.0

set -eu   # Exit on errors and undefined variables
 
ROOT_PATH=$(pwd)

cd libical

mkdir build && cd build

cmake \
    -DCMAKE_INSTALL_PREFIX=${ROOT_PATH}/jpm_tree \
    -DUSE_BUILTIN_TZDATA=TRUE \
    -DICAL_BUILD_DOCS=FALSE \
    -DICAL_GLIB=FALSE \
    -DLIBICAL_BUILD_TESTING=FALSE \
    -DWITH_CXX_BINDINGS=FALSE \
    ..

make
make install
