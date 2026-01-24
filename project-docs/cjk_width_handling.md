- PDCurses SDL2 での CJK 文字幅処理
  - 日付: 2025-12-30
  - 問題と解決策のまとめ

- 背景
  - Lynx + PDCurses SDL2 で日本語テキストを表示する際、以下の問題が発生していた
    - 日本語文字が半分に欠けて表示される
    - 行折返しが正しく動作しない（画面右端を無視して表示される）

- 問題1: 日本語文字が半分に欠けて表示される
  - 原因
    - pdcdisp.c の SDL_BlitSurface で src rect が pdc_fwidth (10 pixels) に制限されていた
    - CJK文字のレンダリングサーフェスは 18 pixels あるため、右半分が切り取られていた
  - 実測結果 (test_sdl_render.c で確認)
    - pdc_fwidth = 10 pixels (ASCII 'W' の幅から計算)
    - CJK文字 (あ, 日, 本, 語, Ａ) のサーフェス幅 = 18 pixels
    - 2セル分の期待幅 = 20 pixels
  - 解決策
    - PDCurses/sdl2/pdcdisp.c の _new_packet 関数を修正
    - SDL_BlitSurface の src を NULL に変更し、サーフェス全体を Blit するようにした
    - 修正前: SDL_BlitSurface(pdc_font, &src, pdc_screen, &dest);
    - 修正後: SDL_BlitSurface(pdc_font, NULL, pdc_screen, &dest);

- 問題2: 行折返しが正しく動作しない
  - 原因
    - GridText.c で HAVE_WCWIDTH が定義されていたため、システムの wcwidth が使用されていた
    - システムの wcwidth とPDCurses の文字幅計算が一致していなかった
  - 解決策
    - GridText.c の条件分岐を修正
    - PDCurses + PDC_WIDE の場合は常に mk_wcwidth を使用するように変更
    - 修正後のコード:
      ```c
      #ifdef EXP_WCWIDTH_SUPPORT
      #  if defined(PDCURSES) && defined(PDC_WIDE)
           /* PDCurses SDL2: always use mk_wcwidth for consistent CJK width handling */
      #    include <wcwidth.h>
      #    define wcwidth(n) mk_wcwidth(n)
      #  elif defined(HAVE_WCWIDTH)
      ...
      ```

- 修正したファイル一覧
  - lynx2.9.2/lynx_cfg.h
    - EXP_WCWIDTH_SUPPORT を有効化
  - lynx2.9.2/src/LYCurses.c
    - PDCurses 用に wcwidth.h をインクルード
    - LYstrExtent0 で mk_wcwidth を使用して文字幅を計算
  - lynx2.9.2/src/GridText.c
    - PDCurses の場合は mk_wcwidth を使用するように条件分岐を修正
  - lynx2.9.2/src/makefile
    - wcwidth$o を OBJS に追加
  - PDCurses/pdcurses/addch.c
    - _pdc_is_wide_char 関数を追加 (CJK文字判定)
    - PDC_WIDE_PLACEHOLDER (値 0) を追加
    - waddch でワイド文字の後にプレースホルダーを挿入
  - PDCurses/sdl2/pdcdisp.c
    - _is_wide_char 関数を追加 (CJK文字判定)
    - プレースホルダーセル (値 0) をスキップ
    - ワイド文字の背景を2セル分クリア
    - SDL_BlitSurface の src を NULL に変更

- 重要な教訓
  - 推測に基づくデバッグは時間の無駄
  - 必ず実際の値を計測・確認してから修正する
  - test_sdl_render.c のようなテストプログラムで実測することが重要

- テストプログラム
  - test_sdl_render.c: SDL2 TTF レンダリングの値を実測
  - test_cjk_display.c: PDCurses での CJK 文字表示テスト
  - test_wcwidth.c: wcwidth の動作確認

- CJK 文字幅判定の Unicode 範囲
  - 0x1100 - 0x115f: Hangul Jamo initial consonants
  - 0x2329, 0x232a: Angle brackets
  - 0x2e80 - 0x4dbf: CJK Radicals, Kangxi, etc. (except 0x303f)
  - 0x4e00 - 0xa4cf: CJK Unified Ideographs, Yi
  - 0xa960 - 0xa97f: Hangul Jamo Extended-A
  - 0xac00 - 0xd7a3: Hangul Syllables
  - 0xf900 - 0xfaff: CJK Compatibility Ideographs
  - 0xfe10 - 0xfe19: Vertical forms
  - 0xfe30 - 0xfe6f: CJK Compatibility Forms
  - 0xff00 - 0xff60: Fullwidth Forms
  - 0xffe0 - 0xffe6: Fullwidth currency symbols
  - 0x20000 - 0x2fffd: Supplementary Ideographic Plane
  - 0x30000 - 0x3fffd: Tertiary Ideographic Plane
