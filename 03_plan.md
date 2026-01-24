- Lynx SDL2 版 実装計画

- 1. フェーズ1: 環境準備

  - 1.1 Ubuntu 依存パッケージインストール
    ```bash
    sudo apt update
    sudo apt install -y build-essential libsdl2-dev libsdl2-ttf-dev \
                        libssl-dev libncurses-dev git
    ```

  - 1.2 PDCurses ソース取得
    ```bash
    cd /home/patakuti/CCROOT/lynx-sdl2
    git clone https://github.com/wmcbrine/PDCurses.git
    ```

  - 1.3 日本語フォント確認
    ```bash
    # Noto Sans CJK がインストールされているか確認
    fc-list | grep -i "noto.*cjk.*mono"
    # なければインストール
    sudo apt install fonts-noto-cjk
    ```

- 2. フェーズ2: PDCurses ビルド

  - 2.1 SDL2 版ビルド (Ubuntu)
    ```bash
    cd /home/patakuti/CCROOT/lynx-sdl2/PDCurses/sdl2
    make WIDE=Y UTF8=Y
    ```
    - 成果物: pdcurses.a (静的ライブラリ)

  - 2.2 動作確認
    ```bash
    # テストプログラムをビルド・実行
    make WIDE=Y UTF8=Y sdltest
    ./sdltest
    ```

- 3. フェーズ3: Lynx ビルド

  - 3.1 configure 実行
    ```bash
    cd /home/patakuti/CCROOT/lynx-sdl2/lynx2.9.2

    # PDCurses のパスを設定
    export PDCURSES_DIR=/home/patakuti/CCROOT/lynx-sdl2/PDCurses

    ./configure \
      --with-screen=pdcurses \
      --enable-widec \
      --with-ssl \
      CPPFLAGS="-I${PDCURSES_DIR}" \
      LDFLAGS="-L${PDCURSES_DIR}/sdl2" \
      LIBS="-lpdcurses $(sdl2-config --libs) -lSDL2_ttf"
    ```

  - 3.2 make 実行
    ```bash
    make
    ```

  - 3.3 成果物確認
    - lynx 実行ファイルが生成されること

- 4. フェーズ4: 動作テスト

  - 4.1 基本動作テスト
    ```bash
    # フォント設定
    export PDC_FONT=/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc

    # Lynx 起動
    ./lynx
    ```
    - SDL2 ウィンドウが表示されること
    - テキストが正しく表示されること

  - 4.2 日本語表示テスト
    ```bash
    ./lynx https://www.yahoo.co.jp/
    ```
    - 日本語が正しく表示されること

  - 4.3 日本語入力テスト
    - 検索フォームで日本語入力
    - IME が動作すること

- 5. フェーズ5: Windows版SSL対応

  - 5.1 OpenSSL for MinGW の準備
    - MSYS2からOpenSSLパッケージを取得
    - または公式サイトからMinGW用ビルド済みバイナリをダウンロード
    - win64-libs/openssl ディレクトリに配置

  - 5.2 configure_win64.sh の修正
    - `--without-ssl` を `--with-ssl` に変更
    - CPPFLAGS に OpenSSL のインクルードパスを追加
    - LDFLAGS に OpenSSL のライブラリパスを追加

  - 5.3 Lynx ビルド
    ```bash
    cd lynx2.9.2
    ./configure_win64.sh
    make clean
    make
    cp lynx.exe ../win64-dist/
    ```

  - 5.4 OpenSSL DLL の配布
    - libssl-3-x64.dll
    - libcrypto-3-x64.dll
    - これらを win64-dist/ にコピー

  - 5.5 動作テスト
    - HTTPSサイトへのアクセステスト
    ```cmd
    lynx.exe https://www.google.com/
    ```

- 6. フェーズ6: ドキュメント整備

  - 6.1 README.md 作成
    - ビルド手順
    - 使用方法
    - フォント設定
    - トラブルシューティング

- 7. 作業チェックリスト

  - [ ] Ubuntu 依存パッケージインストール
  - [ ] PDCurses ソース取得
  - [ ] PDCurses SDL2 ビルド
  - [ ] PDCurses 動作確認 (sdltest)
  - [ ] Lynx configure
  - [ ] Lynx make
  - [ ] Lynx 起動確認
  - [ ] 日本語表示確認
  - [ ] 日本語入力確認
  - [ ] CJK文字幅対応
    - [x] リンク選択時のハイライト位置修正 (GridText.c anchor_ptr->line_pos計算)
    - [ ] 検索文字列のハイライト位置修正
  - [ ] Windows ビルド
  - [ ] README.md 作成
