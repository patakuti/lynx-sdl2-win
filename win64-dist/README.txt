Lynx SDL2 for Windows 64-bit
============================

This is Lynx 2.9.2 built with PDCurses SDL2 backend for Windows.
Japanese text display and input (IME) are supported.

Files:
  lynx.exe             - Main executable
  lynx.cfg             - Configuration file (font, charset settings)
  lynx.lss             - Color style sheet
  lynx-sdl.bmp         - Window icon
  SDL2.dll             - SDL2 library
  SDL2_ttf.dll         - SDL2 TrueType font library
  libssl-3-x64.dll     - OpenSSL SSL library
  libcrypto-3-x64.dll  - OpenSSL crypto library
  fonts/               - Bundled fonts
    NotoSansMonoCJKjp-Regular.otf - Japanese monospace font (SIL OFL)
    LICENSE            - Font license file

Usage:
  Double-click lynx.exe to start Lynx.

  Or run from command prompt:
    lynx.exe https://www.example.com/

Configuration (lynx.cfg):
  PDC_FONT       - Path to TrueType font file
                   Default: fonts/NotoSansMonoCJKjp-Regular.otf
                   Relative paths are resolved from lynx.exe directory.
  PDC_FONT_SIZE  - Font size in points (default: 18)
  CHARACTER_SET  - Display charset (default: utf-8)
  ASSUME_CHARSET - Assumed document charset (default: utf-8)

  To use a different font, copy the font file to the fonts/ directory
  and edit lynx.cfg:
    PDC_FONT:fonts\YourFont.ttf

  Note: Windows 11 restricts user access to C:\Windows\Fonts.

SSL Certificates:
  This program does not include server certificates.
  You can obtain CA certificates from:
    https://curl.haxx.se/docs/caextract.html

  Download the cacert.pem file and save it as "cert.pem" in the Lynx
  installation directory (same directory as lynx.exe).

  For more details about root certificates, see docs/README.rootcerts.

License:
  Lynx: GPLv2
  PDCurses: Public Domain
  SDL2: zlib License
  OpenSSL: Apache License 2.0
  Noto Sans Mono CJK JP: SIL Open Font License (OFL)
