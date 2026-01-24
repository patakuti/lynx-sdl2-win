- Lynx + PDCurses SDL2 ビルド手順
  - 最終更新: 2025-12-31

- 前提条件
  - Linux (Ubuntu) でのクロスコンパイル環境
  - 必要なパッケージ
    - gcc, make
    - x86_64-w64-mingw32-gcc (Windows用クロスコンパイル)
    - libsdl2-dev, libsdl2-ttf-dev (Linux用)

- ディレクトリ構成
  - lynx-sdl2-win/
    - PDCurses/           - PDCurses ソース (submodule, CJK対応修正済み)
    - src/                - Lynx ソース (CJK対応修正済み)
    - WWW/                - WWWライブラリ
    - docs/               - Lynx本家ドキュメント
    - project-docs/       - プロジェクト固有ドキュメント
    - win64-libs/         - Windows用SDL2/SDL2_ttf/OpenSSLライブラリ（要ダウンロード）
    - win64-dist/         - Windows配布用ディレクトリ

- Linux用ビルド
  - 1. PDCursesのビルド
    ```bash
    cd PDCurses/sdl2
    make clean
    make WIDE=Y UTF8=Y
    cp pdcurses.a libpdcurses.a
    ```

  - 2. Lynxのビルド
    - configure済みの場合
      ```bash
      make clean
      make
      ```
    - 未configureの場合
      ```bash
      ./configure --with-screen=pdcurses --enable-widec \
        CPPFLAGS="-I./PDCurses -DPDCURSES -DPDC_WIDE -DPDC_FORCE_UTF8" \
        LDFLAGS="-L./PDCurses/sdl2" \
        LIBS="-lpdcurses -lSDL2 -lSDL2_ttf"
      # lynx_cfg.hで EXP_WCWIDTH_SUPPORT を有効化
      # src/makefileで wcwidth.o を OBJS に追加
      make
      ```

  - 3. 実行
    ```bash
    ./build_lynx.sh https://example.com/
    ```

- Windows用ビルド (クロスコンパイル、SSL対応)
  - 0. 必要なライブラリの準備（初回のみ）
    - win64-libs/ディレクトリを作成
      ```bash
      mkdir -p /home/patakuti/CCROOT/lynx-sdl2/win64-libs
      cd /home/patakuti/CCROOT/lynx-sdl2/win64-libs
      ```
    - SDL2のダウンロードと展開
      ```bash
      wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.10/SDL2-devel-2.30.10-mingw.tar.gz
      tar xzf SDL2-devel-2.30.10-mingw.tar.gz
      ```
    - SDL2_ttfのダウンロードと展開
      ```bash
      wget https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-devel-2.22.0-mingw.tar.gz
      tar xzf SDL2_ttf-devel-2.22.0-mingw.tar.gz
      ```
    - OpenSSLのダウンロードと展開
      ```bash
      wget https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-openssl-3.4.0-1-any.pkg.tar.zst
      tar -I zstd -xf mingw-w64-x86_64-openssl-3.4.0-1-any.pkg.tar.zst
      mv mingw64 openssl-3.4.0
      ```

  - 1. PDCursesのビルド
    ```bash
    cd PDCurses/sdl2
    SDL2_DIR=../../win64-libs/SDL2-2.30.10/x86_64-w64-mingw32
    SDL2_TTF_DIR=../../win64-libs/SDL2_ttf-2.22.0/x86_64-w64-mingw32
    make clean
    make CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar WIDE=Y UTF8=Y \
      CFLAGS="-O2 -DPDC_WIDE -DPDC_FORCE_UTF8 -I${SDL2_DIR}/include/SDL2 -I${SDL2_TTF_DIR}/include/SDL2"
    cd ../..
    ```

  - 2. Lynxのconfigure
    ```bash
    ./configure_win64.sh
    # lynx_cfg.hで EXP_WCWIDTH_SUPPORT を有効化
    # configure_win64.shで自動的に以下が設定される:
    #   - --with-ssl でSSL有効化
    #   - LIBS に -lpdcurses -lSDL2 -lSDL2_ttf が追加
    ```

  - 3. Lynxのビルド
    ```bash
    make
    cp src/lynx.exe win64-dist/
    # OpenSSL DLLもコピー
    cp win64-libs/openssl-3.4.0/bin/libssl-3-x64.dll win64-dist/
    cp win64-libs/openssl-3.4.0/bin/libcrypto-3-x64.dll win64-dist/
    ```

- 重要な設定ファイル
  - lynx_cfg.h
    - `#define EXP_WCWIDTH_SUPPORT 1` を設定すること
  - configure_win64.sh
    - SSL有効化: `--with-ssl="${OPENSSL_DIR}"`
    - LIBS に `-lpdcurses -lSDL2 -lSDL2_ttf` を設定
  - src/makefile (configure後)
    - OBJS に `wcwidth.o` を追加

- フォント設定
  - lynx.cfgで設定（推奨）
    - PDC_FONT: フォントファイルパス
    - PDC_FONT_INDEX: TTCファイル内のフォントインデックス
    - PDC_FONT_SIZE: フォントサイズ（ポイント）
    - lynx.cfgのデフォルト値（Windows用日本語表示向け）:
      ```
      PDC_FONT:C:\Windows\Fonts\msgothic.ttc
      PDC_FONT_INDEX:0
      PDC_FONT_SIZE:18
      ```
  - 環境変数で設定（lynx.cfgより優先される）
    - Linux
      ```bash
      export PDC_FONT=/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc
      export PDC_FONT_INDEX=5
      export PDC_FONT_SIZE=18
      ```
    - Windows
      ```batch
      set PDC_FONT=C:\Windows\Fonts\msgothic.ttc
      set PDC_FONT_INDEX=0
      set PDC_FONT_SIZE=18
      ```

- SSL/TLS対応
  - Windows版はOpenSSL 3.4.0を使用
  - HTTPSサイトへのアクセスが可能
  - 必要なDLL
    - libssl-3-x64.dll
    - libcrypto-3-x64.dll

- 日本語入力対応 (Windows IME)
  - 機能
    - IME候補ウィンドウの表示
    - 日本語文字の入力・確定・表示
  - 技術的詳細
    - SDL_HINT_IME_SHOW_UI をウィンドウ作成前に設定
    - PDC_doupdateで毎回SDL_UpdateWindowSurfaceを呼び出し（即時画面更新）
    - Unicode文字(U+0100以上)をUTF-8に変換してエディタバッファに挿入
    - UTF-8マルチバイト文字をLYwaddnstrで正しく出力
  - 変更ファイル
    - PDCurses/sdl2/pdcscrn.c - SDL_SetHintをウィンドウ作成前に移動
    - PDCurses/sdl2/pdcdisp.c - PDC_doupdateで強制画面更新、SDL_SetTextInputRect
    - src/LYStrings.c - LYDoEdit/LYEditInsert/LYRefreshEditでUTF-8対応
    - src/LYEditmap.c - EditBindingでUnicode文字をLYE_CHARとして返す
