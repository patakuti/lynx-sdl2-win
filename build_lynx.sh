#!/bin/bash

# Build script for Lynx with PDCurses SDL2

cd /home/patakuti/CCROOT/lynx-sdl2/lynx2.9.2

rm -f config.cache

export PDCURSES_DIR=/home/patakuti/CCROOT/lynx-sdl2/PDCurses

./configure \
  --with-screen=pdcurses \
  --enable-widec \
  --with-ssl \
  --without-x \
  CPPFLAGS="-I${PDCURSES_DIR} -DPDCURSES -DPDC_WIDE -DPDC_FORCE_UTF8" \
  LDFLAGS="-L${PDCURSES_DIR}/sdl2"
