Lynx SDL2 for Windows 64-bit
============================

This is Lynx 2.9.2 built with PDCurses SDL2 backend for Windows.
Japanese text display and input (IME) are supported.

Files:
  lynx.exe       - Main executable
  lynx.cfg       - Configuration file (font, charset settings)
  SDL2.dll       - SDL2 library
  SDL2_ttf.dll   - SDL2 TrueType font library
  fonts/         - Bundled fonts
    NotoSansMonoCJKjp-Regular.otf - Japanese monospace font (SIL OFL)
    LICENSE      - Font license file

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

  To use a different font, edit lynx.cfg:
    PDC_FONT:C:\Windows\Fonts\msgothic.ttc

License:
  Lynx: GPLv2
  PDCurses: Public Domain
  SDL2: zlib License
  Noto Sans Mono CJK JP: SIL Open Font License (OFL)
