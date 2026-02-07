#!/bin/bash
# Configure Lynx for Windows 64-bit cross-compilation

PDCURSES_DIR=/home/patakuti/CCROOT/lynx-sdl2/PDCurses
SDL2_DIR=/home/patakuti/CCROOT/lynx-sdl2/win64-libs/SDL2-2.30.10/x86_64-w64-mingw32
SDL2_TTF_DIR=/home/patakuti/CCROOT/lynx-sdl2/win64-libs/SDL2_ttf-2.22.0/x86_64-w64-mingw32
OPENSSL_DIR=/home/patakuti/CCROOT/lynx-sdl2/win64-libs/openssl-3.4.1
ICONV_DIR=/home/patakuti/CCROOT/lynx-sdl2/win64-libs/libiconv-1.17
WIN64_BUILD=/home/patakuti/CCROOT/lynx-sdl2/win64-build

./configure \
  --host=x86_64-w64-mingw32 \
  --with-screen=pdcurses \
  --enable-widec \
  --enable-wcwidth-support \
  --with-ssl="${OPENSSL_DIR}" \
  --without-x \
  --disable-nls \
  CPPFLAGS="-I${PDCURSES_DIR} -I${SDL2_DIR}/include/SDL2 -I${SDL2_TTF_DIR}/include/SDL2 -I${OPENSSL_DIR}/include -I${ICONV_DIR}/include -DPDCURSES -DPDCURSES_SDL2 -DPDC_WIDE -DPDC_FORCE_UTF8 -DUSE_PROGRAM_DIR -DUSE_OPENSSL_INCL -DEXP_WCWIDTH_SUPPORT=1 -DUSE_WCWIDTH_SUPPORT=1" \
  LDFLAGS="-L${WIN64_BUILD} -L${SDL2_DIR}/lib -L${SDL2_TTF_DIR}/lib -L${OPENSSL_DIR}/lib -L${ICONV_DIR}/lib -mwindows" \
  LIBS="-lpdcurses -lSDL2 -lSDL2_ttf -liconv"
